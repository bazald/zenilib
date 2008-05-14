/* This file is part of the Zenipex Library.
* Copyleft (C) 2007 Mitchell Keith Bloch a.k.a. bazald
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
#include <Zeni/Coordinate.hxx>
#include <Zeni/Resource.hxx>
#include <Zeni/Vector3f.hxx>

#include <iostream>
#include <fstream>

using namespace std;

namespace Zeni {

  Sounds::Sounds()
    : m_soundsfile("config/sounds.txt"),
      m_all_loaded(true)
  {
    // Ensure Sound is initialized
    Sound::get_reference();

    reload();
  }

  Sounds::~Sounds() {}

  Sounds & Sounds::get_reference() {
    static Sounds e_sounds;
    return e_sounds;
  }
  
  unsigned long Sounds::set_sound(const std::string &name, const std::string &filename) {
    unsigned long id = Resource::get_reference().assign();
    m_sound_lookup[name] = id;
    m_sounds[id] = Sound_Buffer(filename);
    m_all_loaded = false;
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

  const Sound_Buffer & Sounds::get_sound(const string &sound) const {
    return get_sound(get_sound_id(sound));
  }

  const Sound_Buffer & Sounds::get_sound(const unsigned long &sound_effect) const {
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
    m_sound_lookup.clear();
    m_sounds.clear();

    ifstream soundsin(m_soundsfile.c_str());

    if(!soundsin)
      throw Sounds_Init_Failure();

    string name, filename;

    while(soundsin >> name >> filename) {
      try {
        set_sound(name, filename);
      }
      catch(Sound_Buffer_Init_Failure &) {
        cerr << "Sounds: Error Loading '" << name << "' from '" << filename << "'\n";
        throw;
      }
    }
  }
  
  float Sounds::percent_loaded() const {
    if(m_all_loaded)
      return 100.0f;
    
    int yes = 0;
    int no = 0;
    
    for(stdext::hash_map<unsigned long, Sound_Buffer>::const_iterator it = m_sounds.begin();
        it != m_sounds.end();
        ++it)
      if(it->second.loaded())
        ++yes;
      else
        ++no;
    
    if(!no) {
      m_all_loaded = true;
      return 100.0f;
    }
    
    return 100.0f * yes / (yes + no);
  }

}
