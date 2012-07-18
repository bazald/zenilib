project "zeni_graphics"
  kind "SharedLib"
  language "C++"

  configuration "windows"
    defines { "ZENI_GRAPHICS_DLL=__declspec(dllexport)", "ZENI_GRAPHICS_EXT=" }
    links { "glu32", "opengl32" }
  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration "linux"
    buildoptions { "-std=c++0x" }
    links { "GLU" }
  configuration "macosx"
    links { "OpenGL.framework" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @loader_path/libzeni_graphics_d.dylib" }
    targetdir "../../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @loader_path/libzeni_graphics.dylib" }
    targetdir "../../../../lib/univ"

  configuration "windows"
    buildoptions { "/W4" }
  configuration "not windows"
    buildoptions { "-Wall",
                   "-Wextra" }

  configuration "*"
    includedirs { ".", "../../freetype2/include", "../../libpng", "../../libpng", "../../zlib", "../../lib3ds/src", "../zeni_core", "../zeni", "../../sdl_net", "../../sdl", "../../tinyxml" }

--     pchheader "jni/external/zenilib/zeni_graphics/zeni_graphics.h"
--     pchsource "jni/external/zenilib/zeni_graphics/Video.cpp"

    files { "**.h", "**.cpp", "Zeni/**.h" }
    links { "zeni_core", "zeni", "local_SDL", "local_GLEW", "local_freetype2", "local_png", "local_z", "local_3ds" }
