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

/** String Manipulation Functions **/

string cut_all_directories(const string &file_name) {
  const int cut = file_name.rfind('\\') + 1;
  if(cut > 0)
    return file_name.substr(cut, file_name.size() - cut);
  return file_name;
}

string cut_root_directory(const string &file_name) {
  const int cut = file_name.find('\\') + 1;
  if(cut > 0)
    return file_name.substr(cut, file_name.size() - cut);
  return file_name;
}

string get_directory(const string &file_name) {
  const int cut = file_name.rfind('\\');
  if(cut > -1)
    return file_name.substr(0, cut);
  return "";
}

string get_extension(const string &file_name) {
  const int cut = file_name.rfind('.') + 1;
  if(cut > 0)
    return file_name.substr(cut, file_name.size() - cut);
  return "";
}

string to_lower(string str) {
  for(string::iterator it = str.begin(); it != str.end(); ++it)
    *it = tolower(*it);
  return str;
}

string to_upper(string str) {
  for(string::iterator it = str.begin(); it != str.end(); ++it)
    *it = toupper(*it);
  return str;
}

/** Stream Dumper **/

void dump(ostream &os, istream &is) {
  string s;
  while(getline(is, s))
    if(g_x64 || s.find("x64") == -1 && s.find("X64") == -1)
      os << s << endl;
}

/** NSIS Helpers **/

class NSIS_Helper {
public:
  virtual ~NSIS_Helper() {}

  virtual void on_enter_dir(ostream &os, const string &directory_name) {}
  virtual void on_file(ostream &os, const string &file_name) {}
  virtual void on_exit_dir(ostream &os, const string &directory_name) {}
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

    if(to_upper(get_extension(file_name)) == "TTF")
      fonts.push_back(file_name);
  }

public:
  Install_Files() : dir_set(false) {}

  void install_fonts(ostream &os) const {
    for(vector<string>::const_iterator it = fonts.begin(); it != fonts.end(); ++it) {
      string cmd = "copy /Y ";
      const string dest = to_upper(cut_all_directories(*it));
      cmd += *it + " " + dest;

      system(cmd.c_str());

      os << "  !insertmacro InstallTTFFont '"
         << dest
         << "'\n";
    }
  }

private:
  vector<string> fonts;

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
};

/** WinAPI Functions **/

void recurse_directory(ostream &os, const string &directory_name, NSIS_Helper &helper) {
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
    else
      helper.on_file(os, directory_name + FindFileData.cFileName);
  } while(FindNextFileA(hFind, &FindFileData));

  if(GetLastError() != ERROR_NO_MORE_FILES)
    abort();

  FindClose(hFind);
  
  helper.on_exit_dir(os, directory_name);
}

/** Main **/

int main(int argc, char * argv[]) {
  char c;
  cout << "Include Support for x64 [Y/N]? ";
  for(;;) {
    if(!cin.get(c))
      abort();

    if(c == 'y' || c == 'Y')
      g_x64 = true;
    else if(c == 'n' || c == 'N')
      g_x64 = false;
    else
      continue;

    break;
  }

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

  return 0;
}
