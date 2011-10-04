/* This file is part of the Zenipex Library (zenilib).
 * Copyright (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * zenilib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * zenilib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with zenilib.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ZENI_GAME_HXX
#define ZENI_GAME_HXX

#include <Zeni/Console_State.h>

// HXXed below
#include <Zeni/Gamestate.h>

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
  void Game::write_to_console(const String &text) {
    get_console().write_to_log(text);
#else
  void Game::write_to_console(const String & /*text*/) {
#endif
  }

  size_t Game::size() const {
    return m_states.size();
  }

  void Game::push_state(const Gamestate &state) {
    if(!m_states.empty())
      m_states.top().on_cover();

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

    if(!m_states.empty())
      m_states.top().on_uncover();

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

      case SDL_JOYBUTTONDOWN:
        m_joy_buttons[event.jbutton.which][event.jbutton.button] = true;
        break;

      case SDL_JOYBUTTONUP:
        m_joy_buttons[event.jbutton.which][event.jbutton.button] = false;
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

  bool Game::get_joy_button_state(const int &which, const int &button) const {
    const Unordered_Map<int, Unordered_Map<int, bool> >::const_iterator jt = m_joy_buttons.find(which);

    if(jt != m_joy_buttons.end()) {
      const Unordered_Map<int, bool>::const_iterator it = jt->second.find(button);
      if(it != jt->second.end())
        return it->second;
    }

    return false;
  }

}

#include <Zeni/Gamestate.hxx>

#endif
