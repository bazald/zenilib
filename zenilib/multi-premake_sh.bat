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
  echo "  --dir=DIR         arbitrary build directory"
  echo "        build       (default)"
  echo
  echo "  --macosx=10.6     Mac OS 10.6"
  echo "           10.7     Mac OS 10.7"
  echo "           10.8     Mac OS 10.8"
  echo "           native   Whatever version you happen to be running (default)"
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

case $OSTYPE in
  darwin*)
    PREMAKE=dev/premake/premake4-macosx
    ;;
  linux*)
    PREMAKE=dev/premake/premake4-$(arch)
    ;;
  ?*)
    echo "Unknown"
    popd
    exit -3
    ;;
esac

#
# Generate Makefiles for Linux
#

for dir in $(ls -d "$DIR/gmake" 2> /dev/null); do rm -r $dir; done
chmod +x $PREMAKE
$PREMAKE --os=linux --build=$BUILD --dir="$DIR" --macosx=$MACOSX gmake
if [ $? -ne 0 ]; then
  popd
  exit 1
fi

# Migrate Makefiles to build/linux

mkdir -p "$DIR/linux"
for file in $(ls -d "$DIR/linux/*" 2> /dev/null); do
  if [ -f "$file" ]; then rm "$file"; fi
done

for mf in $(ls "$DIR/gmake/" 2> /dev/null); do
  cat "$DIR/gmake/$mf" | sed 's/ -rcs / -rso /' \
                       | sed 's/CC = gcc//' \
                       | sed 's/CXX = g++//' \
                       > "$DIR/linux/$mf"
done
rm -r "$DIR/gmake"

#
# Generate Makefiles for Mac OS X
#

chmod +x $PREMAKE
$PREMAKE --os=macosx --build=$BUILD --dir="$DIR" --macosx=$MACOSX gmake
if [ $? -ne 0 ]; then
  popd
  exit 1
fi

mkdir -p "$DIR/macosx"
for file in $(ls -d "$DIR/macosx/*" 2> /dev/null); do
  if [ -f "$file" ]; then rm "$file"; fi
done

for mf in $(ls "$DIR/gmake/" 2> /dev/null); do
  cat "$DIR/gmake/$mf" | sed 's/-MF [^ ]* //' \
                       | sed 's/\-arch ppc \{0,1\}//' \
                       | sed 's/\-arch ppc64 \{0,1\}//' \
                       > "$DIR/macosx/$mf"
done
rm -r "$DIR/gmake"

#
# Generate IDE projects
#

for outer_dir in $(ls -d "$DIR/xcode*" 2> /dev/null); do
  for dir in $(ls -d "$outer_dir/*.xcodeproj/" 2> /dev/null); do
    rm -r "$dir"
  done
done
for file in $(ls -d "$DIR/vs2010/*.filters" "$DIR/vs2010/*.user" "$DIR/vs2010/*.vcxproj" 2> /dev/null); do
  if [ -f "$file" ]; then rm "$file"; fi
done

$PREMAKE --os=macosx --build=$BUILD --dir="$DIR" --macosx=$MACOSX xcode4
$PREMAKE --os=windows --build=$BUILD --dir="$DIR" --macosx=$MACOSX vs2010
$PREMAKE --os=windows --build=$BUILD --dir="$DIR" --macosx=$MACOSX vs2012

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
ECHO(
ECHO   --dir=DIR         arbitrary build directory
echo         build       (default)
ECHO(
ECHO   --macosx=10.6     Mac OS 10.6
ECHO            10.7     Mac OS 10.7
ECHO            10.8     Mac OS 10.8
ECHO            native   Whatever version you happen to be running (default)

EXIT /B 1

:DONEARGS 

IF NOT "%STATE%"=="config" (
  ECHO(
  ECHO Error: Trailing Argument
  GOTO ARGERROR 
)


DEL /Q "%DIR%\vs2010\*.filters" "%DIR%\vs2010\*.user" "%DIR%\vs2010\*.vcxproj"

"%DP0%\dev\premake\premake4-windows.exe" --file="%DP0%\premake4.lua" --os=windows --build=%BUILD% --dir=%DIR% --macosx=%MACOSX% vs2010
"%DP0%\dev\premake\premake4-windows.exe" --file="%DP0%\premake4.lua" --os=windows --build=%BUILD% --dir=%DIR% --macosx=%MACOSX% vs2012



EXIT /B
