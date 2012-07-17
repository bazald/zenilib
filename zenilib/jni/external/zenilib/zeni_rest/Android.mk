LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := zeni_rest
LOCAL_CPPFLAGS += -DTINYXML_DLL="" -DTINYXML_EXT="extern" -DZENI_DLL="" -DZENI_EXT="extern" -DZENI_AUDIO_DLL="" -DZENI_AUDIO_EXT="extern" -DZENI_CORE_DLL="" -DZENI_CORE_EXT="extern" -DZENI_GRAPHICS_DLL="" -DZENI_GRAPHICS_EXT="extern" -DZENI_REST_DLL="__attribute__ ((visibility(\"default\")))" -DZENI_REST_EXT=""
LOCAL_C_INCLUDES += . $(LOCAL_PATH)/../zeni_graphics $(LOCAL_PATH)/../zeni_core $(LOCAL_PATH)/../zeni_audio $(LOCAL_PATH)/../zeni $(LOCAL_PATH)/../../tinyxml $(LOCAL_PATH)/../../sdl_subst
#LOCAL_CPP_EXTENSION := .cxx
#LOCAL_SRC_FILES := src/zeni_rest.cxx
LOCAL_SRC_FILES := \
  Game.cpp \
  Gamestate.cpp \
  Gamestate_II.cpp \
  Logo.cpp \
  main.cpp \
  Widget.cpp \
  Widget_Gamestate.cpp
LOCAL_LDLIBS    := -landroid -llog

$(LOCAL_LIBRARIES_TYPE) := zeni_graphics zeni_core zeni_audio zeni tinyxml

include $($(BUILD_TYPE))
