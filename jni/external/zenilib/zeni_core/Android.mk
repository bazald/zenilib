LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := zeni_core
LOCAL_CPPFLAGS += -DTINYXML_DLL="" -DTINYXML_EXT="extern" -DZENI_DLL="" -DZENI_EXT="extern" -DZENI_CORE_DLL="__attribute__ ((visibility(\"default\")))" -DZENI_CORE_EXT=""
LOCAL_C_INCLUDES += . $(LOCAL_PATH)/../zeni $(LOCAL_PATH)/../../tinyxml $(LOCAL_PATH)/../../sdl_subst
#LOCAL_CPP_EXTENSION := .cxx
#LOCAL_SRC_FILES := zeni_core.cxx
LOCAL_SRC_FILES := \
  Core.cpp \
  Joysticks.cpp \
  Timer.cpp
LOCAL_LDLIBS    := -landroid -llog

$(LOCAL_LIBRARIES_TYPE) := zeni tinyxml

include $($(BUILD_TYPE))
