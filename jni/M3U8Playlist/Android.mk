#MC: M3UInformations class - JNI interface
#this class is useful to get informations about a M3U file (.m3u8) specified in the constructor
#the informations are obtained through the selectors provided by this class. 

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := M3U8Playlist
LOCAL_SRC_FILES := M3U8Playlist.cpp 

LOCAL_C_INCLUDES += ./jni/include/
LOCAL_C_INCLUDES += ./jni/include/libstagefright
LOCAL_C_INCLUDES += ./jni/include/media
LOCAL_C_INCLUDES += ./jni

LOCAL_LDLIBS := -ldl -llog -L./obj/local/armeabi -lutils -lstagefright  -lstagefright_foundation

#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
include $(BUILD_SHARED_LIBRARY)

