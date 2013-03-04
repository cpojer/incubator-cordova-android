#!/bin/sh
#
# cordova and audioboo recorder install script for auphonic-mobile

# NOTE: change this as needed!
AUPHONIC_MOBILE_DIR=../../auphonic-mobile

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
cp libs/armeabi/libaudioboo-native.so $AUPHONIC_MOBILE_DIR/Android/libs/armeabi
cp libs/armeabi-v7a/libaudioboo-native.so $AUPHONIC_MOBILE_DIR/Android/libs/armeabi-v7a

echo "\nFinished!\n"

