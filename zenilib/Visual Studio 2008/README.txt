****************************************
********** ENABLING DEBUGGING **********
****************************************

These instructions should no longer be necessary, but just in case
you tamper with your settings, this will fix them.

  1. Right-click on project 'Application' in the Solution Explorer.
  2. Left-click on Properties in the menu that pops up.
  3. Navigate to 'Configuration Properties'/'Debugging'
  4. For 'All Configurations' in 'Win32' and in 'x64'
     Set the 'Working Directory' to '..'
     Set the 'Environment' to 'PATH=bin;bin\x64'



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

  1. Right-click on project 'Application' in the Solution Explorer.
  2. Left-click on Properties in the menu that pops up.
  3. Navigate to 'Configuration Properties'/'C/C++'/'Preprocessor'
  4. Append a semicolon followed by (B) to the Preprocessor Definitions
       Alternatively, right click on the "..." at the end of the Preprocessor Definitions field and add a new line containing (B).
  5. Navigate to 'Configuration Properties'/'Linker'/'Input'
  6. Remove entries (C) from Additional Dependencies.  This can be done in two ways, just like step 4.

To reenable library (A), reverse the changes described above.
