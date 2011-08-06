/* This file is part of the Zenipex Library (zenilib).
 * Copyright (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * zenilib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * zenilib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with zenilib.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ZENI_SOUND_HXX
#define ZENI_SOUND_HXX

// HXXed below
#include <Zeni/Sound_Source.h>

#include <Zeni/Sound.h>

#include <Zeni/Define.h>

namespace Zeni {

  Sound_Renderer & Sound::get_Renderer() {
    return *m_sound_renderer;
  }

  float Sound::get_listener_gain() const {
    return m_listener_gain;
  }

  bool Sound::is_listener_muted() const {
    return m_listener_muted;
  }

  void Sound::set_BGM_pitch(const float & pitch) {
    get_BGM_Source().set_pitch(pitch);
  }

  void Sound::set_BGM_gain(const float &gain) {
    get_BGM_Source().set_gain(gain);
  }

  void Sound::set_BGM_looping(const bool &looping) {
    get_BGM_Source().set_looping(looping);
  }

  void Sound::set_BGM_time(const float &time) {
    get_BGM_Source().set_time(time);
  }

  float Sound::get_BGM_pitch() {
    return get_BGM_Source().get_pitch();
  }

  float Sound::get_BGM_gain() {
    return get_BGM_Source().get_gain();
  }

  bool Sound::is_BGM_looping() {
    return get_BGM_Source().is_looping();
  }

  float Sound::get_BGM_time() {
    return get_BGM_Source().get_time();
  }

  bool Sound::playing_BGM() {
    return get_BGM_Source().is_playing();
  }

  bool Sound::paused_BGM() {
    return get_BGM_Source().is_paused();
  }

  bool Sound::stopped_BGM() {
    return get_BGM_Source().is_stopped();
  }

  void Sound::play_BGM() {
    get_BGM_Source().play();
  }

  void Sound::pause_BGM() {
    get_BGM_Source().pause();
  }
  
  void Sound::stop_BGM() {
    get_BGM_Source().stop();
  }

}

#include <Zeni/Undefine.h>

#include <Zeni/Sound_Source.hxx>

#endif
