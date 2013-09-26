APP_ABI := armeabi-v7a
APP_CFLAGS += -fexceptions -Wno-deprecated -Wall -DDISABLE_AL -DENABLE_SLES -DREQUIRE_GL_ES -DDISABLE_DX9 -DDISABLE_WGL -DDISABLE_CG -DTEMP_DISABLE
APP_CPPFLAGS += -std=gnu++0x -fexceptions -frtti -Wno-deprecated -Wall -DDISABLE_AL -DENABLE_SLES -DREQUIRE_GL_ES -DDISABLE_DX9 -DDISABLE_WGL -DDISABLE_CG -DTEMP_DISABLE
APP_PLATFORM := android-9
APP_LDFLAGS := -Wl,-Map,xxx.map

BUILD_TYPE := BUILD_SHARED_LIBRARY
LOCAL_LIBRARIES_TYPE := LOCAL_SHARED_LIBRARIES
APP_STL := gnustl_shared
APP_MODULES := application zeni_rest zeni_graphics zeni_core zeni_audio zeni lib3ds tinyxml freetype2 libpng libz gnustl_shared

ifneq ($(APP_OPTIM),release)
	APP_CFLAGS += -D_DEBUG
	APP_CPPFLAGS += -D_DEBUG

	BUILD_TYPE := BUILD_STATIC_LIBRARY
	LOCAL_LIBRARIES_TYPE := LOCAL_STATIC_LIBRARIES
	APP_STL := gnustl_static
	APP_MODULES := application
else
	APP_CFLAGS += -Os -DNDEBUG
	APP_CPPFLAGS += -Os -DNDEBUG
endif
