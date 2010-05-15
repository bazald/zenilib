RMDIR /S /Q "..\Visual Studio 2008\SoarLibrary"
RMDIR /S /Q "..\Visual Studio 2008\Win32"
RMDIR /S /Q "..\Visual Studio 2008\x64"
RMDIR /S /Q "..\Visual Studio 2010\SoarLibrary"
RMDIR /S /Q "..\Visual Studio 2010\Win32"
RMDIR /S /Q "..\Visual Studio 2010\x64"
RMDIR /S /Q "..\Visual Studio 2010\ipch"

DEL /Q "..\Visual Studio 2008\Application_Name.h"
DEL /Q "..\Visual Studio 2010\Application_Name.h"

DEL /Q "..\smltrace.txt"
DEL /Q "..\stderr.txt"
DEL /Q "..\stdout.txt"

DEL /Q "..\Visual Studio 2008\*.ncb"
DEL /Q "..\Visual Studio 2010\*.sdf"


RMDIR /S /Q "..\Xcode 3.1.2\build"


DEL /Q "..\*.dblite"

DEL /Q "..\src\TinyXML\*.o"
DEL /Q "..\src\Zeni\*.o"
DEL /Q "..\src\*.o"

DEL /Q "..\*.a"

DEL /Q "..\bin\x64\*.exp"
DEL /Q "..\bin\x64\*.ilk"
DEL /Q "..\bin\x64\*.lib"
DEL /Q "..\bin\x64\*.manifest"
DEL /Q "..\bin\x64\*.map"
DEL /Q "..\bin\x64\*.pdb"

DEL /Q "..\bin\*.exp"
DEL /Q "..\bin\*.ilk"
DEL /Q "..\bin\*.lib"
DEL /Q "..\bin\*.manifest"
DEL /Q "..\bin\*.map"
DEL /Q "..\bin\*.pdb"

DEL /Q "..\src\TinyXML\*.obj"
DEL /Q "..\src\Zeni\*.obj"
DEL /Q "..\src\*.obj"

DEL /Q "..\*.idb"
DEL /Q "..\*.ilk"
DEL /Q "..\*.lib"
DEL /Q "..\*.manifest"
DEL /Q "..\*.map"
DEL /Q "..\*.pdb"

DEL /Q "..\Visual Studio 2008\*.obj"
DEL /Q "..\Visual Studio 2010\*.obj"
