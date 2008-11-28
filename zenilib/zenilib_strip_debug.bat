RMDIR /S /Q "Visual Studio 2008\Debug\"
RMDIR /S /Q "Visual Studio 2008\Debug SCU\"

DEL /Q "lib_win\*_d.lib"
DEL /Q "lib_win\x64\*_d.lib"

DEL /Q "bin\*_d.dll"
DEL /Q "bin\x64\*_x64d.dll"

DEL /Q "bin\Zeniapp_d.exe"
DEL /Q "bin\x64\Zeniapp_x64d.exe"
