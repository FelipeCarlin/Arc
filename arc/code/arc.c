
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

typedef enum operand_type
{
    OperandType_None,
    OperandType_Field,
    OperandType_Register,
    OperandType_Immediate,
    OperandType_Address,
    OperandType_SpecialPurposeRegister,
} operand_type;

typedef struct operand
{
    operand_type Type;
    s32 Value;
    
    b32 IsSigned;
} operand;

enum instruction_flags
{
    InstructionFlag_P = (1 << 0),
    InstructionFlag_O = (1 << 1),
};

typedef struct instruction
{
    u16 Mnemonic;
    char MnemonicName[16];
    b32 Flags;
    
    operand Operands[5];
} instruction;

inline u32
StringCopy(char *A, char *B)
{
    u32 Result = 0;
    
    while(*B)
    {
        *A++ = *B++;
        ++Result;
    }
    
    return Result;
}

typedef enum extended_operand_flags
{
    ExtendedOperandFlags_Immediate = 1,
    ExtendedOperandFlags_RValue,
} extended_operand_flags;

typedef struct extended_operand
{
    u16 Flags;
    s16 Value;
} extended_operand;

typedef struct extended_mnemonic
{
    u16 Op;
    u16 BaseOp;
    
    u32 OperandCount;
    extended_operand Operands[5];
    
    u32 BaseOperandCount;
    extended_operand BaseOperands[5];
} extended_mnemonic;

global_variable extended_mnemonic
ExtendedMnemonics[] = 
{
#define EXTEND_MNEMONIC(ExtendedMnemonic, BaseInstruction, ...) {Op_##ExtendedMnemonic, Op_##BaseInstruction, __VA_ARGS__},
    
#include "ppc_extended_mnemonic_table.inl"
};

int main(int Argc, char **Argv)
{
    loaded_file File = Win32ReadEntireFile("simple.bin");
    PrintHexDump((u8 *)File.Memory, (u32)File.Size);
    
    // TODO:
    // + Decouple instruction decoding from printing.
    // - Support extended mnemonics (ie.  mr ra, rb  ->  or ra, rb, rb).
    // - Support object files.
    // - Arrows for branches and jumps.
    // - Hashmaps!
    
    for(u32 I = 0;
        I < ArrayCount(ExtendedMnemonics);
        ++I)
    {
        extended_mnemonic Mnem = ExtendedMnemonics[I];
        printf("%s -> %s\n", OperationMnemonic[Mnem.Op], OperationMnemonic[Mnem.BaseOp]);
    }
    
    u32 InstructionCount = (u32)(File.Size/4);
    u64 Address = 0x80003000;
    
    u32 AddressSpacing = 5;
    u32 BytesSpacing = 5;
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
        u16 ExtendedOpcode = (Instruction >> (32 - 31)) & 0b111111111;
        
        // Printing
        printf("\033[2m0x%.8llx\033[0m", Address);
        printf("%*s", AddressSpacing, "");
        
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
        
        printf("\033[0m%*s", BytesSpacing, "");
        
        // Finding instruction format in table
        // TODO: Replace with hash map or something.
        ppc_instruction_encoding *InstEncoding = 0;
        for(u32 SetIndex = 0;
            SetIndex < ArrayCount(InstructionSet);
            ++SetIndex)
        {
            ppc_instruction_encoding *TestEncoding = InstructionSet + SetIndex;
            
            u32 ExtendedFormMask = INST_X_FORM | INST_XL_FORM | INST_XO_FORM | INST_XFX_FORM;
            
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
            instruction Inst = {};
            Inst.Mnemonic = InstEncoding->Op;
            
            if(InstEncoding->Flags & INST_P)
            {
                Inst.Flags |= InstructionFlag_P;
                //Inst.MnemonicName[MnemonicLength++] = '.';
            }
            
            if(InstEncoding->Flags & INST_Rc &&
               Instruction & 0x01)
            {
                Inst.Flags |= InstructionFlag_P;
            }
            
            if(InstEncoding->Flags & INST_OE &&
               Instruction & 0x400)
            {
                Inst.Flags |= InstructionFlag_O;
            }
            
            for(u32 I = 0;
                I < ArrayCount(InstEncoding->Operands);
                ++I)
            {
                u16 Operand = InstEncoding->Operands[I];
                ppc_field_encoding Encoding = FieldEncodings[Operand];
                
                operand *Op = Inst.Operands + I;
                
                if(Operand)
                {
                    u32 Shift = 32 - Encoding.EndBit;
                    u32 Mask = ((u32)1 << (Encoding.EndBit - Encoding.StartBit)) - 1;
                    u32 SignMask = ((u32)1 << (Encoding.EndBit - Encoding.StartBit - 1));
                    
                    u32 OperandValue = (Instruction >> Shift) & Mask;
                    
                    Op->Type |= Encoding.Flags & FieldFlag_Field     ? OperandType_Field : 0;
                    Op->Type |= Encoding.Flags & FieldFlag_Register  ? OperandType_Register : 0;
                    Op->Type |= Encoding.Flags & FieldFlag_Immediate ? OperandType_Immediate : 0;
                    Op->Type |= Encoding.Flags & FieldFlag_Address   ? OperandType_Address : 0;
                    Op->Type |= Encoding.Flags & FieldFlag_SpecialPurposeRegister   ? OperandType_SpecialPurposeRegister : 0;
                    
                    Op->Value = OperandValue;
                    
                    if(Encoding.Flags & FieldFlag_Signed)
                    {
                        u32 SignValue = OperandValue & SignMask ? ~Mask : 0;
                        Op->Value = (s32)OperandValue | SignValue;
                    }
                    
                    if(Encoding.Flags & FieldFlag_Shifted)
                    {
                        Op->Value <<= 2;
                    }
                }
            }
            
            // Extended mnemonics
            extended_mnemonic *ExtendedMnem = 0;
            for(u32 I = 0;
                I < ArrayCount(ExtendedMnemonics);
                ++I)
            {
                extended_mnemonic *Mnem = ExtendedMnemonics + I;
                
                if(Inst.Mnemonic == Mnem->BaseOp)
                {
                    b32 MatchOperands = true;
                    
                    b32 RValuesSet[8] = {};
                    s32 RValues[8] = {};
                    for(u32 I = 0;
                        I < Mnem->BaseOperandCount;
                        ++I)
                    {
                        if(Mnem->BaseOperands[I].Flags & ExtendedOperandFlags_RValue)
                        {
                            u32 RIndex = Mnem->BaseOperands[I].Value;
                            if(RValuesSet[RIndex])
                            {
                                if(RValues[RIndex] != Inst.Operands[I].Value)
                                {
                                    MatchOperands = false;
                                    break;
                                }
                            }
                            else
                            {
                                RValues[RIndex] = Inst.Operands[I].Value;
                                RValuesSet[RIndex] = true;
                            }
                        }
                        else if(Mnem->BaseOperands[I].Flags & ExtendedOperandFlags_Immediate)
                        {
                            if(Inst.Operands[I].Value != Mnem->BaseOperands[I].Value)
                            {
                                MatchOperands = false;
                                break;
                            }
                        }
                    }
                    
                    if(MatchOperands)
                    {
                        ExtendedMnem = Mnem;
                        break;
                    }
                }
            }
            
            if(ExtendedMnem)
            {
                // Change is only "cosmetic", meaning that the underlaying operation 
                // remains the original.
                ppc_instruction_encoding *TargetInst = 0;
                for(u32 I = 0;
                    I < ArrayCount(InstructionSet);
                    ++I)
                {
                    ppc_instruction_encoding *TestInstruction = InstructionSet + I;
                    if(TestInstruction->Op == ExtendedMnem->BaseOp)
                    {
                        TargetInst = TestInstruction;
                        break;
                    }
                }
                
                Assert(TargetInst);
                
                Inst.Mnemonic = ExtendedMnem->Op;
                /*
                for(u32 I = 0;
                    I < ArrayCount(Inst.MnemonicName);
                    ++I)
                {
                    Inst.MnemonicName[I] = 0;
                }
                StringCopy(Inst.MnemonicName, OperationMnemonic[ExtendedMnem->Op]);
                */
                
                u32 RValues[8] = {};
                
                operand ZeroOperand = {};
                for(u32 I = 0;
                    I < ArrayCount(Inst.Operands);
                    ++I)
                {
                    if(ExtendedMnem->BaseOperands[I].Flags & ExtendedOperandFlags_RValue)
                    {
                        RValues[ExtendedMnem->BaseOperands[I].Value] = Inst.Operands[I].Value;
                    }
                    Inst.Operands[I] = ZeroOperand;
                }
                
                for(u32 I = 0;
                    I < ArrayCount(Inst.Operands);
                    ++I)
                {
                    if(ExtendedMnem->Operands[I].Flags & ExtendedOperandFlags_RValue)
                    {
                        u16 Operand = TargetInst->Operands[I];
                        ppc_field_encoding Encoding = FieldEncodings[Operand];
                        
                        u16 Type = 0;
                        Type |= Encoding.Flags & FieldFlag_Field     ? OperandType_Field : 0;
                        Type |= Encoding.Flags & FieldFlag_Register  ? OperandType_Register : 0;
                        Type |= Encoding.Flags & FieldFlag_Immediate ? OperandType_Immediate : 0;
                        Type |= Encoding.Flags & FieldFlag_Address   ? OperandType_Address : 0;
                        Type |= Encoding.Flags & FieldFlag_SpecialPurposeRegister   ? OperandType_SpecialPurposeRegister : 0;
                        
                        Inst.Operands[I].Type = Type;
                        Inst.Operands[I].Value = RValues[ExtendedMnem->Operands[I].Value];
                    }
                    
                    //Inst.Operands[I] = ;
                }
            }
            
            // Build string mnemonic
            u32 MnemonicLength = StringCopy(Inst.MnemonicName, OperationMnemonic[Inst.Mnemonic]);
            if(Inst.Flags & InstructionFlag_O)
            {
                Inst.MnemonicName[MnemonicLength++] = 'o';
            }
            if(Inst.Flags & InstructionFlag_P)
            {
                Inst.MnemonicName[MnemonicLength++] = '.';
            }
            
            
            // Printing
            //char *MnemonicName = OperationNemonic[Inst.Mnemonic];
            //u32 MnemonicLength = printf("%s", MnemonicName);
            MnemonicLength = printf("%s", Inst.MnemonicName);
            
            //if(InstEncoding->Flags & INST_P) printf(".");
            
            printf("%*s", Maximum(MnemonicSpacing - MnemonicLength, 0), "");
            u32 LastPrinted = 0;
            for(u32 I = 0;
                I < ArrayCount(Inst.Operands);
                ++I)
            {
                operand Op = Inst.Operands[I];
                
                if(!Op.Type)
                {
                    break;
                }
                
                if(I != 0) printf("\033[2m,\033[0m%*s", Maximum(OperandSpacing - LastPrinted, 0), "");
                
                switch(Op.Type)
                {
                    case OperandType_Field:
                    {
                        LastPrinted = printf("%d", Op.Value);
                    }break;
                    
                    case OperandType_Register:
                    {
                        printf("\033[2m");
                        LastPrinted = printf("r%d", Op.Value);
                    }break;
                    
                    case OperandType_Immediate:
                    {
                        printf("\033[94m");
                        LastPrinted = printf("%d", Op.Value);
                    }break;
                    
                    case OperandType_Address:
                    {
                        printf("\033[34m");
                        LastPrinted = printf("0x%llx", Address + Op.Value);
                    }break;
                    
                    case OperandType_SpecialPurposeRegister:
                    {
                        LastPrinted = printf("mlr");
                    }break;
                    
                    default:
                    {
                    } break;
                }
                printf("\033[0m");
            }
        }
        
        printf("\n");
        Address += 4;
    }
}