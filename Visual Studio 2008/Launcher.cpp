#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

#define BUFFER_SIZE 4096
int launch(const std::string &local_exe);

int WINAPI WinMain(HINSTANCE /*hInstance*/,
                   HINSTANCE /*hPrevInstance*/,
                   LPSTR /*lpCmdLine*/,
                   int /*nCmdShow*/)
{
#ifdef X64
#ifdef NDEBUG
  return launch("bin\\x64\\Zeniapp_x64.exe");
#else
  return launch("bin\\x64\\Zeniapp_x64d.exe");
#endif
#else
#ifdef NDEBUG
  return launch("bin\\Zeniapp.exe");
#else
  return launch("bin\\Zeniapp_d.exe");
#endif
#endif
}

int launch(const std::string &local_exe) {
  char dir[BUFFER_SIZE];
  const DWORD required_length = GetCurrentDirectoryA(0, NULL);
  if(required_length > BUFFER_SIZE)
    return -1;
  GetCurrentDirectoryA(required_length, dir);

  char full_exe[BUFFER_SIZE];
  if(strlen(dir) + strlen(local_exe.c_str()) + 1 > 4096)
    return -2;
  sprintf_s(full_exe, BUFFER_SIZE, "%s\\%s", dir, local_exe.c_str());

  STARTUPINFOA siStartupInfo;
  PROCESS_INFORMATION piProcessInfo;
  memset(&siStartupInfo, 0, sizeof(siStartupInfo));
  memset(&piProcessInfo, 0, sizeof(piProcessInfo));
  siStartupInfo.cb = sizeof(siStartupInfo);

  const bool result = CreateProcessA(full_exe,
                                     full_exe,
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
