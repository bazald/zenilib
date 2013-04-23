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

#ifdef _MACOSX
#include <mach/mach_time.h>
#endif

namespace Zeni {

#ifndef _WINDOWS
  timespec subtract(const timespec &lhs, const timespec &rhs) {
    timespec retval = lhs;

    retval.tv_sec -= rhs.tv_sec;

    if(retval.tv_nsec > rhs.tv_nsec)
      retval.tv_nsec -= rhs.tv_nsec;
    else {
      --retval.tv_sec;
      retval.tv_nsec = 1000000000 - (rhs.tv_nsec - retval.tv_nsec);
    }

    return retval;
  }

  long double to_seconds(const timespec &ticks) {
    return ticks.tv_sec + ticks.tv_nsec / 1000000000.0;
  }

  long double to_useconds(const timespec &ticks) {
    return 1000000000.0 * ticks.tv_sec + ticks.tv_nsec;
  }

#ifdef _MACOSX
  timespec orwl_gettime(void) {
    static const double ORWL_NANO = +1.0e-9;
    static const uint64_t ORWL_GIGA = 1000000000;
    
    static double orwl_timebase = 0.0;
    static uint64_t orwl_timestart = 0;

    // be more careful in a multithreaded environement
    if(!orwl_timestart) {
      mach_timebase_info_data_t tb = {0};
      mach_timebase_info(&tb);
      orwl_timebase = tb.numer;
      orwl_timebase /= tb.denom;
      orwl_timestart = mach_absolute_time();
    }

    struct timespec t;
    double diff = (mach_absolute_time() - orwl_timestart) * orwl_timebase;
    t.tv_sec = diff * ORWL_NANO;
    t.tv_nsec = diff - (t.tv_sec * ORWL_GIGA);
    return t;
  }
#endif
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
    m_ticks_per_second = 1000000000;
#endif
  }

  Timer_HQ & get_Timer_HQ() {
    return Timer_HQ::get();
  }

}
