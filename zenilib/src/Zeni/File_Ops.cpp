/* This file is part of the Zenipex Library (zenilib).
 * Copyright (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * zenilib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * zenilib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with zenilib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <zeni.h>

#include <iostream>
#include <fstream>
#include <cassert>

#ifdef _WINDOWS
#include <shlobj.h>
#include <WinUser.h>
#else
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#endif

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Singleton.hxx>

namespace Zeni {

  static std::streambuf * cerr_bak = 0;
  static std::streambuf * cout_bak = 0;

  ZENI_EXT template class ZENI_DLL Singleton<File_Ops>;

  File_Ops * File_Ops::create() {
    return new File_Ops;
  }

  File_Ops::File_Ops()
    : m_username("username"),
    m_appdata_path("./")
  {
    /** Redirect output **/

    static std::ofstream cerr_file("stderr.txt");
    static std::ofstream cout_file("stdout.txt");

    if(cerr_file.is_open()) {
      cerr_bak = std::cerr.rdbuf();
      std::cerr.rdbuf(cerr_file.rdbuf());
    }

    if(cout_file.is_open()) {
      cout_bak = std::cout.rdbuf();
      std::cout.rdbuf(cout_file.rdbuf());
    }

    /** Get username **/

#ifdef _WINDOWS
    char username[MAX_PATH];
    DWORD username_len = sizeof(username);
    if(!GetUserName(username, &username_len))
      throw File_Ops_Init_Failure();
#else
    passwd * const pws = getpwuid(geteuid());
    const char * const username = pws ? pws->pw_name : "default";
#endif
    m_username = username;

    /** Get appdata path **/

#ifdef _WINDOWS
    char appdata_path[MAX_PATH];
    if(SHGetFolderPath(0, CSIDL_APPDATA | CSIDL_FLAG_CREATE, 0, SHGFP_TYPE_CURRENT, appdata_path) != S_OK)
      throw File_Ops_Init_Failure();
#else
    const char * const appdata_path = pws ? pws->pw_dir : "/tmp/";
#endif
    m_appdata_path = appdata_path;

    /** Ensure g_unique_app_identifier is set **/

    String &unique_app_identifier = get_unique_app_identifier();
    if(unique_app_identifier.empty())
      unique_app_identifier = "zenilib";
  }

  File_Ops::~File_Ops() {
    if(cout_bak)
      std::cout.rdbuf(cout_bak);
    if(cerr_bak)
      std::cerr.rdbuf(cerr_bak);
  }

  File_Ops & get_File_Ops() {
    return File_Ops::get();
  }

  const String & File_Ops::get_username() {
    return m_username;
  }

  String File_Ops::get_appdata_path() {
#ifdef _WINDOWS
    return m_appdata_path + "\\" + get_unique_app_identifier() + "\\";
#else
    return m_appdata_path + "/." + get_unique_app_identifier() + "/";
#endif
  }

  bool File_Ops::create_directory(const String &directory_path) {
#ifdef _WINDOWS
    return CreateDirectory(directory_path.c_str(), NULL) != 0 ||
           GetLastError() == ERROR_ALREADY_EXISTS;
#else
    const int status = mkdir(directory_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    return !status || errno == EEXIST;
#endif
  }

  bool File_Ops::remove_directory(const String &directory_path) {
#ifdef _WINDOWS
    return RemoveDirectory(directory_path.c_str()) != 0 ||
           GetLastError() == ERROR_PATH_NOT_FOUND;
#else
    return !rmdir(directory_path.c_str()) || errno == ENOENT;
#endif
  }

  bool File_Ops::file_exists(const String &file_path) {
    std::ifstream fin(file_path.c_str());
    return fin.good();
  }

  bool File_Ops::delete_file(const String &file_path) {
#ifdef _WINDOWS
    return DeleteFile(file_path.c_str()) != 0 ||
           GetLastError() == ERROR_PATH_NOT_FOUND;
#else
    return system(("rm " + file_path).c_str()) == 0;
#endif
  }

  bool File_Ops::copy_file(const String &from, const String &to) {
    std::ifstream fin(from.c_str());
    if(!fin)
      return false;

    std::ofstream fout(to.c_str());

    for(char c; fin.get(c); fout.put(c));

    return fout.good();
  }

  void File_Ops::preinit(const String &unique_app_identifier_) {
    String &unique_app_identifier = get_unique_app_identifier();

    if(!unique_app_identifier.empty())
      throw File_Ops_Initialized();

    unique_app_identifier = unique_app_identifier_;
  }

  String & File_Ops::get_unique_app_identifier() {
    static String * unique_app_identifier = new String;
    return *unique_app_identifier;
  }

}
