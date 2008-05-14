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

/**
 * \class Zeni::Game
 *
 * \ingroup Zenilib
 *
 * \brief The Gamestate Stack
 *
 * This singleton is a stack of Gamestates.  The main function
 * continually refers to the Game singleton to get the current 
 * Gamestate.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_GAME_H
#define ZENI_GAME_H

#include <Zeni/Gamestate.h>
#include <Zeni/Timer.h>

#include <stack>
#include <string>
#include <vector>

namespace Zeni {

  class Game {
    Game(const std::vector<std::string> * const args = 0);

    // Undefined
    Game(const Game &);
    Game & operator=(const Game &);

  public:
    // Get reference to only instance
    static Game & get_reference(const std::vector<std::string> * const args = 0); ///< Get access to the singleton.

    inline Gamestate_Base & get_current_state(); ///< Get a reference to the current Gamestate.

    inline void push_state(const Gamestate &state); ///< Push a new Gamestate onto the stack.
    inline Gamestate pop_state(); ///< Pop a Gamestate off the stack. Pop the current Gamestate with caution. All members will instantly become invalid.

    inline void on_event(const SDL_Event &event); ///< Called in main, calls the function by the same name in the current Gamestate.
    inline void perform_logic(); ///< Called in main, calls the function by the same name in the current Gamestate.
    inline void render(); ///< Called in main, calls the function by the same name in the current Gamestate.

    inline int get_fps() const; ///< Get the current approximation of the frames displayed per second.

  private:
    void calculate_fps();

    std::stack<Gamestate> m_states;

    Time time;
    int ticks_passed, fps, fps_next;
  };

  struct Zero_Gamestate : public Error {
    Zero_Gamestate() : Error("Zeni Gamestate State is Empty") {}
  };

}

#ifdef ZENI_INLINES
#include <Zeni/Game.hxx>
#endif

#endif
