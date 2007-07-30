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

#ifndef ZENI_GAME_HXX
#define ZENI_GAME_HXX

#include <Zeni/Game.h>

namespace Zeni {

  Gamestate_Base & Game::get_current_state() {
    return m_states.top().get();
  }

  void Game::push_state(const Gamestate &state) {
    m_states.push(state);
  }

  Gamestate Game::pop_state() {
    if(m_states.empty())
      throw Zero_Gamestate();

    Gamestate top = m_states.top();
    m_states.pop();

    return top;
  }

  void Game::on_event(const SDL_Event &event) {
    if(m_states.empty())
      throw Zero_Gamestate();

    m_states.top().on_event(event);
  }

  void Game::perform_logic() {
    if(m_states.empty())
      throw Zero_Gamestate();

    m_states.top().perform_logic();
  }

  void Game::render() {
    if(m_states.empty())
      throw Zero_Gamestate();

    m_states.top().render();

    calculate_fps();
  }

  int Game::get_fps() const {
    return fps;
  }

}

#endif
