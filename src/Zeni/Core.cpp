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
#endif

using namespace std;

namespace Zeni {

  static streambuf * cerr_bak = 0;
  static streambuf * cout_bak = 0;

  Core::Core()
    : m_username("username"),
    m_appdata_path("./"),
    m_joystick(0)
  {
    /** Redirect output **/

    static ofstream cerr_file("stderr.txt");
    static ofstream cout_file("stdout.txt");
    
    if(cerr_file.is_open()) {
      cerr_bak = cerr.rdbuf();
      cerr.rdbuf(cerr_file.rdbuf());
    }

    if(cout_file.is_open()) {
      cout_bak = cout.rdbuf();
      cout.rdbuf(cout_file.rdbuf());
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
    fgets(username, sizeof(username), whoami);
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
    fgets(appdata_path, sizeof(appdata_path), pwd);
    pclose(pwd);
    int appdata_path_len = strlen(appdata_path);
    if(appdata_path_len)
      appdata_path[appdata_path_len - 1] = 0;
#endif
    m_appdata_path = appdata_path;

    /** Initialize SDL itself **/

    if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) == -1)
      throw Core_Init_Failure();

    /** Initialize Joysticks **/

    init_joysticks();
  }

  Core::~Core() {
    quit_joysticks();

    SDL_Quit();

    if(cout_bak)
      cout.rdbuf(cout_bak);
    if(cerr_bak)
      cerr.rdbuf(cerr_bak);
  }

  Core & get_Core() {
    static Core e_core;
    return e_core;
  }

  const std::string & Core::get_username() {
    return m_username;
  }

  std::string Core::get_appdata_path(const std::string &unique_app_identifier) {
#ifdef _WINDOWS
    return m_appdata_path + "\\" + unique_app_identifier + "\\";
#else
    return m_appdata_path + "/." + unique_app_identifier + "/";
#endif
  }

  int Core::get_num_joysticks() const {
    return int(m_joystick.size());
  }

  const std::string & Core::get_joystick_name(const int &index) const {
    assert(-1 < index && index < int(m_joystick.size()));
    return m_joystick[index].second;
  }
  
  int Core::get_joystick_num_axes(const int &index) const {
    assert(-1 < index && index < int(m_joystick.size()));
    return SDL_JoystickNumAxes(m_joystick[index].first);
  }

  int Core::get_joystick_num_balls(const int &index) const {
    assert(-1 < index && index < int(m_joystick.size()));
    return SDL_JoystickNumBalls(m_joystick[index].first);
  }

  int Core::get_joystick_num_hats(const int &index) const {
    assert(-1 < index && index < int(m_joystick.size()));
    return SDL_JoystickNumAxes(m_joystick[index].first);
  }

  int Core::get_joystick_num_buttons(const int &index) const {
    assert(-1 < index && index < int(m_joystick.size()));
    return SDL_JoystickNumAxes(m_joystick[index].first);
  }

  void Core::regenerate_joysticks() {
    quit_joysticks();
    init_joysticks();
  }
  
  void Core::init_joysticks() {
    if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) == -1)
      throw Joystick_Init_Failure();

    for(int i = 0, end = SDL_NumJoysticks(); i < end; ++i) {
      m_joystick.push_back(make_pair(SDL_JoystickOpen(i),
                                    SDL_JoystickName(i)));

      if(!m_joystick[i].first) {
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
      SDL_JoystickClose(m_joystick[i].first);

    m_joystick.clear();

    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
  }

}
