RMDIR /S /Q "SoarLibrary"
RMDIR /S /Q "Win32"
RMDIR /S /Q "x64"

DEL /Q "..\smltrace.txt"
DEL /Q "..\stderr.txt"
DEL /Q "..\stdout.txt"

DEL /Q "*.ncb"


DEL /Q /A:H "*.suo"

DEL /Q "*.user"