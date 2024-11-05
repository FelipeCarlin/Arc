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

#define Assert(X) if(!(X)) {*(int *)0 = 0;}

#define Minimum(a, b) a < b ? a : b
#define Maximum(a, b) a < b ? b : a


void 
PrintHexDump(u8 *Data, u32 Length)
{
    u32 Rows = (Length + 15) / 16;
    u64 Address = 0;
    for(u32 I = 0;
        I < Rows;
        ++I)
    {
        printf("0x%.8llx  ", Address);
        
        // Value column
        u32 RowLength = Minimum(16, Length - I*16);
        for(u32 J = 0;
            J < RowLength;
            ++J)
        {
            if(~J & 0b1)
            {
                printf(" ");
            }
            
            u32 Index = I*16 + J;
            u8 Value = Data[Index];
            printf("%.2hhx", Value);
        }
        
        // Spacing after value column
        u32 BlankWidth = (16 - RowLength)*2 + (16 - RowLength)/2;
        printf("%*s", BlankWidth, "");
        printf("    ");
        
        // Ascii column
        for(u32 J = 0;
            J < RowLength;
            ++J)
        {
            u32 Index = I*16 + J;
            u8 Value = Data[Index];
            
            char Char = '.';
            if(Value >= 32 && Value <= 126)
            {
                Char = Value;
            }
            
            printf("%c", Char);
        }
        
        printf("\n");
    }
}

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

internal loaded_file
Win32ReadEntireFile(char *Filename)
{
    loaded_file Result = {0};
    
    b32 Failed = false;
    
    HANDLE FileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if(FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize;
        if(GetFileSizeEx(FileHandle, &FileSize))
        {
            u32 FileSize32 = SafeTruncateUInt64(FileSize.QuadPart);
            Result.Memory = VirtualAlloc(0, FileSize32, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
            if(Result.Memory)
            {
                DWORD BytesRead;
                if(ReadFile(FileHandle, Result.Memory, FileSize32, &BytesRead, 0) &&
                   (FileSize32 == BytesRead))
                {
                    // NOTE(felipe): File read succesfully.
                    Result.Filename = Filename;
                    Result.Size = FileSize32;
                }
                else
                {
                    VirtualFree(Result.Memory, 0, MEM_RELEASE);
                    Result.Memory = 0;
                }
            }
        }
        
        CloseHandle(FileHandle);
    }
    
    return Result;
}

int main(int Argc, char **Argv)
{
    u8 Bytes[] = 
    {
        0x3A, 0x1F, 0xB2, 0x4D, 0x7E, 0x9A, 0xE5, 0x8C,
        0x5B, 0x39, 0x6D, 0xA1, 0xC3, 0x2F, 0x84, 0x77,
        0x9F, 0x42, 0xDA, 0x60, 0xB8, 0x1A, 0x53, 0xE9,
        0x66, 0x30, 0xAF, 0x7C, 0xB4, 0x8E, 0x5F, 0xD2,
        0x7B, 0xC1, 0xEA, 0x99, 0x4C, 0x12, 0xA7, 0x58,
        0x83, 0x25, 0x91, 0x6F, 0xDE, 0x3B, 0x7A, 0x54,
        0xCD, 0xA3, 0x1C, 0xF9, 0x72, 0x34, 0xBE, 0x8F,
        0x4D, 0x0B, 0x9E, 0x61, 0xAF, 0x29, 0xB5, 0xD7,
        0x19, 0x64, 0x82, 0xF4, 0x58, 0xAC, 0xE7, 0x3C,
        0x2A, 0xD8, 0x90, 0x6B, 0xF2, 0x35, 0xA8, 0x4E,
        0xC7, 0x1D, 0x9B, 0x74, 0x56, 0xE3, 0xBC, 0x08,
        0xFA, 0x41, 0x93, 0x2E, 0xD1, 0x6C, 0xA5, 0xB0,
        0x37, 0xFF, 0x80, 0x5D, 0x28, 0x9C, 0xEC, 0x72,
        0x11, 0xB6, 0x5A, 0x8D, 0xC2, 0x04, 0xE8, 0x69,
        0xB9, 0x30, //0x8A, 0x53, // 0xDD, 0x7F, 0x44, 0xF5,
        //0x67, 0x22, 0x98, 0xCA, //0x4B, 0x2F, 0x7D, 0xAE,
    };
    u32 Length = sizeof(Bytes);
    
    loaded_file File = Win32ReadEntireFile("simple.bin");
    
    PrintHexDump((u8 *)File.Memory, (u32)File.Size);
    
    printf("Hello world");
}