APPLICATION_NAME="game"

project(APPLICATION_NAME)
  kind "WindowedApp"
  language "C++"

  local rebase_i = function (dir) return path.rebase(dir, '.', _OPTIONS.dir..'/dir'); end
  local rebase = nil
  if os.get() == "windows" then
    rebase = function (dir) a,b=string.gsub(rebase_i(dir), '/', '\\'); return a; end
  else
    rebase = rebase_i
  end

  os.chdir('..')
  os.chdir('..')

  configuration "windows"
    prebuildcommands { "xcopy "..rebase("dev/pc_").." "..rebase("").." /E /Y" }
  configuration { "windows", "Debug*", "x32" }
    prebuildcommands { "xcopy "..rebase("jni/external/bin/x32").." "..rebase("bin/d32").." /E /Y" }
  configuration { "windows", "Debug*", "x64" }
    prebuildcommands { "xcopy "..rebase("jni/external/bin/x64").." "..rebase("bin/d64").." /E /Y" }
  configuration { "windows", "Release*", "x32" }
    prebuildcommands { "xcopy "..rebase("jni/external/bin/x32").." "..rebase("bin/x32").." /E /Y" }
  configuration { "windows", "Release*", "x64" }
    prebuildcommands { "xcopy "..rebase("jni/external/bin/x64").." "..rebase("bin/x64").." /E /Y" }
  configuration { "macosx or linux" }
    prebuildcommands { "rsync -av --exclude '.*' "..rebase("dev/pc_").."/ "..rebase("") }
  if _OPTIONS.macosx ~= "native" then
    configuration { "macosx", "Debug*" }
      prebuildcommands { "/usr/libexec/PlistBuddy -c \"Set :LSMinimumSystemVersion ".._OPTIONS.macosx.."\" "..rebase("Info_d.plist") }
    configuration { "macosx", "Release*" }
      prebuildcommands { "/usr/libexec/PlistBuddy -c \"Set :LSMinimumSystemVersion ".._OPTIONS.macosx.."\" "..rebase("Info.plist") }
  else
    local ver=os.getversion()
    if ver.majorversion == 10 and ver.minorversion >= 6 then
      local macosx = string.format("%d.%d", ver.majorversion, ver.minorversion)

      configuration { "macosx", "Debug*" }
        prebuildcommands { "/usr/libexec/PlistBuddy -c \"Set :LSMinimumSystemVersion "..macosx.."\" "..rebase("Info_d.plist") }
      configuration { "macosx", "Release*" }
        prebuildcommands { "/usr/libexec/PlistBuddy -c \"Set :LSMinimumSystemVersion "..macosx.."\" "..rebase("Info.plist") }
    else
      configuration { "macosx", "Debug*" }
        prebuildcommands { "/usr/libexec/PlistBuddy -c \"Delete :LSMinimumSystemVersion\" "..rebase("Info_d.plist") }
      configuration { "macosx", "Release*" }
        prebuildcommands { "/usr/libexec/PlistBuddy -c \"Delete :LSMinimumSystemVersion\" "..rebase("Info.plist") }
    end
  end
  configuration { "macosx", "Debug*" }
    prebuildcommands { "mkdir -p "..rebase("game_d.app/Contents"),
                       "rsync -av --exclude '.*' "..rebase("lib/univ_d").."/ "..rebase("game_d.app/Contents/MacOS"),
                       "rsync -av --exclude '.*' --delete "..rebase("assets").."/ "..rebase("game_d.app/Contents/assets"),
                       "rsync -av --exclude '.*' --delete "..rebase("Resources").."/ "..rebase("game_d.app/Contents/Resources"),
                       "rsync -av --exclude '.*' "..rebase("Info_d.plist").." "..rebase("game_d.app/Contents/Info.plist") }
  configuration { "macosx", "Release*" }
    prebuildcommands { "mkdir -p "..rebase("game.app/Contents"),
                       "rsync -av --exclude '.*' "..rebase("lib/univ").."/ "..rebase("game.app/Contents/MacOS"),
                       "rsync -av --exclude '.*' --delete "..rebase("assets").."/ "..rebase("game.app/Contents/assets"),
                       "rsync -av --exclude '.*' --delete "..rebase("Resources").."/ "..rebase("game.app/Contents/Resources"),
                       "rsync -av --exclude '.*' "..rebase("Info.plist").." "..rebase("game.app/Contents/Info.plist") }
--   configuration { "linux" }
--     prelinkcommands { rebase("dev/brandelf/brandelf").." -f 0 "..rebase("lib/*/*.so") }

  configuration { "linux", "Debug*", "x32" }
    linkoptions { "-Wl,-rpath,'$$ORIGIN/lib/d32'", "-Wl,-rpath-link,"..rebase("lib/d32") }
  configuration { "linux", "Release*", "x32" }
    linkoptions { "-Wl,-rpath,'$$ORIGIN/lib/x32'", "-Wl,-rpath-link,"..rebase("lib/x32") }
  configuration { "linux", "Debug*", "x64" }
    linkoptions { "-Wl,-rpath,'$$ORIGIN/lib/d64'", "-Wl,-rpath-link,"..rebase("lib/d64") }
  configuration { "linux", "Release*", "x64" }
    linkoptions { "-Wl,-rpath,'$$ORIGIN/lib/x64'", "-Wl,-rpath-link,"..rebase("lib/x64") }
  configuration "macosx"
    linkoptions { "-Wl,-rpath,'@loader_path/'" }

  os.chdir('jni')
  os.chdir('application')

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

  configuration "windows"
    links { "glu32", "opengl32" }
  configuration "linux or macosx"
    buildoptions { "-ffast-math", "-fpch-preprocess", "-Wall" }
  configuration "linux"
    links { "GL", "GLU" }
  configuration "macosx"
    files { "../external/sdl/SDLmain/*.h",
            "../external/sdl/SDLmain/*.mm" }
    links { "Cocoa.framework", "OpenGL.framework" }

  if _OPTIONS.build == "all" then
    configuration "windows or linux"
      links { "local_SDLmain" }
  else
    configuration { "windows or linux", "Debug*" }
      links { "local_SDLmain_d" }
    configuration { "windows or linux", "Release*" }
      links { "local_SDLmain" }
    configuration "Debug*"
      links { "zeni_rest_d", "zeni_graphics_d", "zeni_net_d", "zeni_core_d", "zeni_audio_d", "zeni_d", "local_GLEW_d", "local_SDL_d" }
    configuration "Release*"
      links { "zeni_rest", "zeni_graphics", "zeni_net", "zeni_core", "zeni_audio", "zeni", "local_GLEW", "local_SDL" }
  end

  configuration "*"
    flags { "ExtraWarnings" }
    includedirs { ".", "../external/zenilib", "../external/zenilib/zeni_rest", "../external/zenilib/zeni_graphics", "../external/zenilib/zeni_net", "../external/zenilib/zeni_core", "../external/zenilib/zeni_audio", "../external/libvorbis/include", "../external/libogg/include", "../external/zenilib/zeni", "../external/sdl_net", "../external/sdl", "../external/tinyxml" }

--     pchheader "jni/external/zenilib/zenilib.h"
--     pchsource "jni/application/bootstrap.cpp"

    files { "**.h", "**.hxx", "**.cpp" }

    if _OPTIONS.build == "all" then
      links { "zeni_rest", "zeni_graphics", "zeni_net", "zeni_core", "zeni_audio", "zeni", "local_GLEW", "local_SDL" }
    else
      files { "../external/zenilib/**.h" }
    end
