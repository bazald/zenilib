project "local_vorbis"
  kind "SharedLib"
  language "C"

  configuration "windows"
    defines { "VORBIS_DLL=__declspec(dllexport)",
              "_USRDLL",
              "LIBVORBIS_EXPORTS" }
  configuration "linux"
    defines { "_REENTRANT",
              "HAVE_ALLOCA=1",
              "HAVE_ALLOCA_H=1",
              "HAVE_DLFCN_H=1",
              "HAVE_INTTYPES_H=1",
              "HAVE_MEMORY_H=1",
              "HAVE_STDINT_H=1",
              "HAVE_STDLIB_H=1",
              "HAVE_STRINGS_H=1",
              "HAVE_STRING_H=1",
              "HAVE_SYS_STAT_H=1",
              "HAVE_SYS_TYPES_H=1",
              "HAVE_UNISTD_H=1",
              "LT_OBJDIR=\\\".libs/\\\"",
              "PACKAGE=\\\"libvorbis\\\"",
              "PACKAGE_BUGREPORT=\\\"vorbis-dev@xiph.org\\\"",
              "PACKAGE_NAME=\\\"libvorbis\\\"",
              "PACKAGE_STRING=\\\"libvorbis=1.3.2\\\"",
              "PACKAGE_TARNAME=\\\"libvorbis\\\"",
              "PACKAGE_VERSION=\\\"1.3.2\\\"",
              "STDC_HEADERS=1",
              "VERSION=\\\"1.3.2\\\"" }
  configuration "macosx"
    defines { "_REENTRANT",
              "HAVE_ALLOCA=1",
              "HAVE_ALLOCA_H=1",
              "HAVE_DLFCN_H=1",
              "HAVE_INTTYPES_H=1",
              "HAVE_MEMORY_H=1",
              "HAVE_STDINT_H=1",
              "HAVE_STDLIB_H=1",
              "HAVE_STRINGS_H=1",
              "HAVE_STRING_H=1",
              "HAVE_SYS_STAT_H=1",
              "HAVE_SYS_TYPES_H=1",
              "HAVE_UNISTD_H=1",
              "LT_OBJDIR=\\\".libs/\\\"",
              "PACKAGE=\\\"libvorbis\\\"",
              "PACKAGE_BUGREPORT=\\\"vorbis-dev@xiph.org\\\"",
              "PACKAGE_NAME=\\\"libvorbis\\\"",
              "PACKAGE_STRING=\\\"libvorbis=1.3.2\\\"",
              "PACKAGE_TARNAME=\\\"libvorbis\\\"",
              "PACKAGE_VERSION=\\\"1.3.2\\\"",
              "STDC_HEADERS=1",
              "VERSION=\\\"1.3.2\\\"" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/liblocal_vorbis_d.dylib" }
    targetdir "../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/liblocal_vorbis.dylib" }
    targetdir "../../../lib/univ"

  configuration "not windows"
    buildoptions { "-w" }

  configuration "*"
    includedirs { ".", "include", "../libogg/include" }
    files { "include/**.h",
            "lib/mdct.c",
            "lib/smallft.c",
            "lib/block.c",
            "lib/envelope.c",
            "lib/window.c",
            "lib/lsp.c",
            "lib/lpc.c",
            "lib/analysis.c",
            "lib/synthesis.c",
            "lib/psy.c",
            "lib/info.c",
            "lib/floor1.c",
            "lib/floor0.c",
            "lib/res0.c",
            "lib/mapping0.c",
            "lib/registry.c",
            "lib/codebook.c",
            "lib/sharedbook.c",
            "lib/lookup.c",
            "lib/bitrate.c" }
    links { "local_ogg" }

project "local_vorbisfile"
  kind "SharedLib"
  language "C"

  configuration "windows"
    defines { "VORBISFILE_DLL=__declspec(dllexport)",
              "_USRDLL",
              "LIBVORBISFILE_EXPORTS" }
  configuration "linux"
    defines { "_REENTRANT",
              "HAVE_ALLOCA=1",
              "HAVE_ALLOCA_H=1",
              "HAVE_DLFCN_H=1",
              "HAVE_INTTYPES_H=1",
              "HAVE_MEMORY_H=1",
              "HAVE_STDINT_H=1",
              "HAVE_STDLIB_H=1",
              "HAVE_STRINGS_H=1",
              "HAVE_STRING_H=1",
              "HAVE_SYS_STAT_H=1",
              "HAVE_SYS_TYPES_H=1",
              "HAVE_UNISTD_H=1",
              "LT_OBJDIR=\\\".libs/\\\"",
              "PACKAGE=\\\"libvorbis\\\"",
              "PACKAGE_BUGREPORT=\\\"vorbis-dev@xiph.org\\\"",
              "PACKAGE_NAME=\\\"libvorbis\\\"",
              "PACKAGE_STRING=\\\"libvorbis=1.3.2\\\"",
              "PACKAGE_TARNAME=\\\"libvorbis\\\"",
              "PACKAGE_VERSION=\\\"1.3.2\\\"",
              "STDC_HEADERS=1",
              "VERSION=\\\"1.3.2\\\"" }
  configuration "macosx"
    defines { "_REENTRANT",
              "HAVE_ALLOCA=1",
              "HAVE_ALLOCA_H=1",
              "HAVE_DLFCN_H=1",
              "HAVE_INTTYPES_H=1",
              "HAVE_MEMORY_H=1",
              "HAVE_STDINT_H=1",
              "HAVE_STDLIB_H=1",
              "HAVE_STRINGS_H=1",
              "HAVE_STRING_H=1",
              "HAVE_SYS_STAT_H=1",
              "HAVE_SYS_TYPES_H=1",
              "HAVE_UNISTD_H=1",
              "LT_OBJDIR=\\\".libs/\\\"",
              "PACKAGE=\\\"libvorbis\\\"",
              "PACKAGE_BUGREPORT=\\\"vorbis-dev@xiph.org\\\"",
              "PACKAGE_NAME=\\\"libvorbis\\\"",
              "PACKAGE_STRING=\\\"libvorbis=1.3.2\\\"",
              "PACKAGE_TARNAME=\\\"libvorbis\\\"",
              "PACKAGE_VERSION=\\\"1.3.2\\\"",
              "STDC_HEADERS=1",
              "VERSION=\\\"1.3.2\\\"" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/liblocal_vorbisfile_d.dylib" }
    targetdir "../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/liblocal_vorbisfile.dylib" }
    targetdir "../../../lib/univ"

  configuration "not windows"
    buildoptions { "-w" }

  configuration "*"
    includedirs { ".", "include", "../libogg/include" }
    files { "include/**.h",
            "lib/vorbisfile.c" }
    links { "local_vorbis", "local_ogg" }
