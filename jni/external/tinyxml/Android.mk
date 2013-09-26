LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := tinyxml
LOCAL_CPPFLAGS += -DTINYXML_DLL="__attribute__ ((visibility(\"default\")))" -DTINYXML_EXT=""
LOCAL_CPP_EXTENSION := .cxx
LOCAL_SRC_FILES := tinyxml.cxx

include $($(BUILD_TYPE))
