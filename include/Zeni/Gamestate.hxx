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

#ifndef ZENI_GAMESTATE_HXX
#define ZENI_GAMESTATE_HXX

#include <Zeni/Gamestate.h>

namespace Zeni {

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

  Gamestate::Gamestate(Gamestate_Base *state)
    : m_state(state)
  {
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
    if(m_state)
      m_state->decrement();
    m_state=state.m_state;
    if(m_state)
      m_state->increment();
    return *this;
  }

  void Gamestate::on_event(const SDL_Event &event) {
    m_state->on_event(event);
  }

  void Gamestate::perform_logic() {
    m_state->perform_logic();
  }

  void Gamestate::render() {
    m_state->render();
  }
  
  Gamestate_Base & Gamestate::get() {
    return *m_state;
  }
  
  float Gamestate_II::get_min_confidence() const {
    return m_min_confidence;
  }

  float Gamestate_II::get_max_confidence() const {
    return m_max_confidence;
  }

  void Gamestate_II::set_min_confidence(const float &min) {
    m_min_confidence = min;
    if(m_min_confidence < 0.0f || m_min_confidence >= m_max_confidence)
      m_min_confidence = 0.0f;
  }

  void Gamestate_II::set_max_confidence(const float &max) {
    m_max_confidence = max;
    if(m_max_confidence > 1.0f || m_max_confidence <= m_min_confidence)
      m_max_confidence = 1.0f;
  }

}

#endif
