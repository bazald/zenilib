call "%~dp0zenilib_clean_extra.bat"
call "%~dp0zenilib_strip_x64.bat"

RMDIR /S "%~dp0..\Frameworks"
