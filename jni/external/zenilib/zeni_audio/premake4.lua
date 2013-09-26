project "zeni_audio"
  kind "SharedLib"
  language "C++"

  configuration { "windows" }
    defines { "ZENI_AUDIO_DLL=__declspec(dllexport)", "ZENI_AUDIO_EXT=" }
  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration { "macosx" }
    links { "OpenAL.framework" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/libzeni_audio_d.dylib" }
    targetdir "../../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/libzeni_audio.dylib" }
    targetdir "../../../../lib/univ"

  configuration "*"
    flags { "ExtraWarnings" }
    includedirs { ".", "../../libvorbis/include", "../../libogg/include", "../zeni", "../../sdl_net", "../../sdl", "../../tinyxml" }

--     pchheader "jni/external/zenilib/zeni_audio/zeni_audio.h"
--     pchsource "jni/external/zenilib/zeni_audio/Sound.cpp"

    files { "**.h", "**.hxx", "**.cpp" }
    links { "local_vorbisfile", "zeni" }
