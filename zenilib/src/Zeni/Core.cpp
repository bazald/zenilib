/* This file is part of the Zenipex Library.
* Copyleft (C) 2010 Mitchell Keith Bloch a.k.a. bazald
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
*
* As a special exception, you may use this file as part of a free software
* library without restriction.  Specifically, if other files instantiate
* templates or use macros or inline functions from this file, or you compile
* this file and link it with other files to produce an executable, this
* file does not by itself cause the resulting executable to be covered by
* the GNU General Public License.  This exception does not however
* invalidate any other reasons why the executable file might be covered by
* the GNU General Public License.
*/

#include <Zeni/Core.h>

#include <SDL/SDL.h>

#include <iostream>
#include <fstream>
#include <cassert>

#ifdef _WINDOWS
#include <shlobj.h>
#include <WinUser.h>
#else
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace Zeni {

  static std::streambuf * cerr_bak = 0;
  static std::streambuf * cout_bak = 0;

  Core::Core()
    : m_username("username"),
    m_appdata_path("./"),
    m_joystick(0)
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
      throw Core_Init_Failure();
#else
    char username[4096];
    FILE * whoami = popen("whoami", "r");
    if(!fgets(username, sizeof(username), whoami))
      throw Core_Init_Failure();
    pclose(whoami);
    int username_len = strlen(username);
    if(username_len)
      username[username_len - 1] = 0;
#endif
    m_username = username;

    /** Get appdata path **/

#ifdef _WINDOWS
	  char appdata_path[MAX_PATH];
    if(SHGetFolderPath(0, CSIDL_APPDATA | CSIDL_FLAG_CREATE, 0, SHGFP_TYPE_CURRENT, appdata_path) != S_OK)
      throw Core_Init_Failure();
#else
    char appdata_path[4096];
    FILE * pwd = popen("cd ~/ && pwd", "r");
    if(!fgets(appdata_path, sizeof(appdata_path), pwd))
      throw Core_Init_Failure();
    pclose(pwd);
    int appdata_path_len = strlen(appdata_path);
    if(appdata_path_len)
      appdata_path[appdata_path_len - 1] = 0;
#endif
    m_appdata_path = appdata_path;

    /** Initialize SDL itself **/

    if(SDL_Init(SDL_INIT_TIMER) == -1)
      throw Core_Init_Failure();

    /** Initialize Joysticks **/

    init_joysticks();

    /** Ensure g_unique_app_identifier is set **/

    std::string &unique_app_identifier = get_unique_app_identifier();
    if(unique_app_identifier.empty())
      unique_app_identifier = "zenilib";
  }

  Core::~Core() {
    quit_joysticks();

    SDL_Quit();

    if(cout_bak)
      std::cout.rdbuf(cout_bak);
    if(cerr_bak)
      std::cerr.rdbuf(cerr_bak);
  }

  Core & get_Core() {
    static Core e_core;
    return e_core;
  }

  const std::string & Core::get_username() {
    return m_username;
  }

  std::string Core::get_appdata_path() {
#ifdef _WINDOWS
    return m_appdata_path + "\\" + get_unique_app_identifier() + "\\";
#else
    return m_appdata_path + "/." + get_unique_app_identifier() + "/";
#endif
  }

  bool Core::create_directory(const std::string &directory_path) {
#ifdef _WINDOWS
    return CreateDirectory(directory_path.c_str(), NULL) != 0 ||
           GetLastError() == ERROR_ALREADY_EXISTS;
#else
    const int status = mkdir(directory_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    return !status || errno == EEXIST;
#endif
  }

  bool Core::remove_directory(const std::string &directory_path) {
#ifdef _WINDOWS
    return RemoveDirectory(directory_path.c_str()) != 0 ||
           GetLastError() == ERROR_PATH_NOT_FOUND;
#else
    return !rmdir(directory_path.c_str()) || errno == ENOENT;
#endif
  }

  bool Core::file_exists(const std::string &file_path) {
    std::ifstream fin(file_path.c_str());
    return fin.good();
  }

  bool Core::delete_file(const std::string &file_path) {
#ifdef _WINDOWS
    return DeleteFile(file_path.c_str()) != 0 ||
           GetLastError() == ERROR_PATH_NOT_FOUND;
#else
    return system(("rm " + file_path).c_str()) == 0;
#endif
  }

  bool Core::copy_file(const std::string &from, const std::string &to) {
    std::ifstream fin(from.c_str());
    if(!fin)
      return false;

    std::ofstream fout(to.c_str());

    for(char c; fin.get(c); fout.put(c));

    return fout.good();
  }

  bool Core::is_screen_saver_enabled() {
#if SDL_VERSION_ATLEAST(1,3,0)
    return SDL_IsScreenSaverEnabled() != 0;
#elif defined(_WINDOWS)
    BOOL is_active;
    SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &is_active, 0);
    return is_active != 0;
#else
    return true;
#endif
  }

  void Core::set_screen_saver(const bool &
#if SDL_VERSION_ATLEAST(1,3,0) || defined(_WINDOWS)
    enabled
#endif
    ) {
#if SDL_VERSION_ATLEAST(1,3,0)
    if(enabled)
      SDL_EnableScreenSaver();
    else
      SDL_DisableScreenSaver();
#elif defined(_WINDOWS)
    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, UINT(enabled), 0, SPIF_SENDCHANGE);
#endif
  }

  size_t Core::get_num_joysticks() const {
    return m_joystick.size();
  }

  const std::string & Core::get_joystick_name(const size_t &index) const {
    assert(index < m_joystick.size());
    return m_joystick[index].second;
  }
  
  int Core::get_joystick_num_axes(const size_t &index) const {
    assert(index < m_joystick.size());
    return SDL_JoystickNumAxes(m_joystick[index].first);
  }

  int Core::get_joystick_num_balls(const size_t &index) const {
    assert(index < m_joystick.size());
    return SDL_JoystickNumBalls(m_joystick[index].first);
  }

  int Core::get_joystick_num_hats(const size_t &index) const {
    assert(index < m_joystick.size());
    return SDL_JoystickNumAxes(m_joystick[index].first);
  }

  int Core::get_joystick_num_buttons(const size_t &index) const {
    assert(index < m_joystick.size());
    return SDL_JoystickNumAxes(m_joystick[index].first);
  }

  void Core::regenerate_joysticks() {
    quit_joysticks();
    init_joysticks();
  }

  void Core::preinit(const std::string &unique_app_identifier_) {
    std::string &unique_app_identifier = get_unique_app_identifier();

    if(!unique_app_identifier.empty())
      throw Core_Initialized();

    unique_app_identifier = unique_app_identifier_;
  }

  void Core::assert_no_error() {
    if(strlen(SDL_GetError())) {
      std::cerr << "SDL       : " << SDL_GetError() << std::endl;
      SDL_ClearError();
      assert(false);
    }
  }

  bool Core::print_error() {
    if(strlen(SDL_GetError())) {
      std::cerr << "SDL       : " << SDL_GetError() << std::endl;
      SDL_ClearError();
      return true;
    }

    return false;
  }

  void Core::init_joysticks() {
    if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) == -1)
      throw Joystick_Init_Failure();

    for(int i = 0, end = SDL_NumJoysticks(); i < end; ++i) {
      m_joystick.push_back(std::make_pair(SDL_JoystickOpen(i),
                                          SDL_JoystickName(i)));

      if(!m_joystick[size_t(i)].first) {
        m_joystick.pop_back();
        quit_joysticks();

        throw Joystick_Init_Failure();
      }
    }

    SDL_JoystickEventState(SDL_ENABLE);
  }

  void Core::quit_joysticks() {
    SDL_JoystickEventState(SDL_DISABLE);

    for(int i = 0, end = SDL_NumJoysticks(); i < end; ++i)
      SDL_JoystickClose(m_joystick[size_t(i)].first);

    m_joystick.clear();

    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
  }

  std::string & Core::get_unique_app_identifier() {
    static std::string * unique_app_identifier = new std::string;
    return *unique_app_identifier;
  }

}
