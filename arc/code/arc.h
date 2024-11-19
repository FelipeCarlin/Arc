#ifndef ARC_H

#include <stdio.h>
#include <windows.h>

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


#define ARC_H
#endif
