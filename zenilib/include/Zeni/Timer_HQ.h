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
 * \class Zeni::Time_HQ
 *
 * \ingroup zenilib
 *
 * \brief A Snapshot of the Timer_HQ
 *
 * \note Guaranteed sub-millisecond precision (approximately microsecond)
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Timer_HQ
 *
 * \ingroup zenilib
 *
 * \brief A High Quality Timer Singleton
 *
 * Rather than using multiple Timers, I recommend using just this one and 
 * storing Time_HQs wherever necessary.
 *
 * \note Guaranteed sub-millisecond precision (approximately microsecond)
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_TIMER_HQ_H
#define ZENI_TIMER_HQ_H

#include <Zeni/Chronometer.h>
#include <Zeni/Error.h>
#include <Zeni/Singleton.h>

#ifdef _WINDOWS
#include <Windows.h>
#else
#include <sys/time.h>
#endif

namespace Zeni {

  /** High Quality Timer Below **/

#ifdef _WINDOWS
  typedef LONGLONG HQ_Tick_Type;
#else
  typedef long double HQ_Tick_Type;

  timeval subtract(const timeval &lhs, const timeval &rhs);
  long double to_seconds(const timeval &ticks);
  long double to_useconds(const timeval &ticks);
#endif

  class ZENI_DLL Time_HQ {
  public:
    typedef HQ_Tick_Type Tick_Type;
    typedef long double Second_Type;

    Time_HQ(); ///< Initialize to the current time
    Time_HQ(const HQ_Tick_Type &ticks);
    Time_HQ(const HQ_Tick_Type &ticks, const HQ_Tick_Type &ticks_per_second);
    inline Time_HQ & operator=(const HQ_Tick_Type &ticks);

    // Accessors
    // Time passed since last updated
    inline HQ_Tick_Type get_ticks_passed() const; ///< Get the number of clock ticks passed since this Time
    inline long double get_seconds_passed() const; ///< Get the number of seconds passed since this Time
    // From a specific time
    inline HQ_Tick_Type get_ticks_since(const Time_HQ &time) const; ///< Get the number of clock ticks passed between 'time' and this Time
    inline long double get_seconds_since(const Time_HQ &time) const; ///< Get the number of seconds passed between 'time' and this Time

    // Modifiers
    inline void update(); ///< Update to current Time

    // Comparisons
    inline bool operator<(const Time_HQ &rhs) const;

  private:
    HQ_Tick_Type m_ticks;
    HQ_Tick_Type m_ticks_per_second;
  };

  class ZENI_DLL Timer_HQ;

#ifdef _WINDOWS
  ZENI_EXT template class ZENI_DLL Singleton<Timer_HQ>;
  ZENI_EXT template class ZENI_DLL Chronometer<Time_HQ>;
#endif

  class ZENI_DLL Timer_HQ : public Singleton<Timer_HQ> {
    friend class Singleton<Timer_HQ>;

    static Timer_HQ * create();

    Timer_HQ();

    // Undefined
    Timer_HQ(const Timer_HQ &);
    Timer_HQ & operator=(const Timer_HQ &);

  public:
    // Accessors
    inline HQ_Tick_Type get_ticks(); ///< Get the number of ticks passed since instantiation
    inline HQ_Tick_Type get_ticks_per_second(); ///< Get the number of ticks per second
    inline long double get_seconds(); ///< Get the number of seconds passed since instantiation
    inline Time_HQ get_time(); ///< Get the current Time

  private:
    inline void update();

#ifdef _WINDOWS
    HQ_Tick_Type m_ticks;
#else
    timeval m_ticks;
#endif
    HQ_Tick_Type m_ticks_per_second;
  };

  ZENI_DLL Timer_HQ & get_Timer_HQ(); ///< Get access to the singleton.

  struct ZENI_DLL Timer_HQ_Init_Failure : public Error {
    Timer_HQ_Init_Failure() : Error("Zeni Timer_HQ Failed to Initialize Correctly") {}
  };
}

#endif
