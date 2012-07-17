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

#include <zeni_rest.h>

#include <algorithm>
#include <cmath>

#include <Zeni/Define.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace Zeni {

#ifndef ANDROID
  Zeni_Input_ID::Zeni_Input_ID(const Uint16 &type_, const int &subid_, const int &which_)
    : type(type_),
    subid(subid_),
    which(which_),
    previous_confidence(0.0f)
  {
  }
#endif

  bool Zeni_Input_ID::operator<(const Zeni_Input_ID &rhs) const {
    return type < rhs.type ||
           (type == rhs.type && (subid < rhs.subid ||
           (subid == rhs.subid && which < rhs.which)));
  }

  Gamestate_II::Gamestate_II()
    : m_joyball_min(ZENI_DEFAULT_II_JOYBALL_MIN),
    m_joyball_max(ZENI_DEFAULT_II_JOYBALL_MAX),
    m_joystick_min(ZENI_DEFAULT_II_JOYSTICK_MIN),
    m_joystick_max(ZENI_DEFAULT_II_JOYSTICK_MAX),
    m_mouse_min(ZENI_DEFAULT_II_MOUSE_MIN),
    m_mouse_max(ZENI_DEFAULT_II_MOUSE_MAX),
    m_firing_missed_events(false)
  {
  }

#ifndef ANDROID
  void Gamestate_II::on_event(const SDL_Event &event) {
    switch(event.type) {
    case SDL_JOYAXISMOTION:
      {
        float confidence = (float(event.jaxis.value) + 0.5f) / 32767.5f;
        const float ac = float(fabs(confidence));
        const float nm = confidence < 0.0f ? -1.0f : 1.0f;
        confidence = nm * std::min(std::max(ac - m_joystick_min, 0.0f) / (m_joystick_max - m_joystick_min), 1.0f);

        fire_event(Zeni_Input_ID(SDL_JOYAXISMOTION, event.jaxis.axis, event.jaxis.which), confidence);
      }
      break;
    case SDL_JOYBALLMOTION:
      {
        const int ac = abs(event.jball.xrel);
        const int nm = event.jball.xrel < 0.0f ? -1 : 1;
        const float confidence = nm * std::min(std::max(float(ac - m_joyball_min), 0.0f) / (m_joyball_max - m_joyball_min), 1.0f);

        fire_event(Zeni_Input_ID(SDL_JOYBALLMOTION, event.jball.ball * 2 + 0, event.jball.which), confidence);
      }
      {
        const int ac = abs(event.jball.yrel);
        const int nm = event.jball.yrel < 0.0f ? -1 : 1;
        const float confidence = nm * std::min(std::max(float(ac - m_joyball_min), 0.0f) / (m_joyball_max - m_joyball_min), 1.0f);

        fire_event(Zeni_Input_ID(SDL_JOYBALLMOTION, event.jball.ball * 2 + 1, event.jball.which), confidence);
      }
      break;
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
      {
        const float confidence = event.jbutton.state == SDL_PRESSED ? 1.0f : 0.0f;

        fire_event(Zeni_Input_ID(SDL_JOYBUTTONDOWN, event.jbutton.button, event.jbutton.which), confidence);
      }
      break;
    case SDL_JOYHATMOTION:
      {
        float confidence;

        switch(event.jhat.value) {
          case SDL_HAT_LEFT:
          case SDL_HAT_LEFTDOWN:
          case SDL_HAT_LEFTUP:
            confidence = -1.0f;
            break;

          case SDL_HAT_RIGHT:
          case SDL_HAT_RIGHTDOWN:
          case SDL_HAT_RIGHTUP:
            confidence = 1.0f;
            break;

          case SDL_HAT_CENTERED:
          case SDL_HAT_DOWN:
          case SDL_HAT_UP:
          default:
            confidence = 0.0f;
            break;
        }

        fire_event(Zeni_Input_ID(SDL_JOYHATMOTION, 0, event.jhat.which), confidence);
      }
      {
        float confidence;

        switch(event.jhat.value) {
          case SDL_HAT_DOWN:
          case SDL_HAT_LEFTDOWN:
          case SDL_HAT_RIGHTDOWN:
            confidence = -1.0f;
            break;

          case SDL_HAT_UP:
          case SDL_HAT_LEFTUP:
          case SDL_HAT_RIGHTUP:
            confidence = 1.0f;
            break;

          case SDL_HAT_CENTERED:
          case SDL_HAT_LEFT:
          case SDL_HAT_RIGHT:
          default:
            confidence = 0.0f;
            break;
        }

        fire_event(Zeni_Input_ID(SDL_JOYHATMOTION, 1, event.jhat.which), confidence);
      }
      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      {
        const float confidence = event.key.state == SDL_PRESSED ? 1.0f : 0.0f;

        fire_event(Zeni_Input_ID(SDL_KEYDOWN, event.key.keysym.sym, 0 /*event.key.which*/), confidence);
      }
      break;
    case SDL_MOUSEMOTION:
      {
        const int ac = abs(event.motion.xrel);
        const int nm = event.motion.xrel < 0.0f ? -1 : 1;
        const float confidence = nm * std::min(std::max(float(ac - m_mouse_min), 0.0f) / (m_mouse_max - m_mouse_min), 1.0f);

        fire_event(Zeni_Input_ID(SDL_MOUSEMOTION, 0, 0 /*event.motion.which*/), confidence);
      }
      {
        const int ac = abs(event.motion.yrel);
        const int nm = event.motion.yrel < 0.0f ? -1 : 1;
        const float confidence = nm * std::min(std::max(float(ac - m_mouse_min), 0.0f) / (m_mouse_max - m_mouse_min), 1.0f);

        fire_event(Zeni_Input_ID(SDL_MOUSEMOTION, 1, 0 /*event.motion.which*/), confidence);
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      {
        const float confidence = event.button.state == SDL_PRESSED ? 1.0f : 0.0f;

        fire_event(Zeni_Input_ID(SDL_MOUSEBUTTONDOWN, event.button.button, 0 /*event.button.which*/), confidence);
      }
      break;
    default:
      Gamestate_Base::on_event(event);
      break;
    }
  }
#endif

  void Gamestate_II::on_event(const Zeni_Input_ID &id, const float &confidence, const int &) {
#ifndef ANDROID
    if(id.type == SDL_KEYDOWN && id.subid == SDLK_ESCAPE && confidence == 1.0f)
      get_Game().push_state(new Popup_Menu_State);
#endif
  }

  void Gamestate_II::on_push() {
    fire_missed_events();
  }

  void Gamestate_II::on_uncover() {
    fire_missed_events();
  }

  int Gamestate_II::get_action(const Zeni_Input_ID &event) {
    std::map<Zeni_Input_ID, int>::const_iterator it = m_ii.find(event);
    if(it != m_ii.end())
      return it->second;
    return 0;
  }

  Zeni_Input_ID Gamestate_II::get_event(const int &action) {
    std::map<int, Zeni_Input_ID>::const_iterator it = m_rii.find(action);
    if(it != m_rii.end())
      return it->second;
    return Zeni_Input_ID();
  }

  void Gamestate_II::set_action(const Zeni_Input_ID &event, const int &action) {
    m_ii[event] = action;
    m_rii[action] = event;
  }

  void Gamestate_II::fire_missed_events() {
    Game &gr = get_Game();

    for(std::map<Zeni_Input_ID, int>::iterator it = m_ii.begin(), iend = m_ii.end(); it != iend; ++it) {
      switch(it->first.type) {
#ifndef ANDROID
      case SDL_KEYDOWN:
        {
          const float confidence = gr.get_key_state(it->first.subid) ? 1.0f : 0.0f;

          if(m_firing_missed_events && it->first.previous_confidence != confidence)
            on_event(it->first, confidence, it->second);

          it->first.previous_confidence = confidence;
        }
        break;

      case SDL_MOUSEBUTTONDOWN:
        {
          const float confidence = gr.get_mouse_button_state(it->first.subid) ? 1.0f : 0.0f;

          if(m_firing_missed_events && it->first.previous_confidence != confidence)
            on_event(it->first, confidence, it->second);

          it->first.previous_confidence = confidence;
        }
        break;

      case SDL_JOYBUTTONDOWN:
        {
          const float confidence = gr.get_joy_button_state(it->first.which, it->first.subid) ? 1.0f : 0.0f;

          if(m_firing_missed_events && it->first.previous_confidence != confidence)
            on_event(it->first, confidence, it->second);

          it->first.previous_confidence = confidence;
        }
        break;
#endif

      default:
        break;
      }
    }
  }

  void Gamestate_II::fire_event(const Zeni_Input_ID &id, const float &confidence) {
    std::map<Zeni_Input_ID, int>::iterator it = m_ii.find(id);
    if(it != m_ii.end()) {
      float &pc = it->first.previous_confidence;
      if(pc != confidence) {
        pc = confidence;
        on_event(id, confidence, it->second);
      }
    }
    else
      on_event(id, confidence, 0);
  }

}

#include <Zeni/Undefine.h>
