#!/bin/bash

@echo off
GOTO WINDOWS



rm assets/stderr.txt
rm assets/stdout.txt

rm -r bin/d32
rm -r bin/d64

rm bin/x32/*.exp
rm bin/x32/*.idb
rm bin/x32/*.ilk
rm bin/x32/*.lib
rm bin/x32/*.pdb

rm bin/x64/*.exp
rm bin/x64/*.idb
rm bin/x64/*.ilk
rm bin/x64/*.lib
rm bin/x64/*.pdb

rm -r build/vs2010/ipch
rm -r build/vs2010/obj
rm build/vs2010/*.sdf

rm build/*.log
rm build/backupenv.bat

rm *\ Debug.exe
rm *.idb
rm *.ilk
rm *.pdb



exit



:WINDOWS



DEL /Q "%~dp0\assets\stderr.txt"
DEL /Q "%~dp0\assets\stdout.txt"

RMDIR /S /Q "%~dp0\bin\d32"
RMDIR /S /Q "%~dp0\bin\d64"

DEL /Q "%~dp0\bin\x32\*.exp"
DEL /Q "%~dp0\bin\x32\*.idb"
DEL /Q "%~dp0\bin\x32\*.ilk"
DEL /Q "%~dp0\bin\x32\*.lib"
DEL /Q "%~dp0\bin\x32\*.pdb"

DEL /Q "%~dp0\bin\x64\*.exp"
DEL /Q "%~dp0\bin\x64\*.idb"
DEL /Q "%~dp0\bin\x64\*.ilk"
DEL /Q "%~dp0\bin\x64\*.lib"
DEL /Q "%~dp0\bin\x64\*.pdb"

RMDIR /S /Q "%~dp0\build\vs2010\ipch"
RMDIR /S /Q "%~dp0\build\vs2010\obj"
DEL /Q "%~dp0\build\vs2010\*.sdf"

DEL /Q "%~dp0\build\*.log"
DEL /Q "%~dp0\build\backupenv.bat"

DEL /Q "%~dp0\* Debug.exe"
DEL /Q "%~dp0\*.idb"
DEL /Q "%~dp0\*.ilk"
DEL /Q "%~dp0\*.pdb"



EXIT /B
