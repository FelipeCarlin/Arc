@echo off

::
::    To run this at startup, use this as you shortcut target:
::    "%windir%\system32\cmd.exe /k w:\Arc\arc\misc\shell.bat"
::

:: Setup toolchain (MSVC)
::call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
call W:\_Tools\msvc\setup.bat

:: Setup misc project scripts
set path=w:\Arc\arc\misc;%path%

:: Setup working directory
w:
cd Arc