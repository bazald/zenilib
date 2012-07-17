#!/bin/bash

# Pop out of the dev directory (if needed)
DIR_TAIL=$(echo $(pwd) | sed 's/.*\///')
if [ "$DIR_TAIL" == "dev" ]
then
  cd ..
fi

# Copy PC-specific development files
rsync -r dev/pc_/ .

#
# Pick premake4
#

PREMAKE=dev/premake/premake4-linux
$PREMAKE &> /dev/null
if [ $? -ne 1 ]; then
  PREMAKE=dev/premake/premake4-macosx
  $PREMAKE &> /dev/null
  if [ $? -ne 1 ]; then
    PREMAKE=dev/premake/premake4-windows.exe
  fi
fi

#
# Generate Makefiles for Linux
#

rm -r build/gmake
$PREMAKE --os=linux gmake
if [ $? -ne 0 ]
then
  exit 1
fi

mkdir -p build/linux
for file in build/linux/*; do
  if [ -f "$file" ]; then
    rm "$file"
  fi
done

cp build/gmake/* build/linux/

#
# Generate Makefiles for Mac OS X
#

rm -r build/gmake
$PREMAKE --os=macosx gmake
if [ $? -ne 0 ]
then
  exit 1
fi

mkdir -p build/macosx
for file in build/macosx/*; do
  if [ -f "$file" ]; then
    rm "$file"
  fi
done

pushd build/gmake
for mf in *; do
  cat "$mf" | sed 's/-MF [^ ]* //' \
            | sed 's/\-arch ppc \{0,1\}//' \
            | sed 's/\-arch ppc64 \{0,1\}//' \
            > "../../build/macosx/$mf"
done
popd

#
# Generate Visual Studio 2010 solution
#

$PREMAKE --os=windows vs2010

#
# Clean up
#

rm -r build/gmake
