project "zeni_graphics"
  kind "SharedLib"
  language "C++"

  configuration "windows"
    defines { "ZENI_GRAPHICS_DLL=__declspec(dllexport)", "ZENI_GRAPHICS_EXT=" }
    links { "glu32", "opengl32" }
  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration "linux"
    links { "GLU" }
  configuration "macosx"
    links { "OpenGL.framework" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/libzeni_graphics_d.dylib" }
    targetdir "../../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/libzeni_graphics.dylib" }
    targetdir "../../../../lib/univ"

  configuration "*"
    flags { "ExtraWarnings" }
    includedirs { ".", "../../freetype2/include", "../../libpng", "../../libpng", "../../zlib", "../../lib3ds/src", "../zeni_core", "../zeni", "../../sdl_net", "../../sdl", "../../tinyxml", "../../angle/include" }

--     pchheader "jni/external/zenilib/zeni_graphics/zeni_graphics.h"
--     pchsource "jni/external/zenilib/zeni_graphics/Video.cpp"

    files { "**.h", "**.hxx", "**.cpp" }
    links { "zeni_core", "zeni", "local_SDL", "local_GLEW", "local_freetype2", "local_png", "local_z", "local_angle", "local_3ds" }
