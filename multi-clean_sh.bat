#!/bin/bash

@echo off
GOTO WINDOWS



function usage {
  echo
  echo "Usage: $0 [options]"
  echo
  echo "  --build=all       game and all dependencies"
  echo "          mine      game only (default)"
}

function usage_error {
  echo
  echo "Error: $1"
  usage
  exit $2
}

BUILD=mine
CONFIG=release
MACOSX=native

STATE=config
for arg in "$@"; do
  case "$STATE" in
    build)
      case "$arg" in
        all) BUILD=all ;;
        mine) BUILD=mine ;;
        *) usage_error "Invalid Argument '$arg'" 2
      esac
      STATE=config
      ;;
    config)
      case "$arg" in
        --build) STATE=build ;;
          --build=all) BUILD=all ;;
          --build=mine) BUILD=mine ;;
        --macosx) STATE=macosx ;;
          --macosx=10.6) MACOSX=10.6 ;;
          --macosx=10.7) MACOSX=10.7 ;;
          --macosx=10.8) MACOSX=10.8 ;;
          --macosx=native) MACOSX=native ;;
        debug) CONFIG=debug ;;
        release) CONFIG=release ;;
        releaseuniv) CONFIG=release ;;
        *) usage_error "Invalid Argument '$arg'" 3
      esac
      ;;
    macosx)
      case "$arg" in
        10.6) MACOSX=10.6 ;;
        10.7) MACOSX=10.7 ;;
        10.8) MACOSX=10.8 ;;
        native) MACOSX=native ;;
        *) usage_error "Invalid Argument '$arg'" 4
      esac
      STATE=config
      ;;
    *)
      usage_error "Invalid Argument '$arg'" 1
  esac
done
if [ "$STATE" != "config" ]; then
  usage_error "Trailing Argument" 5
fi


rm assets/stderr.txt
rm assets/stdout.txt
rm -r Resources
rm *.plist

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

if [ "$BUILD" == "all" ]; then
  rm -r bin/d32
  rm -r bin/d64
  rm bin/x32/*.lib
  rm bin/x64/*.lib

  rm -r lib/d32
  rm -r lib/d64
  rm lib/x32/liblocal_SDLmain.a
  rm lib/x64/liblocal_SDLmain.a

  rm -r lib/univ_d
else
  rm bin/d32/game.exe
  rm bin/d32/*.exp
  rm bin/d32/*.idb
  rm bin/d32/*.ilk
  rm bin/d64/game.exe
  rm bin/d64/*.exp
  rm bin/d64/*.idb
  rm bin/d64/*.ilk
fi

rm -r build/macosx/obj
rm -r build/xcode3/build
rm -r build/xcode4/build
rm -r game_d.app



exit



:WINDOWS



IF "%1=%2"=="=" (
  SET CLEAN=mine
) ELSE ( IF "%1=%2"=="--build=all" (
  SET CLEAN=all
) ELSE ( IF "%1=%2"=="--build=mine" (
  SET CLEAN=mine
) ELSE (
  ECHO(
  ECHO Illegal argument: %1=%2
  ECHO(
  ECHO multi-clean_sh.bat [--build=all/mine]
  EXIT /B 2
)))

DEL /Q "%~dp0\assets\stderr.txt"
DEL /Q "%~dp0\assets\stdout.txt"
RMDIR /S /Q "%~dp0\Resources"
DEL /Q "%~dp0\*.plist"

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

IF "%CLEAN%"=="all" (
  RMDIR /S /Q "%~dp0\bin\d32"
  RMDIR /S /Q "%~dp0\bin\d64"
  DEL /Q "%~dp0\bin\x32\*.lib"
  DEL /Q "%~dp0\bin\x64\*.lib"

  RMDIR /S /Q "%~dp0\lib\d32"
  RMDIR /S /Q "%~dp0\lib\d64"
  DEL /Q "%~dp0\lib\x32\liblocal_SDLmain.a"
  DEL /Q "%~dp0\lib\x64\liblocal_SDLmain.a"

  RMDIR /S /Q "%~dp0\lib\univ_d"
) ELSE (
  DEL /Q "%~dp0\bin\d32\game.exe"
  DEL /Q "%~dp0\bin\d32\*.exp"
  DEL /Q "%~dp0\bin\d32\*.idb"
  DEL /Q "%~dp0\bin\d32\*.ilk"
  DEL /Q "%~dp0\bin\d64\game.exe"
  DEL /Q "%~dp0\bin\d64\*.exp"
  DEL /Q "%~dp0\bin\d64\*.idb"
  DEL /Q "%~dp0\bin\d64\*.ilk"
)



EXIT /B
