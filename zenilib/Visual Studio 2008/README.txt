****************************************
********** ENABLING DEBUGGING **********
****************************************

Unfortunately, this must be done on a per-host and per-user basis:

  1. Right-click on project Zeniapp in the Solution Explorer.
  2. Left-click on Properties in the menu that pops up.
  3. Navigate to 'Configuration Properties'/'Debugging'
  4. For 'All Configurations' in 'Win32' and in 'x64'
     Set the 'Working Directory' to '..'
     Set the 'Environment' to 'PATH=bin;bin\x64'



**************************************
********** CLEANING SCRIPTS **********
**************************************

zenilib_clean.bat
  -Delete files generated during the build process

zenilib_clean_extra.bat
  -zenilib_clean.bat
  -Delete per-user settings for the Visual Studio project

zenilib_clean+strip_aio.bat
  -zenilib_clean_extra.bat
  -zenilib_strip_debug.bat
  -zenilib_strip_x64.bat

zenilib_strip_debug.bat
  -Delete all debug builds

zenilib_strip_sources.bat.unsafe
  -intended to be used a COPY of your actual project, for public release
  -will erase all source files if you follow my file organization conventions
  -will prompt you on each action
  -must have the '.unsafe' removed in order to run it

zenilib_strip_x64.bat
  -Delete all x64 builds and DLLs



******************************************************
********** ENABLING AND DISABLING LIBRARIES **********
******************************************************

(A)  Direct3D 9     OpenGL         OpenAL
(B)  DISABLE_DX9    DISABLE_GL     DISABLE_AL
(C)  d3d9.lib       opengl32.lib   OpenAL32.lib
     d3dx9.lib      glew32.lib     alut.lib
     cgD3D9.lib     glu32.lib
                    cgGL.lib

To prevent code from using (A),

  1. Right-click on project Zeniapp in the Solution Explorer.
  2. Left-click on Properties in the menu that pops up.
  3. Navigate to 'Configuration Properties'/'C/C++'/'Preprocessor'
  4. Append a semicolon followed by (B) to the Preprocessor Definitions
       Alternatively, right click on the "..." at the end of the Preprocessor Definitions field and add a new line containing (B).
  5. Navigate to 'Configuration Properties'/'Linker'/'Input'
  6. Remove entries (C) from Additional Dependencies.  This can be done in two ways, just like step 4.

To reenable library (A), reverse the changes described above.
