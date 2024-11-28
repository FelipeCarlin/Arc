
#ifndef FIELD
#define FIELD(Name, Start, End, Flags) {FD_##Name, Start, End, Flags},
#endif

#define F   FieldFlag_Field
#define R   FieldFlag_Register
#define I   FieldFlag_Immediate
#define A   FieldFlag_Address
#define SPR FieldFlag_SpecialPurposeRegister

#define S  FieldFlag_Signed
#define Sh FieldFlag_Shifted


//{OP_None, 0,  0,  0, 0}

FIELD(AA,  30, 31, A)

FIELD(BA,  11, 16, F)
FIELD(BB,  16, 21, F)

FIELD(BD,  16, 30, I|S)

FIELD(BF,  6,  9,  F)
FIELD(BFA, 11, 14, F)

FIELD(BH,  19, 21, I|S) // Special conditional branch hint

FIELD(BI,  11, 16, F)
FIELD(BO,  6,  11, I)   // Special contitional branch condition

FIELD(BT,  6,  11, F)

FIELD(D,   16, 32, I|S)
FIELD(DS,  16, 30, I|S|Sh)

FIELD(FRT, 6,  11, R)
FIELD(FRS, 6,  11, R)
FIELD(FRB, 16, 21, R)




FIELD(LI,  6,  30, A)

FIELD(LEV, 20, 27, R)

FIELD(RT,  6,  11, R)
FIELD(RS,  6,  11, R)
FIELD(L,   10, 11, I|S)
FIELD(TO,  6,  11, R)
FIELD(RA,  11, 16, R)

FIELD(SI,  16, 32, I|S)
FIELD(UI,  16, 32, I)


FIELD(TH,  7,  11, I|S)
FIELD(RB,  16, 21, R)
FIELD(NB,  16, 21, I|S)
FIELD(SH,  16, 21, I|S)
FIELD(U,   16, 20, I)
FIELD(Rc,  31, 32, I|S)

FIELD(MB,  21, 26, I|S)
FIELD(ME,  26, 31, I|S)

FIELD(SPR, 11, 21, SPR)


#undef FIELD

#undef F
#undef R
#undef I
#undef A

#undef S
