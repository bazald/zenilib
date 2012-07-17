#!/bin/bash

# Pop out of the dev directory (if needed)
DIR_TAIL=$(echo $(pwd) | sed 's/.*\///')
if [ "$DIR_TAIL" == "dev" ]
then
  cd ..
fi

# premake
if [ ! -d build/macosx ]; then
  dev/premake.sh
fi

#
# Setup the build
#

CCACHE=""
if [ "$(which ccache)" != "" ]; then
  CCACHE=ccache
fi

if [ "$CONFIG" == "" ]; then
  CONFIG=debug
fi

#export CC="$CCACHE llvm-gcc"
#export CXX="$CCACHE llvm-g++"
export CC="$CCACHE clang"
export CXX="$CCACHE clang++"

#
# Build
#

#make -j 4 -C build/macosx config=$CONFIG
make -C build/macosx config=$CONFIG
if [ $? -ne 0 ]
then
  exit 2
fi
