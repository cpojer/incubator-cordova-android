LOCAL_PATH := $(call my-dir)

# Build libsndfile statically
#
include $(CLEAR_VARS)

LOCAL_MODULE    := auphonic-sndfile
LOCAL_SRC_FILES := \
    sndfile/src/common.c \
    sndfile/src/file_io.c \
    sndfile/src/command.c \
    sndfile/src/pcm.c \
    sndfile/src/ulaw.c \
    sndfile/src/alaw.c \
    sndfile/src/float32.c \
    sndfile/src/double64.c \
    sndfile/src/ima_adpcm.c \
    sndfile/src/ms_adpcm.c \
    sndfile/src/gsm610.c \
    sndfile/src/dwvw.c \
    sndfile/src/vox_adpcm.c \
    sndfile/src/interleave.c \
    sndfile/src/strings.c \
    sndfile/src/dither.c \
    sndfile/src/broadcast.c \
    sndfile/src/audio_detect.c \
    sndfile/src/ima_oki_adpcm.c \
    sndfile/src/chunk.c \
    sndfile/src/ogg.c \
    sndfile/src/chanmap.c \
    sndfile/src/windows.c \
    sndfile/src/id3.c \
    sndfile/src/sndfile.c \
    sndfile/src/aiff.c \
    sndfile/src/au.c \
    sndfile/src/avr.c \
    sndfile/src/caf.c \
    sndfile/src/dwd.c \
    sndfile/src/flac.c \
    sndfile/src/g72x.c \
    sndfile/src/htk.c \
    sndfile/src/ircam.c \
    sndfile/src/macbinary3.c \
    sndfile/src/macos.c \
    sndfile/src/mat4.c \
    sndfile/src/mat5.c \
    sndfile/src/nist.c \
    sndfile/src/paf.c \
    sndfile/src/pvf.c \
    sndfile/src/raw.c \
    sndfile/src/rx2.c \
    sndfile/src/sd2.c \
    sndfile/src/sds.c \
    sndfile/src/svx.c \
    sndfile/src/txw.c \
    sndfile/src/voc.c \
    sndfile/src/wve.c \
    sndfile/src/w64.c \
    sndfile/src/wav_w64.c \
    sndfile/src/wav.c \
    sndfile/src/xi.c \
    sndfile/src/mpc2k.c \
    sndfile/src/rf64.c \
    sndfile/src/GSM610/add.c \
    sndfile/src/GSM610/code.c \
    sndfile/src/GSM610/decode.c \
    sndfile/src/GSM610/gsm_create.c \
    sndfile/src/GSM610/gsm_decode.c \
    sndfile/src/GSM610/gsm_destroy.c \
    sndfile/src/GSM610/gsm_encode.c \
    sndfile/src/GSM610/gsm_option.c \
    sndfile/src/GSM610/long_term.c \
    sndfile/src/GSM610/lpc.c \
    sndfile/src/GSM610/preprocess.c \
    sndfile/src/GSM610/rpe.c \
    sndfile/src/GSM610/short_term.c \
    sndfile/src/GSM610/table.c \
    sndfile/src/G72x/g721.c \
    sndfile/src/G72x/g723_16.c \
    sndfile/src/G72x/g723_24.c \
    sndfile/src/G72x/g723_40.c \
    sndfile/src/G72x/g72x.c

include $(BUILD_STATIC_LIBRARY)

# Then build flac statically
#
include $(CLEAR_VARS)

LOCAL_MODULE    := audioboo-flac
LOCAL_SRC_FILES := \
	flac/src/libFLAC/bitmath.c \
	flac/src/libFLAC/bitreader.c \
	flac/src/libFLAC/cpu.c \
	flac/src/libFLAC/crc.c \
	flac/src/libFLAC/fixed.c \
	flac/src/libFLAC/float.c \
	flac/src/libFLAC/format.c \
	flac/src/libFLAC/lpc.c \
	flac/src/libFLAC/md5.c \
	flac/src/libFLAC/memory.c \
	flac/src/libFLAC/metadata_iterators.c \
	flac/src/libFLAC/metadata_object.c \
	flac/src/libFLAC/ogg_decoder_aspect.c \
	flac/src/libFLAC/ogg_encoder_aspect.c \
	flac/src/libFLAC/ogg_helper.c \
	flac/src/libFLAC/ogg_mapping.c \
	flac/src/libFLAC/stream_decoder.c \
	flac/src/libFLAC/stream_encoder.c \
	flac/src/libFLAC/stream_encoder_framing.c \
	flac/src/libFLAC/window.c \
	flac/src/libFLAC/bitwriter.c

include $(BUILD_STATIC_LIBRARY)

# Lastly build the JNI wrapper and link both other libs against it
#
include $(CLEAR_VARS)

LOCAL_MODULE    := audioboo-native
LOCAL_SRC_FILES := \
	jni/FLACStreamEncoder.cpp \
	jni/FLACStreamDecoder.cpp \
	jni/util.cpp
LOCAL_LDLIBS := -llog

LOCAL_STATIC_LIBRARIES := audioboo-flac auphonic-sndfile

include $(BUILD_SHARED_LIBRARY)
