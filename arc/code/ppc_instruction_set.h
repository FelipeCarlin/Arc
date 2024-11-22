#ifndef PPC_INSTRUCTION_SET_H

#include "ppc_encoding.h"

#define SIMPLE_INS(Opcode, Mnemonic, Form, ...) {Opcode, 0, Mnemonic, Form, __VA_ARGS__}
#define EXTENDED_INS(Opcode, ExtendedOpcode, Mnemonic, Form, ...) {Opcode, ExtendedOpcode, Mnemonic, Form, __VA_ARGS__}

// TODO: Add extended mnemonics.
// TODO: Hash table for Operands?

global_variable ppc_operand_encoding 
OperandEncodings[] = 
{
    {OP_None, 0,  0,  0, 0},
    
    {OP_LI,  6,  30, 0, 1}, // LI
    {OP_BO,  6,  11, 1, 0}, // BO
    {OP_BI,  11, 16, 1, 0}, // BI
    {OP_BD,  16, 30, 1, 0}, // BD
    
    {OP_LEV, 20, 27, 0, 0}, // LEV
    
    {OP_RT,  6,  11, 1, 0}, // RT
    {OP_RS,  6,  11, 1, 0}, // RS
    {OP_BF,  6,  9,  0, 0}, // BF
    {OP_L,   10, 11, 0, 0}, // L
    {OP_TO,  6,  11, 1, 0}, // TO
    {OP_FRT, 6,  11, 1, 0}, // FRT
    {OP_FRS, 6,  11, 1, 0}, // FRS
    {OP_RA,  11, 16, 1, 0}, // RA
    
    {OP_D,   16, 32, 0, 1}, // D
    {OP_SI,  16, 32, 0, 1}, // SI
    {OP_UI,  16, 32, 0, 0}, // UI
    
    {OP_DS,  16, 30, 0, 1}, // DS
    
    {OP_TH,  7,  11, 0, 0}, // TH
    {OP_BT,  6,  11, 0, 0}, // BT
    {OP_RB,  16, 21, 1, 1}, // RB
    {OP_NB,  16, 21, 0, 0}, // NB
    {OP_SH,  16, 21, 0, 1}, // SH
    {OP_FRB, 16, 21, 1, 0}, // FRB  // FPR
    {OP_U,   16, 20, 0, 0}, // U    // signed?
    {OP_Rc,  31, 32, 0, 0}, // Rc
    
    {OP_BA,  11, 16, 0, 0}, // BA
    {OP_BFA, 11, 14, 0, 0}, // BFA
    {OP_BB,  16, 21, 0, 0}, // BB
    {OP_BH,  19, 21, 0, 0}, // BH
};

// TODO: Hash map

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
    
    EXTENDED_INS(19, 0,   "mcrf", XL_Form, {OP_BF, OP_BFA}),
    EXTENDED_INS(19, 16,  "bclr", XL_Form, {OP_BO, OP_BI, OP_BH}),
    EXTENDED_INS(19, 18,  "rfid", XL_Form),
    EXTENDED_INS(19, 33,  "crnor", XL_Form, {OP_BT, OP_BA, OP_BB}),
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
    EXTENDED_INS(31, 444, "or", X_Form, {OP_RA, OP_RS, OP_RB}),
    
    SIMPLE_INS(32, "lwz", D_Form),
    SIMPLE_INS(33, "lwzu", D_Form),
    SIMPLE_INS(34, "lbz", D_Form),
    SIMPLE_INS(35, "lbzu", D_Form),
    
    SIMPLE_INS(36, "stw", D_Form),
    SIMPLE_INS(37, "stwu", D_Form),
    SIMPLE_INS(38, "stb", D_Form),
    SIMPLE_INS(39, "stbu", D_Form),
    
    SIMPLE_INS(40, "lhz", D_Form),
    SIMPLE_INS(41, "lhzu", D_Form),
    SIMPLE_INS(42, "lha", D_Form),
    SIMPLE_INS(43, "lhau", D_Form),
    
    SIMPLE_INS(44, "sth", D_Form),
    SIMPLE_INS(45, "sthu", D_Form),
    
    SIMPLE_INS(46, "lmw", D_Form),
    SIMPLE_INS(47, "stmw", D_Form),
};

#define PPC_INSTRUCTION_SET_H
#endif
