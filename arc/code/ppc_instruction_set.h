#ifndef PPC_INSTRUCTION_SET_H

#include "ppc_encoding.h"

//#define SIMPLE_INS(Opcode, Mnemonic, Form, ...) {Opcode, 0, Mnemonic, Form, __VA_ARGS__}
//#define EXTENDED_INS(Opcode, ExtendedOpcode, Mnemonic, Form, ...) {Opcode, ExtendedOpcode, Mnemonic, Form, __VA_ARGS__}

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


#define PPC_INSTRUCTION_SET_H
#endif
