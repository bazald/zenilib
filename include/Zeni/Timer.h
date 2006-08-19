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

/**
 * \class Zeni::Time
 *
 * \ingroup Zenilib
 *
 * \brief A Snapshot of the Timer
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Timer
 *
 * \ingroup Zenilib
 *
 * \brief A Timer Singleton
 *
 * Rather than using multiple Timers, I recommend using just this one and 
 * storing Times wherever necessary.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_TIMER_H
#define ZENI_TIMER_H

namespace Zeni {

  class Time {
  public:
    Time(const int &ticks);
    inline Time & operator=(const int &ticks);

    // Accessors
    // Time passed since last updated
    inline int get_ticks_passed() const; ///< Get the number of clock ticks passed since current Time
    inline float get_seconds_passed() const; ///< Get the number of seconds passed since current Time
    // From a specific time
    inline int get_ticks_since(const Time &time) const; ///< Get the number of clock ticks passed since this Time
    inline float get_seconds_since(const Time &time) const; ///< Get the number of seconds passed since this Time

    // Modifiers
    inline void update(); ///< Update to current Time
  private:
    int m_ticks;
  };

  class Timer {
    Timer();

    // Undefined
    Timer(const Timer &);
    Timer & operator=(const Timer &);

  public:
    // Get reference to only instance;
    static Timer & get_reference(); ///< Get access to the singleton

    // Accessors
    inline int get_ticks() const; ///< Get the number of ticks passed since instantiation
    inline float get_seconds() const; ///< Get the number of seconds passed since instantiation
    inline Time get_time() const; ///< Get the current Time

  private:
    inline void update();

    int m_ticks; // Wraps at around 49 days
  };

}

#ifdef ZENI_INLINES
#include "Timer.hxx"
#endif

#endif
