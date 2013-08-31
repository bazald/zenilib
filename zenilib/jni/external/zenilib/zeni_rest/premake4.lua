project "zeni_rest"
  kind "SharedLib"
  language "C++"

  configuration { "windows" }
    defines { "ZENI_REST_DLL=__declspec(dllexport)", "ZENI_REST_EXT=" }
  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/libzeni_rest_d.dylib" }
    targetdir "../../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/libzeni_rest.dylib" }
    targetdir "../../../../lib/univ"

  configuration "*"
    flags { "ExtraWarnings" }
    includedirs { ".", "../zeni_graphics", "../zeni_core", "../../libvorbis/include", "../../libogg/include", "../zeni_audio", "../zeni", "../../sdl_net", "../../sdl", "../../tinyxml" }

--     pchheader "jni/external/zenilib/zeni_rest/zeni_rest.h"
--     pchsource "jni/external/zenilib/zeni_rest/Gamestate.cpp"

    files { "**.h", "**.hxx", "**.cpp" }
    links { "zeni_graphics", "zeni_core", "zeni_audio", "zeni", "local_SDL", "local_SDL_net" }
