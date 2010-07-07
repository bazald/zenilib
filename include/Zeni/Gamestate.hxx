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

#ifndef ZENI_GAMESTATE_HXX
#define ZENI_GAMESTATE_HXX

#include <Zeni/Gamestate.h>

namespace Zeni {

  const bool & Gamestate_Base::is_pausable() const {
    return m_pausable;
  }

  void Gamestate_Base::set_pausable(const bool &pausable_) {
    m_pausable = pausable_;
  }

  void Gamestate_Base::increment() {
    ++m_count;
  }

  void Gamestate_Base::decrement() {
    if(!--m_count)
      delete this;
  }
  
  Gamestate::Gamestate()
    : m_state(0)
  {
  }

  Gamestate::Gamestate(Gamestate_Base * const &state)
    : m_state(state)
  {
    assert(m_state);

    if(m_state)
      m_state->increment();
  }

  Gamestate::Gamestate(const Gamestate &state)
    : m_state(state.m_state)
  {
    if(m_state)
      m_state->increment();
  }

  Gamestate & Gamestate::operator=(const Gamestate &state) {
    if(state.m_state)
      state.m_state->increment();

    if(m_state)
      m_state->decrement();

    m_state = state.m_state;

    return *this;
  }

  void Gamestate::on_event(const SDL_Event &event) {
    m_state->on_event(event);
  }

  void Gamestate::perform_logic() {
    m_state->perform_logic();
  }

  void Gamestate::prerender() {
    m_state->prerender();
  }

  void Gamestate::render() {
    m_state->render();
  }

  void Gamestate::on_push() {
    m_state->on_push();
  }

  void Gamestate::on_pop() {
    m_state->on_pop();
  }

  const bool & Gamestate::is_pausable() const {
    return m_state->is_pausable();
  }

  Gamestate_Base & Gamestate::get() {
    return *m_state;
  }

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

}

#endif
