# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := playlisthandler-jni
LOCAL_SRC_FILES := playlistHandler-jni.cpp M3U8Playlist/M3U8Playlist.cpp  

LOCAL_C_INCLUDES += ./jni/M3U8Playlist
LOCAL_C_INCLUDES += ./jni/MPDPlaylist
LOCAL_C_INCLUDES += ./jni/include/
LOCAL_C_INCLUDES += ./jni/include/libstagefright
LOCAL_C_INCLUDES += ./jni/include/media
LOCAL_C_INCLUDES += ./jni


LOCAL_CFLAGS    := -Werror
LOCAL_LDLIBS := -ldl -llog -L./obj/local/armeabi -lutils -lstagefright -lstagefright_foundation -licuuc -lxml2

LOCAL_STATIC_LIBRARIES := libMPDPlaylist

include $(BUILD_SHARED_LIBRARY)

   include $(CLEAR_VARS)
   LOCAL_MODULE := stagefright-prebuilt
   LOCAL_SRC_FILES := ../obj/local/armeabi/libstagefright.so
include $(PREBUILT_SHARED_LIBRARY)
   

#include ./jni/M3U8Playlist/Android.mk
include ./jni/MPDPlaylist/Android.mk
