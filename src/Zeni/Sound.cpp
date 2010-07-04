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

#include <Zeni/Sound.hxx>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Mutex.hxx>
#include <Zeni/Vector3f.hxx>

#include <SDL/SDL.h>
#include <iostream>
#include <vector>
#include <iomanip>

#ifndef DISABLE_AL
#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>
#undef OV_EXCLUDE_STATIC_CALLBACKS
#endif

namespace Zeni {

  Sound::Sound(const Sound_Base::SOUND_MODE &vtype_)
    : Sound_Base::IV(vtype_),
    m_bgm(0),
    m_bgm_source(0)
  {
    // Ensure Core is initialized
    get_Core();
  }

  Sound::~Sound() {
    delete m_bgm_source;
    delete m_bgm;
  }

  Sound & get_Sound() {
    if(!Sound::e_sound) {
#ifndef DISABLE_AL
      try {
        Sound::e_sound = new Sound_AL;
      }
      catch(Sound_Init_Failure &) {
#endif
        Sound::e_sound = new Sound_NULL;
#ifndef DISABLE_AL
      }
#endif
    }

    return *Sound::e_sound;
  }

  void Sound::set_BGM(const std::string &filename) {
    assert_m_bgm();

    bool playing = m_bgm_source->is_playing() ? true : false;

    m_bgm_source->stop();
    m_bgm_source->set_buffer(get_Hello_World_Buffer());
    delete m_bgm;
    m_bgm = 0;

    m_bgmusic = filename;
    m_bgm = new Sound_Buffer(m_bgmusic);
    m_bgm_source->set_buffer(*m_bgm);
    m_bgm_source->set_time(0.0f);

    if(playing)
      m_bgm_source->play();
  }

  void Sound::assert_m_bgm() {
    if(!m_bgm)
      m_bgm = new Sound_Buffer();

    if(!m_bgm_source)
      m_bgm_source = new Sound_Source(*m_bgm);
  }

  Sound * Sound::e_sound = 0;

}
