call "%~dp0zenilib_clean_extra.bat"
call "%~dp0zenilib_strip_x64.bat"

DEL /Q "%~dp0..\bin\*.so"
DEL /Q "%~dp0..\bin\x64\*.so"

RMDIR /S "%~dp0..\Frameworks"
