#!/bin/sh
#
# cordova and auphonic recorder install script for auphonic-mobile

AUPHONIC_MOBILE_DIR=$(cat auphonic.path)

echo "\Build java code ...\n"
android update project -p . -t android-17
ant jar

echo "\nBuild native code ...\n"

# build C code
ndk/ndk-build

echo "\nNow install new C code ...\n"

# copy jar file
cp cordova-*.jar $AUPHONIC_MOBILE_DIR/Android/libs

# copy native libraries
cp libs/armeabi/libauphonic-recorder.so $AUPHONIC_MOBILE_DIR/Android/libs/armeabi
cp libs/armeabi-v7a/libauphonic-recorder.so $AUPHONIC_MOBILE_DIR/Android/libs/armeabi-v7a

echo "\nFinished!\n"

