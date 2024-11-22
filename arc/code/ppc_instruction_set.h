#ifndef PPC_INSTRUCTION_SET_H

#include "ppc_encoding.h"

#define SIMPLE_INS(Opcode, Mnemonic, Form, ...) {Opcode, 0, Mnemonic, Form, __VA_ARGS__}
#define EXTENDED_INS(Opcode, ExtendedOpcode, Mnemonic, Form) {Opcode, ExtendedOpcode, Mnemonic, Form}

// TODO: Add extended mnemonics.


global_variable ppc_operand_encoding 
OperandEncodings[] = 
{
    {0,  0,  0, 0},
    
    {6,  30, 0, 1}, // LI
    {6,  11, 1, 0}, // BO
    {11, 16, 1, 0}, // BI
    {16, 30, 1, 0}, // BD
    
    {20, 27, 0, 0}, // LEV
    
    {6,  11, 1, 0}, // RT
    {6,  11, 1, 0}, // RS
    {6,  9,  1, 0}, // BF
    {10, 11, 0, 0}, // L
    {6,  11, 1, 0}, // TO
    {6,  11, 1, 0}, // FRT
    {6,  11, 1, 0}, // FRS
    {11, 16, 1, 0}, // RA
    
    {16, 32, 0, 1}, // D
    {16, 32, 0, 1}, // SI
    {16, 32, 0, 0}, // UI
};


global_variable ppc_instruction 
InstructionSet[] = 
{
    //{2, "tdi", D_Form}, // ppc64 // TODO: Add extended mnemonics.
    SIMPLE_INS(3,  "twi", D_Form, {OP_TO, OP_RA, OP_SI}),
    
    SIMPLE_INS(7,  "mulli",  D_Form, {OP_RT, OP_RA, OP_SI}),
    SIMPLE_INS(8,  "subfic", D_Form, {OP_RT, OP_RA, OP_SI}),
    
    SIMPLE_INS(10, "cmpli",  D_Form, {OP_BF, OP_L, OP_RA, OP_UI}),
    SIMPLE_INS(11, "cmpi",   D_Form, {OP_BF, OP_L, OP_RA, OP_SI}), // TODO: This one is signed.
    
    SIMPLE_INS(12, "addic",  D_Form, {OP_RT, OP_RA, OP_SI}),
    SIMPLE_INS(13, "addic.", D_Form, {OP_RT, OP_RA, OP_SI}),
    SIMPLE_INS(14, "addi",   D_Form, {OP_RT, OP_RA, OP_SI}),
    SIMPLE_INS(15, "addis",  D_Form, {OP_RT, OP_RA, OP_SI}),
    
    SIMPLE_INS(16, "bc",    B_Form,  {OP_BO, OP_BI, OP_BD}),
    SIMPLE_INS(17, "sc",    SC_Form, {OP_LEV}),
    SIMPLE_INS(18, "b",     I_Form,  {OP_LI}),
    
    EXTENDED_INS(19, 0, "mcrf", XL_Form),
    EXTENDED_INS(19, 16, "bclr", XL_Form),
    EXTENDED_INS(19, 18, "rfid", XL_Form),
    EXTENDED_INS(19, 33, "crnor", XL_Form),
    EXTENDED_INS(19, 129, "crandc", XL_Form),
    EXTENDED_INS(19, 150, "isync", XL_Form),
    EXTENDED_INS(19, 193, "crxor", XL_Form),
    EXTENDED_INS(19, 225, "crnand", XL_Form),
    EXTENDED_INS(19, 257, "crand", XL_Form),
    EXTENDED_INS(19, 274, "hrfid", XL_Form),
    EXTENDED_INS(19, 289, "creqv", XL_Form),
    EXTENDED_INS(19, 417, "crocc", XL_Form),
    EXTENDED_INS(19, 449, "cror", XL_Form),
    EXTENDED_INS(19, 528, "bcctr", XL_Form),
    
    SIMPLE_INS(24, "ori", D_Form, (0)),
    SIMPLE_INS(25, "oris", D_Form, (0)),
    SIMPLE_INS(26, "xor", D_Form, (0)),
    SIMPLE_INS(27, "xors", D_Form, (0)),
    SIMPLE_INS(28, "and.", D_Form, (0)),
    SIMPLE_INS(29, "ands.", D_Form, (0)),
    
    //{31, "or", X_Form, 444},
    
    SIMPLE_INS(32, "lwz", D_Form, (0)),
    SIMPLE_INS(33, "lwzu", D_Form, (0)),
    SIMPLE_INS(34, "lbz", D_Form, (0)),
    SIMPLE_INS(35, "lbzu", D_Form, (0)),
    
    SIMPLE_INS(36, "stw", D_Form, (0)),
    SIMPLE_INS(37, "stwu", D_Form, (0)),
    SIMPLE_INS(38, "stb", D_Form, (0)),
    SIMPLE_INS(39, "stbu", D_Form, (0)),
    
    SIMPLE_INS(40, "lhz", D_Form, (0)),
    SIMPLE_INS(41, "lhzu", D_Form, (0)),
    SIMPLE_INS(42, "lha", D_Form, (0)),
    SIMPLE_INS(43, "lhau", D_Form, (0)),
    
    SIMPLE_INS(44, "sth", D_Form, (0)),
    SIMPLE_INS(45, "sthu", D_Form, (0)),
    
    SIMPLE_INS(46, "lmw", D_Form, (0)),
    SIMPLE_INS(47, "stmw", D_Form, (0)),
};

#define PPC_INSTRUCTION_SET_H
#endif
