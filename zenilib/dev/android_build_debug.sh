#!/bin/bash

DIR_TAIL=$(echo $(pwd) | sed 's/.*\///')
if [ "$DIR_TAIL" == "dev" ]
then
  cd ..
fi

# Copy Android-specific development files
rsync -r dev/android_/ .

# Clear some built files
for FILE in bin/*.ap_* bin/*debug*
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
ndk-build --jobs=4 NDK_DEBUG=1 APP_OPTIM=debug
rv=$?
if [ $rv -ne 0 ]
then
  exit $rv
fi

# Do the rest of the build
ant debug
rv=$?
if [ $rv -ne 0 ]
then
  exit $rv
fi

# Update the JNI code
cd bin/classes
javah -jni com.zenilib.app.ZenilibActivity
cd ../..

#if [ -e ~/Dropbox/ZenilibActivity-debug.apk ]
#then
#  rm ~/Dropbox/ZenilibActivity-debug.apk
#fi
#
#sleep 1
#
#cp bin/ZenilibActivity-debug.apk ~/Dropbox/
#rv=$?
#if [ $rv -ne 0 ]
#then
#  exit $rv
#fi
