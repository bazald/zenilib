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

#ifndef ZENI_TIMER_HQ_HXX
#define ZENI_TIMER_HQ_HXX

#include <Zeni/Timer_HQ.h>

namespace Zeni {

  Time_HQ & Time_HQ::operator=(const HQ_Tick_Type &ticks) {
    m_ticks = ticks;
    m_ticks_per_second = get_Timer_HQ().get_ticks_per_second();
    return *this;
  }

  HQ_Tick_Type Time_HQ::get_ticks_passed() const {
    return get_Timer_HQ().get_ticks() - m_ticks;
  }

  long double Time_HQ::get_seconds_passed() const {
    return static_cast<long double>(get_ticks_passed()) / m_ticks_per_second;
  }

  HQ_Tick_Type Time_HQ::get_ticks_since(const Time_HQ &time) const {
    return m_ticks - time.m_ticks;
  }

  long double Time_HQ::get_seconds_since(const Time_HQ &time) const {
    return static_cast<long double>(get_ticks_since(time)) / m_ticks_per_second;
  }

  void Time_HQ::update() {
    m_ticks = get_Timer_HQ().get_ticks();
  }

  HQ_Tick_Type Timer_HQ::get_ticks() {
    update();
#ifdef _WINDOWS
    return m_ticks;
#else
    return to_useconds(m_ticks);
#endif
  }

  HQ_Tick_Type Timer_HQ::get_ticks_per_second() {
    return m_ticks_per_second;
  }

  long double Timer_HQ::get_seconds() {
    return static_cast<long double>(double(get_ticks())) / m_ticks_per_second;
  }

  Time_HQ Timer_HQ::get_time() {
    update();
#ifdef _WINDOWS
    return Time_HQ(m_ticks, m_ticks_per_second);
#else
    return Time_HQ(to_useconds(m_ticks), m_ticks_per_second);
#endif
  }

  void Timer_HQ::update() {
#ifdef _WINDOWS
    LARGE_INTEGER lpPerformanceCount;
    QueryPerformanceCounter(&lpPerformanceCount);

    m_ticks = lpPerformanceCount.QuadPart;
#else
    gettimeofday(&m_ticks, 0);
#endif
  }

  bool Time_HQ::operator<(const Time_HQ &rhs) const {
    return m_ticks < rhs.m_ticks;
  }

}

#endif
