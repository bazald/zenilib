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
  echo "  --macosx=10.6     Mac OS 10.6"
  echo "           10.7     Mac OS 10.7"
  echo "           10.8     Mac OS 10.8"
  echo "           native   Whatever version you happen to be running (default)"
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


case $OSTYPE in
  darwin*)
    PREMAKE=dev/premake/premake4-macosx
    ;;
  linux*)
    PREMAKE=dev/premake/premake4-linux
    ;;
  ?*)
    echo "Unknown"
    exit 2
    ;;
esac

#
# Generate Makefiles for Linux
#

rm -r build/gmake
chmod +x $PREMAKE
$PREMAKE --os=linux --build=$BUILD --macosx=$MACOSX gmake
if [ $? -ne 0 ]; then exit 1; fi

# Migrate Makefiles to build/linux

mkdir -p build/linux
for file in build/linux/*; do
  if [ -f "$file" ]; then rm "$file"; fi
done
cp build/gmake/* build/linux/
rm -r build/gmake

#
# Generate Makefiles for Mac OS X
#

chmod +x $PREMAKE
$PREMAKE --os=macosx --build=$BUILD --macosx=$MACOSX gmake
if [ $? -ne 0 ]; then exit 1; fi

mkdir -p build/macosx
for file in build/macosx/*; do
  if [ -f "$file" ]; then rm "$file"; fi
done

pushd build/gmake
for mf in *; do
  cat "$mf" | sed 's/-MF [^ ]* //' \
            | sed 's/\-arch ppc \{0,1\}//' \
            | sed 's/\-arch ppc64 \{0,1\}//' \
            > "../../build/macosx/$mf"
done
popd
rm -r build/gmake

#
# Generate IDE projects
#

for dir in build/xcode*/*.xcodeproj/; do
  rm -r "$dir"
done
for file in build/vs2010/*.filters build/vs2010/*.user build/vs2010/*.vcxproj; do
  if [ -f "$file" ]; then rm "$file"; fi
done

$PREMAKE --os=macosx --build=$BUILD --macosx=$MACOSX xcode3
$PREMAKE --os=macosx --build=$BUILD --macosx=$MACOSX xcode4
$PREMAKE --os=windows --build=$BUILD --macosx=$MACOSX vs2010

exit



:WINDOWS



IF "%1=%2"=="=" (
  SET ARG1=
) ELSE ( IF "%1=%2"=="--build=all" (
  SET ARG1=%1=%2
) ELSE ( IF "%1=%2"=="--build=mine" (
  SET ARG1=%1=%2
) ELSE ( IF "%1=%2"=="--macosx=10.6" (
  SET ARG1=%1=%2
) ELSE ( IF "%1=%2"=="--macosx=10.7" (
  SET ARG1=%1=%2
) ELSE ( IF "%1=%2"=="--macosx=10.8" (
  SET ARG1=%1=%2
) ELSE ( IF "%1=%2"=="--macosx=native" (
  SET ARG1=%1=%2
) ELSE (
  ECHO(
  ECHO Illegal argument: %1=%2
  ECHO(
  ECHO multi-premake_sh.bat [--build=all/mine]
  EXIT /B 2
)))))))

IF "%3=%4"=="=" (
  SET ARG2=
) ELSE ( IF "%3=%4"=="--build=all" (
  SET ARG2=%3=%4
) ELSE ( IF "%3=%4"=="--build=mine" (
  SET ARG2=%3=%4
) ELSE ( IF "%3=%4"=="--macosx=10.6" (
  SET ARG2=%3=%4
) ELSE ( IF "%3=%4"=="--macosx=10.7" (
  SET ARG2=%3=%4
) ELSE ( IF "%3=%4"=="--macosx=10.8" (
  SET ARG2=%3=%4
) ELSE ( IF "%3=%4"=="--macosx=native" (
  SET ARG2=%3=%4
) ELSE (
  ECHO(
  ECHO Illegal argument: %3=%4
  ECHO(
  ECHO multi-premake_sh.bat [--build=all/mine]
  EXIT /B 3
)))))))

%~dp0\dev\premake\premake4-windows.exe --file=%~dp0\premake4.lua --os=windows %ARG1% %ARG2% vs2010



EXIT /B
