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

#ifndef ZENI_SOUND_HXX
#define ZENI_SOUND_HXX

#include "Sound.h"

namespace Zeni {

  const ALuint & Sound_Buffer::get_id() const {
    return m_buffer;
  }

  void Sound_Source::set_buffer(const Sound_Buffer &buffer) {
    alSourcei(m_source, AL_BUFFER, buffer.get_id());
  }

  void Sound_Source::set_buffer(const ALuint &buffer) {
    alSourcei(m_source, AL_BUFFER, buffer);
  }

  void Sound_Source::set_pitch(const float &pitch) {
    alSourcef(m_source, AL_PITCH, pitch);
  }

  void Sound_Source::set_gain(const float &gain) {
    alSourcef(m_source, AL_GAIN, gain);
  }

  void Sound_Source::set_position(const Point3f &position) {
    alSourcefv(m_source, AL_POSITION, reinterpret_cast<const float *>(&position));
  }

  void Sound_Source::set_velocity(const Point3f &velocity) {
    alSourcefv(m_source, AL_VELOCITY, reinterpret_cast<const float *>(&velocity));
  }

  void Sound_Source::set_looping(const bool &looping) {
    alSourcei(m_source, AL_LOOPING, looping);
  }

  void Sound_Source::set_time(const float &time) {
    alSourcef(m_source, AL_SEC_OFFSET, time);
  }

  ALuint Sound_Source::get_buffer() const {
    ALint buffer;
    alGetSourcei(m_source, AL_BUFFER, &buffer);
    return buffer;
  }

  float Sound_Source::get_pitch() const {
    float pitch;
    alGetSourcef(m_source, AL_PITCH, &pitch);
    return pitch;
  }

  float Sound_Source::get_gain() const {
    float gain;
    alGetSourcef(m_source, AL_GAIN, &gain);
    return gain;
  }

  Point3f Sound_Source::get_position() const {
    Point3f position;
    alGetSourcefv(m_source, AL_POSITION, reinterpret_cast<float *>(&position));
    return position;
  }

  Point3f Sound_Source::get_velocity() const {
    Point3f velocity;
    alGetSourcefv(m_source, AL_VELOCITY, reinterpret_cast<float *>(&velocity));
    return velocity;
  }

  bool Sound_Source::is_looping() const {
    ALint looping;
    alGetSourcei(m_source, AL_LOOPING, &looping);
    return looping != AL_FALSE;
  }

  float Sound_Source::get_time() const {
    float time;
    alGetSourcef(m_source, AL_SEC_OFFSET, &time);
    return time;
  }

  void Sound_Source::play() {
    alSourcePlay(m_source);
  }

  void Sound_Source::pause() {
    alSourcePause(m_source);
  }

  void Sound_Source::stop() {
    alSourceStop(m_source);
  }

  bool Sound_Source::is_playing() {
    ALenum state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
  }

  bool Sound_Source::is_paused() {
    ALenum state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
  }

  bool Sound_Source::is_stopped() {
    ALenum state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
  }

  bool Sound::playing_BGM() {
    assert_m_bgm();
    return m_bgm_source->is_playing();
  }

  bool Sound::paused_BGM() {
    assert_m_bgm();
    return m_bgm_source->is_paused();
  }

  bool Sound::stopped_BGM() {
    assert_m_bgm();
    return m_bgm_source->is_stopped();
  }

  void Sound::play_BGM() {
    assert_m_bgm();
    return m_bgm_source->play();
  }

  void Sound::pause_BGM() {
    assert_m_bgm();
    return m_bgm_source->pause();
  }
  
  void Sound::stop_BGM() {
    assert_m_bgm();
    return m_bgm_source->stop();
  }

  void Sound::loop_BGM(const bool &looping) {
    assert_m_bgm();
    return m_bgm_source->set_looping(looping);
  }

}

#endif
