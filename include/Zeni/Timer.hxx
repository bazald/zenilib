/* This file is part of the Zenipex Library.
* Copyleft (C) 2006 Mitchell Keith Bloch a.k.a. bazald
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

#include "Timer.h"

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

  int Timer::get_ticks() const {
    return m_ticks;
  }

  float Timer::get_seconds() const {
    return get_ticks() * 0.001f;
  }

  Time Timer::get_time() const {
    return Time(m_ticks);
  }

  void Timer::update() {
    m_ticks = SDL_GetTicks();
  }

}

#endif
