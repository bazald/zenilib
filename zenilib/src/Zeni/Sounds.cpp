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

#include <Zeni/Sounds.h>

#include <Zeni/Sound.hxx>
#include <Zeni/Sound_Source_Pool.h>
#include <Zeni/Coordinate.hxx>
#include <Zeni/Mutex.hxx>
#include <Zeni/Resource.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/XML.hxx>

#include <iostream>
#include <fstream>

using namespace std;

namespace Zeni {

  Sounds::Sounds()
    : m_soundsfile("config/sounds.xml")
  {
    // Ensure Sound is initialized
    get_Sound();

    reload();
  }

  Sounds::~Sounds() {
    get_Sound_Source_Pool().purge();
  }

  Sounds & get_Sounds() {
    static Sounds e_sounds;
    return e_sounds;
  }
  
  unsigned long Sounds::set_sound(const std::string &name, const std::string &filename) {
    unsigned long id = get_Resource().assign();
    m_sound_lookup[name] = id;
    m_sounds[id] = Sound_Buffer(filename);
    return id;
  }

  void Sounds::clear_sound(const std::string &name) {
    stdext::hash_map<string, unsigned long>::iterator it = m_sound_lookup.find(name);

    if(it == m_sound_lookup.end())
      throw Sound_Effect_Not_Found(name);

    m_sounds.erase(it->second);
    m_sound_lookup.erase(it);
  }

  unsigned long Sounds::get_sound_id(const string &sound) const {
    stdext::hash_map<string, unsigned long>::const_iterator it = m_sound_lookup.find(sound);

    if(it == m_sound_lookup.end() || !it->second)
      throw Sound_Effect_Not_Found(sound);

    return it->second;
  }

  const Sound_Buffer & Sounds::operator[](const std::string &sound) const {
    return (*this)[get_sound_id(sound)];
  }

  const Sound_Buffer & Sounds::operator[](const unsigned long &sound_effect) const {
    stdext::hash_map<unsigned long, Sound_Buffer>::const_iterator it = m_sounds.find(sound_effect);

    if(it == m_sounds.end()) {
      char buf[64];
#ifdef _WINDOWS
      sprintf_s
#else
      sprintf
#endif
        (buf, "ID = %u", static_cast<unsigned int>(sound_effect));
      throw Sound_Effect_Not_Found(buf);
    }

    return it->second;
  }

  void Sounds::reload(const string &soundsfile) {
    if(soundsfile.length())
      m_soundsfile = soundsfile;

    init();
  }

  void Sounds::init() {
    get_Sound_Source_Pool().purge();

    m_sound_lookup.clear();
    m_sounds.clear();

    XML_Reader sounds_xml(m_soundsfile.c_str());
    XML_Element sounds = sounds_xml["Sounds"];
    string name;
    bool error = false;

    try {
      for(XML_Element it = sounds.first();; it = it.next()) {
        const string name = it.value();
        error = true;
        const string filepath = it["filepath"].to_string();
        error = false;

        try {
          unsigned long id = get_Resource().assign();
          m_sound_lookup[name] = id;
          m_sounds[id] = Sound_Buffer(filepath);
        }
        catch(Sound_Buffer_Init_Failure &) {
          cerr << "Sounds: Error Loading '" << name << "' from '" << filepath << "'\n";
          throw;
        }
      }
    }
    catch(Bad_XML_Access &)
    {
      if(error) {
        cerr << "Error loading Sound '" << name << "'\n";
        throw;
      }
    }
  }

}
