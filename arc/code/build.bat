@echo off

:: COMPILER AND LINKER SWITCHES
set CompilerFlags=                 -nologo
set LibraryPaths=                  
set Libraries= %LibraryPaths%      
set LinkerFlags= %Libraries%       -incremental:no -opt:ref


:: COMPILER INCLUDE PATHS
set CompilerFlags= %CompilerFlags%


:: MISC COMPILER SWITCHER
set CompilerFlags= %CompilerFlags% -MTd                &REM Include CRT library in the executable (static link, debug version)
set CompilerFlags= %CompilerFlags% -DCOMPILER_MSVC


:: GENERAL WARNING FLAGS
set CompilerFlags= %CompilerFlags% -WX -W4
set CompilerFlags= %CompilerFlags% -wd4201 -wd4100 -wd4189 -wd4505 -wd4456 -wd4706 -wd4200


:: OPTIMISATIONS AND SUCH
set CompilerFlags= %CompilerFlags% -Od -Oi
set CompilerFlags= %CompilerFlags% -fp:fast -Gm- -GR- -EHa-


:: DEBUG VARIABLES
set DebugFlags=                    -FC                 &REM Produce the full path of the source code file.
set DebugFlags= %DebugFlags%       -Z7                 &REM Produce debug information (c 7.0).


:: CROSS PLATFORM DEFINES
set Defines=                       -DARC_INTERNAL=1
set Defines= %Defines%             -DARC_SLOW=1
set Defines= %Defines%             -DARC_WIN32=1


IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\build

:: 64-bit build

del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp
::cl %CompilerFlags% %DebugFlags% %Defines% ..\ui\code\ui.cpp -Fmradiant.map -LD /link -incremental:no -opt:ref -PDB:radiant_%random%.pdb -EXPORT:GameUpdateAndRender
del lock.tmp
cl %CompilerFlags% %DebugFlags% %Defines% ..\arc\code\arc.c -Fmarc.map -link %LinkerFlags%
popd