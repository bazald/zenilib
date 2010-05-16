RMDIR /S /Q "%~dp0..\Visual Studio 2008\x64"
RMDIR /S /Q "%~dp0..\Visual Studio 2010\x64"

RMDIR /S /Q "%~dp0..\bin\x64"
RMDIR /S /Q "%~dp0..\lib_win\x64"

DEL /Q "%~dp0..\*_x64.exe"
DEL /Q "%~dp0..\*_x64d.exe"
DEL /Q "%~dp0..\*_x64"
DEL /Q "%~dp0..\*_x64d"
