project "local_tinyxml"
  kind "SharedLib"
  language "C++"

  configuration "windows"
    defines { "TINYXML_DLL=__declspec(dllexport)" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/liblocal_tinyxml_d.dylib" }
    targetdir "../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/liblocal_tinyxml.dylib" }
    targetdir "../../../lib/univ"

  configuration "not windows"
    buildoptions { "-w" }

  configuration "*"
    includedirs { "." }
    files { "**.h", "**.cpp" }
