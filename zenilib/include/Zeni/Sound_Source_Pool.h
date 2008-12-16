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

/**
 * \class Zeni::Sounds
 *
 * \ingroup Zenilib
 *
 * \brief A Sound_Buffer Database Singleton
 *
 * The Sounds Singleton stores Sound_Buffers to be played from Sound_Sources.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_SOUND_SOURCE_POOL_H
#define ZENI_SOUND_SOURCE_POOL_H

#include <Zeni/Sound_Source.h>

#include <list>

namespace Zeni {

  class Sound_Source_Pool {
    // Get reference to only instance;
    friend Sound_Source_Pool & get_Sound_Source_Pool(); ///< Get access to the singleton.

    Sound_Source_Pool();
    ~Sound_Source_Pool();

    // Undefined
    Sound_Source_Pool(const Sound_Source_Pool &);
    Sound_Source_Pool & operator=(const Sound_Source_Pool &);

  public:
    enum Replacement_Policy {BESP_NONE, // No Replacment of Playing Sounds
                             BESP_OLDEST}; // Oldest

    Replacement_Policy get_Replacement_Policy() const; ///< Get the BESP Replacement_Policy
    void set_Replacement_Policy(const Replacement_Policy &replacement_policy); ///< Set the BESP Replacement_Policy

    void pause_all(); ///< Pause all BESP Sound_Sources.
    void unpause_all(); ///< Unpause all paused BESP Sound_Sources.
    void purge(); ///< Purge all BESP Sound_Sources

    Sound_Source_HW * take_Sound_Source(); ///< Request a Sound_Source from the BESP system
    void give_Sound_Source(Sound_Source_HW * const &sound_source); ///< Add a Sound_Source to the BESP system; BESP will then play the Sound_Source

  private:
    std::list<Sound_Source_HW *> m_sound_sources;
    Replacement_Policy m_replacement_policy;
  };

  Sound_Source_Pool & get_Sound_Source_Pool(); ///< Get access to the singleton.

}

#endif
