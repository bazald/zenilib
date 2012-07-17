LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := zeni
LOCAL_CPPFLAGS += -DTINYXML_DLL="" -DTINYXML_EXT="extern" -DZENI_DLL="__attribute__ ((visibility(\"default\")))" -DZENI_EXT=""
LOCAL_C_INCLUDES += . $(LOCAL_PATH)/../../tinyxml $(LOCAL_PATH)/../../sdl_subst
#LOCAL_CPP_EXTENSION := .cxx
#LOCAL_SRC_FILES := zeni.cxx
LOCAL_SRC_FILES := \
  Camera.cpp \
  Collision.cpp \
  Color.cpp \
  Colors.cpp \
  Coordinate.cpp \
  File_Ops.cpp \
  Matrix4f.cpp \
  Quaternion.cpp \
  Quit_Event.cpp \
  Random.cpp \
  Resource.cpp \
  Serialization.cpp \
  String.cpp \
  Timer_HQ.cpp \
  Vector2f.cpp \
  Vector3f.cpp \
  XML.cpp
LOCAL_LDLIBS    := -landroid -llog

$(LOCAL_LIBRARIES_TYPE) := tinyxml

include $($(BUILD_TYPE))
