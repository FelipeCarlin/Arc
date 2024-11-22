
#ifndef FIELD
#define FIELD(Name, Start, End, IsRegister, IsSigned) {FD_##Name, Start, End, IsRegister, IsSigned},
#endif


//{OP_None, 0,  0,  0, 0}

FIELD(LI,  6,  30, 0, 1)
FIELD(BO,  6,  11, 1, 0)

FIELD(BI,  11, 16, 1, 0)
FIELD(BD,  16, 30, 0, 1)

FIELD(LEV, 20, 27, 0, 0)

FIELD(RT,  6,  11, 1, 0)
FIELD(RS,  6,  11, 1, 0)
FIELD(BF,  6,  9,  0, 0)
FIELD(L,   10, 11, 0, 0)
FIELD(TO,  6,  11, 1, 0)
FIELD(FRT, 6,  11, 1, 0)
FIELD(FRS, 6,  11, 1, 0)
FIELD(RA,  11, 16, 1, 0)

FIELD(D,   16, 32, 0, 1)
FIELD(SI,  16, 32, 0, 1)
FIELD(UI,  16, 32, 0, 0)

FIELD(DS,  16, 30, 0, 1)

FIELD(TH,  7,  11, 0, 0)
FIELD(BT,  6,  11, 0, 0)
FIELD(RB,  16, 21, 1, 1)
FIELD(NB,  16, 21, 0, 0)
FIELD(SH,  16, 21, 0, 1)
FIELD(FRB, 16, 21, 1, 0)
FIELD(U,   16, 20, 0, 0)
FIELD(Rc,  31, 32, 0, 0)

FIELD(BA,  11, 16, 0, 0)
FIELD(BFA, 11, 14, 0, 0)
FIELD(BB,  16, 21, 0, 0)
FIELD(BH,  19, 21, 0, 0)

FIELD(MB,  21, 26, 0, 0)
FIELD(ME,  26, 31, 0, 0)

FIELD(SPR, 11, 21, 0, 0)


#undef FIELD