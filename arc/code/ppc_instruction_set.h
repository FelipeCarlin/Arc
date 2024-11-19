#ifndef PPC_INSTRUCTION_SET_H

#include "ppc_encoding.h"

global_variable ppc_instruction 
InstructionSet[] = 
{
    //{2, "tdi", D_Form}, // ppc64 // TODO: Add extended mnemonics.
    //{3, "twi", D_Form, 0, TrapDecoder}, // TODO: Add extended mnemonics.
    {3, "twi", D_Form}, // TODO: Add extended mnemonics.
    
    {7, "mulli", D_Form},
    {8, "subfic", D_Form},
    
    //{10, "cmpli", D_Form, 0, CmpliDecoder},
    //{11, "cmpi", D_Form, 0, CmpliDecoder}, // TODO: This one is signed.
    {10, "cmpli", D_Form},
    {11, "cmpi", D_Form}, // TODO: This one is signed.
    
    {12, "addic", D_Form},
    {13, "addic.", D_Form},
    {14, "addi", D_Form},
    {15, "addis", D_Form},
    
    {16, "bc", B_Form},
    {17, "sc", SC_Form},
    {18, "b", I_Form},
    
    {19, "mcrf", XL_Form, 0},
    {19, "bclr", XL_Form, 16},
    {19, "rfid", XL_Form, 18},
    {19, "crnor", XL_Form, 33},
    {19, "crandc", XL_Form, 129},
    {19, "isync", XL_Form, 150},
    {19, "crxor", XL_Form, 193},
    {19, "crnand", XL_Form, 225},
    {19, "crand", XL_Form, 257},
    {19, "hrfid", XL_Form, 274},
    {19, "creqv", XL_Form, 289},
    {19, "crocc", XL_Form, 417},
    {19, "cror", XL_Form, 449},
    {19, "bcctr", XL_Form, 528},
    
    {24, "ori", D_Form},
    {25, "oris", D_Form},
    {26, "xor", D_Form},
    {27, "xors", D_Form},
    {28, "and.", D_Form},
    {29, "ands.", D_Form},
    
    //{31, "or", X_Form, 444},
    
    {32, "lwz", D_Form},
    {33, "lwzu", D_Form},
    {34, "lbz", D_Form},
    {35, "lbzu", D_Form},
    
    {36, "stw", D_Form},
    {37, "stwu", D_Form},
    {38, "stb", D_Form},
    {39, "stbu", D_Form},
    
    {40, "lhz", D_Form},
    {41, "lhzu", D_Form},
    {42, "lha", D_Form},
    {43, "lhau", D_Form},
    
    {44, "sth", D_Form},
    {45, "sthu", D_Form},
    
    {46, "lmw", D_Form},
    {47, "stmw", D_Form},
};

#define PPC_INSTRUCTION_SET_H
#endif
