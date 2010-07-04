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

#include <Zeni/Sound_AL.hxx>

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

namespace Zeni {

  Sound_AL::Sound_AL()
    : Sound(Sound_Base::ZENI_SOUND_AL),
    m_device(0),
    m_context(0),
    m_bgm(0),
    m_bgm_source(0)
  {
    m_device = alcOpenDevice(0);
    if(!m_device)
      throw Sound_Init_Failure();

    m_context = alcCreateContext(m_device, 0);
    if(!m_context) {
      alcCloseDevice(m_device);
      throw Sound_Init_Failure();
    }

    if(!alcMakeContextCurrent(m_context)) {
      alcDestroyContext(m_context);
      alcCloseDevice(m_device);
      throw Sound_Init_Failure();
    }

    // Check for Vorbis extension functionality; seems to always fail :(
    alIsExtensionPresent("AL_EXT_vorbis");
    //cerr << "Valid Audio Formats: " << alutGetMIMETypes(ALUT_LOADER_BUFFER) << std::endl;

    ALfloat listener_position[] = {0.0f, 0.0f, 0.0f};
    ALfloat listener_velocity[] = {0.0f, 0.0f, 0.0f};
    ALfloat listener_forward_and_up[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    alListenerfv(AL_POSITION, listener_position);
    alListenerfv(AL_VELOCITY, listener_velocity);
    alListenerfv(AL_ORIENTATION, listener_forward_and_up);
  }

  Sound_AL::~Sound_AL() {
    alcDestroyContext(m_context);
    alcCloseDevice(m_device);
  }

}

#else

namespace Zeni {
  void * this_pointer_is_silent_sound = (void *)0xDEADBEEF;
}

#endif
