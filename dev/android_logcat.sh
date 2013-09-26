#!/bin/bash

adb logcat -c
sleep 1
adb logcat | grep -v Amazon | grep -v dalvikvm | grep -v ActivityManager | grep -v dropbox | grep -v TitaniumBackup

