LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := zeni_graphics
LOCAL_CPPFLAGS += -DTINYXML_DLL="" -DTINYXML_EXT="extern" -DZENI_DLL="" -DZENI_EXT="extern" -DZENI_CORE_DLL="" -DZENI_CORE_EXT="extern" -DZENI_GRAPHICS_DLL="__attribute__ ((visibility(\"default\")))" -DZENI_GRAPHICS_EXT=""
LOCAL_C_INCLUDES += . $(LOCAL_PATH)/../zeni_core $(LOCAL_PATH)/../zeni $(LOCAL_PATH)/../../tinyxml $(LOCAL_PATH)/../../freetype2/include $(LOCAL_PATH)/../../libpng $(LOCAL_PATH)/../../sdl_subst
#LOCAL_CPP_EXTENSION := .cxx
#LOCAL_SRC_FILES := src/zeni_graphics.cxx
LOCAL_SRC_FILES := \
  EZ2D.cpp \
  Fog.cpp \
  Font.cpp \
  Fonts.cpp \
  Image.cpp \
  Light.cpp \
  Material.cpp \
  Model.cpp \
  Projector.cpp \
  Renderable.cpp \
  Shader.cpp \
  Texture.cpp \
  Textures.cpp \
  Vertex2f.cpp \
  Vertex3f.cpp \
  Vertex_Buffer.cpp \
  Video.cpp \
  Video_DX9.cpp \
  Video_GL.cpp \
  Window.cpp
LOCAL_LDLIBS    := -landroid -lEGL -lGLESv1_CM -llog

$(LOCAL_LIBRARIES_TYPE) := zeni_core zeni tinyxml freetype2 libpng

include $($(BUILD_TYPE))
