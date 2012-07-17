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

#ifdef ANDROID
  void Gamestate::on_event(android_app &app, const AInputEvent &event) {
    m_state->on_event(app, event);
  }
#else
  void Gamestate::on_event(const SDL_Event &event) {
    m_state->on_event(event);
  }
#endif

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

  void Gamestate::on_cover() {
    m_state->on_cover();
  }

  void Gamestate::on_uncover() {
    m_state->on_uncover();
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

}

#endif
