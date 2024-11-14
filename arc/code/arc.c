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
    union
    {
        u8 RT;
        u8 RS;
        u8 TO;
        u8 FRT;
        u8 FRS;
    };
    
    u32 RA;
    
    union
    {
        s32 D;
        u32 UI;
        s32 SI;
    };
    
} d_format_data;

inline d_format_data
ExtractInstructionDFormat(char *Mnemonic, u32 Instruction)
{
    d_format_data Data = {};
    
    Data.RT = (u8)((Instruction & 0b00000011111000000000000000000000) >> (32 - 11));
    Data.RA = (u8)((Instruction & 0b00000000000111110000000000000000) >> (32 - 16));
    
    Data.D = (s32)(s16)(Instruction & 0xffff);
    
    return Data;
}

typedef struct i_format_data
{
    s32 LI;
    u8 AA;
    u8 LK;
} i_format_data;

inline i_format_data
ExtractInstructionIFormat(u32 Instruction)
{
    i_format_data Data = {};
    //Data = *(i_format_data *)&Instruction;
    
    Data.LI = (s32)((Instruction & 0b00000011111111111111111111111100) >> (32 - 32));
    Data.AA = (u8)((Instruction & 0b00000000000000000000000000000010) >> (32 - 31));
    Data.LK = (u8)((Instruction & 0b00000000000000000000000000000001) >> (32 - 32));
    
    if(Data.LI & 0b00000010000000000000000000000000)
    {
        Data.LI |= 0b11111100000000000000000000000000;
    }
    
    return Data;
}

typedef struct b_format_data
{
    u32 BO;
    u32 BI;
    u32 BD;
    
    u8 AA;
    u8 LK;
} b_format_data;

inline b_format_data
ExtractInstructionBFormat(u32 Instruction)
{
    b_format_data Data = {};
    
    Data.BO = (Instruction & 0b00000011111000000000000000000000) >> (32 - 11);
    Data.BI = (Instruction & 0b00000000000111110000000000000000) >> (32 - 16);
    Data.BD = (Instruction & 0b00000000000000001111100000000000) >> (32 - 21);
    
    Data.AA = Instruction & 0b10 ? 1 : 0;
    Data.LK = Instruction & 0b01 ? 1 : 0;
    
    return Data;
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

internal void
TrapDecoder(d_format_data *Data)
{
    char *ConditionSymbol = "";
    switch(Data->TO)
    {
        case 0b10000: { ConditionSymbol = "lt"; } break;
        case 0b10100: { ConditionSymbol = "le"; } break;
        case 0b00100: { ConditionSymbol = "eq"; } break;
        case 0b01100: { ConditionSymbol = "ge"; } break;
        case 0b01000: { ConditionSymbol = "gt"; } break;
        //case 0b00110: { ConditionSymbol = "nl"; } break;
        case 0b11000: { ConditionSymbol = "ne"; } break;
        //case 0b10100: { ConditionSymbol = "ng"; } break;
        case 0b00010: { ConditionSymbol = "llt"; } break;
        case 0b00110: { ConditionSymbol = "lle"; } break;
        case 0b00101: { ConditionSymbol = "lge"; } break;
        case 0b00001: { ConditionSymbol = "lgt"; } break;
        //case 0b00101: { ConditionSymbol = "lnl"; } break;
        //case 0b00110: { ConditionSymbol = "lng"; } break;
        case 0b11111: { ConditionSymbol = "u"; } break;
        //case 0b11111: { ConditionSymbol = ""; } break;
    }
    
    printf("tw%si  r%d, %d", ConditionSymbol, Data->RA, Data->SI);
}

internal void
CmpliDecoder(d_format_data *Data)
{
    u32 BF = (Data->RS & 0b11100) >> 2;
    u32 L  = (Data->RS & 0b00001) >> 0;
    
    printf("cmpli r%d %d r%d, %d", BF, L, Data->RA, (u16)Data->UI);
}

#include "ppc_instruction_set.h"

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
                case D_Form:
                {
                    d_format_data Data = ExtractInstructionDFormat(FormatInstruction->Mnemonic, Instruction);
                    
                    if(FormatInstruction->CustomDecoding)
                    {
                        FormatInstruction->CustomDecoding(&Data);
                    }
                    else
                    {
                        char *Format = "%-6s r%d,  r%d, %d";
                        printf(Format, FormatInstruction->Mnemonic, Data.RT, Data.RA, Data.D);
                    }
                } break;
                
                case I_Form:
                {
                    i_format_data Data = ExtractInstructionIFormat(Instruction);
                    
                    printf(FormatInstruction->Mnemonic);
                    
                    s32 BranchAddress = Data.LI;
                    if(Data.LK)
                    {
                        printf("l");
                    }
                    
                    if(Data.AA)
                    {
                        printf("a");
                    }
                    else
                    {
                        BranchAddress = (s32)(BranchAddress + Address);
                    }
                    
                    printf("  0x%lx", BranchAddress);
                    
                } break;
                
                case B_Form:
                {
                    b_format_data Data = ExtractInstructionBFormat(Instruction);
                    
                    printf(FormatInstruction->Mnemonic);
                    
                    
                    s32 OperandAddress = Data.BD;
                    if(Data.LK)
                    {
                        printf("l");
                    }
                    
                    if(Data.AA)
                    {
                        printf("a");
                    }
                    else
                    {
                        OperandAddress = (s32)(OperandAddress + Address);
                    }
                    
                    //printf("  0x%lx", LI);
                } break;
                
                case X_Form:
                {
                    ExtractInstructionXFormat(FormatInstruction->Mnemonic, Instruction);
                } break;
            }
        }
        
        int BH = 169;
        
        printf("\n");
        Address += 4;
    }
}