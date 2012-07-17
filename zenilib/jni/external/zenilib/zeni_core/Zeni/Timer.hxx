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

#ifndef ZENI_TIMER_HXX
#define ZENI_TIMER_HXX

#include <Zeni/Timer.h>

#include <SDL/SDL.h>

namespace Zeni {

  Time & Time::operator=(const Time::Tick_Type &ticks) {
    m_ticks = ticks;
    return *this;
  }

  Time::Tick_Type Time::get_ticks_passed() const {
    return get_Timer().get_ticks() - m_ticks;
  }

  float Time::get_seconds_passed() const {
    return float(get_ticks_passed()) * 0.001f;
  }

  Time::Tick_Type Time::get_ticks_since(const Time &time) const {
    return m_ticks - time.m_ticks;
  }

  float Time::get_seconds_since(const Time &time) const {
    return float(get_ticks_since(time)) * 0.001f;
  }

  void Time::update() {
    m_ticks = get_Timer().get_ticks();
  }

  bool Time::operator<(const Time &rhs) const {
    return m_ticks < rhs.m_ticks;
  }

  Time::Tick_Type Timer::get_ticks() {
    update();
    return m_ticks;
  }

  Time::Tick_Type Timer::get_ticks_per_second() {
    return 1000u;
  }

  float Timer::get_seconds() {
    return float(get_ticks()) * 0.001f;
  }

  Time Timer::get_time() {
    update();
    return Time(m_ticks);
  }

}

#endif
