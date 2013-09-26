project "local_3ds"
  kind "SharedLib"
  language "C"

  configuration "windows"
    defines { "LIB3DSAPI=__declspec(dllexport)" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/liblocal_3ds_d.dylib" }
    targetdir "../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/liblocal_3ds.dylib" }
    targetdir "../../../lib/univ"

  configuration "not windows"
    buildoptions { "-w" }

  configuration "*"
    files { "src/**.h", "src/**.c" }
