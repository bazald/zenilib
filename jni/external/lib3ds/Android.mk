LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := lib3ds
LOCAL_SRC_FILES := \
  src/lib3ds_atmosphere.c \
  src/lib3ds_background.c \
  src/lib3ds_camera.c \
  src/lib3ds_chunk.c \
  src/lib3ds_chunktable.c \
  src/lib3ds_file.c \
  src/lib3ds_io.c \
  src/lib3ds_light.c \
  src/lib3ds_material.c \
  src/lib3ds_math.c \
  src/lib3ds_matrix.c \
  src/lib3ds_mesh.c \
  src/lib3ds_node.c \
  src/lib3ds_quat.c \
  src/lib3ds_shadow.c \
  src/lib3ds_track.c \
  src/lib3ds_util.c \
  src/lib3ds_vector.c \
  src/lib3ds_viewport.c

include $($(BUILD_TYPE))
