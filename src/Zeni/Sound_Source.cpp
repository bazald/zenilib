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

#include <Zeni/Sound_Source.hxx>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Mutex.hxx>
#include <Zeni/Sound.hxx>
#include <Zeni/Sound_Buffer.hxx>
#include <Zeni/Sound_Source_Pool.h>
#include <Zeni/Timer.hxx>
#include <Zeni/Vector3f.hxx>

#include <SDL/SDL.h>
#include <iostream>
#include <utility>
#include <vector>

#ifndef DISABLE_AL
#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>
#undef OV_EXCLUDE_STATIC_CALLBACKS
#endif

#include <Zeni/Global.h>

namespace Zeni {

  const Sound_Buffer & get_Hello_World_Buffer() {
    static Sound_Buffer hello_world_buffer;
    return hello_world_buffer;
  }

  Sound_Source_HW::Sound_Source_HW(const ALuint &source)
    : m_source(source),
    m_buffer(0)
  {
    get_Sound();

    init(get_Hello_World_Buffer());
  }

  Sound_Source_HW::Sound_Source_HW(const Sound_Buffer &buffer, const float &pitch, const float &gain,
    const Point3f &position, const Vector3f &velocity, const bool &looping)
    : m_source(AL_NONE)
  {
    init(buffer, pitch, gain, position, velocity, looping);
  }

  Sound_Source_HW::~Sound_Source_HW() {
    stop(); ///< Bounds stall time on quit

#ifndef DISABLE_AL
    if(m_source != AL_NONE && !Quit_Event::has_fired())
      alDeleteSources(1, &m_source);
#endif
  }

  Sound_Source_HW * Sound_Source_HW::Try_Construct() {
    ALuint source = AL_NONE;
#ifndef DISABLE_AL
    alGenSources(1, &source);
#endif
    return source != AL_NONE ? new Sound_Source_HW(source) : 0;
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
    if(m_source == AL_NONE) {
      alGenSources(1, &m_source);

      if(m_source == AL_NONE) {
        std::cerr << "OpenAL error: " << alErrorString(alGetError()) << std::endl;
        throw Sound_Source_HW_Init_Failure();
      }
    }

    alSourcei(m_source, AL_BUFFER, ALint(buffer.get_id()));
    alSourcef(m_source, AL_PITCH, pitch);
    alSourcef(m_source, AL_GAIN, gain);
    alSourcefv(m_source, AL_POSITION, const_cast<ALfloat *>(reinterpret_cast<const ALfloat *>(&position)));
    alSourcefv(m_source, AL_VELOCITY, const_cast<ALfloat *>(reinterpret_cast<const ALfloat *>(&velocity)));
    alSourcei(m_source, AL_LOOPING, looping);
#endif

    set_reference_distance();
    set_max_distance();
  }

  Sound_Source::Sound_Source()
    : m_hw(0),
    m_priority(ZENI_DEFAULT_SOUND_PRIORITY),
    m_buffer(&get_Hello_World_Buffer()),
    m_pitch(ZENI_DEFAULT_PITCH),
    m_gain(ZENI_DEFAULT_GAIN),
    m_looping(false),
    m_reference_distance(ZENI_DEFAULT_REFERENCE_DISTANCE),
    m_max_distance(ZENI_DEFAULT_MAX_SOUND_DISTANCE),
    m_rolloff(ZENI_DEFAULT_ROLLOFF),
    m_play_position(0.0f),
    m_playing(false),
    m_paused(false),
    m_stopped(true),
    m_remove_from_Pool_on_destruction(true)
  {
    get_Sound_Source_Pool().insert_Sound_Source(*this);
  }

  Sound_Source::Sound_Source(const Sound_Buffer &buffer,
                             const float &pitch,
                             const float &gain,
                             const Point3f &position,
                             const Vector3f &velocity,
                             const bool &looping)
    : m_hw(0),
    m_priority(ZENI_DEFAULT_SOUND_PRIORITY),
    m_buffer(&buffer),
    m_pitch(pitch),
    m_gain(gain),
    m_position(position),
    m_velocity(velocity),
    m_looping(looping),
    m_reference_distance(ZENI_DEFAULT_REFERENCE_DISTANCE),
    m_max_distance(ZENI_DEFAULT_MAX_SOUND_DISTANCE),
    m_rolloff(ZENI_DEFAULT_ROLLOFF),
    m_play_position(0.0f),
    m_playing(false),
    m_paused(false),
    m_stopped(true),
    m_remove_from_Pool_on_destruction(true)
  {
    get_Sound_Source_Pool().insert_Sound_Source(*this);
  }

  Sound_Source::~Sound_Source() {
    if(m_remove_from_Pool_on_destruction)
      get_Sound_Source_Pool().remove_Sound_Source(*this);
  }

  float Sound_Source::get_time() const {
    if(m_playing) {
      const float current_play_position = m_play_position + m_play_time.get_seconds_passed();

      if(current_play_position < m_buffer->get_duration())
        return current_play_position;
      else if(m_looping)
        return fmod(current_play_position, m_buffer->get_duration());
      else
        return m_buffer->get_duration();
    }
    else
      return m_play_position;
  }

  float Sound_Source::calculate_gain(const Point3f &listener_position) const {
    float distance = (listener_position - m_position).magnitude();

    // 'OpenAL 1.1 Specification.pdf' - '3.4.4. Linear Distance Clamped Model'
    distance = std::max(distance, m_reference_distance);
    distance = std::min(distance, m_max_distance);
    return 1.0f - m_rolloff * (distance - m_reference_distance) /
                        (m_max_distance - m_reference_distance);
  }

  void Sound_Source::assign(Sound_Source_HW &hw) {
    m_hw = &hw;

    const float time = get_time();

    hw.set_buffer(*m_buffer);
    hw.set_pitch(m_pitch);
    hw.set_gain(m_gain);
    hw.set_position(m_position);
    hw.set_velocity(m_velocity);
    hw.set_looping(m_looping);
    hw.set_time(time);
    hw.set_reference_distance(m_reference_distance);
    hw.set_max_distance(m_max_distance);
    hw.set_rolloff(m_rolloff);

    if(m_playing && time < hw.get_buffer().get_duration())
      hw.play();
  }

  Sound_Source_HW * Sound_Source::unassign() {
    Sound_Source_HW * ptr = m_hw;

    if(m_hw) {
      update_state();

      m_hw = 0;
    }

    return ptr;
  }

  void Sound_Source::update_state() const {
    if(m_hw)
      update_state(m_hw->get_state());
  }

  void Sound_Source::update_state(const Sound_Source_HW::STATE &state) const {
    if(state == Sound_Source_HW::PLAYING) {
      m_play_time.update();

      m_playing = true;
      m_paused = m_stopped = false;
    }
    else if(state == Sound_Source_HW::PAUSED) {
      m_play_position = get_time();

      m_paused = true;
      m_playing = m_stopped = false;
    }
    else if(!m_paused && state == Sound_Source_HW::STOPPED) {
      m_play_position = 0.0f;

      m_stopped = true;
      m_playing = false;
    }
  }

}

#include <Zeni/Global_Undef.h>
