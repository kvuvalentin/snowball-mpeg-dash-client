LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := gstmpdparser
LOCAL_SRC_FILES := gstmpdparse.c  ../glibchelper/glibchelper.c ../glibchelper/glist.c ../mpdcommon/gstmpdcommon.c


LOCAL_C_INCLUDES += ./jni/include/libxml2/include
LOCAL_C_INCLUDES += ./jni/include/icu4c/common
LOCAL_C_INCLUDES += ./jni/MPDPlaylist/mpdmodule/glibchelper
LOCAL_C_INCLUDES += ./
LOCAL_C_INCLUDES += ./jni/MPDPlaylist


LOCAL_STATIC_LIBRARIES := \
	libstagefright_httplive \

#LOCAL_CFLAGS    := -Werror  -DDEBUG
LOCAL_LDLIBS := -ldl -llog 


include $(BUILD_STATIC_LIBRARY)