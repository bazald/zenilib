APPLICATION_NAME="game"

project(APPLICATION_NAME)
  kind "WindowedApp"
  language "C++"

  configuration "windows"
    prebuildcommands { "xcopy ..\\..\\dev\\pc_ ..\\.. /E /Y" }
  configuration { "macosx or linux" }
    prebuildcommands { "rsync -av --exclude '.*' ../../dev/pc_/ ../../" }
  if _OPTIONS.macosx ~= "native" then
    configuration { "macosx", "Debug*" }
      prebuildcommands { "/usr/libexec/PlistBuddy -c \"Set :LSMinimumSystemVersion ".._OPTIONS.macosx.."\" ../../Info_d.plist" }
    configuration { "macosx", "Release*" }
      prebuildcommands { "/usr/libexec/PlistBuddy -c \"Set :LSMinimumSystemVersion ".._OPTIONS.macosx.."\" ../../Info.plist" }
  else
    local ver=os.getversion()
    if ver.majorversion == 10 and ver.minorversion >= 6 then
      local macosx = string.format("%d.%d", ver.majorversion, ver.minorversion)

      configuration { "macosx", "Debug*" }
        prebuildcommands { "/usr/libexec/PlistBuddy -c \"Set :LSMinimumSystemVersion "..macosx.."\" ../../Info_d.plist" }
      configuration { "macosx", "Release*" }
        prebuildcommands { "/usr/libexec/PlistBuddy -c \"Set :LSMinimumSystemVersion "..macosx.."\" ../../Info.plist" }
    else
      configuration { "macosx", "Debug*" }
        prebuildcommands { "/usr/libexec/PlistBuddy -c \"Delete :LSMinimumSystemVersion\" ../../Info_d.plist" }
      configuration { "macosx", "Release*" }
        prebuildcommands { "/usr/libexec/PlistBuddy -c \"Delete :LSMinimumSystemVersion\" ../../Info.plist" }
    end
  end
  configuration { "macosx", "Debug*" }
    prebuildcommands { "mkdir -p ../../game_d.app/Contents",
                       "rsync -av --exclude '.*' ../../lib/univ_d/ ../../game_d.app/Contents/MacOS/",
                       "rsync -av --exclude '.*' --delete ../../assets/ ../../game_d.app/Contents/assets/",
                       "rsync -av --exclude '.*' --delete ../../Resources/ ../../game_d.app/Contents/Resources/",
                       "rsync -av --exclude '.*' ../../Info_d.plist ../../game_d.app/Contents/Info.plist" }
  configuration { "macosx", "Release*" }
    prebuildcommands { "mkdir -p ../../game.app/Contents",
                       "rsync -av --exclude '.*' ../../lib/univ/ ../../game.app/Contents/MacOS/",
                       "rsync -av --exclude '.*' --delete ../../assets/ ../../game.app/Contents/assets/",
                       "rsync -av --exclude '.*' --delete ../../Resources/ ../../game.app/Contents/Resources/",
                       "rsync -av --exclude '.*' ../../Info.plist ../../game.app/Contents/Info.plist" }
--   configuration { "linux" }
--     prelinkcommands { "../../dev/brandelf/brandelf -f 0 ../../lib/*/*.so" }

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

  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration "macosx"
    files { "../external/sdl/SDLmain/*.h",
            "../external/sdl/SDLmain/*.m" }
    links { "Cocoa.framework" }

  if _OPTIONS.build == "all" then
    configuration "windows or linux"
      links { "local_SDLmain" }
  else
    configuration { "windows or linux", "Debug*" }
      links { "local_SDLmain_d" }
    configuration { "windows or linux", "Release*" }
      links { "local_SDLmain" }
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
