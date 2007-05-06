/* This file is part of the Zenipex Library.
* Copyleft (C) 2006 Mitchell Keith Bloch a.k.a. bazald
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

  static ofstream cerr_file("stderr.txt");
  static ofstream cout_file("stdout.txt");

  Core::Core() {
    cerr_bak = cerr.rdbuf();
    cout_bak = cout.rdbuf();

    cerr.rdbuf(cerr_file.rdbuf());
    cout.rdbuf(cout_file.rdbuf());

    if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) == -1)
      throw Core_Init_Failure();
  }

  Core::~Core() {
    SDL_Quit();

    cout.rdbuf(cout_bak);
    cerr.rdbuf(cerr_bak);
  }

  Core & Core::get_reference() {
    static Core e_core;
    return e_core;
  }

}
