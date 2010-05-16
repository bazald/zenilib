/* This file is part of the Zenipex Library.
* Copyleft (C) 2009 Mitchell Keith Bloch a.k.a. bazald
*
* The Zenipex Library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU General Public License as 
* published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
*
* The Zenipex Library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with the Zenipex Library; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
* 02110-1301 USA.
*/

#include <iostream>
#include <string>
#include <windows.h>

#include "Application_Name.h"

using namespace std;

// See http://msdn.microsoft.com/en-us/library/ms682425.aspx
#define BUFFER_SIZE 32768
int launch(const std::string &local_exe, const std::string &arguments);

int WINAPI WinMain(HINSTANCE /*hInstance*/,
                   HINSTANCE /*hPrevInstance*/,
                   LPSTR lpCmdLine,
                   int /*nCmdShow*/)
{
#ifdef X64
#ifdef NDEBUG
  return launch(APPLICATION_NAME "_x64.exe", lpCmdLine);
#else
  return launch(APPLICATION_NAME "_x64d.exe", lpCmdLine);
#endif
#else
#ifdef NDEBUG
  return launch(APPLICATION_NAME ".exe", lpCmdLine);
#else
  return launch(APPLICATION_NAME "_d.exe", lpCmdLine);
#endif
#endif
}

int launch(const std::string &local_exe, const std::string &arguments) {
  char dir[BUFFER_SIZE];
  const DWORD nSize = GetModuleFileNameA(NULL, dir, BUFFER_SIZE - 1);
  if(!nSize || GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    return -1;
  else
    dir[nSize] = '\0';

  for(int i = int(nSize) - 1; i != -1; --i)
    if(dir[i] == '\\') {
#ifdef X64
      if(i + 9 > BUFFER_SIZE)
        return -2;
      memcpy(dir + i, "\\bin\\x64", 9);
#else
      if(i + 5 > BUFFER_SIZE)
        return -2;
      memcpy(dir + i, "\\bin", 5);
#endif
      break;
    }
    else
      dir[i] = '\0';

  char full_exe[BUFFER_SIZE];
  if(strlen(dir) + strlen(local_exe.c_str()) + 1 > BUFFER_SIZE)
    return -2;
  sprintf_s(full_exe, BUFFER_SIZE, "%s\\%s", dir, local_exe.c_str());

  char full_exe_with_args[BUFFER_SIZE];
  if(strlen(full_exe) + arguments.size() + 1 > BUFFER_SIZE)
    return -3;
  sprintf_s(full_exe_with_args, BUFFER_SIZE, "%s %s", full_exe, arguments.c_str());

  STARTUPINFOA siStartupInfo;
  PROCESS_INFORMATION piProcessInfo;
  memset(&siStartupInfo, 0, sizeof(siStartupInfo));
  memset(&piProcessInfo, 0, sizeof(piProcessInfo));
  siStartupInfo.cb = sizeof(siStartupInfo);

  bool result = SetCurrentDirectoryA(dir) != 0;

  if(!result)
    return 1;

  result = CreateProcessA(full_exe,
                          full_exe_with_args,
                          NULL,
                          NULL,
                          FALSE,
                          0,
                          NULL,
                          NULL,
                          &siStartupInfo,
                          &piProcessInfo) != 0;

  if(!result)
    return 2;

  if(WaitForSingleObject(piProcessInfo.hProcess, INFINITE) == WAIT_FAILED)
    return int(GetLastError());

  CloseHandle(piProcessInfo.hThread);
  CloseHandle(piProcessInfo.hProcess);

  return 0;
}
