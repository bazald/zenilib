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

/**
 * \class Zeni::Time
 *
 * \ingroup zenilib
 *
 * \brief A Snapshot of the Timer
 *
 * \note Guaranteed millisecond precision
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Timer
 *
 * \ingroup zenilib
 *
 * \brief A Timer Singleton
 *
 * Rather than using multiple Timers, I recommend using just this one and 
 * storing Times wherever necessary.
 *
 * \note Guaranteed millisecond precision
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_TIMER_H
#define ZENI_TIMER_H

#include <Zeni/Chronometer.h>
#include <Zeni/Singleton.h>

#ifdef _WINDOWS
#include <Windows.h>
#else
#include <sys/time.h>
#endif

namespace Zeni {

  class ZENI_CORE_DLL Time {
  public:
    typedef size_t Tick_Type;
    typedef float Second_Type;

    Time(); ///< Initialize to the current time
    Time(const Tick_Type &ticks);
    inline Time & operator=(const Tick_Type &ticks);

    // Accessors
    // Time passed since last updated
    inline Tick_Type get_ticks_passed() const; ///< Get the number of clock ticks passed since this Time
    inline float get_seconds_passed() const; ///< Get the number of seconds passed since this Time
    // From a specific time
    inline Tick_Type get_ticks_since(const Time &time) const; ///< Get the number of clock ticks passed between 'time' and this Time
    inline float get_seconds_since(const Time &time) const; ///< Get the number of seconds passed between 'time' and this Time

    // Modifiers
    inline void update(); ///< Update to current Time

    // Comparisons
    inline bool operator<(const Time &rhs) const;

  private:
    Tick_Type m_ticks;
  };

  class ZENI_CORE_DLL Timer;

#ifdef _WINDOWS
  ZENI_CORE_EXT template class ZENI_CORE_DLL Singleton<Timer>;
  ZENI_CORE_EXT template class ZENI_CORE_DLL Chronometer<Time>;
#endif

  class ZENI_CORE_DLL Timer : public Singleton<Timer> {
    friend class Singleton<Timer>;

    static Timer * create();
    
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    static Uninit g_uninit;
    static Reinit g_reinit;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
    
    Timer();
    ~Timer();

    // Undefined
    Timer(const Timer &);
    Timer & operator=(const Timer &);

  public:
    // Accessors
    inline Time::Tick_Type get_ticks(); ///< Get the number of ticks passed since instantiation
    inline Time::Tick_Type get_ticks_per_second(); ///< Get the number of ticks per second
    inline float get_seconds(); ///< Get the number of seconds passed since instantiation
    inline Time get_time(); ///< Get the current Time

  private:
    void update();

    Time::Tick_Type m_ticks; // Wraps at around 49 days
  };

  ZENI_CORE_DLL Timer & get_Timer(); ///< Get access to the singleton.

}

#endif
