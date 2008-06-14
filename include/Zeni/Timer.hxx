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

#ifndef ZENI_TIMER_HXX
#define ZENI_TIMER_HXX

#include <Zeni/Timer.h>

#include <SDL/SDL.h>

namespace Zeni {

  Time & Time::operator=(const int &ticks) {
    m_ticks = ticks;
    return *this;
  }

  int Time::get_ticks_passed() const {
    return Timer::get_reference().get_ticks() - m_ticks;
  }

  float Time::get_seconds_passed() const {
    return get_ticks_passed() * 0.001f;
  }

  int Time::get_ticks_since(const Time &time) const {
    return m_ticks - time.m_ticks;
  }

  float Time::get_seconds_since(const Time &time) const {
    return get_ticks_since(time) * 0.001f;
  }

  void Time::update() {
    m_ticks = Timer::get_reference().get_ticks();
  }

  int Timer::get_ticks() {
    Mutex::Lock lock(ticks_mutex);
    update();
    return m_ticks;
  }

  int Timer::get_ticks_per_second() {
    return 1000;
  }

  float Timer::get_seconds() {
    return get_ticks() * 0.001f;
  }

  Time Timer::get_time() {
    Mutex::Lock lock(ticks_mutex);
    update();
    return Time(m_ticks);
  }

  void Timer::update() {
    m_ticks = SDL_GetTicks();
  }

  Time_HQ & Time_HQ::operator=(const HQ_Tick_Type &ticks) {
    m_ticks = ticks;
    m_ticks_per_second = Timer_HQ::get_reference().get_ticks_per_second();
    return *this;
  }

  HQ_Tick_Type Time_HQ::get_ticks_passed() const {
#ifdef _WINDOWS
    return Timer_HQ::get_reference().get_ticks() - m_ticks;
#else
    return subtract(Timer_HQ::get_reference().get_ticks(), m_ticks);
#endif
  }

  double Time_HQ::get_seconds_passed() const {
#ifdef _WINDOWS
    return double(get_ticks_passed()) / m_ticks_per_second;
#else
    return to_seconds(m_ticks);
#endif
  }

  HQ_Tick_Type Time_HQ::get_ticks_since(const Time_HQ &time) const {
#ifdef _WINDOWS
    return m_ticks - time.m_ticks;
#else
    return subtract(m_ticks, time.m_ticks);
#endif
  }

  double Time_HQ::get_seconds_since(const Time_HQ &time) const {
#ifdef _WINDOWS
    return double(get_ticks_since(time)) / m_ticks_per_second;
#else
    return to_seconds(get_ticks_since(time));
#endif
  }

  void Time_HQ::update() {
    m_ticks = Timer_HQ::get_reference().get_ticks();
  }

  HQ_Tick_Type Timer_HQ::get_ticks() {
    Mutex::Lock lock(ticks_mutex);
    update();
    return m_ticks;
  }

  unsigned long Timer_HQ::get_ticks_per_second() {
    return m_ticks_per_second;
  }

  double Timer_HQ::get_seconds() {
#ifdef _WINDOWS
    return double(get_ticks()) / m_ticks_per_second;
#else
    return to_seconds(get_ticks());
#endif
  }

  Time_HQ Timer_HQ::get_time() {
    Mutex::Lock lock(ticks_mutex);
    update();
    return Time_HQ(m_ticks, m_ticks_per_second);
  }

  void Timer_HQ::update() {
#ifdef _WINDOWS
    if(m_available) {
      LARGE_INTEGER lpPerformanceCount;
      QueryPerformanceCounter(&lpPerformanceCount);

      m_ticks = lpPerformanceCount.QuadPart;
    }
    else
      m_ticks = SDL_GetTicks();
#else
    gettimeofday(&m_ticks, 0);
#endif
  }

}

#endif
