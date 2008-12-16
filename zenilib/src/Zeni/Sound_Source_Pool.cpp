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

#include <Zeni/Sound_Source_Pool.h>

#include <Zeni/Sound_Source.hxx>
#include <Zeni/Coordinate.hxx>
#include <Zeni/Mutex.hxx>
#include <Zeni/Vector3f.hxx>

#include <iostream>

using namespace std;

namespace Zeni {

  Sound_Source_Pool::Sound_Source_Pool()
    : m_replacement_policy(BESP_OLDEST)
  {
    // Ensure Sound is initialized
    get_Sound();
  }

  Sound_Source_Pool::~Sound_Source_Pool() {
    purge();
  }

  Sound_Source_Pool & get_Sound_Source_Pool() {
    static Sound_Source_Pool e_Sound_Source_Pool;
    return e_Sound_Source_Pool;
  }

  Sound_Source_Pool::Replacement_Policy Sound_Source_Pool::get_Replacement_Policy() const {
    return m_replacement_policy;
  }

  void Sound_Source_Pool::set_Replacement_Policy(const Sound_Source_Pool::Replacement_Policy &replacement_policy) {
    m_replacement_policy = replacement_policy;
  }

  void Sound_Source_Pool::pause_all() {
    for(list<Sound_Source_HW *>::iterator it = m_sound_sources.begin();
        it != m_sound_sources.end();
        ++it)
    {
      if((*it)->is_playing())
        (*it)->pause();
    }
  }

  void Sound_Source_Pool::unpause_all() {
    for(list<Sound_Source_HW *>::iterator it = m_sound_sources.begin();
        it != m_sound_sources.end();
        ++it)
    {
      if((*it)->is_paused())
        (*it)->play();
    }
  }

  void Sound_Source_Pool::purge() {
    for(list<Sound_Source_HW *>::iterator it = m_sound_sources.begin();
        it != m_sound_sources.end();
        ++it)
    {
      delete *it;
    }

    m_sound_sources.clear();
  }

  Sound_Source_HW * Sound_Source_Pool::take_Sound_Source() {
    Sound_Source_HW *ss_ptr = 0;

    { // Reuse and Recycle
      list<Sound_Source_HW *> keepers;

      for(list<Sound_Source_HW *>::iterator it = m_sound_sources.begin();
          it != m_sound_sources.end();
          ++it)
      {
        if((*it)->is_playing())
          keepers.push_back(*it);
        else if(ss_ptr)
          delete *it;
        else
          ss_ptr = *it;
      }

      m_sound_sources.swap(keepers);
    }

    try {
      if(ss_ptr)
        return ss_ptr;
      else
        return new Sound_Source_HW();
    }
    catch(Sound_Source_HW_Init_Failure &) {
      if(m_sound_sources.empty())
        throw;

      // Replace

      switch(m_replacement_policy) {
        case BESP_NONE:
          throw;

        case BESP_OLDEST:
          ss_ptr = m_sound_sources.front();
          m_sound_sources.pop_front();
          break;

        default:
          throw;
      }

      ss_ptr->stop();
      return ss_ptr;
    }
  }

  void Sound_Source_Pool::give_Sound_Source(Sound_Source_HW * const &sound_source) {
    if(sound_source) {
      sound_source->play();
      m_sound_sources.push_back(sound_source);
    }
  }

}
