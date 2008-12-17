/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
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
  return launch("bin\\x64\\Zeniapp_x64.exe", lpCmdLine);
#else
  return launch("bin\\x64\\Zeniapp_x64d.exe", lpCmdLine);
#endif
#else
#ifdef NDEBUG
  return launch("bin\\Zeniapp.exe", lpCmdLine);
#else
  return launch("bin\\Zeniapp_d.exe", lpCmdLine);
#endif
#endif
}

int launch(const std::string &local_exe, const std::string &arguments) {
  char dir[BUFFER_SIZE];
  const DWORD required_length = GetCurrentDirectoryA(0, NULL);
  if(required_length > BUFFER_SIZE)
    return -1;
  GetCurrentDirectoryA(required_length, dir);

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

  const bool result = CreateProcessA(full_exe,
                                     full_exe_with_args,
                                     NULL,
                                     NULL,
                                     FALSE,
                                     0,
                                     NULL,
                                     NULL,
                                     &siStartupInfo,
                                     &piProcessInfo) != 0;

   if(result)
     return 0;

   return GetLastError();
}
