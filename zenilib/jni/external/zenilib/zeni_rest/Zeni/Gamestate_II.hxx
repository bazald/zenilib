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

#ifndef ZENI_GAMESTATE_II_HXX
#define ZENI_GAMESTATE_II_HXX

#include <Zeni/Gamestate_II.h>

namespace Zeni {

  const int & Gamestate_II::get_joyball_min() const {
    return m_joyball_min;
  }

  const int & Gamestate_II::get_joyball_max() const {
    return m_joyball_max;
  }

  const float & Gamestate_II::get_joystick_min() const {
    return m_joystick_min;
  }

  const float & Gamestate_II::get_joystick_max() const {
    return m_joystick_max;
  }

  const int & Gamestate_II::get_mouse_min() const {
    return m_mouse_min;
  }

  const int & Gamestate_II::get_mouse_max() const {
    return m_mouse_max;
  }

  const bool & Gamestate_II::is_firing_missed_events() const {
    return m_firing_missed_events;
  }

  void Gamestate_II::set_joyball_min(const int &min) {
    if(min < 0)
      m_joyball_min = 0;
    else
      m_joyball_min = min;
  }

  void Gamestate_II::set_joyball_max(const int &max) {
    if(max < 1)
      m_joyball_max = 1;
    else
      m_joyball_max = max;
  }

  void Gamestate_II::set_joystick_min(const float &min) {
    if(min < 0.0f)
      m_joystick_min = 0.0f;
    else
      m_joystick_min = min;
  }

  void Gamestate_II::set_joystick_max(const float &max) {
    if(max < 0.0f)
      m_joystick_max = 0.0f;
    else
      m_joystick_max = max;
  }

  void Gamestate_II::set_mouse_min(const int &min) {
    if(min < 0)
      m_mouse_min = 0;
    else
      m_mouse_min = min;
  }

  void Gamestate_II::set_mouse_max(const int &max) {
    if(max < 1)
      m_mouse_max = 1;
    else
      m_mouse_max = max;
  }

  void Gamestate_II::set_firing_missed_events(const bool &firing) {
    m_firing_missed_events = firing;
  }

}

#endif
