LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libz
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS += -O3 -DUSE_MMAP
LOCAL_SRC_FILES := \
  adler32.c \
  compress.c \
  crc32.c \
  gzclose.c \
  gzlib.c \
  gzread.c \
  gzwrite.c \
  uncompr.c \
  deflate.c \
  trees.c \
  zutil.c \
  inflate.c \
  infback.c \
  inftrees.c \
  inffast.c
ifneq ($(TARGET_ARCH),x86)
  LOCAL_NDK_VERSION := 5
  LOCAL_SDK_VERSION := 9
endif

include $($(BUILD_TYPE))
