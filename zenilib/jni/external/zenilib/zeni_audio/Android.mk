LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := zeni_audio
LOCAL_CPPFLAGS += -DTINYXML_DLL="" -DTINYXML_EXT="extern" -DZENI_DLL="" -DZENI_EXT="extern" -DZENI_AUDIO_DLL="__attribute__ ((visibility(\"default\")))" -DZENI_AUDIO_EXT=""
LOCAL_C_INCLUDES += . $(LOCAL_PATH)/../zeni $(LOCAL_PATH)/../../tinyxml $(LOCAL_PATH)/../../sdl_subst
#LOCAL_CPP_EXTENSION := .cxx
#LOCAL_SRC_FILES := zeni_audio.cxx
LOCAL_SRC_FILES := \
  Sound.cpp \
  Sound_Buffer.cpp \
  Sound_Renderer_AL.cpp \
  Sound_Renderer_SLES.cpp \
  Sound_Source.cpp \
  Sound_Source_Pool.cpp \
  Sounds.cpp
LOCAL_LDLIBS := -landroid -llog -lOpenSLES

$(LOCAL_LIBRARIES_TYPE) := zeni tinyxml

include $($(BUILD_TYPE))
