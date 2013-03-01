# Build both ARMv5TE and ARMv7-A machine code.
APP_ABI := armeabi armeabi-v7a

APP_MODULES = audioboo-native auphonic-sndfile
APP_OPTIM = release

AUDIOBOO_NATIVE_FLAGS = \
	-Ijni/config \
	-Ijni/ogg/include \
    -Ijni/sndfile/deps/libogg/include \
    -Ijni/sndfile/deps/libvorbis/include \
    -Ijni/sndfile/src \
	-DVERSION=\"1.2\" \
	-Ijni/flac/include \
	-Ijni/flac/src/libFLAC/include

APP_CFLAGS += $(AUDIOBOO_NATIVE_FLAGS)
APP_CXXFLAGS += $(AUDIOBOO_NATIVE_FLAGS)
