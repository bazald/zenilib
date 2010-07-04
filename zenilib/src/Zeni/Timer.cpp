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

#include <Zeni/Timer.hxx>

#include <Zeni/Core.h>

namespace Zeni {

  Time::Time(const Time::Tick_Type &ticks)
    : m_ticks(ticks)
  {
  }

  Time::Time()
    : m_ticks(get_Timer().get_ticks())
  {
  }

  Timer::Timer() {
    // Ensure Core is initialized
    get_Core();
  }

  Timer & get_Timer() {
    static Timer timer;
    return timer;
  }

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

  Timer_HQ::Timer_HQ()
    : m_ticks_per_second(1000)
  {
#ifdef _WINDOWS
    LARGE_INTEGER lpFrequency;
    m_available = QueryPerformanceFrequency(&lpFrequency) != 0;

    if(m_available)
      m_ticks_per_second = unsigned long(lpFrequency.QuadPart);
#else
    m_available = true;
    m_ticks_per_second = 1000000;
#endif
  }

  Timer_HQ & get_Timer_HQ() {
    static Timer_HQ timer;
    return timer;
  }

}
