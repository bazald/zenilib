#!/bin/bash

@echo off
GOTO WINDOWS 



function usage {
  echo
  echo "Usage: $0 [options]"
  echo
  echo "  --build=all       game and all dependencies"
  echo "          mine      game only (default)"
  echo
}

function usage_error {
  echo
  echo "Error: $1"
  usage
  exit $2
}

BUILD=mine
CONFIG=release
DIR=build
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
        --dir) STATE=dir ;;
          --dir=*) DIR=$(echo "$arg" | sed 's/--dir=//') ;;
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
    dir)
      DIR="$arg"
      STATE=config
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

pushd $(echo $0 | sed 's/\/[^\/]*$//')
if [ $? -ne 0 ]; then exit -4; fi

for dir in $(ls -d Resources 2> /dev/null); do rm -r $dir; done
for file in $(ls assets/stderr.txt \
                 assets/stdout.txt \
                 *.plist 2> /dev/null); do rm $file; done

for dir in $(ls -d build/vs2010/ipch \
                   build/vs2010/obj \
                   build/vs2012/ipch \
                   build/vs2012/obj 2> /dev/null); do rm -r $dir; done
for file in $(ls bin/x32/*.exp \
                 bin/x32/*.idb \
                 bin/x32/*.ilk \
                 bin/x32/*.pdb \
                 bin/x64/*.exp \
                 bin/x64/*.idb \
                 bin/x64/*.ilk \
                 bin/x64/*.pdb \
                 build/vs2010/*.sdf \
                 build/vs2012/*.sdf \
                 build/*.log \
                 build/backupenv.bat \
                 *\ Debug.exe \
                 *.idb \
                 *.ilk \
                 *.pdb 2> /dev/null); do rm $file; done

for dir in $(ls -d build/linux/obj 2> /dev/null); do rm -r $dir; done
for file in $(ls game_d32 \
                 game_d64 \
                 lsbappchk_filtered.txt \
                 lsbappchk_full.txt 2> /dev/null); do rm $file; done

for dir in $(ls -d build/macosx/obj \
                   build/xcode3/build \
                   build/xcode4/build \
                   game_d.app 2> /dev/null); do rm -r $dir; done

if [ "$BUILD" == "all" ]; then
  for dir in $(ls -d bin/d32 \
                     bin/d64 2> /dev/null); do rm -r $dir; done
  for file in $(ls bin/x32/*.lib \
                   bin/x64/*.lib 2> /dev/null); do rm $file; done

  for dir in $(ls -d lib/d32 \
                     lib/d64 2> /dev/null); do rm -r $dir; done
  for file in $(ls lib/x32/liblocal_SDLmain.a \
                   lib/x64/liblocal_SDLmain.a 2> /dev/null); do rm $file; done

  for dir in $(ls -d lib/univ_d 2> /dev/null); do rm -r $dir; done
else
  for file in $(ls bin/d32/game.exe \
                   bin/d32/game.idb \
                   bin/d32/game.pdb \
                   bin/d32/*.exp \
                   bin/d32/*.idb \
                   bin/d32/*.ilk \
                   bin/d64/game.exe \
                   bin/d64/game.idb \
                   bin/d64/game.pdb \
                   bin/d64/*.exp \
                   bin/d64/*.idb \
                   bin/d64/*.ilk 2> /dev/null); do rm $file; done
fi

popd
exit



:WINDOWS 



SET DP0=%~dp0
SET BUILD=mine
SET CONFIG=release32
SET DIR=build
SET MACOSX=native

SET STATE=config
:NEXTARG 
IF "%1" == "" GOTO DONEARGS 

IF "%STATE%"=="build" (
  IF "%1"=="all" (
    SET BUILD=all
  ) ELSE ( IF "%1"=="mine" (
    SET BUILD=mine
  ) ELSE (
    ECHO(
    ECHO Error: Invalid Argument '%1'
    GOTO ARGERROR 
  ))
  SET STATE=config
) ELSE ( IF "%STATE%"=="config" (
  IF "%1"=="--build" (
    SET STATE=build
  ) ELSE ( IF "%1"=="--macosx" (
    SET STATE=macosx
  ) ELSE ( IF "%1"=="--dir" (
    SET STATE=dir
  ) ELSE ( IF "%1"=="debug" (
    SET CONFIG=debug
  ) ELSE ( IF "%1"=="debug32" (
    SET CONFIG=debug32
  ) ELSE ( IF "%1"=="debug64" (
    SET CONFIG=debug64
  ) ELSE ( IF "%1"=="release" (
    SET CONFIG=release
  ) ELSE ( IF "%1"=="release32" (
    SET CONFIG=release32
  ) ELSE ( IF "%1"=="release64" (
    SET CONFIG=release64
  ) ELSE (
    ECHO(
    ECHO Error: Invalid Argument '%1'
    GOTO ARGERROR 
  )))))))))
) ELSE ( IF "%STATE%"=="dir" (
  SET DIR=%1
  SET STATE=config
) ELSE ( IF "%STATE%"=="macosx" (
  IF "%1"=="10.6" (
    SET MACOSX=10.6
  ) ELSE ( IF "%1"=="10.7" (
    SET MACOSX=10.7
  ) ELSE ( IF "%1"=="10.8" (
    SET MACOSX=10.8
  ) ELSE ( IF "%1"=="native" (
    SET MACOSX=native
  ) ELSE (
    ECHO(
    ECHO Error: Invalid Argument '%1'
    GOTO ARGERROR 
  ))))
  SET STATE=config
) ELSE (
  ECHO(
  ECHO Error: Invalid Argument '%1'
  GOTO ARGERROR 
))))

SHIFT
GOTO NEXTARG 

:ARGERROR 

ECHO(
ECHO Usage: multi-build_sh.bat [options]
ECHO(
ECHO   --build=all       game and all dependencies
ECHO           mine      game only (default)

EXIT /B 1

:DONEARGS 

IF NOT "%STATE%"=="config" (
  ECHO(
  ECHO Error: Trailing Argument
  GOTO ARGERROR 
)


IF EXIST "%DP0%\Resources" RMDIR /S /Q "%DP0%\Resources"
IF EXIST "%DP0%\assets\stderr.txt" DEL /Q "%DP0%\assets\stderr.txt"
IF EXIST "%DP0%\assets\stdout.txt" DEL /Q "%DP0%\assets\stdout.txt"
IF EXIST "%DP0%\*.plist" DEL /Q "%DP0%\*.plist"

IF EXIST "%DP0%\build\vs2010\ipch" RMDIR /S /Q "%DP0%\build\vs2010\ipch"
IF EXIST "%DP0%\build\vs2010\obj" RMDIR /S /Q "%DP0%\build\vs2010\obj"
IF EXIST "%DP0%\build\vs2012\ipch" RMDIR /S /Q "%DP0%\build\vs2012\ipch"
IF EXIST "%DP0%\build\vs2012\obj" RMDIR /S /Q "%DP0%\build\vs2012\obj"
IF EXIST "%DP0%\bin\x32\*.exp" DEL /Q "%DP0%\bin\x32\*.exp"
IF EXIST "%DP0%\bin\x32\*.idb" DEL /Q "%DP0%\bin\x32\*.idb"
IF EXIST "%DP0%\bin\x32\*.ilk" DEL /Q "%DP0%\bin\x32\*.ilk"
IF EXIST "%DP0%\bin\x32\*.pdb" DEL /Q "%DP0%\bin\x32\*.pdb"
IF EXIST "%DP0%\bin\x64\*.exp" DEL /Q "%DP0%\bin\x64\*.exp"
IF EXIST "%DP0%\bin\x64\*.idb" DEL /Q "%DP0%\bin\x64\*.idb"
IF EXIST "%DP0%\bin\x64\*.ilk" DEL /Q "%DP0%\bin\x64\*.ilk"
IF EXIST "%DP0%\bin\x64\*.pdb" DEL /Q "%DP0%\bin\x64\*.pdb"
IF EXIST "%DP0%\build\vs2010\*.sdf" DEL /Q "%DP0%\build\vs2010\*.sdf"
IF EXIST "%DP0%\build\vs2012\*.sdf" DEL /Q "%DP0%\build\vs2012\*.sdf"
IF EXIST "%DP0%\build\*.log" DEL /Q "%DP0%\build\*.log"
IF EXIST "%DP0%\build\backupenv.bat" DEL /Q "%DP0%\build\backupenv.bat"
IF EXIST "%DP0%\* Debug.exe" DEL /Q "%DP0%\* Debug.exe"
IF EXIST "%DP0%\*.idb" DEL /Q "%DP0%\*.idb"
IF EXIST "%DP0%\*.ilk" DEL /Q "%DP0%\*.ilk"
IF EXIST "%DP0%\*.pdb" DEL /Q "%DP0%\*.pdb"

IF EXIST "%DP0%\build\linux\obj" RMDIR /S /Q "%DP0%\build\linux\obj"
IF EXIST "%DP0%\game_d32" DEL /Q "%DP0%\game_d32"
IF EXIST "%DP0%\game_d64" DEL /Q "%DP0%\game_d64"
IF EXIST "%DP0%\lsbappchk_filtered.txt" DEL /Q "%DP0%\lsbappchk_filtered.txt"
IF EXIST "%DP0%\lsbappchk_full.txt" DEL /Q "%DP0%\lsbappchk_full.txt"

IF EXIST "%DP0%\build\macosx\obj" RMDIR /S /Q "%DP0%\build\macosx\obj"
IF EXIST "%DP0%\build\xcode3\build" RMDIR /S /Q "%DP0%\build\xcode3\build"
IF EXIST "%DP0%\build\xcode4\build" RMDIR /S /Q "%DP0%\build\xcode4\build"
IF EXIST "%DP0%\game_d.app" RMDIR /S /Q "%DP0%\game_d.app"

IF "%BUILD%"=="all" (
  IF EXIST "%DP0%\bin\d32" RMDIR /S /Q "%DP0%\bin\d32"
  IF EXIST "%DP0%\bin\d64" RMDIR /S /Q "%DP0%\bin\d64"
  IF EXIST "%DP0%\bin\x32\*.lib" DEL /Q "%DP0%\bin\x32\*.lib"
  IF EXIST "%DP0%\bin\x64\*.lib" DEL /Q "%DP0%\bin\x64\*.lib"

  IF EXIST "%DP0%\lib\d32" RMDIR /S /Q "%DP0%\lib\d32"
  IF EXIST "%DP0%\lib\d64" RMDIR /S /Q "%DP0%\lib\d64"
  IF EXIST "%DP0%\lib\x32\liblocal_SDLmain.a" DEL /Q "%DP0%\lib\x32\liblocal_SDLmain.a"
  IF EXIST "%DP0%\lib\x64\liblocal_SDLmain.a" DEL /Q "%DP0%\lib\x64\liblocal_SDLmain.a"

  IF EXIST "%DP0%\lib\univ_d" RMDIR /S /Q "%DP0%\lib\univ_d"
) ELSE (
  IF EXIST "%DP0%\bin\d32\game.exe" DEL /Q "%DP0%\bin\d32\game.exe"
  IF EXIST "%DP0%\bin\d32\game.idb" DEL /Q "%DP0%\bin\d32\game.idb"
  IF EXIST "%DP0%\bin\d32\game.pdb" DEL /Q "%DP0%\bin\d32\game.pdb"
  IF EXIST "%DP0%\bin\d32\*.exp" DEL /Q "%DP0%\bin\d32\*.exp"
  IF EXIST "%DP0%\bin\d32\*.idb" DEL /Q "%DP0%\bin\d32\*.idb"
  IF EXIST "%DP0%\bin\d32\*.ilk" DEL /Q "%DP0%\bin\d32\*.ilk"
  IF EXIST "%DP0%\bin\d64\game.exe" DEL /Q "%DP0%\bin\d64\game.exe"
  IF EXIST "%DP0%\bin\d64\game.idb" DEL /Q "%DP0%\bin\d64\game.idb"
  IF EXIST "%DP0%\bin\d64\game.pdb" DEL /Q "%DP0%\bin\d64\game.pdb"
  IF EXIST "%DP0%\bin\d64\*.exp" DEL /Q "%DP0%\bin\d64\*.exp"
  IF EXIST "%DP0%\bin\d64\*.idb" DEL /Q "%DP0%\bin\d64\*.idb"
  IF EXIST "%DP0%\bin\d64\*.ilk" DEL /Q "%DP0%\bin\d64\*.ilk"
)

ECHO(
ECHO Clean completed successfully.



EXIT /B
