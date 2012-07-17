#!/bin/bash

DIR_TAIL=$(echo $(pwd) | sed 's/.*\///')
if [ "$DIR_TAIL" == "dev" ]
then
  cd ..
fi

# Copy Android-specific development files
rsync -r dev/android_/ .

# Clear some built files
for FILE in bin/*.ap_* bin/*release*
do
  if [ -e "$FILE" ]
  then
    rm -r "$FILE"
  fi
done

# Update the project, just in case
android update project -p .
rv=$?
if [ $rv -ne 0 ]
then
  exit $rv
fi

# Do the C++ build
ndk-build --jobs=4 NDK_DEBUG=0 APP_OPTIM=release
rv=$?
if [ $rv -ne 0 ]
then
  exit $rv
fi

# Do the rest of the build
ant release
rv=$?
if [ $rv -ne 0 ]
then
  exit $rv
fi

#if [ -e ~/Dropbox/ZenilibActivity.apk ]
#then
#  rm ~/Dropbox/ZenilibActivity.apk
#fi

# Finalize the .APK
jarsignzipalign.sh
rv=$?
if [ $rv -ne 0 ]
then
  exit $rv
fi

#cp bin/ZenilibActivity-release.apk ~/Dropbox/ZenilibActivity.apk
#rv=$?
#if [ $rv -ne 0 ]
#then
#  exit $rv
#fi
