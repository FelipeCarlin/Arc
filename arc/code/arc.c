
#include "arc.h"

#include <stdio.h>
#include <windows.h>

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

#include "ppc_instruction_set.h"

internal char *
TOFieldString(u32 TO)
{
    char *ConditionSymbol = "";
    switch(TO)
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
    
    return ConditionSymbol;
}

int main(int Argc, char **Argv)
{
    loaded_file File = Win32ReadEntireFile("simple.bin");
    PrintHexDump((u8 *)File.Memory, (u32)File.Size);
    
    // TODO:
    // - Decouple instruction decoding from printing.
    // - Support extended mnemonics (ie.  mr ra, rb  ->  or ra, rb, rb).
    // - Support object files.
    
    u32 InstructionCount = (u32)(File.Size/4);
    u64 Address = 0x80003000;
    
    u32 AddressSpacing = 3;
    u32 MnemonicSpacing = 8;
    u32 OperandSpacing = 4;
    
    for(u32 InstructionIndex = 0;
        InstructionIndex < InstructionCount;
        ++InstructionIndex)
    {
        u32 InstructionBytes = *(u32 *)((u8 *)File.Memory + InstructionIndex*4);
        u32 Instruction = (((InstructionBytes >> 24)& 0xff)     |   // move byte 3 to byte 0
                           ((InstructionBytes << 8) & 0xff0000) |   // move byte 1 to byte 2
                           ((InstructionBytes >> 8) & 0xff00)   |   // move byte 2 to byte 1
                           ((InstructionBytes << 24)& 0xff000000)); // byte 0 to byte 3
        
        u8 Opcode = Instruction >> (32 - 6);
        u16 ExtendedOpcode = (Instruction >> (32 - 31)) & 0b1111111111;
        
        // Printing
        printf("\033[2m0x%.8llx\033[0m      ", Address);
        
        //printf("%.8lx    ", Instruction);
        for(s32 I = 0;
            I < 4;
            ++I)
        {
            u8 Byte = (InstructionBytes >> (I*8)) & 0xff;
            u8 ColorIndex = Byte >> 5;
            
            u32 Colors[] =
            {
                240,
                248,
                256,
                260,
                260,
                256,
                248,
                240,
            };
            
            printf("\033[38;5;%dm%.2lx", Colors[ColorIndex], Byte);
        }
        
        printf("\033[0m%*s", AddressSpacing, "");
        
        // Finding instruction format in table
        ppc_instruction_encoding *InstEncoding = 0;
        for(u32 SetIndex = 0;
            SetIndex < ArrayCount(InstructionSet);
            ++SetIndex)
        {
            ppc_instruction_encoding *TestEncoding = InstructionSet + SetIndex;
            
            u32 ExtendedFormMask = INST_X_FORM | INST_XL_FORM | INST_XFX_FORM;
            
            if(TestEncoding->Opcode == Opcode &&
               (!(TestEncoding->Flags & ExtendedFormMask) ||
                TestEncoding->ExtendedOpcode == ExtendedOpcode))
            {
                InstEncoding = TestEncoding;
                break;
            }
        }
        
        if(InstEncoding)
        {
            char *Mnemonic = OperationNemonic[InstEncoding->Op];
            u32 MnemonicLength = printf("%s", Mnemonic);
            if(InstEncoding->Flags & INST_P) printf(".");
            
            printf("%*s", Maximum(MnemonicSpacing - MnemonicLength, 0), "");
            u32 LastPrinted = 0;
            for(u32 I = 0;
                I < ArrayCount(InstEncoding->Operands);
                ++I)
            {
                u16 Operand = InstEncoding->Operands[I];
                ppc_field_encoding Encoding = FieldEncodings[Operand];
                
                if(Operand)
                {
                    u32 Shift = 32 - Encoding.EndBit;
                    u32 Mask = ((u32)1 << (Encoding.EndBit - Encoding.StartBit)) - 1;
                    u32 SignMask = ((u32)1 << (Encoding.EndBit - Encoding.StartBit - 1));
                    
                    u32 OperandValue = (Instruction >> Shift) & Mask;
                    
                    if(I != 0) printf(",%*s", Maximum(OperandSpacing - LastPrinted, 0), "");
                    
                    if(Encoding.IsRegister)
                    {
                        LastPrinted = printf("r%d", OperandValue);
                    }
                    else
                    {
                        printf("\033[94m");
                        
                        if(Encoding.IsSigned)
                        {
                            u32 SignValue = OperandValue & SignMask ? ~Mask : 0;
                            u32 SOperandValue = (s32)OperandValue | SignValue;
                            
                            LastPrinted = printf("%d", SOperandValue);
                        }
                        else
                        {
                            LastPrinted = printf("%u", OperandValue);
                        }
                        printf("\033[0m");
                    }
                }
            }
        }
        
        printf("\n");
        Address += 4;
    }
}