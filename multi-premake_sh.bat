#!/bin/bash

@echo off
GOTO WINDOWS



function verify_arg {
  if [ "$1" == "" ] || [ "$1" == "--build=all" ] || [ "$1" == "--build=mine" ]; then
    return 0
  elif [ "$1" == "" ] || [ "$1" == "--macosx=10.6" ] || [ "$1" == "--macosx=10.7" ] || [ "$1" == "--macosx=10.8" ] || [ "$1" == "--macosx=native" ]; then
    return 0
  else
    return 1
  fi
}

function usage {
  echo
  echo "multi-build_sh.bat [--build=all/mine]"
  echo "                   [--macosx=10.6/10.7/10.8/native]"
}

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
$PREMAKE --os=linux $1 $2 gmake
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
$PREMAKE --os=macosx $1 $2 gmake
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

$PREMAKE --os=macosx $1 $2 xcode3
$PREMAKE --os=macosx $1 $2 xcode4
$PREMAKE --os=windows $1 $2 vs2010

exit



:WINDOWS



%~dp0\dev\premake\premake4-windows.exe --file=%~dp0\premake4.lua --os=windows %1 %2 vs2010



EXIT /B
