project "local_z"
  kind "SharedLib"
  language "C"

  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @rpath/liblocal_z_d.dylib" }
    targetdir "../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @rpath/liblocal_z.dylib" }
    targetdir "../../../lib/univ"

  configuration "not windows"
    buildoptions { "-w" }

  configuration "*"
    files { "**.h",
            "adler32.c",
            "compress.c",
            "crc32.c",
            "gzclose.c",
            "gzlib.c",
            "gzread.c",
            "gzwrite.c",
            "uncompr.c",
            "deflate.c",
            "trees.c",
            "zutil.c",
            "inflate.c",
            "infback.c",
            "inftrees.c",
            "inffast.c"
    }
