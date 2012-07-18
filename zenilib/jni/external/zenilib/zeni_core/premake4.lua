project "zeni_core"
  kind "SharedLib"
  language "C++"

  configuration { "windows" }
    defines { "ZENI_CORE_DLL=__declspec(dllexport)", "ZENI_CORE_EXT=" }
  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration "linux"
    buildoptions { "-std=c++0x" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @loader_path/libzeni_core_d.dylib" }
    targetdir "../../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @loader_path/libzeni_core.dylib" }
    targetdir "../../../../lib/univ"

  configuration "windows"
    buildoptions { "/W4" }
  configuration "not windows"
    buildoptions { "-Wall",
                   "-Wextra" }

  configuration "*"
    includedirs { ".", "../zeni", "../../sdl_net", "../../sdl", "../../tinyxml" }

--     pchheader "jni/external/zenilib/zeni_core/zeni_core.h"
--     pchsource "jni/external/zenilib/zeni_core/Core.cpp"

    files { "**.h", "**.cpp", "Zeni/**.h" }
    links { "zeni", "local_SDL" }
