@echo off

:: Backup environment variables
IF NOT EXIST build MKDIR build
IF EXIST "dev\backupenv.bat" DEL "dev\backupenv.bat"
FOR /f "tokens=1* delims==" %%a in ('SET') DO ECHO SET %%a=%%b>> build\backupenv.bat

:: If logged in over Cygwin SSH, must use a password or commands fail.
SET WHOAMI=
FOR /F %%v in ('C:\Windows\System32\whoami.exe /USER') DO SET WHOAMI=%%v
FOR /F %%v in ('echo.exe %WHOAMI% ^| sed.exe "s/.*\\\\//"') DO SET WHOAMI=%%v
IF "%WHOAMI%"=="cyg_server" (
  echo Logged in as cyg_server.  Please relog in using your password.
  exit 1
)

:: If logged in over Cygwin SSH, necessary variables are unset.
CALL dev\VCVarsQueryRegistry.bat
IF "%AppData%"=="" SET AppData=C:\Users\%WHOAMI%\AppData

:: Generate Visual Studio 2010 solution and projects
IF NOT EXIST build\vs2010 (
  IF EXIST dev\premake\premake4-windows.exe (
    dev\premake\premake4-windows.exe --os=windows vs2010
  )
)

:: Setup devenv
CALL "%VS100COMNTOOLS%vsvars32.bat"

MSBuild build\vs2010\zenilib.sln /m /p:MultiProcessorCompilation=true /t:Build /p:Configuration=Release /p:Platform=Win32 /fileLogger /fileLoggerParameters:LogFile=build\x32.log;Encoding=UTF-8
COPY jni\external\bin\x32\* bin\x32\

MSBuild build\vs2010\zenilib.sln /m /p:MultiProcessorCompilation=true /t:Build /p:Configuration=Release /p:Platform=x64 /fileLogger /fileLoggerParameters:LogFile=build\x64.log;Encoding=UTF-8
COPY jni\external\bin\x64\* bin\x64\

:: Restore environment variables
FOR /f "tokens=1* delims==" %%a in ('SET') DO SET %%a=
CALL build\backupenv.bat
