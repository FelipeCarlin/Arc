#ifndef PPC_ENCODING_H

enum ppc_instruction_form
{
    None_Form,
    
    I_Form,
    B_Form,
    SC_Form,
    
    D_Form,
    X_Form,
    
    XL_Form,
};

typedef enum ppc_instruction_operand_field
{
    OP_None = 0,
    
    OP_LI,
    OP_BO,
    OP_BI,
    OP_BD,
    
    OP_LEV,
    
    OP_RT,
    OP_RS,
    OP_BF,
    OP_L,
    OP_TO,
    OP_FRT,
    OP_FRS,
    OP_RA,
    
    OP_D,
    OP_SI,
    OP_UI,
    
    OP_DS,
    
    OP_TH,
    OP_BT,
    OP_RB,
    OP_NB,
    OP_SH,
    OP_FRB,
    OP_U,
    OP_Rc,
    
    OP_BA,
    OP_BFA,
    OP_BB,
    OP_BH,
} ppc_instruction_operand_field;

typedef struct ppc_operand_encoding
{
    // In powerpc fashion, this is msb first.
    u8 Field;
    
    u8 StartBit;
    u8 EndBit;
    
    u8 IsRegister;
    u8 IsSigned;
} ppc_operand_encoding;

typedef enum ppc_operation_type
{
    Op_None,
    
#define INST(Opcode, ExtendedOpcode, Mnemonic, ...) Op_##Mnemonic,
#include "ppc_instruction_table.inl"
#undef INST
    
    Op_Count,
} ppc_operation_type;

typedef struct ppc_instruction_encoding
{
    u16 Opcode;
    u16 ExtendedOpcode;
    
    ppc_operation_type Op;
    
    //char Mnemonic[8];
    
    u16 Form;
    u16 Operands[4];
    
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
#include "ppc_instruction_table.inl"
#undef INST
};

//
// Instruction Formats
//


typedef union i_format_data
{
    struct
    {
        u32 LK : 1;
        u32 AA : 1;
        u32 LI : 24;
        u32 Opcode : 6;
    };
} i_format_data;

typedef union b_format_data
{
    struct
    {
        u32 LK : 1;
        u32 AA : 1;
        u32 BD : 14;
        u32 BI : 5;
        u32 BO : 5;
        u32 Opcode : 6;
    };
} b_format_data;

typedef union sc_format_data
{
    struct
    {
        u32    : 1; // /
        u32 ONE: 1;
        u32    : 3; // /
        u32 LEV: 7;
        u32    : 4; // /
        u32    : 5; // /
        u32    : 5; // /
        u32 Opcode : 6;
    };
} sc_format_data;

typedef union d_format_data
{
    struct
    {
        u32 D  : 16;
        u32 RA : 5;
        u32 RT : 5;
        u32 Opcode : 6;
    };
    
    struct
    {
        s32 SI : 16;
        u32    : 5;
        u32 RS : 5;
        u32    : 6;
    };
    
    struct
    {
        u32 UI : 16;
        u32    : 5;
        u32 L  : 1;
        u32    : 1; // /
        u32 BF : 3;
        u32    : 6;
    };
    
    struct
    {
        u32    : 16;
        u32    : 5;
        u32 TO : 5;
        u32    : 6;
    };
    
    struct
    {
        u32    : 16;
        u32    : 5;
        u32 FRT: 5;
        u32    : 6;
    };
    
    struct
    {
        u32    : 16;
        u32    : 5;
        u32 FRS: 5;
        u32    : 6;
    };
} d_format_data;

typedef union ds_format_data
{
    struct
    {
        u32 XO : 2;
        u32 DS : 14;
        u32 RA : 5;
        u32 RT : 5;
        u32 Opcode : 6;
    };
    
    struct
    {
        u32    : 2;
        u32    : 14;
        u32    : 5;
        u32 RS : 5;
        u32    : 6;
    };
} ds_format_data;

typedef union x_format_data
{
    struct
    {
        u32    : 1;   // /
        u32 XO : 10;
        u32 RB : 5;
        u32 RA : 5;
        u32 RT : 5;
        u32 Opcode : 6;
    };
    
    // TODO
} x_format_data;

typedef union xl_format_data
{
    struct
    {
        u32    : 1;   // /
        u32 XO : 10;
        u32 BB : 5;
        u32 BA : 5;
        u32 BT : 5;
        u32 Opcode : 6;
    };
    
    struct
    {
        u32 LK : 1;   // LK
        u32    : 10;  // XO
        u32 BH : 2;
        u32    : 3;   // /
        u32 BI : 5;
        u32 BO : 5;
        u32    : 6;   // Opcode
    };
    
    struct
    {
        u32    : 1;   // /
        u32    : 10;  // XO
        u32    : 5;   // /
        u32    : 2;   // /
        u32 BFA: 3;
        u32 BF : 5;
        u32    : 6;   // Opcode
    };
} xl_format_data;

#define PPC_ENCODING_H
#endif
