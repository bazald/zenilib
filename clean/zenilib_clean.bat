RMDIR /S /Q "%~dp0..\Visual Studio 2008\SoarLibrary"
RMDIR /S /Q "%~dp0..\Visual Studio 2008\Win32"
RMDIR /S /Q "%~dp0..\Visual Studio 2008\x64"
RMDIR /S /Q "%~dp0..\Visual Studio 2010\SoarLibrary"
RMDIR /S /Q "%~dp0..\Visual Studio 2010\Win32"
RMDIR /S /Q "%~dp0..\Visual Studio 2010\x64"
RMDIR /S /Q "%~dp0..\Visual Studio 2010\ipch"

DEL /Q "%~dp0..\Visual Studio 2008\Application_Name.h"
DEL /Q "%~dp0..\Visual Studio 2010\Application_Name.h"

DEL /Q "%~dp0..\smltrace.txt"
DEL /Q "%~dp0..\stderr.txt"
DEL /Q "%~dp0..\stdout.txt"

DEL /Q "%~dp0..\Visual Studio 2008\*.ncb"
DEL /Q "%~dp0..\Visual Studio 2010\*.sdf"


RMDIR /S /Q "%~dp0..\Xcode\build"


DEL /Q "%~dp0..\*.dblite"

DEL /Q "%~dp0..\src\TinyXML\*.o"
DEL /Q "%~dp0..\src\Zeni\*.o"
DEL /Q "%~dp0..\src\*.o"

DEL /Q "%~dp0..\*.a"

DEL /Q "%~dp0..\bin\x64\*.exp"
DEL /Q "%~dp0..\bin\x64\*.ilk"
DEL /Q "%~dp0..\bin\x64\*.lib"
DEL /Q "%~dp0..\bin\x64\*.manifest"
DEL /Q "%~dp0..\bin\x64\*.map"
DEL /Q "%~dp0..\bin\x64\*.pdb"

DEL /Q "%~dp0..\bin\*.exp"
DEL /Q "%~dp0..\bin\*.ilk"
DEL /Q "%~dp0..\bin\*.lib"
DEL /Q "%~dp0..\bin\*.manifest"
DEL /Q "%~dp0..\bin\*.map"
DEL /Q "%~dp0..\bin\*.pdb"

DEL /Q "%~dp0..\src\TinyXML\*.obj"
DEL /Q "%~dp0..\src\Zeni\*.obj"
DEL /Q "%~dp0..\src\*.obj"

DEL /Q "%~dp0..\*.idb"
DEL /Q "%~dp0..\*.ilk"
DEL /Q "%~dp0..\*.lib"
DEL /Q "%~dp0..\*.manifest"
DEL /Q "%~dp0..\*.map"
DEL /Q "%~dp0..\*.pdb"

DEL /Q "%~dp0..\Visual Studio 2008\*.obj"
DEL /Q "%~dp0..\Visual Studio 2010\*.obj"
