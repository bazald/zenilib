call zenilib_clean.bat


DEL /Q /A:H "*.suo"

FOR /f %%a IN ('dir /b ..\Visual Studio 2008\*.user') DO (
  IF "%%a" NEQ "..\Visual Studio 2008\Application_2008.vcproj.user" (
    IF "%%a" NEQ "..\Visual Studio 2008\Application_SoarSML_2008.vcproj.user" (
      ECHO DEL /Q "%%a"
      DEL /Q "%%a"
    )
  )
)


FOR /F "tokens=*" %%G IN ('DIR /B /AD /S ..\Visual Studio 2008\Win32\*_d') DO RMDIR /S /Q "%%G"
FOR /F "tokens=*" %%G IN ('DIR /B /AD /S ..\Visual Studio 2008\x64\*_d') DO RMDIR /S /Q "%%G"

DEL /Q "..\bin\*_d.exe"
DEL /Q "..\bin\x64\*_x64d.exe"
DEL /Q "..\*_d.exe"
DEL /Q "..\*_x64d.exe"
DEL /Q "..\*_d"
DEL /Q "..\*_x64d"
