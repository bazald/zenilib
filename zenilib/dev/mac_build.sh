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

#
# Marshall the App bundle
#

for file in $(find ./ -name '*.dylib'); do
  DEST=game.app/Contents/MacOS/$(echo "$file" | sed 's/.*\///g')
  if [ ! -f "$DEST" ] || [ "$file" -nt "$DEST" ]; then
    cp "$file" "$DEST"
  fi
done

if [ "$CONFIG" == "debug" ]; then
  cp lib/univ_d/* game.app/Contents/MacOS/
else
  rm game.app/Contents/MacOS/*_d.dylib
  rm game.app/Contents/MacOS/game_d
  cp lib/univ/* game.app/Contents/MacOS/
fi

rsync -avz --delete assets/ game.app/Contents/assets/
rsync -avz --delete Resources/ game.app/Contents/Resources/
if [ "$CONFIG" == "debug" ]; then
  cp Info_d.plist game.app/Contents/Info.plist
else
  cp Info.plist   game.app/Contents/Info.plist
fi
