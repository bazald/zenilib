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

#include <Zeni/Sounds.h>

#include <Zeni/Sound.hxx>

#include <fstream>

using std::string;
using std::map;
using std::ifstream;

namespace Zeni {

  Sound_Effect::Sound_Effect()
    : m_mix_chunk(0)
  {
  }

  Sound_Effect::Sound_Effect(const string &filename) {
    m_mix_chunk = Mix_LoadWAV(filename.c_str());
    if(!m_mix_chunk)
      throw Sounds_Init_Failure();
  }

  Sound_Effect::~Sound_Effect() {
    delete m_mix_chunk;
  }

  Sounds::Sounds()
    : m_soundsfile("config/sounds.txt")
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

  void Sounds::play_sound(const string &sound_effect, const int &loop_times) const {
    map<std::string, Sound_Effect>::const_iterator it = m_sounds.find(sound_effect);

    if(it == m_sounds.end())
      throw Sound_Effect_Not_Found();

    Sound::get_reference().play_sound(it->second, loop_times);
  }

  void Sounds::reload(const string &soundsfile) {
    if(soundsfile.length())
      m_soundsfile = soundsfile;

    init();
  }

  void Sounds::init() {
    m_sounds.clear();

    ifstream soundsin(m_soundsfile.c_str());

    if(!soundsin)
      throw Sounds_Init_Failure();

    string name, filename;

    while(soundsin >> name >> filename)
      m_sounds[name] = Sound_Effect(filename);
  }

}
