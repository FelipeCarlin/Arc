
#ifndef INST
#define INST(Opcode, ExtendedOpcode, Mnemonic, ...) {Opcode, ExtendedOpcode, Op_##Mnemonic, __VA_ARGS__},
#endif

#ifndef SIMPLE_INS
#define SIMPLE_INS(Opcode, Mnemonic, ...) INST(Opcode, 0, Mnemonic, __VA_ARGS__)
#endif

#ifndef EXTEND__INS
#define EXTEND_INS(Opcode, ExtendedOpcode, Mnemonic, ...) INST(Opcode, ExtendedOpcode, Mnemonic, __VA_ARGS__)
#endif


//{2, "tdi", D_Form}, // ppc64 // TODO: Add extended mnemonics.
SIMPLE_INS(3,  twi,    D_Form, {OP_TO, OP_RA, OP_SI})

SIMPLE_INS(7,  mulli,  D_Form, {OP_RT, OP_RA, OP_SI})
SIMPLE_INS(8,  subfic, D_Form, {OP_RT, OP_RA, OP_SI})

SIMPLE_INS(10, cmpli,  D_Form, {OP_BF, OP_L, OP_RA, OP_UI})
SIMPLE_INS(11, cmpi,   D_Form, {OP_BF, OP_L, OP_RA, OP_SI}) // TODO: This one is signed.

SIMPLE_INS(12, addic,  D_Form, {OP_RT, OP_RA, OP_SI})
//SIMPLE_INS(13, addic., D_Form, {OP_RT, OP_RA, OP_SI})
SIMPLE_INS(14, addi,   D_Form, {OP_RT, OP_RA, OP_SI})
SIMPLE_INS(15, addis,  D_Form, {OP_RT, OP_RA, OP_SI})

SIMPLE_INS(16, bc,    B_Form,  {OP_BO, OP_BI, OP_BD})
SIMPLE_INS(17, sc,    SC_Form, {OP_LEV})
SIMPLE_INS(18, b,     I_Form,  {OP_LI})

EXTEND_INS(19, 0,   mcrf, XL_Form, {OP_BF, OP_BFA})
EXTEND_INS(19, 16,  bclr, XL_Form, {OP_BO, OP_BI, OP_BH})
EXTEND_INS(19, 18,  rfid, XL_Form)
EXTEND_INS(19, 33,  crnor, XL_Form, {OP_BT, OP_BA, OP_BB})
EXTEND_INS(19, 129, crandc, XL_Form)
EXTEND_INS(19, 150, isync, XL_Form)
EXTEND_INS(19, 193, crxor, XL_Form)
EXTEND_INS(19, 225, crnand, XL_Form)
EXTEND_INS(19, 257, crand, XL_Form)
EXTEND_INS(19, 274, hrfid, XL_Form)
EXTEND_INS(19, 289, creqv, XL_Form)
EXTEND_INS(19, 417, crocc, XL_Form)
EXTEND_INS(19, 449, cror, XL_Form)
EXTEND_INS(19, 528, bcctr, XL_Form)

SIMPLE_INS(24, ori, D_Form, (0))
SIMPLE_INS(25, oris, D_Form, (0))
SIMPLE_INS(26, xor, D_Form, (0))
SIMPLE_INS(27, xors, D_Form, (0))
//SIMPLE_INS(28, and., D_Form, (0))
//SIMPLE_INS(29, ands., D_Form, (0))

EXTEND_INS(31, 444, or, X_Form, {OP_RA, OP_RS, OP_RB})

SIMPLE_INS(32, lwz, D_Form)
SIMPLE_INS(33, lwzu, D_Form)
SIMPLE_INS(34, lbz, D_Form)
SIMPLE_INS(35, lbzu, D_Form)

SIMPLE_INS(36, stw, D_Form)
SIMPLE_INS(37, stwu, D_Form)
SIMPLE_INS(38, stb, D_Form)
SIMPLE_INS(39, stbu, D_Form)

SIMPLE_INS(40, lhz, D_Form)
SIMPLE_INS(41, lhzu, D_Form)
SIMPLE_INS(42, lha, D_Form)
SIMPLE_INS(43, lhau, D_Form)

SIMPLE_INS(44, sth, D_Form)
SIMPLE_INS(45, sthu, D_Form)

SIMPLE_INS(46, lmw, D_Form)
SIMPLE_INS(47, stmw, D_Form)


#undef INST
#undef SIMPLE_INS
#undef EXTEND__INS