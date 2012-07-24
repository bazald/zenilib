APPLICATION_NAME="game"

project(APPLICATION_NAME)
  kind "WindowedApp"
  language "C++"

  configuration "windows"
    prebuildcommands { "xcopy ..\\..\\dev\\pc_ ..\\.. /E /Y" }
  configuration { "macosx or linux" }
    prebuildcommands { "rsync -av ../../dev/pc_/ ../../" }
  configuration { "macosx", "Debug*" }
    prebuildcommands { "mkdir -p ../../game_d.app/Contents",
                       "rsync -av ../../lib/univ_d/ ../../game_d.app/Contents/MacOS/",
                       "rsync -av --delete ../../assets/ ../../game_d.app/Contents/assets/",
                       "rsync -av --delete ../../Resources/ ../../game_d.app/Contents/Resources/",
                       "rsync -av ../../Info_d.plist ../../game_d.app/Contents/Info.plist" }
  configuration { "macosx", "Release*" }
    prebuildcommands { "mkdir -p ../../game.app/Contents",
                       "rsync -av ../../lib/univ/ ../../game.app/Contents/MacOS/",
                       "rsync -av --delete ../../assets/ ../../game.app/Contents/assets/",
                       "rsync -av --delete ../../Resources/ ../../game.app/Contents/Resources/",
                       "rsync -av ../../Info.plist ../../game.app/Contents/Info.plist" }
  configuration { "linux" }
    prelinkcommands { "../../dev/brandelf/brandelf -f 0 ../../lib/*/*.so" }

  configuration "linux or macosx"
    targetdir "../.."
  configuration { "linux", "Debug*", "x32" }
    targetsuffix "_d32"
  configuration { "linux", "Release*", "x32" }
    targetsuffix "_x32"
  configuration { "linux", "Debug*", "x64" }
    targetsuffix "_d64"
  configuration { "linux", "Release*", "x64" }
    targetsuffix "_x64"
  configuration { "macosx", "Debug*" }
    targetname(APPLICATION_NAME.."_d")
    targetsuffix ""
  configuration { "windows", "Debug*" }
    targetname(APPLICATION_NAME)
    targetsuffix ""

--   os.copydir("assets_shared", "assets")
--   os.copydir("assets_pc", "assets")

  configuration { "linux", "Debug*", "x32" }
    linkoptions { "-Wl,-rpath,'$$ORIGIN/lib/d32'", "-Wl,-rpath-link,../../lib/d32" }
  configuration { "linux", "Release*", "x32" }
    linkoptions { "-Wl,-rpath,'$$ORIGIN/lib/x32'", "-Wl,-rpath-link,../../lib/x32" }
  configuration { "linux", "Debug*", "x64" }
    linkoptions { "-Wl,-rpath,'$$ORIGIN/lib/d64'", "-Wl,-rpath-link,../../lib/d64" }
  configuration { "linux", "Release*", "x64" }
    linkoptions { "-Wl,-rpath,'$$ORIGIN/lib/x64'", "-Wl,-rpath-link,../../lib/x64" }

  configuration "windows or linux"
    links { "local_SDLmain" }
  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration "macosx"
    files { "../external/sdl/SDLmain/*.h",
            "../external/sdl/SDLmain/*.m" }
    links { "Cocoa.framework" }

  if _OPTIONS.build == "mine" then
    configuration "Debug*"
      links { "zeni_rest_d", "zeni_graphics_d", "zeni_net_d", "zeni_core_d", "zeni_audio_d", "zeni_d", "local_SDL_d" }
    configuration "Release*"
      links { "zeni_rest", "zeni_graphics", "zeni_net", "zeni_core", "zeni_audio", "zeni", "local_SDL" }
  end

  configuration "*"
    flags { "ExtraWarnings" }
    includedirs { ".", "../external/zenilib", "../external/zenilib/zeni_rest", "../external/zenilib/zeni_graphics", "../external/zenilib/zeni_net", "../external/zenilib/zeni_core", "../external/zenilib/zeni_audio", "../external/libvorbis/include", "../external/libogg/include", "../external/zenilib/zeni", "../external/sdl_net", "../external/sdl", "../external/tinyxml" }

--     pchheader "jni/external/zenilib/zenilib.h"
--     pchsource "jni/application/bootstrap.cpp"

    files { "**.h", "**.hxx", "**.cpp" }

    if _OPTIONS.build == "all" then
      links { "zeni_rest", "zeni_graphics", "zeni_net", "zeni_core", "zeni_audio", "zeni", "local_SDL" }
    end
