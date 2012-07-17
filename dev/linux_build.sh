#!/bin/bash

# Pop out of the dev directory (if needed)
DIR_TAIL=$(echo $(pwd) | sed 's/.*\///')
if [ "$DIR_TAIL" == "dev" ]
then
  cd ..
fi

# premake
if [ ! -d build/linux ]; then
  dev/premake.sh
fi

#
# Setup the build
#

if [ "$LSB_HOME" == "" ]; then
  LSB_HOME=/opt/lsb
fi

CCACHE=""
if [ -x $(which ccache) ]; then
  CCACHE=ccache
fi

LSBCC_LIB_PREFIX=$LSB_HOME/lib-
BIT=32
if [ $(uname -m) == "x86_64" ]; then
  LSBCC_LIB_PREFIX=$LSB_HOME/lib64-
  BIT=64
fi

if [ "$CONFIG" == "" ]; then
  CONFIG=debug
fi

if [ "$CONFIG" == "debug" ]; then
  CONFIG_CHAR=d
elif [ "$CONFIG" == "release" ]; then
  CONFIG_CHAR=x
fi

export LSBCC_LIB_PREFIX
export LSBCC=gcc-4.6
export LSBCXX=g++-4.6
export LSBCC_LSBVERSION=4.0
export LSBCC_LIBS=$LSBCC_LIB_PREFIX$LSBCC_LSBVERSION
export LSB_SHAREDLIBPATH="$(pwd)/lib/$CONFIG_CHAR$BIT"
export CCACHE
export CCACHE_BASEDIR=$HOME
export CCACHE_CC=$LSB_HOME/bin/lsbcc
export CCACHE_CXX=$LSB_HOME/bin/lsbc++
export CCACHE_SLOPPINESS=time_macros
export CCACHE_COMPILERCHECK="$CCACHE_CC -v"

if [ -x $(which $LSBCC) ] && [ -x $(which $LSBCXX) ] && [ -x "$LSB_HOME/bin/lsbcc" ] && [ -x "$LSB_HOME/bin/lsbc++" ]
then
  COMPILING_WITH_LSB=1
  echo "Compiling for LSB"

  export CC="$CCACHE $CCACHE_CC"
  export CXX="$CCACHE $CCACHE_CXX"

  make -j 4 -C build/linux config=$CONFIG$BIT
  EXIT_CODE=$?

  if [ $EXIT_CODE -eq 0 ] && [ -x game_x$BIT ]
  then
    $LSB_HOME/bin/lsbappchk --no-journal --missing-symbols --lsb-version=4.0 game_x$BIT &> lsbappchk_full.txt
    cat lsbappchk_full.txt \
      | grep -v "WARNING" \
      | grep -v "ignoring" \
      | grep -v "not checked" \
      | grep -v "Not recognized by name" \
      | grep -v "not a compliance issue" \
      | grep -v "please rerun" \
      | grep -v "email the output" \
      | grep -v "lsb-discuss" \
      | grep -v "deprecated" \
      | grep -v "SO-Pass" \
      > lsbappchk_filtered.txt
  #   rm lsbappchk_full.txt
    cat lsbappchk_filtered.txt
  fi
else
  make -j 4 -C build/linux config=$CONFIG
fi
