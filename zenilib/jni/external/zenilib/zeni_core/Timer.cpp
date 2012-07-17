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

#include <zeni_core.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Chronometer.hxx>
#include <Zeni/Singleton.hxx>

namespace Zeni {

  Time::Time(const Time::Tick_Type &ticks)
    : m_ticks(ticks)
  {
  }

  Time::Time()
    : m_ticks(get_Timer().get_ticks())
  {
  }

  void Timer::update() {
#ifdef ANDROID
    Timer_HQ &tr = get_Timer_HQ();
    m_ticks = 1000lu * tr.get_ticks() / tr.get_ticks_per_second();
#else
    m_ticks = SDL_GetTicks();
#endif
  }

  template class Singleton<Timer>;
  template class Chronometer<Time>;

  Timer * Timer::create() {
    return new Timer;
  }

  Singleton<Timer>::Uninit Timer::g_uninit;
  Singleton<Timer>::Reinit Timer::g_reinit;

  Timer::Timer() {
    Core::remove_post_reinit(&g_reinit);

    // Ensure Core is initialized
    Core &cr = get_Core();
    
    cr.lend_pre_uninit(&g_uninit);
    cr.lend_post_reinit(&g_reinit);
  }

  Timer::~Timer() {
    Core::remove_pre_uninit(&g_uninit);
  }

  Timer & get_Timer() {
    return Timer::get();
  }

}
