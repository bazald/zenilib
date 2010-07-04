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

#ifndef ZENI_CHRONOMETER_HXX
#define ZENI_CHRONOMETER_HXX

// HXXed below
#include <Zeni/Timer.h>

#include <Zeni/Chronometer.h>
#include <cassert>

namespace Zeni {

  template <class TIME>
  Chronometer<TIME>::Chronometer()
    : m_seconds_counted(0.0f),
    m_running(false),
    m_scaling_factor(typename TIME::Second_Type(1))
  {
    get_chronometers().insert(this);
  }

  template <class TIME>
  Chronometer<TIME>::~Chronometer() {
    get_chronometers().erase(this);
    get_paused().erase(this);
  }

  template <class TIME>
  Chronometer<TIME>::Chronometer(const Chronometer<TIME> &rhs)
    : m_seconds_counted(rhs.m_seconds_counted),
    m_start_time(rhs.m_start_time),
    m_end_time(rhs.m_end_time),
    m_running(rhs.m_running),
    m_scaling_factor(rhs.m_scaling_factor)
  {
    get_chronometers().insert(this);

    if(g_are_paused && m_running)
      get_paused().insert(this);
  }

  template <class TIME>
  Chronometer<TIME> & Chronometer<TIME>::operator =(const Chronometer<TIME> &rhs) {
    if(g_are_paused) {
      if(m_running && !rhs.m_running)
        get_paused().erase(this);
      else if(rhs.m_running && !m_running)
        get_paused().insert(this);
    }

    m_seconds_counted = rhs.m_seconds_counted;
    m_start_time = rhs.m_start_time;
    m_end_time = rhs.m_end_time;
    m_running = rhs.m_running;
    m_scaling_factor = rhs.m_scaling_factor;
    return *this;
  }

  template <class TIME>
  const bool & Chronometer<TIME>::running() const {
    return m_running;
  }

  template <class TIME>
  const TIME & Chronometer<TIME>::start() {
    if(!m_running) {
      if(g_are_paused) {
        get_paused().insert(this);
      }
      else {
        m_running = true;

        m_start_time.update();
      }
    }

    return m_start_time;
  }

  template <class TIME>
  const TIME & Chronometer<TIME>::stop() {
    if(m_running) {
      if(g_are_paused) {
        get_paused().erase(this);
      }
      else {
        m_end_time.update();

        m_running = false;
        m_seconds_counted += m_end_time.get_seconds_since(m_start_time) * m_scaling_factor;
      }
    }

    return m_end_time;
  }

  template <class TIME>
  typename TIME::Second_Type Chronometer<TIME>::seconds() const {
    return m_seconds_counted + (m_running ?
                                m_start_time.get_seconds_passed() * m_scaling_factor :
                                0.0f);
  }

  template <class TIME>
  void Chronometer<TIME>::set(const typename TIME::Second_Type &time) {
    if(m_running)
      m_start_time.update();

    m_seconds_counted = time;
  }

  template <class TIME>
  void Chronometer<TIME>::reset() {
    m_start_time.update();
    m_seconds_counted = 0.0f;
  }

  template <class TIME>
  const typename TIME::Second_Type & Chronometer<TIME>::scaling_factor() const {
    return m_scaling_factor;
  }

  template <class TIME>
  void Chronometer<TIME>::scale(const typename TIME::Second_Type &scaling_factor) {
    if(m_running) {
      m_end_time.update();
      m_seconds_counted += m_end_time.get_seconds_since(m_start_time) * m_scaling_factor;
      m_start_time = m_end_time;
    }

    m_scaling_factor = scaling_factor;
  }

  template <class TIME>
  bool Chronometer<TIME>::are_paused() {
    return g_are_paused;
  }

  template <class TIME>
  void Chronometer<TIME>::pause_all() {
    std::set<Chronometer<TIME> *> &chronometers = get_chronometers();
    std::set<Chronometer<TIME> *> &paused = get_paused();

    for(typename std::set<Chronometer<TIME> *>::iterator it = chronometers.begin();
        it != chronometers.end();
        ++it)
    {
      if((*it)->running()) {
        (*it)->stop();
        paused.insert(*it);
      }
    }

    g_are_paused = true;
  }

  template <class TIME>
  void Chronometer<TIME>::unpause_all() {
    g_are_paused = false;

    std::set<Chronometer<TIME> *> &paused = get_paused();

    for(typename std::set<Chronometer<TIME> *>::iterator it = paused.begin();
        it != paused.end();
        ++it)
    {
      (*it)->start();
    }

    paused.clear();
  }

  template <class TIME>
  std::set<Chronometer<TIME> *> & Chronometer<TIME>::get_chronometers() {
    static std::set<Chronometer<TIME> *> * chronometers = new std::set<Chronometer<TIME> *>;
    return *chronometers;
  }

  template <class TIME>
  std::set<Chronometer<TIME> *> & Chronometer<TIME>::get_paused() {
    static std::set<Chronometer<TIME> *> * paused = new std::set<Chronometer<TIME> *>;
    return *paused;
  }

  template <class TIME>
  bool Chronometer<TIME>::g_are_paused = false;

}

#include <Zeni/Chronometer.hxx>

#endif
