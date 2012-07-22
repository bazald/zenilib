@echo off

GOTO WINDOWS



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
chmod +x dev/premake/premake4-linux
dev/premake/premake4-linux --os=linux gmake
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

chmod +x dev/premake/premake4-macosx
dev/premake/premake4-macosx --os=macosx gmake
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

$PREMAKE --os=macosx xcode3
$PREMAKE --os=macosx xcode4
$PREMAKE --os=windows vs2010

exit



:WINDOWS



dev\premake\premake4-windows.exe --os=windows vs2010



EXIT /B
