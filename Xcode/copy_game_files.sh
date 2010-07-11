#!/bin/bash

cd ..

for App in *.app; do
  mkdir -p "$App/Contents/zenilib"

  FILES="*"
  for File in $FILES; do
    AppName=`echo $File | grep -v ".app"`
    if [ "$AppName" != "" ] && [ "$File" != "Doxyfile" ] && [ "$File" != "Frameworks" ] && [ "$File" != "SConstruct" ] && [ "$File" != "Visual Studio 2008" ] && [ "$File" != "Xcode 3.1.2" ] && [ "$File" != "clean" ] && [ "$File" != "include" ] && [ "$File" != "src" ]; then
      echo "rsync -r --delete \"$File\" \"$App/Contents/zenilib/\""
      rsync -r --delete "$File" "$App/Contents/zenilib/"
    fi
  done
done
