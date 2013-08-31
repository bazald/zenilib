project "local_GLEW"
  kind "SharedLib"
  language "C"

  configuration "windows"
    defines { "GLEW_BUILD" }
    links { "opengl32" }
  configuration "linux"
    links { "GL" }
  configuration "macosx"
    links { "OpenGL.framework" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/liblocal_GLEW_d.dylib" }
    targetdir "../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/liblocal_GLEW.dylib" }
    targetdir "../../../lib/univ"

  configuration "not windows"
    buildoptions { "-w" }

  configuration "*"
    includedirs { "include" }
    files { "include/**.h",
            "src/glew.c" }
