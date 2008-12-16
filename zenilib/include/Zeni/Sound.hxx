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

#ifndef ZENI_SOUND_HXX
#define ZENI_SOUND_HXX

// HXXed below
#include <Zeni/Sound_Source.h>

#include <Zeni/Sound.h>

namespace Zeni {

  const ALuint & Sound_Buffer::get_id() const {
    if(m_loader)
      finish_loading();
    
    return m_buffer;
  }

  const float & Sound_Buffer::get_duration() const {
    if(m_loader)
      finish_loading();

    return m_duration;
  }

  void Sound::set_listener_position(const Point3f &
#ifndef DISABLE_AL
    position
#endif
    ) {
#ifndef DISABLE_AL
    ALfloat listener_position[3] = {position.x, position.y, position.z};
    alListenerfv(AL_POSITION, listener_position);

    m_bgm_source->set_position(position);
#endif
  }

  void Sound::set_listener_velocity(const Vector3f &
#ifndef DISABLE_AL
    velocity
#endif
    ) {
#ifndef DISABLE_AL
    ALfloat listener_velocity[3] = {velocity.i, velocity.j, velocity.k};
    alListenerfv(AL_VELOCITY, listener_velocity);

    m_bgm_source->set_velocity(velocity);
#endif
  }

  void Sound::set_listener_forward_and_up(const Vector3f &
#ifndef DISABLE_AL
    forward
#endif
    , const Vector3f &
#ifndef DISABLE_AL
    up
#endif
    ) {
#ifndef DISABLE_AL
    ALfloat listener_forward_and_up[6] = {forward.i, forward.j, forward.k, up.i, up.j, up.k};
    alListenerfv(AL_ORIENTATION, listener_forward_and_up);
#endif
  }

  Point3f Sound::get_listener_position() const {
    return m_bgm_source->get_position();
    ALfloat listener_position[3] = {0.0f, 0.0f, 0.0f};
#ifndef DISABLE_AL
    alGetListenerfv(AL_POSITION, listener_position);
#endif
    return Point3f(listener_position[0], listener_position[1], listener_position[2]);
  }

  Vector3f Sound::get_listener_velocity() const {
    ALfloat listener_velocity[3] = {0.0f, 0.0f, 0.0f};
#ifndef DISABLE_AL
    alGetListenerfv(AL_VELOCITY, listener_velocity);
#endif
    return Vector3f(listener_velocity[0], listener_velocity[1], listener_velocity[2]);
  }

  std::pair<Vector3f, Vector3f> Sound::get_listener_forward_and_up() const {
    ALfloat lfau[6] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
#ifndef DISABLE_AL
    alGetListenerfv(AL_ORIENTATION, lfau);
#endif
    return std::make_pair(Vector3f(lfau[0], lfau[1], lfau[2]), Vector3f(lfau[3], lfau[4], lfau[5]));
  }

  void Sound::set_BGM_pitch(const float &
#ifndef DISABLE_AL
    pitch
#endif
    ) {
#ifndef DISABLE_AL
    m_bgm_source->set_pitch(pitch);
#endif
  }

  void Sound::set_BGM_gain(const float &
#ifndef DISABLE_AL
    gain
#endif
    ) {
#ifndef DISABLE_AL
    if(m_bgm_source)
      m_bgm_source->set_gain(gain);
#endif
  }

  void Sound::set_BGM_looping(const bool &
#ifndef DISABLE_AL
    looping
#endif
    ) {
#ifndef DISABLE_AL
    if(m_bgm_source)
      m_bgm_source->set_looping(looping);
#endif
  }

  void Sound::set_BGM_time(const float &
#ifndef DISABLE_AL
    time
#endif
    ) {
#ifndef DISABLE_AL
    if(m_bgm_source)
      m_bgm_source->set_time(time);
#endif
  }

  float Sound::get_BGM_pitch() {
    return m_bgm_source->get_pitch();
  }

  float Sound::get_BGM_gain() {
    return m_bgm_source->get_gain();
  }

  bool Sound::is_BGM_looping() {
    return m_bgm_source->is_looping();
  }

  float Sound::get_BGM_time() {
    return m_bgm_source->get_time();
  }

  bool Sound::playing_BGM() {
    return m_bgm_source->is_playing();
  }

  bool Sound::paused_BGM() {
    return m_bgm_source->is_paused();
  }

  bool Sound::stopped_BGM() {
    return m_bgm_source->is_stopped();
  }

  void Sound::play_BGM() {
    return m_bgm_source->play();
  }

  void Sound::pause_BGM() {
    return m_bgm_source->pause();
  }
  
  void Sound::stop_BGM() {
    return m_bgm_source->stop();
  }

}

#include <Zeni/Sound_Source.hxx>

#endif
