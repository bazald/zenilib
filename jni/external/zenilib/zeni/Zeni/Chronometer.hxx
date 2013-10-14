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

#ifndef ZENI_CHRONOMETER_HXX
#define ZENI_CHRONOMETER_HXX

// HXXed below
#include <Zeni/Timer_HQ.h>

#include <Zeni/Chronometer.h>
#include <cassert>

namespace Zeni {

  template <class TIME>
  Chronometer<TIME>::Chronometer()
    : m_seconds_before_start(TIME::Second_Type()),
    m_running(false),
    m_scaling_factor(typename TIME::Second_Type(1))
  {
  }

  template <class TIME>
  const bool & Chronometer<TIME>::is_running() const {
    return m_running;
  }

  template <class TIME>
  typename Chronometer<TIME>::Second_Type Chronometer<TIME>::start() {
    m_seconds_before_start = seconds();
    m_start = get_time_passed();
    m_running = true;
    return m_seconds_before_start;
  }

  template <class TIME>
  typename Chronometer<TIME>::Second_Type Chronometer<TIME>::stop() {
    m_seconds_before_start = seconds();
    m_running = false;
    return m_seconds_before_start;
  }

  template <class TIME>
  typename TIME::Second_Type Chronometer<TIME>::seconds() const {
    return m_seconds_before_start + (m_running ? (get_time_passed() - m_start) * m_scaling_factor : 0.0f);
  }

  template <class TIME>
  void Chronometer<TIME>::set(const typename TIME::Second_Type &time) {
    m_seconds_before_start = time;
    m_start = get_time_passed();
  }

  template <class TIME>
  void Chronometer<TIME>::reset() {
    m_seconds_before_start = 0.0f;
    m_start = get_time_passed();
  }

  template <class TIME>
  const typename TIME::Second_Type & Chronometer<TIME>::scaling_factor() const {
    return m_scaling_factor;
  }

  template <class TIME>
  void Chronometer<TIME>::scale(const typename TIME::Second_Type &scaling_factor) {
    m_seconds_before_start = seconds();
    m_start = get_time_passed();
    m_scaling_factor = scaling_factor;
  }

  template <class TIME>
  bool Chronometer<TIME>::are_paused() {
    return g_are_paused;
  }

  template <class TIME>
  void Chronometer<TIME>::pause_all() {
    g_seconds_before_start = get_time_passed();
    g_are_paused = true;
  }

  template <class TIME>
  void Chronometer<TIME>::unpause_all() {
    get_start_time() = TIME();
    g_are_paused = false;
  }
  
  template <class TIME>
  typename TIME::Second_Type Chronometer<TIME>::get_time_passed() {
    return g_seconds_before_start + (g_are_paused ? TIME::Second_Type() : TIME().get_seconds_since(get_start_time()));
  }
  
  template <class TIME>
  TIME & Chronometer<TIME>::get_start_time() {
    static TIME g_time;
    return g_time;
  }
  template <class TIME>
  typename TIME::Second_Type Chronometer<TIME>::g_seconds_before_start = TIME::Second_Type();
  template <class TIME>
  bool Chronometer<TIME>::g_are_paused = false;

}

#include <Zeni/Timer_HQ.hxx>

#endif
