project "local_SDL"
  kind "SharedLib"
  language "C"

  configuration "windows"
    files {
            "src/audio/directsound/*.c",
            "src/audio/winmm/*.c",
            "src/audio/xaudio2/*.c",
            "src/core/windows/*.c",
            "src/filesystem/windows/*.c",
            "src/haptic/windows/*.c",
            "src/input/evdev/*.c",
            "src/joystick/windows/*.c",
            "src/loadso/windows/*.c",
            "src/power/windows/*.c",
            "src/render/direct3d/*.c",
            "src/thread/generic/SDL_syscond.c",
            "src/thread/windows/*.c",
            "src/timer/windows/*.c",
            "src/video/windows/*.c"
    }
    links { "dxguid", "Imm32", "Version", "Winmm" }
  configuration { "windows", "x32" }
    libdirs { "../lib/x32" }
  configuration { "windows", "x64" }
    libdirs { "../lib/x64" }
  configuration "linux"
    files { "src/core/linux/*.c",
            "src/filesystem/unix/*.c",
            "src/haptic/linux/*.c",
            "src/loadso/dlopen/*.c",
            "src/video/x11/*.c",
  --           "src/video/Xext/Xxf86dga/*.c",
  --           "src/video/dga/*.c",
  --           "src/video/Xext/Xxf86vm/*.c",
  --           "src/video/Xext/Xv/*.c",
  --           "src/video/Xext/Xinerama/*.c",
  --           "src/video/Xext/XME/*.c",
  --           "src/video/fbcon/*.c",
            "src/power/linux/*.c",
            "src/thread/pthread/*.c",
            "src/joystick/linux/*.c",
  --           "src/cdrom/linux/*.c",
            "src/timer/unix/*.c" }
    links { "m", "dl", "pthread", "X11", "Xext" }
  configuration "macosx"
    buildoptions { "-fvisibility=hidden",
                   "-I/usr/X11R6/include",
                   "-force_cpusubtype_ALL",
                   "-fpascal-strings",
                   "-Wall" }
    includedirs { "/usr/X11R6/include" }
    files { "src/audio/disk/*.c",
            "src/filesystem/cocoa/*.m",
            "src/loadso/dlopen/*.c",
            "src/file/cocoa/*.m",
            "src/video/cocoa/*.m",
            "src/video/quartz/*.m",
--            "src/video/x11/*.c",
--            "src/video/Xext/Xxf86dga/*.c",
--            "src/video/dga/*.c",
--            "src/video/Xext/Xxf86vm/*.c",
--            "src/video/Xext/Xv/*.c",
--            "src/video/Xext/Xinerama/*.c",
--            "src/video/Xext/XME/*.c",
            "src/thread/pthread/*.c",
            "src/audio/coreaudio/*.c",
            "src/audio/macosx/*.c",
            "src/haptic/darwin/*.c",
            "src/joystick/darwin/*.c",
            "src/power/macosx/*.c",
            "src/timer/unix/*.c" }
    links { "ApplicationServices.framework",
            "AudioUnit.framework",
            "Carbon.framework",
            "Cocoa.framework",
            "CoreAudio.framework",
            "ForceFeedback.framework",
            "IOKit.framework",
            "OpenGL.framework" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/liblocal_SDL_d.dylib" }
    targetdir "../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/liblocal_SDL.dylib" }
    targetdir "../../../lib/univ"

  configuration "not windows"
    buildoptions { "-w" }

  configuration "*"
    includedirs { "SDL" }
    files { "include/**.h",
            "src/*.c",
            "src/atomic/*.c",
            "src/audio/*.c",
            "src/audio/disk/*.c",
            "src/audio/dummy/*.c",
            "src/cpuinfo/*.c",
            "src/events/*.c",
            "src/file/*.c",
            "src/haptic/*.c",
            "src/joystick/*.c",
            "src/libm/*.c",
            "src/power/*.c",
            "src/render/*.c",
            "src/render/opengl/*.c",
            "src/render/software/*.c",
            "src/stdlib/*.c",
            "src/thread/*.c",
            "src/timer/*.c",
            "src/video/*.c",
            "src/video/dummy/*.c" }

if os.get() ~= "macosx" then
  project "local_SDLmain"
    kind "StaticLib"
    language "C"

    configuration "windows"
      files { "src/main/windows/*.c" }
    configuration "linux"
      files { "src/main/dummy/*.c" }

  configuration "not windows"
    buildoptions { "-w" }

  configuration "*"
    includedirs { ".", "SDL" }
end
