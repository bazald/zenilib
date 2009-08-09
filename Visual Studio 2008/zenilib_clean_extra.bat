call zenilib_clean.bat


DEL /Q /A:H "*.suo"

FOR /f %%a IN ('dir /b *.user') DO (
  IF "%%a" NEQ "Application_2008.vcproj.user" (
    IF "%%a" NEQ "Application_SoarSML_2008.vcproj.user" (
      ECHO DEL /Q "%%a"
      DEL /Q "%%a"
    )
  )
)
