/* This file is part of the Zenipex Library (zenilib).
 * Copyright (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * zenilib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * zenilib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with zenilib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <windows.h>

// See http://msdn.microsoft.com/en-us/library/ms682425.aspx
#define BUFFER_SIZE 32768
int launch(const char local_exe[], const char arguments[]);

int main(int argc, char **argv) {
  const char executable_name[] = EXECUTABLE_NAME;
  char lpCmdLine[BUFFER_SIZE] = "";
  int i;
  char *d, *dend, *s;

  d = lpCmdLine;
  dend = lpCmdLine + BUFFER_SIZE;
  for(i = 1; i < argc; ++i) {
    for(s = argv[i]; *s; ++s) {
      if(d == dend)
        return -1;

      *(d++) = *s;
    }

    if(d == dend)
      return -1;

    *(d++) = i + 1 == argc ? '\0' : ' ';
  }

  return launch(executable_name, lpCmdLine);
}

int launch(const char local_exe[], const char arguments[]) {
  char dir[BUFFER_SIZE];
  const DWORD nSize = GetModuleFileNameA(NULL, dir, BUFFER_SIZE - 1);
  int i;
  char full_exe[BUFFER_SIZE];
  char full_exe_with_args[BUFFER_SIZE];
  STARTUPINFOA siStartupInfo;
  PROCESS_INFORMATION piProcessInfo;

  if(!nSize || GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    return -2;
  else
    dir[nSize] = '\0';

  for(i = (int)nSize - 1; i != -1; --i)
    if(dir[i] == '\\') {
      if(i + 9 > BUFFER_SIZE)
        return -3;
#ifdef X64
#ifdef NDEBUG
      memcpy(dir + i, "\\bin\\x64", 9);
#else
      memcpy(dir + i, "\\bin\\d64", 9);
#endif
#else
#ifdef NDEBUG
      memcpy(dir + i, "\\bin\\x32", 9);
#else
      memcpy(dir + i, "\\bin\\d32", 9);
#endif
#endif
      break;
    }
    else
      dir[i] = '\0';

  if(strlen(dir) + strlen(local_exe) + 1 > BUFFER_SIZE)
    return -4;
  sprintf_s(full_exe, BUFFER_SIZE, "%s\\%s", dir, local_exe);

  if(strlen(full_exe) + strlen(arguments) + 1 > BUFFER_SIZE)
    return -5;
  sprintf_s(full_exe_with_args, BUFFER_SIZE, "%s %s", full_exe, strlen(arguments));

  memset(&siStartupInfo, 0, sizeof(siStartupInfo));
  memset(&piProcessInfo, 0, sizeof(piProcessInfo));
  siStartupInfo.cb = sizeof(siStartupInfo);

  if(!SetCurrentDirectoryA(dir)) {
#ifdef X64
#ifdef NDEBUG
    MessageBoxA(0, "Failed to set the current working directory to bin\\x64\\", 0, MB_OK);
#else
    MessageBoxA(0, "Failed to set the current working directory to bin\\d64\\", 0, MB_OK);
#endif
#else
#ifdef NDEBUG
    MessageBoxA(0, "Failed to set the current working directory to bin\\x32\\", 0, MB_OK);
#else
    MessageBoxA(0, "Failed to set the current working directory to bin\\d32\\", 0, MB_OK);
#endif
#endif

    return 1;
  }

  if(!CreateProcessA(full_exe,
                     full_exe_with_args,
                     NULL,
                     NULL,
                     FALSE,
                     0,
                     NULL,
                     NULL,
                     &siStartupInfo,
                     &piProcessInfo))
  {
    MessageBoxA(0, "Failed to launch \"" EXECUTABLE_NAME "\"", 0, MB_OK);

    return 2;
  }

  if(WaitForSingleObject(piProcessInfo.hProcess, INFINITE) == WAIT_FAILED)
    return (int)GetLastError();

  CloseHandle(piProcessInfo.hThread);
  CloseHandle(piProcessInfo.hProcess);

  return 0;
}
