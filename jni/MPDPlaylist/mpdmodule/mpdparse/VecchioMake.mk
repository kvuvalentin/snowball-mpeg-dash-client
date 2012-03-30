# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");3

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

LOCAL_MODULE    := gstmpdparser
LOCAL_SRC_FILES := gstmpdparse.c  ../glibchelper/glibchelper.c ../glibchelper/glist.c ../mpdcommon/gstmpdcommon.c

#Note : find a more elegnat way to handle absolute path!!
LOCAL_C_INCLUDES += ./jni/libxml2/include
LOCAL_C_INCLUDES += ./jni/icu4c/common
LOCAL_C_INCLUDES += ./jni/mpdmodule/glibchelper
LOCAL_C_INCLUDES += ./
	

LOCAL_CFLAGS    := -Werror  -DDEBUG
LOCAL_LDLIBS := -ldl -llog -L./obj/local/armeabi -licuuc
#LOCAL_LDLIBS := -ldl -llog -L/home/manuele/ST_Workspace/android_tool/android-ndk-r7/samples/MPDParser/obj/local/armeabi -licuuc


LOCAL_STATIC_LIBRARIES := libxml2

#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true


#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)
