project "local_angle"
  kind "SharedLib"
  language "C++"

  configuration "windows"
    defines { "TINYXML_DLL=__declspec(dllexport)" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/liblocal_angle_d.dylib" }
    targetdir "../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/liblocal_angle.dylib" }
    targetdir "../../../lib/univ"

  configuration "not windows"
    buildoptions { "-w" }
    files { "ossrc/compiler/ossource_posix.cpp" }
  configuration "windows"
    files { "ossrc/compiler/ossource_win.cpp" }

  configuration "*"
    defines { "COMPONENT_BUILD", "COMPILER_IMPLEMENTATION" }
    includedirs { "include", "src" }
    files { "src/common/**.cpp",
            "src/compiler/**.cpp",
            "src/third_party/compiler/*.cpp",
            "src/third_party/murmurhash/*.cpp" }
