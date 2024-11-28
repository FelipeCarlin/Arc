#ifndef ARC_H

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef u32 b32;

#define internal static
#define local_variable static
#define global_variable static

#define true 1
#define false 0

#define ArrayCount(A) (sizeof(A) / sizeof((A)[0]))

#define Assert(X) if(!(X)) {*(int *)0 = 0;}

#define Minimum(a, b) a < b ? a : b
#define Maximum(a, b) a < b ? b : a

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

typedef struct loaded_file
{
    char *Filename;
    
    void *Memory;
    u64 Size;
} loaded_file;


inline u32
SafeTruncateUInt64(u64 Value)
{
    // TODO(felipe): Defines for maximum values.
    Assert(Value <= 0xFFFFFFFF);
    u32 Result = (u32)Value;
    return Result;
}


#define ARC_H
#endif
