
#include "arc.h"

#include <stdio.h>
#include <windows.h>

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

static void
PrintHexDump(u8 *Data, u32 Length)
{
    const u32 BytesPerLine = 16 * 2;
    const u32 Lines = Length / BytesPerLine;
    
    printf("Memory Dump:\n");
    
    u32 Remaining = Length;
    for(u32 I = 0; 
        I < Lines; 
        ++I)
    {
        u8 *Line = Data + BytesPerLine * I;
        printf("0x%p:  ", Line);
        
        if(Remaining >= BytesPerLine)
        {
            u8 *LineCursor = Line;
            
            for(u32 Block = 0; 
                Block < BytesPerLine / 8; 
                ++Block)
            {
                for(u32 J = 0; 
                    J < 8; 
                    ++J)
                {
                    u8 Char = *LineCursor++;
                    
                    char *ColorString = "0";
                    if(Char == 0)
                    {
                        ColorString = "2;90";
                    }
                    else if(Char <= 0x30)
                    {
                        ColorString = "0;90";
                    }
                    else if(Char <= 0x70)
                    {
                        ColorString = "0;37";
                    }
                    else
                    {
                        ColorString = "0;97";
                    }
                    
                    printf("\033[%sm%02hhx ", ColorString, Char);
                }
                
                printf("  ");
            }
            
            printf("\033[0m");
            
            LineCursor = Line;
            for(u32 J = 0; 
                J < BytesPerLine; 
                ++J)
            {
                char Char = *LineCursor++;
                if(Char < 0x20 || Char >= 0x7f)
                {
                    Char = '.';
                }
                
                printf("%c", Char);
            }
        }
        
        printf("\033[0m\n");
        Remaining -= BytesPerLine;
    }
}

inline u32
StringCopy(char *A, char *B)
{
    u32 Result = 0;
    
    while(*B)
    {
        *A++ = *B++;
        ++Result;
    }
    
    return Result;
}

#include "ppc.c"

int main(int Argc, char **Argv)
{
    char *Filename = 0;
    if(Argc == 2)
    {
        Filename = Argv[1];
    }
    else
    {
        printf("usage: arc <filename>\n");
    }
    
    printf("%s\n", Filename);
    
    if(Filename)
    {
        loaded_file File = Win32ReadEntireFile(Filename);
        PrintHexDump((u8 *)File.Memory, (u32)File.Size);
        
        DecompilePPC(&File);
    }
}


/*
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
        
        Address += RowLength;
    }
}
*/
