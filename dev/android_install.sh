#!/bin/bash

DIR_TAIL=$(echo $(pwd) | sed 's/.*\///')
if [ "$DIR_TAIL" == "dev" ]
then
  cd ..
fi

ADB=$(ps -e | grep -c adb)
APK=bin/ZenilibActivity-debug.apk

if [ $ADB != 0 ]
then
	adb install -r "$APK"
else
	sudo adb install -r "$APK"
fi

if [ $? != 0 ]
then
	sudo killall adb
 	sudo /home/bazald/bin/adb root
fi
