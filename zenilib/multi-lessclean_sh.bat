#!/bin/bash

@echo off
GOTO WINDOWS



rm assets/stderr.txt
rm assets/stdout.txt
rm -r Resources
rm *.plist

rm bin/d32/game.exe
rm bin/d32/*.exp
rm bin/d32/*.idb
rm bin/d32/*.ilk
rm bin/d64/game.exe
rm bin/d64/*.exp
rm bin/d64/*.idb
rm bin/d64/*.ilk
rm bin/x32/*.exp
rm bin/x32/*.idb
rm bin/x32/*.ilk
rm bin/x32/*.pdb
rm bin/x64/*.exp
rm bin/x64/*.idb
rm bin/x64/*.ilk
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

rm -r build/linux/obj
rm game_d32
rm game_d64
rm lsbappchk_filtered.txt
rm lsbappchk_full.txt

rm -r build/macosx/obj
rm -r build/xcode3/build
rm -r build/xcode4/build
rm -r game_d.app



exit



:WINDOWS



DEL /Q "%~dp0\assets\stderr.txt"
DEL /Q "%~dp0\assets\stdout.txt"
RMDIR /S /Q "%~dp0\Resources"
DEL /Q "%~dp0\*.plist"

DEL /Q "%~dp0\bin\d32\game.exe"
DEL /Q "%~dp0\bin\d32\*.exp"
DEL /Q "%~dp0\bin\d32\*.idb"
DEL /Q "%~dp0\bin\d32\*.ilk"
DEL /Q "%~dp0\bin\d64\game.exe"
DEL /Q "%~dp0\bin\d64\*.exp"
DEL /Q "%~dp0\bin\d64\*.idb"
DEL /Q "%~dp0\bin\d64\*.ilk"
DEL /Q "%~dp0\bin\x32\*.exp"
DEL /Q "%~dp0\bin\x32\*.idb"
DEL /Q "%~dp0\bin\x32\*.ilk"
DEL /Q "%~dp0\bin\x32\*.pdb"
DEL /Q "%~dp0\bin\x64\*.exp"
DEL /Q "%~dp0\bin\x64\*.idb"
DEL /Q "%~dp0\bin\x64\*.ilk"
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

RMDIR /S /Q "%~dp0\build\linux\obj"
DEL /Q "%~dp0\game_d32"
DEL /Q "%~dp0\game_d64"
DEL /Q "%~dp0\lsbappchk_filtered.txt"
DEL /Q "%~dp0\lsbappchk_full.txt"

RMDIR /S /Q "%~dp0\build\macosx\obj"
RMDIR /S /Q "%~dp0\build\xcode3\build"
RMDIR /S /Q "%~dp0\build\xcode4\build"
RMDIR /S /Q "%~dp0\game_d.app"



EXIT /B
