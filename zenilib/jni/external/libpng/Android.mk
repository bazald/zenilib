LOCAL_PATH := $(call my-dir)

#
# Build libpng
#

include $(CLEAR_VARS)

common_CFLAGS := -fvisibility=hidden ## -fomit-frame-pointer
ifeq ($(HOST_OS),windows)
  ifeq ($(USE_MINGW),)
    # Case where we're building windows but not under linux (so it must be cygwin)
    # In this case, gcc cygwin doesn't recognize -fvisibility=hidden
    $(info libpng: Ignoring gcc flag $(common_CFLAGS) on Cygwin)
    common_CFLAGS := 
  endif
endif

LOCAL_MODULE:= libpng
LOCAL_CFLAGS += $(common_CFLAGS) -DPNG_IMPEXP="__attribute__((visibility(\"default\")))"
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../zlib
LOCAL_SRC_FILES := \
  png.c \
  pngerror.c \
  pngget.c \
  pngmem.c \
  pngpread.c \
  pngread.c \
  pngrio.c \
  pngrtran.c \
  pngrutil.c \
  pngset.c \
  pngtest.c \
  pngtrans.c \
  pngwio.c \
  pngwrite.c \
  pngwtran.c \
  pngwutil.c

$(LOCAL_LIBRARIES_TYPE) := libz

include $($(BUILD_TYPE))
