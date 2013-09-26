project "zeni"
  kind "SharedLib"
  language "C++"

  configuration "windows"
    defines { "ZENI_DLL=__declspec(dllexport)", "ZENI_EXT=" }
  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration "macosx"
    files { "**.mm" }
    links { "Cocoa.framework" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/libzeni_d.dylib" }
    targetdir "../../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/libzeni.dylib" }
    targetdir "../../../../lib/univ"

  configuration "*"
    flags { "ExtraWarnings" }
    includedirs { ".", "../../sdl_net", "../../sdl", "../../tinyxml" }

--     pchheader "jni/external/zenilib/zeni/zeni.h"
--     pchsource "jni/external/zenilib/zeni/String.cpp"

    files { "**.h", "**.hxx", "**.cpp" }
    links { "local_tinyxml" }
