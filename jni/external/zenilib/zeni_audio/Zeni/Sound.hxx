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
#ifdef DISABLE_AL
    get_BGM_Source().set_pitch(pitch);
#else
    init_BGM_Sound_Stream_AL();
    if(m_bgm)
      m_bgm->set_pitch(pitch);
#endif
  }

  void Sound::set_BGM_gain(const float &gain) {
#ifdef DISABLE_AL
    get_BGM_Source().set_gain(gain);
#else
    init_BGM_Sound_Stream_AL();
    if(m_bgm)
      m_bgm->set_gain(gain);
#endif
  }

  void Sound::set_BGM_looping(const bool &looping) {
#ifdef DISABLE_AL
    get_BGM_Source().set_looping(looping);
#else
    init_BGM_Sound_Stream_AL();
    if(m_bgm)
      m_bgm->set_looping(looping);
#endif
  }

  void Sound::set_BGM_time(const float &time) {
#ifdef DISABLE_AL
    get_BGM_Source().set_time(time);
#else
    init_BGM_Sound_Stream_AL();
    if(m_bgm)
      m_bgm->set_time(time);
#endif
  }

  void Sound::update() {
#ifndef DISABLE_AL
    init_BGM_Sound_Stream_AL();
    if(m_bgm)
      m_bgm->update();
#endif
  }

  float Sound::get_BGM_pitch() {
#ifdef DISABLE_AL
    return get_BGM_Source().get_pitch();
#else
    init_BGM_Sound_Stream_AL();
    return m_bgm ? m_bgm->get_pitch() : ZENI_DEFAULT_PITCH;
#endif
  }

  float Sound::get_BGM_gain() {
#ifdef DISABLE_AL
    return get_BGM_Source().get_gain();
#else
    init_BGM_Sound_Stream_AL();
    return m_bgm ? m_bgm->get_gain() : ZENI_DEFAULT_GAIN;
#endif
  }

  bool Sound::is_BGM_looping() {
#ifdef DISABLE_AL
    return get_BGM_Source().is_looping();
#else
    init_BGM_Sound_Stream_AL();
    return m_bgm ? m_bgm->is_looping() : false;
#endif
  }

  float Sound::get_BGM_time() {
#ifdef DISABLE_AL
    return get_BGM_Source().get_time();
#else
    init_BGM_Sound_Stream_AL();
    return m_bgm ? m_bgm->get_time() : 0.0f;
#endif
  }

  bool Sound::playing_BGM() {
#ifdef DISABLE_AL
    return get_BGM_Source().is_playing();
#else
    init_BGM_Sound_Stream_AL();
    return m_bgm ? m_bgm->is_playing() : false;
#endif
  }

  bool Sound::paused_BGM() {
#ifdef DISABLE_AL
    return get_BGM_Source().is_paused();
#else
    init_BGM_Sound_Stream_AL();
    return m_bgm ? m_bgm->is_paused() : false;
#endif
  }

  bool Sound::stopped_BGM() {
#ifdef DISABLE_AL
    return get_BGM_Source().is_stopped();
#else
    init_BGM_Sound_Stream_AL();
    return m_bgm ? m_bgm->is_stopped() : true;
#endif
  }

  void Sound::play_BGM() {
#ifdef DISABLE_AL
    get_BGM_Source().play();
#else
    init_BGM_Sound_Stream_AL();
    if(m_bgm)
      m_bgm->play();
#endif
  }

  void Sound::pause_BGM() {
#ifdef DISABLE_AL
    get_BGM_Source().pause();
#else
    init_BGM_Sound_Stream_AL();
    if(m_bgm)
      m_bgm->pause();
#endif
  }
  
  void Sound::stop_BGM() {
#ifdef DISABLE_AL
    get_BGM_Source().stop();
#else
    init_BGM_Sound_Stream_AL();
    if(m_bgm)
      m_bgm->stop();
#endif
  }

}

#include <Zeni/Undefine.h>

#include <Zeni/Sound_Source.hxx>

#endif
