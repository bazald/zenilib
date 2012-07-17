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

#include <zeni.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Chronometer.hxx>
#include <Zeni/Singleton.hxx>

namespace Zeni {

#ifndef _WINDOWS
  timeval subtract(const timeval &lhs, const timeval &rhs) {
    timeval retval = lhs;

    retval.tv_sec -= rhs.tv_sec;

    if(retval.tv_usec > rhs.tv_usec)
      retval.tv_usec -= rhs.tv_usec;
    else {
      --retval.tv_sec;
      retval.tv_usec = 1000000 - (rhs.tv_usec - retval.tv_usec);
    }

    return retval;
  }

  long double to_seconds(const timeval &ticks) {
    return ticks.tv_sec + ticks.tv_usec / 1000000.0;
  }

  long double to_useconds(const timeval &ticks) {
    return 1000000.0 * ticks.tv_sec + ticks.tv_usec;
  }
#endif

  Time_HQ::Time_HQ(const HQ_Tick_Type &ticks)
    : m_ticks(ticks),
    m_ticks_per_second(get_Timer_HQ().get_ticks_per_second())
  {
  }

  Time_HQ::Time_HQ(const HQ_Tick_Type &ticks, const HQ_Tick_Type &ticks_per_second)
    : m_ticks(ticks),
    m_ticks_per_second(ticks_per_second)
  {
  }

  Time_HQ::Time_HQ()
    : m_ticks(get_Timer_HQ().get_ticks()),
    m_ticks_per_second(get_Timer_HQ().get_ticks_per_second())
  {
  }

  template class Singleton<Timer_HQ>;
  template class Chronometer<Time_HQ>;

  Timer_HQ * Timer_HQ::create() {
    return new Timer_HQ;
  }

  Timer_HQ::Timer_HQ()
    : m_ticks_per_second(1000)
  {
#ifdef _WINDOWS
    LARGE_INTEGER lpFrequency;
    if(QueryPerformanceFrequency(&lpFrequency) == 0)
      throw Timer_HQ_Init_Failure();

    m_ticks_per_second = unsigned long(lpFrequency.QuadPart);
#else
    m_ticks_per_second = 1000000;
#endif
  }

  Timer_HQ & get_Timer_HQ() {
    return Timer_HQ::get();
  }

}
