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
