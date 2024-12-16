
#ifndef EXTEND_MNEMONIC
#define EXTEND_MNEMONIC(ExtendedMnemonic, BaseInstruction, ...)
#endif

#define OPS0()                             0, {}
#define OPS1(Operand1)                     1, {Operand1}
#define OPS2(Operand1, Operand2)           2, {Operand1, Operand2}
#define OPS3(Operand1, Operand2, Operand3) 3, {Operand1, Operand2, Operand3}

#define Imm(Value) {ExtendedOperandFlags_Immediate, Value}
#define R(Value)   {ExtendedOperandFlags_RValue,    Value}


EXTEND_MNEMONIC(nop, ori, OPS0(),           OPS3(Imm(0), Imm(0), Imm(0)))
EXTEND_MNEMONIC(mr,  or,  OPS2(R(0), R(1)), OPS3(R(0), R(1), R(1)))

EXTEND_MNEMONIC(li,  addi,  OPS2(R(0), R(1)), OPS3(R(0), Imm(0), R(1)))


/*
Sign extended mnemonic
subic {Rx, Ry, value} -> addic {Rx, Ry, -value}

Size extended mnemonic
cmpd {0, Rx, Ry}      -> cmp {0, 1, Rx, Ry}

 Comparison extended mnemonic
tdlti {Rx, value}     -> tdi {16, Rx, value}

 Specific extended mnemonic
nop                   -> ori {0, 0, 0}
mr  {Rx, Ry}          -> or  {Rx, Ry, Ry}   
*/



#undef OPS0
#undef OPS1
#undef OPS2
#undef OPS3

#undef EXTEND_MNEMONIC