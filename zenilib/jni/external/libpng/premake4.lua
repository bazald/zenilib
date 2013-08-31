project "local_png"
  kind "SharedLib"
  language "C"

  configuration "windows"
    defines { "_USRDLL" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/liblocal_png_d.dylib" }
    targetdir "../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/liblocal_png.dylib" }
    targetdir "../../../lib/univ"

  configuration "not windows"
    buildoptions { "-w" }

  configuration "*"
    includedirs { "../zlib" }
    files { "**.h", "png*.c" }
    links { "local_z" }
