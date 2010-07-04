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

#ifndef ZENI_SOUND_SOURCE_HXX
#define ZENI_SOUND_SOURCE_HXX

// HXXed below
#include <Zeni/Sound_Buffer.h>
#include <Zeni/Timer.h>

#include <Zeni/Sound_Source.h>

#include <cassert>
#include <cmath>

#ifndef DISABLE_AL
#ifndef AL_SEC_OFFSET
#define AL_SEC_OFFSET 0x1024
#endif
#endif

#include <Zeni/Global.h>

namespace Zeni {

  void Sound_Source_HW::set_buffer(const Sound_Buffer &
#ifndef DISABLE_AL
    buffer
#endif
    ) {
#ifndef DISABLE_AL
    m_buffer = &buffer;
    alSourcei(m_source, AL_BUFFER, ALint(buffer.get_id()));
#endif
  }

  void Sound_Source_HW::set_pitch(const float &
#ifndef DISABLE_AL
    pitch
#endif
    ) {
#ifndef DISABLE_AL
    alSourcef(m_source, AL_PITCH, pitch);
#endif
  }

  void Sound_Source_HW::set_gain(const float &
#ifndef DISABLE_AL
    gain
#endif
    ) {
#ifndef DISABLE_AL
    alSourcef(m_source, AL_GAIN, gain);
#endif
  }

  void Sound_Source_HW::set_position(const Point3f &
#ifndef DISABLE_AL
    position
#endif
    ) {
#ifndef DISABLE_AL
    alSourcefv(m_source, AL_POSITION, const_cast<ALfloat *>(reinterpret_cast<const ALfloat *>(&position)));
#endif
  }

  void Sound_Source_HW::set_velocity(const Vector3f &
#ifndef DISABLE_AL
    velocity
#endif
    ) {
#ifndef DISABLE_AL
    alSourcefv(m_source, AL_VELOCITY, const_cast<ALfloat *>(reinterpret_cast<const ALfloat *>(&velocity)));
#endif
  }

  void Sound_Source_HW::set_looping(const bool &
#ifndef DISABLE_AL
    looping
#endif
    ) {
#ifndef DISABLE_AL
    alSourcei(m_source, AL_LOOPING, looping);
#endif
  }

  void Sound_Source_HW::set_time(const float &
#ifndef DISABLE_AL
    time
#endif
    ) {
#ifndef DISABLE_AL
    alSourcef(m_source, AL_SEC_OFFSET, time);
#endif
  }
  
  void Sound_Source_HW::set_reference_distance(const float &
#ifndef DISABLE_AL
    reference_distance
#endif
    ) {
#ifndef DISABLE_AL
    alSourcef(m_source, AL_REFERENCE_DISTANCE, reference_distance);
#endif
  }

  void Sound_Source_HW::set_max_distance(const float &
#ifndef DISABLE_AL
    max_distance
#endif
    ) {
#ifndef DISABLE_AL
    alSourcef(m_source, AL_MAX_DISTANCE, max_distance);
#endif
  }

  void Sound_Source_HW::set_rolloff(const float &
#ifndef DISABLE_AL
    rolloff
#endif
    ) {
#ifndef DISABLE_AL
    alSourcef(m_source, AL_ROLLOFF_FACTOR, rolloff);
#endif
  }

  const Sound_Buffer & Sound_Source_HW::get_buffer() const {
    assert(m_buffer);
    return *m_buffer;
  }

  float Sound_Source_HW::get_pitch() const {
    float pitch = ZENI_DEFAULT_PITCH;
#ifndef DISABLE_AL
    alGetSourcef(m_source, AL_PITCH, &pitch);
#endif
    return pitch;
  }

  float Sound_Source_HW::get_gain() const {
    float gain = ZENI_DEFAULT_GAIN;
#ifndef DISABLE_AL
    alGetSourcef(m_source, AL_GAIN, &gain);
#endif
    return gain;
  }

  Point3f Sound_Source_HW::get_position() const {
    Point3f position;
#ifndef DISABLE_AL
    alGetSourcefv(m_source, AL_POSITION, reinterpret_cast<ALfloat *>(&position));
#endif
    return position;
  }

  Vector3f Sound_Source_HW::get_velocity() const {
    Vector3f velocity;
#ifndef DISABLE_AL
    alGetSourcefv(m_source, AL_VELOCITY, reinterpret_cast<ALfloat *>(&velocity));
#endif
    return velocity;
  }

  bool Sound_Source_HW::is_looping() const {
    ALint looping = AL_FALSE;
#ifndef DISABLE_AL
    alGetSourcei(m_source, AL_LOOPING, &looping);
#endif
    return looping != AL_FALSE;
  }

  float Sound_Source_HW::get_time() const {
    float time = 0.0f;
#ifndef DISABLE_AL
    alGetSourcef(m_source, AL_SEC_OFFSET, &time);
#endif
    return time;
  }
  
  float Sound_Source_HW::get_reference_distance() const {
    float reference_distance = ZENI_DEFAULT_REFERENCE_DISTANCE;
#ifndef DISABLE_AL
    alGetSourcef(m_source, AL_REFERENCE_DISTANCE, &reference_distance);
#endif
    return reference_distance;
  }

  float Sound_Source_HW::get_max_distance() const {
    float max_distance = ZENI_DEFAULT_MAX_SOUND_DISTANCE;
#ifndef DISABLE_AL
    alGetSourcef(m_source, AL_MAX_DISTANCE, &max_distance);
#endif
    return max_distance;
  }

  float Sound_Source_HW::get_rolloff() const {
    float rolloff = ZENI_DEFAULT_ROLLOFF;
#ifndef DISABLE_AL
    alGetSourcef(m_source, AL_ROLLOFF_FACTOR, &rolloff);
#endif
    return rolloff;
  }

  void Sound_Source_HW::play() {
#ifndef DISABLE_AL
    alSourcePlay(m_source);
#endif
  }

  void Sound_Source_HW::pause() {
#ifndef DISABLE_AL
    alSourcePause(m_source);
#endif
  }

  void Sound_Source_HW::stop() {
#ifndef DISABLE_AL
    alSourceStop(m_source);
#endif
  }

  Sound_Source_HW::STATE Sound_Source_HW::get_state() const {
#ifndef DISABLE_AL
    ALenum state = AL_STOPPED;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING ? PLAYING :
      state == AL_PAUSED ? PAUSED :
      STOPPED;
#else
    return Sound_Source_HW::STATE();
#endif
  }

  bool Sound_Source_HW::is_playing() const {
#ifndef DISABLE_AL
    ALenum state = AL_STOPPED;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
#else
    return false;
#endif
  }

  bool Sound_Source_HW::is_paused() const {
#ifndef DISABLE_AL
    ALenum state = AL_STOPPED;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
#else
    return false;
#endif
  }

  bool Sound_Source_HW::is_stopped() const {
#ifndef DISABLE_AL
    ALenum state = AL_STOPPED;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
#else
    return true;
#endif
  }

  void Sound_Source::set_priority(const int &priority)       {m_priority = priority;}
  void Sound_Source::set_buffer(const Sound_Buffer &buffer)  {m_buffer = &buffer;        if(m_hw) m_hw->set_buffer(buffer);}
  void Sound_Source::set_pitch(const float &pitch)           {m_pitch = pitch;           if(m_hw) m_hw->set_pitch(pitch);}
  void Sound_Source::set_gain(const float &gain)             {m_gain = gain;             if(m_hw) m_hw->set_gain(gain);}
  void Sound_Source::set_position(const Point3f &position)   {m_position = position;     if(m_hw) m_hw->set_position(position);}
  void Sound_Source::set_velocity(const Vector3f &velocity)  {m_velocity = velocity;     if(m_hw) m_hw->set_velocity(velocity);}
  void Sound_Source::set_looping(const bool &looping)        {m_looping = looping;       if(m_hw) m_hw->set_looping(looping);}

  void Sound_Source::set_time(const float &time) {
    m_play_position = time;
    if(m_playing)
      m_play_time.update();

    if(m_hw)
      m_hw->set_time(time);
  }

  void Sound_Source::set_reference_distance(const float &reference_distance) {m_reference_distance = reference_distance; if(m_hw) m_hw->set_reference_distance(reference_distance);}
  void Sound_Source::set_max_distance(const float &max_distance)             {m_max_distance = max_distance;             if(m_hw) m_hw->set_max_distance(max_distance);}
  void Sound_Source::set_rolloff(const float &rolloff)                       {m_rolloff = rolloff;                       if(m_hw) m_hw->set_rolloff(rolloff);}

  int Sound_Source::get_priority() const {return m_priority;}
  Time Sound_Source::get_unstop_time() const {return m_unstop_time;}
  const Sound_Buffer & Sound_Source::get_buffer() const {assert(m_buffer); return *m_buffer;}
  float Sound_Source::get_pitch() const {return m_pitch;}
  float Sound_Source::get_gain() const {return m_gain;}
  Point3f Sound_Source::get_position() const {return m_position;}
  Vector3f Sound_Source::get_velocity() const {return m_velocity;}
  bool Sound_Source::is_looping() const {return m_looping;}
  float Sound_Source::get_reference_distance() const {return m_reference_distance;}
  float Sound_Source::get_max_distance() const {return m_max_distance;}
  float Sound_Source::get_rolloff() const {return m_rolloff;}

  void Sound_Source::play()  {update_state(Sound_Source_HW::PLAYING); if(m_hw) m_hw->play(); m_unstop_time.update();}
  void Sound_Source::pause() {update_state(Sound_Source_HW::PAUSED); if(m_hw) m_hw->pause(); m_unstop_time.update();}
  void Sound_Source::stop()  {update_state(Sound_Source_HW::STOPPED); if(m_hw) m_hw->stop();}

  bool Sound_Source::is_playing() const {update_state(); return m_playing;}
  bool Sound_Source::is_paused() const  {update_state(); return m_paused;}
  bool Sound_Source::is_stopped() const {update_state(); return m_stopped;}

  bool Sound_Source::is_assigned() const {return m_hw != 0;}

}

#include <Zeni/Global_Undef.h>

#include <Zeni/Sound_Buffer.hxx>
#include <Zeni/Timer.hxx>

#endif
