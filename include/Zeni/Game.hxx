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

#ifndef ZENI_GAME_HXX
#define ZENI_GAME_HXX

// HXXed below
#include <Zeni/Gamestate.h>
#include <Zeni/Mutex.h>

#include <Zeni/Game.h>

namespace Zeni {

  Gamestate Game::get_top() {
    if(m_states.empty())
      throw Zero_Gamestate();

    return m_states.top();
  }

#ifndef NDEBUG
  Console_State & Game::get_console() {
    return static_cast<Console_State &>(get_console_instance().get());
  }
#endif

#ifndef NDEBUG
  void Game::write_to_console(const std::string &text) {
    get_console().write_to_log(text);
#else
  void Game::write_to_console(const std::string & /*text*/) {
#endif
  }

  size_t Game::size() const {
    return m_states.size();
  }

  void Game::push_state(const Gamestate &state) {
    m_states.push(state);

#ifndef NDEBUG
    if(m_console_active)
      get_console().set_child(m_states.top());
#endif

    m_states.top().on_push();
  }

  Gamestate Game::pop_state() {
    Gamestate gs;

    {
      if(m_states.empty())
        throw Zero_Gamestate();

      gs = m_states.top();
      m_states.pop();

#ifndef NDEBUG
      if(m_console_active) {
        Console_State &csr = get_console();

        if(!m_states.empty())
          csr.set_child(m_states.top());
        else
          csr.clear_child();
      }
#endif
    }

    gs.on_pop();

    return gs;
  }

  void Game::on_event(const SDL_Event &event) {
    switch(event.type) {
      case SDL_KEYDOWN:
        m_keys[event.key.keysym.sym] = true;
        break;

      case SDL_KEYUP:
        m_keys[event.key.keysym.sym] = false;
        break;

      case SDL_MOUSEBUTTONDOWN:
        m_mouse_buttons[event.button.button] = true;
        break;

      case SDL_MOUSEBUTTONUP:
        m_mouse_buttons[event.button.button] = false;
        break;

      default:
        break;
    }

    Gamestate gs;
#ifndef NDEBUG
    Gamestate console_child;
#endif

    {
      if(m_states.empty())
        throw Zero_Gamestate();

#ifndef NDEBUG
      if(m_console_active) {
        gs = get_console_instance();
        console_child = get_console().get_child();
      }
      else
#endif
      {
        gs = m_states.top();
      }
    }

    gs.on_event(event);
  }

  void Game::perform_logic() {
    Gamestate gs;
#ifndef NDEBUG
    Gamestate console_child;
#endif

    {
      if(m_states.empty())
        throw Zero_Gamestate();

#ifndef NDEBUG
      if(m_console_active) {
        gs = get_console_instance();
        console_child = get_console().get_child();
      }
      else
#endif
      {
        gs = m_states.top();
      }
    }

    gs.perform_logic();
  }

  void Game::prerender() {
    Gamestate gs;
#ifndef NDEBUG
    Gamestate console_child;
#endif

    {
      if(m_states.empty())
        throw Zero_Gamestate();

#ifndef NDEBUG
      if(m_console_active) {
        gs = get_console_instance();
        console_child = get_console().get_child();
      }
      else
#endif
      {
        gs = m_states.top();
      }
    }

    gs.prerender();
  }

  void Game::render() {
    Gamestate gs;
#ifndef NDEBUG
    Gamestate console_child;
#endif

    {
      if(m_states.empty())
        throw Zero_Gamestate();

#ifndef NDEBUG
      if(m_console_active) {
        gs = get_console_instance();
        console_child = get_console().get_child();
      }
      else
#endif
      {
        gs = m_states.top();
      }
    }

    gs.render();

    calculate_fps();
  }

  size_t Game::get_fps() const {
    return fps;
  }

  bool Game::get_key_state(const int &button) const {
    const Unordered_Map<int, bool>::const_iterator it = m_keys.find(button);
    if(it != m_keys.end())
      return it->second;
    return false;
  }

  bool Game::get_mouse_button_state(const int &button) const {
    const Unordered_Map<int, bool>::const_iterator it = m_mouse_buttons.find(button);
    if(it != m_mouse_buttons.end())
      return it->second;
    return false;
  }

}

#include <Zeni/Gamestate.hxx>
#include <Zeni/Mutex.hxx>

#endif
