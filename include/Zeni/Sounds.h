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

#ifndef ZENI_SOUNDS_H
#define ZENI_SOUNDS_H

#include <Zeni/Core.h>
#include <Zeni/Hash_Map.h>
#include <Zeni/Resource.h>
#include <Zeni/Sound.h>

#include <string>
#include <list>

namespace Zeni {

  class Sounds {
    // Get reference to only instance;
    friend Sounds & get_Sounds(); ///< Get access to the singleton.

    Sounds();
    ~Sounds();

    // Undefined
    Sounds(const Sounds &);
    Sounds & operator=(const Sounds &);

  public:
    unsigned long get_sound_id(const std::string &sound_effect) const; ///< Get a Sound_Buffer id by name.

    const Sound_Buffer & operator[](const std::string &sound_effect) const; ///< Get a Sound_Buffer by name
    const Sound_Buffer & operator[](const unsigned long &id) const; ///< Get a Sound_Buffer by id

    unsigned long set_sound(const std::string &name, const std::string &filename); ///< Load a Sound_Buffer.
    void clear_sound(const std::string &name); ///< Clear a sound by name.
    void reload(const std::string &sounds = ""); ///< (Re)Load a Sound_Buffer database

    /// Best Effort Sound Player
    
    enum Replacement_Policy {BESP_NONE, // No Replacment of Playing Sounds
                             BESP_OLDEST}; // Oldest

    Replacement_Policy get_Replacement_Policy() const; ///< Get the BESP Replacement_Policy
    void set_Replacement_Policy(const Replacement_Policy &replacement_policy); ///< Set the BESP Replacement_Policy

    bool play_sound(const std::string &sound_effect); ///< Play a Sound_Buffer
    bool play_sound(const unsigned long &id); ///< Play a Sound_Buffer

    void pause_all(); ///< Pause all BESP Sound_Sources.
    void unpause_all(); ///< Unpause all paused BESP Sound_Sources.
    void purge(); ///< Purge all BESP Sound_Sources

    Sound_Source * take_Sound_Source(); ///< Request a Sound_Source from the BESP system
    void give_Sound_Source(Sound_Source * const &sound_source); ///< Add a Sound_Source to the BESP system; BESP will then play the Sound_Source

  private:
    void init();

    std::string m_soundsfile;
    stdext::hash_map<std::string, unsigned long> m_sound_lookup;
    stdext::hash_map<unsigned long, Sound_Buffer> m_sounds;

    /// Best Effort Sound Player
    std::list<Sound_Source *> m_sound_sources;
    Replacement_Policy m_replacement_policy;
  };

  struct Sound_Effect_Not_Found : public Error {
    Sound_Effect_Not_Found(const std::string &identifier) : Error("Zeni Sound Effect '" + identifier + "' Not Found") {}
  };

  struct Sounds_Init_Failure : public Error {
    Sounds_Init_Failure() : Error("Zeni Sounds Failed to Initialize Correctly") {}
  };

}

#endif
