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

#include <Zeni/Sound_Source.hxx>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Mutex.hxx>
#include <Zeni/Sound.hxx>
#include <Zeni/Sound_Source_Pool.h>
#include <Zeni/Vector3f.hxx>

#include <SDL/SDL.h>
#include <iostream>
#include <vector>

#ifndef DISABLE_AL
#include <vorbis/vorbisfile.h>
#endif

using std::string;

namespace Zeni {

  const Sound_Buffer & get_Hello_World_Buffer() {
    static Sound_Buffer hello_world_buffer;
    return hello_world_buffer;
  }

  Sound_Source_HW::Sound_Source_HW()
    : m_source(AL_NONE),
    m_buffer(0)
  {
    init(get_Hello_World_Buffer());
  }

  Sound_Source_HW::Sound_Source_HW(const Sound_Buffer &buffer, const float &pitch, const float &gain,
    const Point3f &position, const Vector3f &velocity, const bool &looping)
    : m_source(AL_NONE)
  {
    init(buffer, pitch, gain, position, velocity, looping);
  }

  Sound_Source_HW::~Sound_Source_HW() {
#ifndef DISABLE_AL
    if(m_source != AL_NONE && !Quit_Event::has_fired())
      alDeleteSources(1, &m_source);
#endif
  }

  void Sound_Source_HW::init(const Sound_Buffer &buffer, const float &
#ifndef DISABLE_AL
    pitch
#endif
    , const float &
#ifndef DISABLE_AL
    gain
#endif
    , const Point3f &
#ifndef DISABLE_AL
    position
#endif
    , const Vector3f &
#ifndef DISABLE_AL
    velocity
#endif
    , const bool &
#ifndef DISABLE_AL
    looping
#endif
    ) {
    m_buffer = &buffer;
#ifndef DISABLE_AL
    alGenSources(1, &m_source);

    if(m_source == AL_NONE) {
      std::cerr << "ALUT error" << ": " << alutGetErrorString(alutGetError()) << std::endl;
      throw Sound_Source_HW_Init_Failure();
    }

    alSourcei(m_source, AL_BUFFER, buffer.get_id());
    alSourcef(m_source, AL_PITCH, pitch);
    alSourcef(m_source, AL_GAIN, gain);
    alSourcefv(m_source, AL_POSITION, reinterpret_cast<const float *>(&position));
    alSourcefv(m_source, AL_VELOCITY, reinterpret_cast<const float *>(&velocity));
    alSourcei(m_source, AL_LOOPING, looping);
#endif

    set_near_clamp();
    set_far_clamp();
  }

  Sound_Source::Sound_Source()
    : m_buffer(&get_Hello_World_Buffer()),
    m_pitch(1.0f),
    m_gain(1.0f),
    m_looping(false),
    m_time(0.0f),
    m_near_clamp(10.0f),
    m_far_clamp(1000.0f),
    m_rolloff(1.0f)
  {
    init();
  }

  Sound_Source::Sound_Source(const Sound_Buffer &buffer,
                             const float &pitch,
                             const float &gain,
                             const Point3f &position,
                             const Vector3f &velocity,
                             const bool &looping)
    : m_buffer(&buffer),
    m_pitch(pitch),
    m_gain(gain),
    m_position(position),
    m_velocity(velocity),
    m_looping(looping),
    m_time(0.0f),
    m_near_clamp(10.0f),
    m_far_clamp(1000.0f),
    m_rolloff(1.0f)
  {
    init();
  }

  Sound_Source::~Sound_Source() {
    get_Sound_Source_Pool().give_Sound_Source(m_hw);
  }

  void Sound_Source::init() {
    try {
      m_hw = get_Sound_Source_Pool().take_Sound_Source();
    }
    catch(...)
    {
      m_hw = 0;
      return;
    }

    m_hw->set_buffer(*m_buffer);
    m_hw->set_pitch(m_pitch);
    m_hw->set_gain(m_gain);
    m_hw->set_position(m_position);
    m_hw->set_velocity(m_velocity);
    m_hw->set_looping(m_looping);
    m_hw->set_time(m_time);
    m_hw->set_near_clamp(m_near_clamp);
    m_hw->set_far_clamp(m_far_clamp);
    m_hw->set_rolloff(m_rolloff);
  }

}
