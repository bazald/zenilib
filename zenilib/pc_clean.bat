@echo off

IF EXIST assets (
  pushd assets
  IF EXIST stderr.txt DEL stderr.txt
  IF EXIST stdout.txt DEL stdout.txt
  popd
)

IF EXIST "bin\d32" RMDIR /S /Q "bin\d32"
IF EXIST "bin\x32" (
  pushd bin\x32
  FOR /f "usebackq delims=|" %%f in (`DIR /B "*.exp"`) DO DEL %%f
  FOR /f "usebackq delims=|" %%f in (`DIR /B "*.idb"`) DO DEL %%f
  FOR /f "usebackq delims=|" %%f in (`DIR /B "*.ilk"`) DO DEL %%f
  FOR /f "usebackq delims=|" %%f in (`DIR /B "*.lib"`) DO DEL %%f
  FOR /f "usebackq delims=|" %%f in (`DIR /B "*.pdb"`) DO DEL %%f
  popd
)
IF EXIST "bin\d64" RMDIR /S /Q "bin\d64"
IF EXIST "bin\x64" (
  pushd bin\x64
  FOR /f "usebackq delims=|" %%f in (`DIR /B "*.exp"`) DO DEL %%f
  FOR /f "usebackq delims=|" %%f in (`DIR /B "*.idb"`) DO DEL %%f
  FOR /f "usebackq delims=|" %%f in (`DIR /B "*.ilk"`) DO DEL %%f
  FOR /f "usebackq delims=|" %%f in (`DIR /B "*.lib"`) DO DEL %%f
  FOR /f "usebackq delims=|" %%f in (`DIR /B "*.pdb"`) DO DEL %%f
  popd
)

IF EXIST "build" (
  pushd build
  IF EXIST "vs2010" (
    pushd vs2010
    IF EXIST zenilib.sdf DEL zenilib.sdf
    IF EXIST ipch RMDIR /S /Q ipch
    IF EXIST obj RMDIR /S /Q obj
    popd
  )
  FOR /f "usebackq delims=|" %%f in (`DIR /B "*.log"`) DO DEL %%f
  IF EXIST backupenv.bat DEL backupenv.bat
  popd
)

FOR /f "usebackq delims=|" %%f in (`DIR /B "* Debug.exe"`) DO DEL %%f
