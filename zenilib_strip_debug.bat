RMDIR /S /Q "Debug\"
RMDIR /S /Q "Debug SCU\"

DEL /Q "lib\*_d.lib"
DEL /Q "lib\x64\*_d.lib"

DEL /Q "*_d.dll"
DEL /Q "*_x64d.dll"
