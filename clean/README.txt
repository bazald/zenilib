**************************************
********** CLEANING SCRIPTS **********
**************************************

Batch files (.bat) and Python scripts should have equivalent results.
The batch files are for Windows.
The Python scripts are for any OS with a Python interpreter installed.

zenilib_clean.bat/py
  -Delete files generated during the build process

zenilib_clean_extra.bat/py
  -zenilib_clean.bat/py
  -Delete per-user settings for the Visual Studio project
  -Delete all debug builds

zenilib_clean_extra_x86.bat/py
  -zenilib_clean_extra.bat/py
  -zenilib_strip_x64.bat/py
  -Delete Mac OS X Frameworks

zenilib_strip_sources.bat/py.unsafe
  -intended to be used on a COPY of your actual project, for public release
  -will erase all source files if you follow my file organization conventions
  -will prompt you on each action (but not recursively)
  -must have the '.unsafe' removed in order to run it

zenilib_strip_x64.bat/py
  -Delete all x64 builds and DLLs
