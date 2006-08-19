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

#include <Zeni/Game.h>
#include <Zeni/zeniapp.h>

#include <Zeni/Gamestate.hxx>

namespace Zeni {

  Game::Game(const std::vector<std::string> * const args)
    : fps(0x36)
  {
    // Ensure Core is initialized
    Core::get_reference();

    m_states.push(Gamestate(new Gamestate_One(args)));
  }

  Game & Game::get_reference(const std::vector<std::string> * const args) {
    static Game e_game(args);
    return e_game;
  }

  void Game::calculate_fps() {
    static unsigned int
      frames_output = 0, frames_passed = 0,
      ticks_output = 0,  ticks_passed = 0,  ticks_prev = 0;

    ticks_passed = SDL_GetTicks();

    static bool first_time = true;
    if(first_time) {
      ticks_output = ticks_passed;
      first_time = false;
    }
    else
      ++frames_passed;

    if((ticks_passed - ticks_output) > 1000) {
      fps =  (frames_passed - frames_output) * 1000 / (ticks_passed - ticks_output);

      frames_output = frames_passed;
      ticks_output =  ticks_passed;
    }

    if((ticks_passed - ticks_prev) < 10) {
      SDL_Delay(5);
    }
    ticks_prev = ticks_passed;
  }

}
