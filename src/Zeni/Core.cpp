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

using namespace std;

namespace Zeni {

  static streambuf * cerr_bak = 0;
  static streambuf * cout_bak = 0;

  Core::Core()
    : joystick(0)
  {
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

    if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) == -1)
      throw Core_Init_Failure();

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

  int Core::get_num_joysticks() const {
    return joystick.size();
  }

  const std::string & Core::get_joystick_name(const int &index) const {
    return joystick[index].second;
  }

  void Core::regenerate_joysticks() {
    quit_joysticks();
    init_joysticks();
  }
  
  void Core::init_joysticks() {
    if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) == -1)
      throw Joystick_Init_Failure();

    for(int i = 0, end = SDL_NumJoysticks(); i < end; ++i) {
      joystick.push_back(make_pair(SDL_JoystickOpen(i),
                                   SDL_JoystickName(i)));

      if(!joystick[i].first) {
        joystick.pop_back();
        quit_joysticks();

        throw Joystick_Init_Failure();
      }
    }

    SDL_JoystickEventState(SDL_ENABLE);
  }

  void Core::quit_joysticks() {
    SDL_JoystickEventState(SDL_DISABLE);

    for(int i = 0, end = SDL_NumJoysticks(); i < end; ++i)
      SDL_JoystickClose(joystick[i].first);

    joystick.clear();

    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
  }

}
