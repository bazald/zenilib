LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := application
LOCAL_CPPFLAGS += -DTINYXML_DLL="" -DTINYXML_EXT="extern" -DZENI_DLL="" -DZENI_EXT="extern" -DZENI_AUDIO_DLL="" -DZENI_AUDIO_EXT="extern" -DZENI_CORE_DLL="" -DZENI_CORE_EXT="extern" -DZENI_GRAPHICS_DLL="" -DZENI_GRAPHICS_EXT="extern" -DZENI_NET_DLL="" -DZENI_NET_EXT="extern" -DZENI_REST_DLL="" -DZENI_REST_EXT="extern"
LOCAL_C_INCLUDES += \
  $(LOCAL_PATH)/../external/zenilib \
  $(LOCAL_PATH)/../external/zenilib/zeni_rest \
  $(LOCAL_PATH)/../external/zenilib/zeni_net \
  $(LOCAL_PATH)/../external/zenilib/zeni_graphics \
  $(LOCAL_PATH)/../external/zenilib/zeni_core \
  $(LOCAL_PATH)/../external/zenilib/zeni_audio \
  $(LOCAL_PATH)/../external/zenilib/zeni \
  $(LOCAL_PATH)/../external/tinyxml \
  $(LOCAL_PATH)/../external/sdl_subst
LOCAL_SRC_FILES := ./main.cpp
LOCAL_LDLIBS := -landroid -lEGL -lGLESv1_CM -llog
#-lGLESv2

$(LOCAL_LIBRARIES_TYPE) := zeni_rest zeni_graphics zeni_core zeni_audio zeni

ifeq ($(LOCAL_LIBRARIES_TYPE),LOCAL_STATIC_LIBRARIES)
  LOCAL_LDLIBS += -lOpenSLES
	$(LOCAL_LIBRARIES_TYPE) += 3ds tinyxml freetype2 png z
endif

LOCAL_STATIC_LIBRARIES += android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
