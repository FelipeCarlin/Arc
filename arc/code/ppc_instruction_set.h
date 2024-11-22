#ifndef PPC_ENCODING_H

typedef enum ppc_field_type
{
    FD_None,
    
#define FIELD(Name, ...) FD_##Name,
#include "ppc_field_table.inl"
    
} ppc_field_type;

typedef struct ppc_field_encoding
{
    // In powerpc fashion, this is msb first.
    u8 Field;
    
    u8 StartBit;
    u8 EndBit;
    
    u8 IsRegister;
    u8 IsSigned;
} ppc_field_encoding;

global_variable ppc_field_encoding
FieldEncodings[] = 
{
    {FD_None},
    
#include "ppc_field_table.inl"
    
};

typedef enum ppc_operation_type
{
    Op_None,
    
#define INST(Opcode, ExtendedOpcode, Mnemonic, ...) Op_##Mnemonic,
#include "ppc_instruction_table.inl"
    
    Op_Count,
} ppc_operation_type;

typedef enum ppc_instruction_flags
{
    INST_D_FORM   = (1 << 0),
    INST_B_FORM   = (1 << 1),
    INST_SC_FORM  = (1 << 2),
    INST_I_FORM   = (1 << 3),
    INST_M_FORM   = (1 << 4),
    INST_X_FORM   = (1 << 5),
    INST_XL_FORM  = (1 << 6),
    INST_XFX_FORM = (1 << 7),
    
    INST_P        = (1 << 8),
} ppc_instruction_flags;

typedef struct ppc_instruction_encoding
{
    u16 Opcode;
    u16 ExtendedOpcode;
    
    ppc_operation_type Op;
    
    //u16 Form;
    ppc_instruction_flags Flags;
    
    u16 Operands[5];
    
    void (*CustomDecoding)(void *FormatData);
} ppc_instruction_encoding;

global_variable ppc_instruction_encoding
InstructionSet[] = 
{
    
#include "ppc_instruction_table.inl"
    
};

global_variable char *
OperationNemonic[] =
{
    "None",
    
#define INST(Opcode, ExtendedOpcode, Mnemonic, ...) #Mnemonic,
#define INSTP(Opcode, ExtendedOpcode, Mnemonic, ...) #Mnemonic,
#include "ppc_instruction_table.inl"
    
};

#define PPC_ENCODING_H
#endif
