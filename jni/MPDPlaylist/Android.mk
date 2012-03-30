#MC: MPDInformations class - JNI interface 

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := MPDPlaylist
LOCAL_SRC_FILES := MPDPlaylist.cpp 

LOCAL_C_INCLUDES += ./jni/include/
LOCAL_C_INCLUDES += ./jni/include/libstagefright
LOCAL_C_INCLUDES += ./jni/include/media
LOCAL_C_INCLUDES += ./jni
LOCAL_C_INCLUDES += ./jni/MPDPlaylist
#LOCAL_C_INCLUDES += ./jni/include/libxml2/include/
#LOCAL_C_INCLUDES += ./jni/MPDPlaylist/icu4c
#LOCAL_C_INCLUDES += ./jni/MPDPlaylist/icu4c/common/  


LOCAL_LDLIBS := -ldl -llog -L./obj/local/armeabi -lutils -lstagefright  -lstagefright_foundation -licuuc 

LOCAL_STATIC_LIBRARIES := libxml2 gstmpdparser

#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
include $(BUILD_STATIC_LIBRARY)
include ./jni/MPDPlaylist/mpdmodule/mpdparse/Android.mk

