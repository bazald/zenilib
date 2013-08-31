project "local_SDL_net"
  kind "SharedLib"
  language "C"

  configuration "windows"
    links { "wsock32", "iphlpapi" }
  configuration "linux"
    defines { "PACKAGE_NAME=\"\"",
              "PACKAGE_TARNAME=\"\"",
              "PACKAGE_VERSION=\"\"",
              "PACKAGE_STRING=\"\"",
              "PACKAGE_BUGREPORT=\"\"",
              "STDC_HEADERS=1",
              "HAVE_SYS_TYPES_H=1",
              "HAVE_SYS_STAT_H=1",
              "HAVE_STDLIB_H=1",
              "HAVE_STRING_H=1",
              "HAVE_MEMORY_H=1",
              "HAVE_STRINGS_H=1",
              "HAVE_INTTYPES_H=1",
              "HAVE_STDINT_H=1",
              "HAVE_UNISTD_H=1",
              "HAVE_DLFCN_H=1",
              "LT_OBJDIR=\".libs/\"",
              "PACKAGE=\"SDL_net\"",
              "VERSION=\"1.2.8\"" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/liblocal_SDL_net_d.dylib" }
    targetdir "../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/liblocal_SDL_net.dylib" }
    targetdir "../../../lib/univ"

  configuration "not windows"
    buildoptions { "-w" }

  configuration "*"
    includedirs { "SDL", "../sdl/SDL", "../sdl" }
    files { "include/**.h",
            "SDLnetUDP.c",
            "SDLnetselect.c",
            "SDLnetTCP.c",
            "SDLnet.c" }
    links { "local_SDL" }
