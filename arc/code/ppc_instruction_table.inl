
#ifndef INST
#define INST(Opcode, ExtendedOpcode, Mnemonic, ...) {Opcode, ExtendedOpcode, Op_##Mnemonic, __VA_ARGS__},
#endif

#ifndef INSTP
#define INSTP(Opcode, ExtendedOpcode, Mnemonic, ...) INST(Opcode, ExtendedOpcode, Mnemonic##p, __VA_ARGS__)
#endif

#ifndef SIMPLE_INS
#define SIMPLE_INS(Opcode, Mnemonic, ...) INST(Opcode, 0, Mnemonic, __VA_ARGS__)
#endif

#ifndef SIMPLE_INSP
#define SIMPLE_INSP(Opcode, Mnemonic, Flags, ...) INSTP(Opcode, 0, Mnemonic, Flags|INST_P, __VA_ARGS__)
#endif

#ifndef EXTEND__INS
#define EXTEND_INS(Opcode, ExtendedOpcode, Mnemonic, ...) INST(Opcode, ExtendedOpcode, Mnemonic, __VA_ARGS__)
#endif


#define D  INST_D_FORM
#define B  INST_B_FORM
#define SC INST_SC_FORM
#define I  INST_I_FORM
#define M  INST_M_FORM
#define X  INST_X_FORM
#define XL INST_XL_FORM
#define XO INST_XO_FORM
#define XFX INST_XFX_FORM

#define OE INST_OE
#define Rc INST_Rc


SIMPLE_INS(2,  tdi,    D, {FD_TO, FD_RA, FD_SI})
SIMPLE_INS(3,  twi,    D, {FD_TO, FD_RA, FD_SI})

SIMPLE_INS(7,  mulli,  D, {FD_RT, FD_RA, FD_SI})
SIMPLE_INS(8,  subfic, D, {FD_RT, FD_RA, FD_SI})

SIMPLE_INS(10, cmpli,  D, {FD_BF, FD_L, FD_RA, FD_UI})
SIMPLE_INS(11, cmpi,   D, {FD_BF, FD_L, FD_RA, FD_SI})

SIMPLE_INS(12, addic,  D, {FD_RT, FD_RA, FD_SI})
SIMPLE_INSP(13, addic, D, {FD_RT, FD_RA, FD_SI})
SIMPLE_INS(14, addi,   D, {FD_RT, FD_RA, FD_SI})
SIMPLE_INS(15, addis,  D, {FD_RT, FD_RA, FD_SI})

SIMPLE_INS(16, bc,     B, {FD_BO, FD_BI, FD_BD})
SIMPLE_INS(17, sc,     SC,{FD_LEV})
SIMPLE_INS(18, b,      I, {FD_LI})

EXTEND_INS(19, 0,   mcrf,  XL,  {FD_BF, FD_BFA})
EXTEND_INS(19, 16,  bclr,  XL,  {FD_BO, FD_BI, FD_BH})
EXTEND_INS(19, 18,  rfid,  XL)
EXTEND_INS(19, 33,  crnor, XL,  {FD_BT, FD_BA, FD_BB})
EXTEND_INS(19, 129, crandc, XL, {FD_BT, FD_BA, FD_BB})
EXTEND_INS(19, 150, isync, XL)
EXTEND_INS(19, 193, crxor, XL,  {FD_BT, FD_BA, FD_BB})
EXTEND_INS(19, 225, crnand, XL, {FD_BT, FD_BA, FD_BB})
EXTEND_INS(19, 257, crand, XL,  {FD_BT, FD_BA, FD_BB})
EXTEND_INS(19, 274, hrfid, XL)
EXTEND_INS(19, 289, creqv, XL,  {FD_BT, FD_BA, FD_BB})
EXTEND_INS(19, 417, crorc, XL,  {FD_BT, FD_BA, FD_BB})
EXTEND_INS(19, 449, cror,  XL,  {FD_BT, FD_BA, FD_BB})
EXTEND_INS(19, 528, bcctr, XL,  {FD_BO, FD_BI, FD_BH})

SIMPLE_INS(20, rlwimi, M, {FD_RA, FD_RS, FD_SH, FD_MB})
SIMPLE_INS(21, rlwinm, M, {FD_RA, FD_RS, FD_SH, FD_MB, FD_ME})
SIMPLE_INS(23, rlwnm,  M, {FD_RA, FD_RS, FD_RB, FD_MB, FD_ME})

SIMPLE_INS(24, ori,  D, {FD_RS, FD_RA, FD_UI})
SIMPLE_INS(25, oris, D)
SIMPLE_INS(26, xor,  D)
SIMPLE_INS(27, xors, D)
SIMPLE_INSP(28, andi,  D, {FD_RA, FD_RS, FD_UI})
SIMPLE_INSP(29, andis, D)

EXTEND_INS(31, 0,   cmp, X, {FD_BF, FD_L, FD_RA, FD_RB})
EXTEND_INS(31, 4,    tw, X, {FD_TO, FD_RA, FD_RB})

EXTEND_INS(31,  8,  subfc, XO,  {FD_RT, FD_RA, FD_RB})
EXTEND_INS(31,  9, mulhdu, XO,  {FD_RT, FD_RA, FD_RB})
EXTEND_INS(31, 10,   addc, XO,  {FD_RT, FD_RA, FD_RB})
EXTEND_INS(31, 11, mulhwu, XO,  {FD_RT, FD_RA, FD_RB})

EXTEND_INS(31, 19,   mfcr, XFX, {FD_RT})

EXTEND_INS(31, 20,  lwarx, X)   // No idea how its encoded
EXTEND_INS(31, 21,    ldx, X,   {FD_RT, FD_RA, FD_RB})
EXTEND_INS(31, 23,   ldzx, X,   {FD_RT, FD_RA, FD_RB})
EXTEND_INS(31, 24,    slw, X,   {FD_RA, FD_RS, FD_RB})

EXTEND_INS(31, 26, cntlzw, X,   {FD_RA, FD_RS})
EXTEND_INS(31, 27,    sld, X,   {FD_RA, FD_RS, FD_RB})
EXTEND_INS(31, 28,    and, X,   {FD_RA, FD_RS, FD_RB})
EXTEND_INS(31, 32,   cmpl, X,   {FD_BF, FD_L,  FD_RA, FD_RB})

EXTEND_INS(31, 40,   subf, XO,  {FD_RT, FD_RA, FD_RB})

EXTEND_INS(31, 53,   ldux, X,   {FD_RT, FD_RA, FD_RB})
EXTEND_INS(31, 54,  dcbst, X)   // Book II
EXTEND_INS(31, 55,  lwzux, X,   {FD_RT, FD_RA, FD_RB})
EXTEND_INS(31, 58, cntlzd, X,   {FD_RA, FD_RS})
EXTEND_INS(31, 60,   andc, X,   {FD_RA, FD_RS, FD_RB})

//
//

EXTEND_INS(31, 266,   add, XO|OE|Rc,  {FD_RT, FD_RA, FD_RB})

//
//

EXTEND_INS(31, 339, mfspr, XFX, {FD_RT, FD_SPR})
EXTEND_INS(31, 444, or,    X,   {FD_RA, FD_RS, FD_RB})
EXTEND_INS(31, 467, mtspr, XFX, {FD_SPR, FD_RT})

SIMPLE_INS(32, lwz,  D, {FD_RT, FD_D, FD_RA})
SIMPLE_INS(33, lwzu, D)
SIMPLE_INS(34, lbz,  D)
SIMPLE_INS(35, lbzu, D)

SIMPLE_INS(36, stw,  D)
SIMPLE_INS(37, stwu, D)
SIMPLE_INS(38, stb,  D)
SIMPLE_INS(39, stbu, D)

SIMPLE_INS(40, lhz,  D)
SIMPLE_INS(41, lhzu, D)
SIMPLE_INS(42, lha,  D)
SIMPLE_INS(43, lhau, D)

SIMPLE_INS(44, sth,  D)
SIMPLE_INS(45, sthu, D)

SIMPLE_INS(46, lmw,  D)
SIMPLE_INS(47, stmw, D)


#undef INST
#undef INSTP
#undef SIMPLE_INS
#undef SIMPLE_INSP
#undef EXTEND__INS


#undef D
#undef B
#undef SC
#undef I
#undef M
#undef X
#undef XL
#undef XO
#undef XFX


#undef OE
#undef Rc
