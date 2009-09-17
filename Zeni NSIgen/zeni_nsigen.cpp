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
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

bool g_x64 = false;
bool g_win2k_winxpb4sp2 = false;

vector<string> g_excluded;

/** String Manipulation Functions **/

static string cut_all_directories(const string &file_name) {
  const int cut = file_name.rfind('\\') + 1;
  if(cut > 0)
    return file_name.substr(cut, file_name.size() - cut);
  return file_name;
}

static string cut_root_directory(const string &file_name) {
  const int cut = file_name.find('\\') + 1;
  if(cut > 0)
    return file_name.substr(cut, file_name.size() - cut);
  return file_name;
}

static string get_directory(const string &file_name) {
  const int cut = file_name.rfind('\\');
  if(cut > -1)
    return file_name.substr(0, cut);
  return "";
}

static string get_extension(const string &file_name) {
  const int cut = file_name.rfind('.') + 1;
  if(cut > 0)
    return file_name.substr(cut, file_name.size() - cut);
  return "";
}

static string to_lower(string str) {
  for(string::iterator it = str.begin(); it != str.end(); ++it)
    *it = char(tolower(*it));
  return str;
}

static string to_upper(string str) {
  for(string::iterator it = str.begin(); it != str.end(); ++it)
    *it = char(toupper(*it));
  return str;
}

static bool exclude_path(const std::string &str, const bool &report) {
  const string sl = to_lower(str);
  const bool exclude = !g_x64 && sl.find("x64") != -1 ||
                       !g_win2k_winxpb4sp2 && (sl.find("dxdllreg_x86.cab") != -1 ||
                                               sl.find("dxnt.cab") != -1);
  if(exclude && report)
    g_excluded.push_back(str.substr(str.find('\\') + 1));
  return exclude;
}

/** Stream Dumper **/

static void dump(ostream &os, istream &is) {
  string s;
  while(getline(is, s))
    if(!exclude_path(s, false))
      os << s << endl;
}

/** NSIS Helpers **/

class NSIS_Helper {
public:
  virtual ~NSIS_Helper() {}

  virtual void on_enter_dir(ostream &/*os*/, const string &/*directory_name*/) {}
  virtual void on_file(ostream &/*os*/, const string &/*file_name*/) {}
  virtual void on_exit_dir(ostream &/*os*/, const string &/*directory_name*/) {}

  virtual bool report() const {return false;}
};

class Install_Files : public NSIS_Helper {
  void change_dir(ostream &os, const string &dir) {
    last_dir = dir;

    os << "\n  SetOutPath \"$INSTDIR";
    if(!dir.empty())
      os << "\\" << dir;
    os << "\"\n";

    dir_set = true;
  }

  void on_file(ostream &os, const string &file_name) {
    const string dir = get_directory(cut_root_directory(file_name));
    if(!dir_set || dir != last_dir)
      change_dir(os, dir);

    os << "    File \""
       << file_name
       << "\"\n";
  }

  bool report() const {return true;}

public:
  Install_Files() : dir_set(false) {}

private:
  bool dir_set;
  string last_dir;
};

class Uninstall_Files : public NSIS_Helper {
  void on_file(ostream &os, const string &file_name) {
    const string file = cut_root_directory(file_name);

    os << "    Delete \"$INSTDIR\\"
       << file
       << "\"\n";
  }

  void on_exit_dir(ostream &os, const string &directory_name) {
    const string dir = cut_root_directory(get_directory(directory_name));

    os << "  RMDir \"$INSTDIR";
    if(!dir.empty())
      os << "\\" << dir;
    os << "\"\n\n";
  }

  bool report() const {return false;}
};

/** WinAPI Functions **/

static void recurse_directory(ostream &os, const string &directory_name, NSIS_Helper &helper) {
  if(exclude_path(directory_name, helper.report()))
    return;

  helper.on_enter_dir(os, directory_name);

  WIN32_FIND_DATAA FindFileData;
  HANDLE hFind = FindFirstFileA(const_cast<char *>((directory_name + "*").c_str()),
                                &FindFileData);

  if(hFind == INVALID_HANDLE_VALUE)
    return;

  do {
    if(!strcmp(FindFileData.cFileName, ".") ||
       !strcmp(FindFileData.cFileName, ".."))
      continue;

    if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      recurse_directory(os, directory_name + FindFileData.cFileName + "\\", helper);
    else if(!exclude_path(directory_name + FindFileData.cFileName, helper.report()))
      helper.on_file(os, directory_name + FindFileData.cFileName);
  } while(FindNextFileA(hFind, &FindFileData));

  if(GetLastError() != ERROR_NO_MORE_FILES)
    abort();

  FindClose(hFind);
  
  helper.on_exit_dir(os, directory_name);
}

/** Main **/

static bool yes_or_no(const bool &default_value) {
  std::string s;
  char c;

  while(cin.get(c) && c != '\n')
    s += char(tolower(c));

  if(s == "y" || s == "yes")
    return true;
  if(s == "n" || s == "no")
    return false;

  return default_value;
}

int main(int /*argc*/, char ** /*argv*/) {
  cout << "Include 64-bit binaries [y/N]? ";
  g_x64 = yes_or_no(false);

  cout << "Install additional files for Windows 2000, XP, and XP SP1 [y/N]? ";
  g_win2k_winxpb4sp2 = yes_or_no(false);

  ofstream nsi("Zenilib.nsi");
  ifstream nsi_0("Zeni NSIgen\\NSI_0_Preuser.txt");
  ifstream nsi_u("Zeni NSIS Settings.txt");
  ifstream nsi_1("Zeni NSIgen\\NSI_1_Postuser_+_Preinstall.txt");
  ifstream nsi_2("Zeni NSIgen\\NSI_2_Postinstall_+_Preuninstall.txt");
  ifstream nsi_3("Zeni NSIgen\\NSI_3_Postuninstall.txt");

  if(!nsi ||
    !nsi_0 ||
    !nsi_u ||
    !nsi_1 ||
    !nsi_2 ||
    !nsi_3)
    abort();

  Install_Files installer;
  Uninstall_Files uninstaller;

  dump(nsi, nsi_0);
  dump(nsi, nsi_u);
  dump(nsi, nsi_1);
  recurse_directory(nsi, "zenilib\\", installer);
  dump(nsi, nsi_2);
  recurse_directory(nsi, "zenilib\\", uninstaller);
  dump(nsi, nsi_3);

  cout << "\nThe following paths were excluded from the installer:\n\n";
  for(vector<string>::const_iterator it = g_excluded.begin(); it != g_excluded.end(); ++it)
    cout << "  " << *it << endl;

  {
    cout << "\nHit Enter to exit.\n";
    string s;
    getline(cin, s);
  }

  return 0;
}
