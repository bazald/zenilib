RMDIR /S /Q "SoarLibrary"
RMDIR /S /Q "Win32"
RMDIR /S /Q "x64"

DEL /Q "..\smltrace.txt"
DEL /Q "..\stderr.txt"
DEL /Q "..\stdout.txt"

DEL /Q "*.ncb"


DEL /Q /A:H "*.suo"

FOR /f %%a IN ('dir /b *.user') DO (
  IF "%%a" NEQ "Application_2008.vcproj.user" (
    IF "%%a" NEQ "Application_SoarSML_2008.vcproj.user" (
      ECHO DEL /Q "%%a"
      DEL /Q "%%a"
    )
  )
)


RMDIR /S /Q "..\bin\x64"
RMDIR /S /Q "..\lib_win\x64"

DEL /Q "..\bin\*.exe"
DEL /Q "..\bin\x64\*.exe"
DEL /Q "..\*.exe"
