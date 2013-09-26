if os.get() == "windows" then
  project("Play "..APPLICATION_NAME)
    kind "WindowedApp"
    language "C"

    targetdir "../.."

    configuration { "Debug*", "x32" }
      targetsuffix " x32 Debug"
    configuration { "Release*", "x32" }
      targetsuffix " x32"
    configuration { "Debug*", "x64" }
      targetsuffix " x64 Debug"
    configuration { "Release*", "x64" }
      targetsuffix " x64"

    configuration "*"
      flags { "ExtraWarnings", "StaticRuntime" }
      defines { "EXECUTABLE_NAME=\""..APPLICATION_NAME..".exe\"" }
      files { "Launcher.c", "Launcher.rc" }
end
