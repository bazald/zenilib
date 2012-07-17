project "zeni_audio"
  kind "SharedLib"
  language "C++"

  configuration { "windows" }
    defines { "ZENI_AUDIO_DLL=__declspec(dllexport)", "ZENI_AUDIO_EXT=" }
  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration "linux"
    buildoptions { "-std=c++0x" }
  configuration { "macosx" }
    links { "OpenAL.framework" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @loader_path/libzeni_audio_d.dylib" }
    targetdir "../../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @loader_path/libzeni_audio.dylib" }
    targetdir "../../../../lib/univ"

  configuration "*"
    includedirs { ".", "../../libvorbis/include", "../../libogg/include", "../zeni", "../../sdl_net", "../../sdl", "../../tinyxml" }

--     pchheader "jni/external/zenilib/zeni_audio/zeni_audio.h"
--     pchsource "jni/external/zenilib/zeni_audio/Sound.cpp"

    files { "**.h", "**.cpp", "Zeni/**.h" }
    links { "local_vorbisfile", "zeni" }
