project "zeni"
  kind "SharedLib"
  language "C++"

  configuration "windows"
    defines { "ZENI_DLL=__declspec(dllexport)", "ZENI_EXT=" }
  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration "linux"
    buildoptions { "-std=c++0x" }
  configuration "macosx"
    files { "**.mm" }
    links { "Cocoa.framework" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @loader_path/libzeni_d.dylib" }
    targetdir "../../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @loader_path/libzeni.dylib" }
    targetdir "../../../../lib/univ"

  configuration "windows"
    buildoptions { "/W4" }
  configuration "not windows"
    buildoptions { "-Wall",
                   "-Wextra",
                   "-Wno-ignored-qualifiers" }

  configuration "*"
    includedirs { ".", "../../sdl_net", "../../sdl", "../../tinyxml" }

--     pchheader "jni/external/zenilib/zeni/zeni.h"
--     pchsource "jni/external/zenilib/zeni/String.cpp"

    files { "**.h", "**.cpp", "Zeni/**.h" }
    links { "local_tinyxml" }
