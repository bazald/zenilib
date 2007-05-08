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
 * \class Zeni::Sound_Effect
 *
 * \ingroup Zenilib
 *
 * \brief Stores Sound Data
 *
 * A Sound_Effect stores sound data to be played back by the Sound system.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Sounds
 *
 * \ingroup Zenilib
 *
 * \brief A Sound_Effect Database Singleton
 *
 * The Sounds Singleton stores Sound_Effects to be played by the Sound system.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_SOUNDS_H
#define ZENI_SOUNDS_H

#include "Core.h"
#include "Hash_Map.h"
#include "Sound.h"

#include <AL/alut.h>
#include <string>

namespace Zeni {

  class Sounds {
    Sounds();
    ~Sounds();

    // Undefined
    Sounds(const Sounds &);
    Sounds & operator=(const Sounds &);

  public:
    // Get reference to only instance;
    static Sounds & get_reference(); ///< Get access to the singleton.

    const Sound_Buffer & get_sound(const std::string &sound_effect) const; ///< Get a Sound_Effect

    void reload(const std::string &sounds = ""); ///< (Re)Load a Sound_Effect database

  private:
    void init();

    std::string m_soundsfile;
    stdext::hash_map<std::string, Sound_Buffer> m_sounds;
  };

  struct Sound_Effect_Not_Found : public Error {
    Sound_Effect_Not_Found() : Error("Zeni Sound Effect Not Found") {}
  };

  struct Sounds_Init_Failure : public Error {
    Sounds_Init_Failure() : Error("Zeni Sounds Failed to Initialize Correctly") {}
  };

}

#endif
