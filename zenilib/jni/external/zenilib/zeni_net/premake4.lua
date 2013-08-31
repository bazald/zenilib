project "zeni_net"
  kind "SharedLib"
  language "C++"

  configuration { "windows" }
    defines { "ZENI_NET_DLL=__declspec(dllexport)", "ZENI_NET_EXT=" }
  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/libzeni_net_d.dylib" }
    targetdir "../../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/libzeni_net.dylib" }
    targetdir "../../../../lib/univ"

  configuration "*"
    flags { "ExtraWarnings" }
    includedirs { ".", "../zeni_core", "../zeni", "../../sdl_net", "../../sdl", "../../tinyxml" }

--     pchheader "jni/external/zenilib/zeni_net/zeni_net.h"
--     pchsource "jni/external/zenilib/zeni_net/Net.cpp"

    files { "**.h", "**.hxx", "**.cpp" }
    links { "zeni_core", "zeni", "local_SDL_net", "local_SDL" }
