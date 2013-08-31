project "local_ogg"
  kind "SharedLib"
  language "C"

  configuration "windows"
    defines { "OGG_DLL=__declspec(dllexport)" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/liblocal_ogg_d.dylib" }
    targetdir "../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/liblocal_ogg.dylib" }
    targetdir "../../../lib/univ"

  configuration "not windows"
    buildoptions { "-w" }

  configuration "*"
    includedirs { "include" }
    files { "include/**.h",
            "src/**.c" }
