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
* \class Zeni::Chronometer
*
* \ingroup zenilib
*
* \brief A stoppable running timer.
*
* \author bazald
*
* Contact: bazald@zenipex.com
*/

#ifndef ZENI_CHRONOMETER_H
#define ZENI_CHRONOMETER_H

/* \cond */
#include <set>
/* \endcond */

namespace Zeni {
  
  template <class TIME>
  class Chronometer {
  public:
    Chronometer();
    ~Chronometer();

    Chronometer(const Chronometer<TIME> &rhs);
    Chronometer & operator =(const Chronometer<TIME> &rhs);

    // Starting and Stopping
    const bool & is_running() const; ///< Get whether the Chronometer is currently counting or stopped.
    const TIME & start(); ///< Start the Chronometer and get the current TIME.  This does NOT reset the Chronometer.
    const TIME & stop(); ///< Stop the Chronometer and get the current TIME.

    // Time Accessor and Modifiers
    typename TIME::Second_Type seconds() const; ///< Get the number of seconds counted by the Chronometer.
    void set(const typename TIME::Second_Type &time); ///< Set the number of seconds counted by the Chronometer.
    void reset(); ///< Reset the Chronometer.  This does NOT stop the Chronometer.

    // Scaling
    const typename TIME::Second_Type & scaling_factor() const; ///< Get the scaling factor.
    void scale(const typename TIME::Second_Type &scaling_factor = typename TIME::Second_Type(1)); ///< Scale the amount of time that passes by some amount.

  private:
    typename TIME::Second_Type m_seconds_counted;

    TIME m_start_time;
    TIME m_end_time;
    bool m_running;

    typename TIME::Second_Type m_scaling_factor;

  public:
    static bool are_paused(); ///< Check to see if all Chronometer<TIME> objects are paused
    static void pause_all(); ///< Pause all Chronometer<TIME> objects
    static void unpause_all(); ///< Unpause all Chronometer<TIME> objects

  private:
    static std::set<Chronometer<TIME> *> & get_chronometers();
    static std::set<Chronometer<TIME> *> & get_paused();
    static bool g_are_paused;
  };

}

#endif
