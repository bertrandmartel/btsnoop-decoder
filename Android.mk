LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -std=gnu++11
LOCAL_CPPFLAGS += -fexceptions

LOCAL_MODULE := btsnoopdecoder

BTSNOOP_DECODER_PATH := snoop-stream-lib/btsnoop

LOCAL_C_INCLUDES := $NDK/sources/cxx-stl/gnu-libstdc++/4.8/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(BTSNOOP_DECODER_PATH)/..

LOCAL_SRC_FILES := $(BTSNOOP_DECODER_PATH)/btsnoopfileinfo.cpp \
	$(BTSNOOP_DECODER_PATH)/btsnooppacket.cpp \
	$(BTSNOOP_DECODER_PATH)/btsnoopparser.cpp \
	$(BTSNOOP_DECODER_PATH)/btsnooptask.cpp

include $(BUILD_SHARED_LIBRARY)