project "zeni_core"
  kind "SharedLib"
  language "C++"

  configuration { "windows" }
    defines { "ZENI_CORE_DLL=__declspec(dllexport)", "ZENI_CORE_EXT=" }
  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/libzeni_core_d.dylib" }
    targetdir "../../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/libzeni_core.dylib" }
    targetdir "../../../../lib/univ"

  configuration "*"
    flags { "ExtraWarnings" }
    includedirs { ".", "../zeni", "../../sdl_net", "../../sdl", "../../tinyxml" }

--     pchheader "jni/external/zenilib/zeni_core/zeni_core.h"
--     pchsource "jni/external/zenilib/zeni_core/Core.cpp"

    files { "**.h", "**.hxx", "**.cpp" }
    links { "zeni", "local_SDL" }
