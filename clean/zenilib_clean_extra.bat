call zenilib_clean.bat


DEL /Q /A:H "..\Visual Studio 2008\*.suo"

FOR /F "tokens=*" %%G IN ('DIR /B "..\Visual Studio 2008\*.user"') DO (
  IF "%%G" NEQ "Application_2008.vcproj.user" (
    IF "%%G" NEQ "Application_SoarSML_2008.vcproj.user" (
      ECHO DEL /Q "..\Visual Studio 2008\%%G"
      DEL /Q "..\Visual Studio 2008\%%G"
    )
  )
)


FOR /F "tokens=*" %%G IN ('DIR /B /AD /S "..\Visual Studio 2008\Win32\*_d"') DO RMDIR /S /Q "%%G"
FOR /F "tokens=*" %%G IN ('DIR /B /AD /S "..\Visual Studio 2008\x64\*_d"') DO RMDIR /S /Q "%%G"

DEL /Q "..\bin\*_d.exe"
DEL /Q "..\bin\x64\*_x64d.exe"
DEL /Q "..\*_d.exe"
DEL /Q "..\*_x64d.exe"
DEL /Q "..\*_d"
DEL /Q "..\*_x64d"

FOR /F "tokens=*" %%G IN ('DIR /B /AD /S "..\*_d.app"') DO RMDIR /S /Q "%%G"


RMDIR /S /Q "..\docs\Zeni"
