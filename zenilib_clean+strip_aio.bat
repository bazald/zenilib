RMDIR /S /Q "Visual Studio 2008\Debug\"
RMDIR /S /Q "Visual Studio 2008\Debug SCU\"
RMDIR /S /Q "Visual Studio 2008\Release\"
RMDIR /S /Q "Visual Studio 2008\Release SCU\"
RMDIR /S /Q "Visual Studio 2008\x64\"

DEL /Q "stderr.txt"
DEL /Q "stdout.txt"

DEL /Q "Visual Studio 2008\*.ncb"

DEL /Q /A:H "Visual Studio 2008\Zeniapp_2008.suo"

DEL /Q "Visual Studio 2008\*.user"

RMDIR /S /Q "lib_win\x64\"

RMDIR /S /Q "bin\x64\"

DEL /Q "bin\Zeniapp_d.exe"
DEL /Q "bin\x64\Zeniapp_x64.exe"
DEL /Q "bin\x64\Zeniapp_x64d.exe"
