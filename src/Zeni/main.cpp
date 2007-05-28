/* This file is part of the Zenipex Library.
* Copyleft (C) 2007 Mitchell Keith Bloch a.k.a. bazald
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

#include <Zeni/Game.hxx>

#include <Zeni/Video.h>

#include <Zeni/Gamestate.hxx>

#include <iostream>
#include <ctime>

using namespace std;
using namespace Zeni;

int main(int argc, char *argv[]) {
  srand(static_cast<unsigned int>(time(0)));

  vector<string> args(argc - 1);
  for(int i = 1; i < argc; ++i)
    args[i - 1] = argv[i];

  try {
    // Start Engines
    Game::get_reference(&args);

    SDL_Event event;

    for(;;) {
      while(SDL_PollEvent(&event))
        Game::get_reference().on_event(event);

      Game::get_reference().perform_logic();

      Video::get_reference().render_all();
    }
  }
  catch(Quit_Event &nonerror) {
    cerr << nonerror.msg << endl;
  }
  catch(Error &error) {
    cerr << error.msg << endl;
    return 1;
  }
  catch(...) {
    cerr << "Unrecognized Error Captured in main\n";
    return -1;
  }

  return 0;
}
