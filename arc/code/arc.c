#include <stdio.h>
#include <windows.h>

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef u32 b32;

#define internal static
#define local_variable static
#define global_variable static

#define true 1
#define false 0

#define Assert(X) if(!(X)) {*(int *)0 = 0;}

#define Minimum(a, b) a < b ? a : b
#define Maximum(a, b) a < b ? b : a


typedef struct loaded_file
{
    char *Filename;
    
    void *Memory;
    u64 Size;
} loaded_file;

inline u32
SafeTruncateUInt64(u64 Value)
{
    // TODO(felipe): Defines for maximum values.
    Assert(Value <= 0xFFFFFFFF);
    u32 Result = (u32)Value;
    return Result;
}

internal loaded_file
Win32ReadEntireFile(char *Filename)
{
    loaded_file Result = {0};
    
    b32 Failed = false;
    
    HANDLE FileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if(FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize;
        if(GetFileSizeEx(FileHandle, &FileSize))
        {
            u32 FileSize32 = SafeTruncateUInt64(FileSize.QuadPart);
            Result.Memory = VirtualAlloc(0, FileSize32, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
            if(Result.Memory)
            {
                DWORD BytesRead;
                if(ReadFile(FileHandle, Result.Memory, FileSize32, &BytesRead, 0) &&
                   (FileSize32 == BytesRead))
                {
                    // NOTE(felipe): File read succesfully.
                    Result.Filename = Filename;
                    Result.Size = FileSize32;
                }
                else
                {
                    VirtualFree(Result.Memory, 0, MEM_RELEASE);
                    Result.Memory = 0;
                }
            }
        }
        
        CloseHandle(FileHandle);
    }
    
    return Result;
}


void 
PrintHexDump(u8 *Data, u32 Length)
{
    u32 Rows = (Length + 15) / 16;
    u64 Address = 0;
    for(u32 I = 0;
        I < Rows;
        ++I)
    {
        printf("0x%.8llx  ", Address);
        
        // Value column
        u32 RowLength = Minimum(16, Length - I*16);
        for(u32 J = 0;
            J < RowLength;
            ++J)
        {
            if(~J & 0b1)
            {
                printf(" ");
            }
            
            u32 Index = I*16 + J;
            u8 Value = Data[Index];
            printf("%.2hhx", Value);
        }
        
        // Spacing after value column
        u32 BlankWidth = (16 - RowLength)*2 + (16 - RowLength)/2;
        printf("%*s", BlankWidth, "");
        printf("    ");
        
        // Ascii column
        for(u32 J = 0;
            J < RowLength;
            ++J)
        {
            u32 Index = I*16 + J;
            u8 Value = Data[Index];
            
            char Char = '.';
            if(Value >= 32 && Value <= 126)
            {
                Char = Value;
            }
            
            printf("%c", Char);
        }
        
        printf("\n");
        
        Address += RowLength;
    }
}

typedef struct d_format_data
{
    u32 RT;
    u32 RA;
    s32 D;
} d_format_data;

inline void
ExtractInstructionIFormat(char *Mnemonic, u32 Instruction, u64 Address)
{
    u8 AA = Instruction & 0b10 ? 1 : 0;
    u8 LK = Instruction & 0b01 ? 1 : 0;
    
    s32 LI = Instruction & 0b00000011111111111111111111111100;
    
    if(LI & 0b00000010000000000000000000000000)
    {
        LI |= 0b11111100000000000000000000000000;
    }
    
    printf(Mnemonic);
    
    if(LK)
    {
        printf("l");
    }
    
    if(AA)
    {
        printf("a");
    }
    else
    {
        LI = (s32)(Address + LI);
    }
    
    printf("  0x%lx", LI);
}

inline void
ExtractInstructionBFormat(char *Mnemonic, u32 Instruction, u64 Address)
{
    u8 AA = Instruction & 0b10 ? 1 : 0;
    u8 LK = Instruction & 0b01 ? 1 : 0;
    
    u32 BO = (Instruction & 0b00000011111000000000000000000000) >> (32 - 11);
    u32 BI = (Instruction & 0b00000000000111110000000000000000) >> (32 - 16);
    u32 BD = (Instruction & 0b00000000000000001111100000000000) >> (32 - 21);
    
    printf(Mnemonic);
    
    if(LK)
    {
        printf("l");
    }
    
    if(AA)
    {
        printf("a");
    }
    else
    {
        //LI = (s32)(Address + LI);
    }
    
    //printf("  0x%lx", LI);
}

inline void
ExtractInstructionSCFormat(char *Mnemonic, u32 Instruction, u64 Address)
{
    u32 LEV = (Instruction & 0b00000000000000000000111111100000) >> (32 - 27);
    
    printf(Mnemonic);
    if(LEV)
    {
        printf("  0x%lx", LEV);
    }
}

inline void
ExtractInstructionDFormat(char *Mnemonic, u32 Instruction)
{
    u32 RT = (Instruction & 0b00000011111000000000000000000000) >> (32 - 11);
    u32 RA = (Instruction & 0b00000000000111110000000000000000) >> (32 - 16);
    
    s32 D = (s32)(s16)(Instruction & 0xffff);
    
    // Printing, probably should compress to separate function.
    char *Format = "";
    if(RT < 10)
        Format = "%-6s r%d,  %d(r%d)";
    else
        Format = "%-6s r%d, %d(r%d)";
    
    printf(Format, Mnemonic, RT, D, RA);
}

inline void
ExtractInstructionXFormat(char *Mnemonic, u32 Instruction)
{
    u32 RS = (Instruction & 0b00000011111000000000000000000000) >> (32 - 11);
    u32 RA = (Instruction & 0b00000000000111110000000000000000) >> (32 - 16);
    u32 RB = (Instruction & 0b00000000000000000111110000000000) >> (32 - 21);
    
    // Printing, probably should compress to separate function.
    //
    
    // Extended mnemonics
    u8 Opcode = Instruction >> (32 - 6);
    u16 ExtendedOpcode = (Instruction >> (32 - 31)) & 0b1111111111;
    if(Opcode == 31 && ExtendedOpcode == 444 &&
       RS == RB)
    {
        char *Format = "%-6s r%d,  r%d";
        
        printf(Format, "mr", RA, RS);
    }
    else
    {
        char *Format = "%-6s r%d,  r%d,  r%d";
        
        printf(Format, Mnemonic, RA, RS, RB);
    }
}



enum ppc_instruction_form
{
    None_Form,
    
    I_Form,
    B_Form,
    SC_Form,
    
    D_Form,
    X_Form,
};

typedef struct ppc_instruction
{
    u16 Opcode;
    char Mnemonic[8];
    
    u16 Form;
    
    u16 ExtendedOpcode;
} ppc_instruction;

global_variable ppc_instruction 
InstructionSet[] = 
{
    {16, "bc", B_Form},
    {17, "sc", SC_Form},
    {18, "b", I_Form},
    
    {31, "or", X_Form, 444},
    
    {32, "lwz", D_Form},
    {33, "lwzu", D_Form},
    {34, "lbz", D_Form},
    {35, "lbzu", D_Form},
    
    {36, "stw", D_Form},
    {37, "stwu", D_Form},
    {38, "stb", D_Form},
    {39, "stbu", D_Form},
};

#define ArrayCount(A) (sizeof(A) / sizeof((A)[0]))

int main(int Argc, char **Argv)
{
    loaded_file File = Win32ReadEntireFile("simple.bin");
    PrintHexDump((u8 *)File.Memory, (u32)File.Size);
    
    u32 InstructionCount = (u32)(File.Size/4);
    //u32 InstructionCount = 1;
    u64 Address = 0x80003000;
    
    for(u32 InstructionIndex = 0;
        InstructionIndex < InstructionCount;
        ++InstructionIndex)
    {
        u32 Instruction = *(u32 *)((u8 *)File.Memory + InstructionIndex*4);
        Instruction = (((Instruction>>24)&0xff) | // move byte 3 to byte 0
                       ((Instruction<<8)&0xff0000) | // move byte 1 to byte 2
                       ((Instruction>>8)&0xff00) | // move byte 2 to byte 1
                       ((Instruction<<24)&0xff000000)); // byte 0 to byte 3
        
        //u32 Instruction = 0x4bffffb5;
        u8 Opcode = Instruction >> (32 - 6);
        u16 ExtendedOpcode = (Instruction >> (32 - 31)) & 0b1111111111;
        
        // Printing
        printf("0x%.8llx      ", Address);
        
        printf("%.8lx    ", Instruction);
        
        // Dissasembling
        ppc_instruction *FormatInstruction = 0;
        for(u32 SetIndex = 0;
            SetIndex < ArrayCount(InstructionSet);
            ++SetIndex)
        {
            ppc_instruction *TestFormatInst = InstructionSet + SetIndex;
            
            if(TestFormatInst->Opcode == Opcode &&
               (TestFormatInst->Form != X_Form || TestFormatInst->ExtendedOpcode == ExtendedOpcode))
            {
                FormatInstruction = TestFormatInst;
                break;
            }
        }
        
        if(FormatInstruction)
        {
            switch(FormatInstruction->Form)
            {
                case I_Form:
                {
                    ExtractInstructionIFormat(FormatInstruction->Mnemonic, Instruction, Address);
                } break;
                
                case D_Form:
                {
                    ExtractInstructionDFormat(FormatInstruction->Mnemonic, Instruction);
                } break;
                
                case X_Form:
                {
                    ExtractInstructionXFormat(FormatInstruction->Mnemonic, Instruction);
                } break;
            }
        }
        
        /*
        switch(Opcode)
        {
            // branch I-form
            case 18:
            {
                u8 AA = Instruction & 0b10 ? 1 : 0;
                u8 LK = Instruction & 0b01 ? 1 : 0;
                
                s32 LI = Instruction & 0b00000011111111111111111111111100;
                
                if(LI & 0b00000010000000000000000000000000)
                {
                    LI |= 0b11111100000000000000000000000000;
                }
                
                printf("b");
                
                if(LK)
                {
                    printf("l");
                }
                
                if(AA)
                {
                    printf("a");
                }
                else
                {
                    LI = (s32)(Address + LI);
                }
                
                printf("  0x%lx", LI);
                
            } break;
            
            
            case 32:
            {
                d_format_data Data = ExtractInstructionDFormat("lwz", Instruction);
                
                //printf("lwz  r%d, %d(r%d)", Data.RT, Data.D, Data.RA);
            } break;
            
            case 36:
            {
                d_format_data Data = ExtractInstructionDFormat("stw", Instruction);
                
                //printf("stw  r%d, %d(r%d)", Data.RT, Data.D, Data.RA);
            } break;
            
            case 38:
            {
                d_format_data Data = ExtractInstructionDFormat("stb", Instruction);
                
                //printf("stb  r%d, %d(r%d)", Data.RT, Data.D, Data.RA);
            } break;
            
            default:
            {
                //Assert("Invalid opcode");
            }
        }
*/
        
        int BH = 169;
        
        printf("\n");
        Address += 4;
    }
}