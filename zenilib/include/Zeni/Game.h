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

/**
 * \class Zeni::Game
 *
 * \ingroup zenilib
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

#include <Zeni/Console_State.h>
#include <Zeni/Gamestate.h>
#include <Zeni/Hash_Map.h>
#include <Zeni/Timer.h>

#include <stack>
#include <string>
#include <vector>

namespace Zeni {

  class Game {
    // Get reference to only instance;
    friend Game & get_Game(const std::vector<std::string> * const &args = 0); ///< Get access to the singleton.

    Game(const std::vector<std::string> * const args = 0);

    // Undefined
    Game(const Game &);
    Game & operator=(const Game &);

  public:
    inline Gamestate get_top(); ///< Get a reference to the current Gamestate.

#ifndef NDEBUG
    inline Console_State & get_console(); ///< Get a reference to the Console_State
#endif
    inline void write_to_console(const std::string &text); ///< Write text to the console when in Debug mode

    inline size_t size() const; ///< Get the current size of the Gamestate stack.
    inline void push_state(const Gamestate &state); ///< Push a new Gamestate onto the stack.
    inline Gamestate pop_state(); ///< Pop a Gamestate off the stack. Pop the current Gamestate with caution. All members will instantly become invalid.

    inline void on_event(const SDL_Event &event); ///< Called in main, calls the function by the same name in the current Gamestate.
    inline void perform_logic(); ///< Called in main, calls the function by the same name in the current Gamestate.
    inline void prerender(); ///< Called in main, calls the function by the same name in the current Gamestate.
    inline void render(); ///< Called in main, calls the function by the same name in the current Gamestate.

    inline size_t get_fps() const; ///< Get the current approximation of the frames displayed per second.
    inline bool get_key_state(const int &key) const; ///< Get the state of a key.
    inline bool get_mouse_button_state(const int &button) const; ///< Get the state of a mouse button.

    void run();

  private:
    void calculate_fps();

    std::stack<Gamestate> m_states;
    Unordered_Map<int, bool> m_keys;
    Unordered_Map<int, bool> m_mouse_buttons;

    Time time;
    Time::Tick_Type ticks_passed, fps, fps_next;

#ifndef NDEBUG
    void activate_console();
    void deactivate_console();
    Gamestate & get_console_instance();

    bool m_console_active;
#endif
  };

  Game & get_Game(const std::vector<std::string> * const &args); ///< Get access to the singleton.

  struct Zero_Gamestate : public Error {
    Zero_Gamestate() : Error("Zeni Gamestate State is Empty") {}
  };

}

#endif
