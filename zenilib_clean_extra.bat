RMDIR /S /Q "Debug\"
RMDIR /S /Q "Debug SCU\"
RMDIR /S /Q "Release\"
RMDIR /S /Q "Release SCU\"
RMDIR /S /Q "x64\"

DEL /Q "stderr.txt"
DEL /Q "stdout.txt"

DEL /Q "*.ncb"

DEL /Q /A:H "Zeniapp_2008.suo"
DEL /Q /A:H "Zeniapp_2005.suo"

DEL /Q "*.user"
