#!/bin/bash

DIR_TAIL=$(echo $(pwd) | sed 's/.*\///')
if [ "$DIR_TAIL" == "dev" ]
then
  cd ..
fi

if [ -f "premake4.lua" ]
then
  premake4 clean
fi

for FILE in $(find dev/android_ dev/pc_ -type f)
do
  MIRROR=$(echo "$FILE" | sed "s/dev\/[^\/]*_\///")
  if [ -f "$MIRROR" ] && [ $(echo "$FILE" | grep -c "\.svn") -eq 0 ]
  then
    rm "$MIRROR"
  fi
done

for FILE in AndroidManifest.xml ant.properties assets/stderr.txt assets/stdout.txt build.xml local.properties proguard.cfg project.properties lsbappchk_full.txt lsbappchk_filtered.txt lsbappchk_full_x64.txt lsbappchk_filtered_x64.txt lsbappchk_full_x32.txt lsbappchk_filtered_x32.txt game_d32 game_d64 game_x32 game_x64 $(find jni -name '*.dylib') $(find ./ -name '*\.DS_Store*')
do
  if [ -e "$FILE" ]
  then
    rm -r "$FILE"
  fi
done
for DIR in bin build gen lib libs obj res src Resources game.app
do
  if [ -e "$DIR" ]
  then
    rm -r "$DIR"
  fi
done

for FILE in $(find ./ -name "*~")
do
  rm "$FILE"
done
