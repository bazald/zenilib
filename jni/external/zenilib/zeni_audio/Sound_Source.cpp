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

#include <zeni_audio.h>

#include <iostream>
#include <utility>
#include <vector>
#include <cmath>

#ifndef DISABLE_AL
#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>
#undef OV_EXCLUDE_STATIC_CALLBACKS
#endif

#include <Zeni/Define.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace Zeni {

  Sound_Source_HW::Sound_Source_HW(const ALuint &source)
    : 
#ifndef DISABLE_AL
    m_source(source),
#endif
#ifdef ENABLE_SLES
    fdPlayerObject(NULL),
    fdPlayerPlay(NULL),
    fdPlayerSeek(NULL),
    fdPlayerMuteSolo(NULL),
    fdPlayerVolume(NULL),
#endif
    m_buffer(0)
  {
    Sound &sr = get_Sound();

#ifdef ENABLE_SLES
    if(Sound_Renderer_SLES * const sles = dynamic_cast<Sound_Renderer_SLES *>(&sr.get_Renderer())) {
      // configure audio sink
      loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, sles->get_outputMixObject()};
      audioSnk = {&loc_outmix, NULL};
    }
#endif

    init(sr.get_Hello_World_Buffer());
  }

  Sound_Source_HW::Sound_Source_HW(const Sound_Buffer &buffer, const float &pitch, const float &gain,
    const Point3f &position, const Vector3f &velocity, const bool &looping)
    : 
#ifndef DISABLE_AL
    m_source(AL_NONE),
#endif
#ifdef ENABLE_SLES
    fdPlayerObject(NULL),
    fdPlayerPlay(NULL),
    fdPlayerSeek(NULL),
    fdPlayerMuteSolo(NULL),
    fdPlayerVolume(NULL),
#endif
    m_buffer(0)
  {
#ifdef ENABLE_SLES
    Sound &sr = get_Sound();

    if(Sound_Renderer_SLES * const sles = dynamic_cast<Sound_Renderer_SLES *>(&sr.get_Renderer())) {
      // configure audio sink
      loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, sles->get_outputMixObject()};
      audioSnk = {&loc_outmix, NULL};
    }
#endif

    init(buffer, pitch, gain, position, velocity, looping);
  }

  Sound_Source_HW::~Sound_Source_HW() {
    uninit();
  }

  Sound_Source_HW * Sound_Source_HW::Try_Construct() {
    ALuint source = AL_NONE;
#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer()) && Sound_Renderer_AL::alGenSources())
      Sound_Renderer_AL::alGenSources()(1, &source);
#endif
#ifdef ENABLE_SLES
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer()))
      return new Sound_Source_HW(AL_NONE);
#endif
    return source != AL_NONE ? new Sound_Source_HW(source) : 0;
  }

  void Sound_Source_HW::set_buffer(const Sound_Buffer &
#if !defined(DISABLE_AL) || defined(ENABLE_SLES)
    buffer
#endif
    ) {
#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer())) {
      m_buffer = &buffer;
      Sound_Renderer_AL::alSourcei()(m_source, AL_BUFFER, ALint(buffer.get_id()));
    }
#endif
#ifdef ENABLE_SLES
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer())) {
      const float gain = get_gain();
      const bool looping = is_looping();
      uninit();
      init(buffer, float(), gain, Point3f(), Vector3f(), looping);
    }
#endif
  }

  void Sound_Source_HW::set_pitch(const float &
#ifndef DISABLE_AL
    pitch
#endif
    ) {
#ifndef DISABLE_AL
    Sound_Renderer_AL::alSourcef()(m_source, AL_PITCH, pitch);
#endif
  }

  void Sound_Source_HW::set_gain(const float &
#if !defined(DISABLE_AL) || defined(ENABLE_SLES)
    gain
#endif
    ) {
#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer())) {
      Sound_Renderer_AL::alSourcef()(m_source, AL_GAIN, gain);
    }
#endif
#ifdef ENABLE_SLES
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer())) {
      (*fdPlayerVolume)->SetVolumeLevel(fdPlayerVolume, SLmillibel(2000.0f * log10(std::max(1.0f, gain))));
    }
#endif
  }

  void Sound_Source_HW::set_position(const Point3f &
#ifndef DISABLE_AL
    position
#endif
    ) {
#ifndef DISABLE_AL
    Sound_Renderer_AL::alSourcefv()(m_source, AL_POSITION, const_cast<ALfloat *>(reinterpret_cast<const ALfloat *>(&position)));
#endif
  }

  void Sound_Source_HW::set_velocity(const Vector3f &
#ifndef DISABLE_AL
    velocity
#endif
    ) {
#ifndef DISABLE_AL
    Sound_Renderer_AL::alSourcefv()(m_source, AL_VELOCITY, const_cast<ALfloat *>(reinterpret_cast<const ALfloat *>(&velocity)));
#endif
  }

  void Sound_Source_HW::set_looping(const bool &
#if !defined(DISABLE_AL) || defined(ENABLE_SLES)
    looping
#endif
    ) {
#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer())) {
      Sound_Renderer_AL::alSourcei()(m_source, AL_LOOPING, looping);
    }
#endif
#ifdef ENABLE_SLES
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer())) {
      (*fdPlayerSeek)->SetLoop(fdPlayerSeek, looping ? SL_BOOLEAN_TRUE : SL_BOOLEAN_FALSE, 0, SL_TIME_UNKNOWN);
    }
#endif
  }

  void Sound_Source_HW::set_time(const float &
#if !defined(DISABLE_AL) || defined(ENABLE_SLES)
    time
#endif
    ) {
#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer())) {
      Sound_Renderer_AL::alSourcef()(m_source, AL_SEC_OFFSET, time);
    }
#endif
#ifdef ENABLE_SLES
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer())) {
      (*fdPlayerSeek)->SetPosition(fdPlayerSeek, SLmillisecond(1000.0f * time), SL_SEEKMODE_FAST);
    }
#endif
  }
  
  void Sound_Source_HW::set_reference_distance(const float &
#ifndef DISABLE_AL
    reference_distance
#endif
    ) {
#ifndef DISABLE_AL
    Sound_Renderer_AL::alSourcef()(m_source, AL_REFERENCE_DISTANCE, reference_distance);
#endif
  }

  void Sound_Source_HW::set_max_distance(const float &
#ifndef DISABLE_AL
    max_distance
#endif
    ) {
#ifndef DISABLE_AL
    Sound_Renderer_AL::alSourcef()(m_source, AL_MAX_DISTANCE, max_distance);
#endif
  }

  void Sound_Source_HW::set_rolloff(const float &
#ifndef DISABLE_AL
    rolloff
#endif
    ) {
#ifndef DISABLE_AL
    Sound_Renderer_AL::alSourcef()(m_source, AL_ROLLOFF_FACTOR, rolloff);
#endif
  }

  const Sound_Buffer & Sound_Source_HW::get_buffer() const {
    assert(m_buffer);
    return *m_buffer;
  }

  const float Sound_Source_HW::get_duration() const {
#ifdef ENABLE_SLES
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer())) {
      SLmillisecond duration;
      (*fdPlayerPlay)->GetDuration(fdPlayerPlay, &duration);
      if(duration != SL_TIME_UNKNOWN)
        return duration * 0.001f;
    }
#endif
    return m_buffer->get_duration();
  }

  float Sound_Source_HW::get_pitch() const {
    float pitch = ZENI_DEFAULT_PITCH;
#ifndef DISABLE_AL
    Sound_Renderer_AL::alGetSourcef()(m_source, AL_PITCH, &pitch);
#endif
    return pitch;
  }

  float Sound_Source_HW::get_gain() const {
    float gain = ZENI_DEFAULT_GAIN;
#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer())) {
      Sound_Renderer_AL::alGetSourcef()(m_source, AL_GAIN, &gain);
    }
#endif
#ifdef ENABLE_SLES
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer())) {
      SLmillibel millibels;
      if((*fdPlayerVolume)->GetVolumeLevel(fdPlayerVolume, &millibels) == SL_RESULT_SUCCESS)
        gain = pow(10.0f, millibels / 2000.0f);
    }
#endif
    return gain;
  }

  Point3f Sound_Source_HW::get_position() const {
    Point3f position;
#ifndef DISABLE_AL
    Sound_Renderer_AL::alGetSourcefv()(m_source, AL_POSITION, reinterpret_cast<ALfloat *>(&position));
#endif
    return position;
  }

  Vector3f Sound_Source_HW::get_velocity() const {
    Vector3f velocity;
#ifndef DISABLE_AL
    Sound_Renderer_AL::alGetSourcefv()(m_source, AL_VELOCITY, reinterpret_cast<ALfloat *>(&velocity));
#endif
    return velocity;
  }

  bool Sound_Source_HW::is_looping() const {
    ALint looping = AL_FALSE;
#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer())) {
      Sound_Renderer_AL::alGetSourcei()(m_source, AL_LOOPING, &looping);
    }
#endif
#ifdef ENABLE_SLES
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer())) {
      SLboolean slooping;
      SLmillisecond start, end;
      if((*fdPlayerSeek)->GetLoop(fdPlayerSeek, &slooping, &start, &end) == SL_RESULT_SUCCESS)
        return slooping != SL_BOOLEAN_FALSE;
    }
#endif
    return looping != AL_FALSE;
  }

  float Sound_Source_HW::get_time() const {
    float time = 0.0f;
#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer())) {
      Sound_Renderer_AL::alGetSourcef()(m_source, AL_SEC_OFFSET, &time);
    }
#endif
#ifdef ENABLE_SLES
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer())) {
      SLmillisecond milliseconds;
      if((*fdPlayerPlay)->GetPosition(fdPlayerPlay, &milliseconds) == SL_RESULT_SUCCESS)
        time = milliseconds / 1000.0f;
    }
#endif
    return time;
  }
  
  float Sound_Source_HW::get_reference_distance() const {
    float reference_distance = ZENI_DEFAULT_REFERENCE_DISTANCE;
#ifndef DISABLE_AL
    Sound_Renderer_AL::alGetSourcef()(m_source, AL_REFERENCE_DISTANCE, &reference_distance);
#endif
    return reference_distance;
  }

  float Sound_Source_HW::get_max_distance() const {
    float max_distance = ZENI_DEFAULT_MAX_SOUND_DISTANCE;
#ifndef DISABLE_AL
    Sound_Renderer_AL::alGetSourcef()(m_source, AL_MAX_DISTANCE, &max_distance);
#endif
    return max_distance;
  }

  float Sound_Source_HW::get_rolloff() const {
    float rolloff = ZENI_DEFAULT_ROLLOFF;
#ifndef DISABLE_AL
    Sound_Renderer_AL::alGetSourcef()(m_source, AL_ROLLOFF_FACTOR, &rolloff);
#endif
    return rolloff;
  }

  void Sound_Source_HW::play() {
#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer())) {
      Sound_Renderer_AL::alSourcePlay()(m_source);
    }
#endif
#ifdef ENABLE_SLES
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer())) {
      ZENI_LOGD("SLES play executing.");
      (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_PLAYING);
    }
#endif
  }

  void Sound_Source_HW::pause() {
#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer())) {
      Sound_Renderer_AL::alSourcePause()(m_source);
    }
#endif
#ifdef ENABLE_SLES
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer())) {
      (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_PAUSED);
    }
#endif
  }

  void Sound_Source_HW::stop() {
#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer())) {
      Sound_Renderer_AL::alSourceStop()(m_source);
    }
#endif
#ifdef ENABLE_SLES
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer())) {
      (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_STOPPED);
    }
#endif
  }

  Sound_Source_HW::STATE Sound_Source_HW::get_state() const {
#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer())) {
      ALenum state = AL_STOPPED;
      Sound_Renderer_AL::alGetSourcei()(m_source, AL_SOURCE_STATE, &state);
      return state == AL_PLAYING ? PLAYING :
        state == AL_PAUSED ? PAUSED :
        STOPPED;
    }
#endif
#ifdef ENABLE_SLES
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer())) {
      SLuint32 state;
      if((*fdPlayerPlay)->GetPlayState(fdPlayerPlay, &state) != SL_RESULT_SUCCESS)
        return STOPPED;
      return state == SL_PLAYSTATE_PLAYING ? PLAYING :
        state == SL_PLAYSTATE_PAUSED ? PAUSED :
        STOPPED;
    }
#endif
    return Sound_Source_HW::STATE();
  }

  bool Sound_Source_HW::is_playing() const {
    return get_state() == PLAYING;
  }

  bool Sound_Source_HW::is_paused() const {
    return get_state() == PAUSED;
  }

  bool Sound_Source_HW::is_stopped() const {
    return get_state() == STOPPED;
  }

  void Sound_Source_HW::init(const Sound_Buffer &buffer, const float &
#ifndef DISABLE_AL
    pitch
#endif
    , const float &
#if !defined(DISABLE_AL) || defined(ENABLE_SLES)
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
#if !defined(DISABLE_AL) || defined(ENABLE_SLES)
    looping
#endif
    )
  {
    m_buffer = &buffer;

    Sound &sr = get_Sound();

#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&sr.get_Renderer())) {
      if(!Sound_Renderer_AL::alGenSources())
        throw Sound_Source_HW_Init_Failure();

      if(m_source == AL_NONE) {
        Sound_Renderer_AL::alGenSources()(1, &m_source);

        if(m_source == AL_NONE) {
          std::cerr << "OpenAL error: " << Sound_Renderer_AL::errorString() << std::endl;
          throw Sound_Source_HW_Init_Failure();
        }
      }

      Sound_Renderer_AL::alSourcei()(m_source, AL_BUFFER, ALint(buffer.get_id()));
      Sound_Renderer_AL::alSourcef()(m_source, AL_PITCH, pitch);
      Sound_Renderer_AL::alSourcef()(m_source, AL_GAIN, gain);
      Sound_Renderer_AL::alSourcefv()(m_source, AL_POSITION, const_cast<ALfloat *>(reinterpret_cast<const ALfloat *>(&position)));
      Sound_Renderer_AL::alSourcefv()(m_source, AL_VELOCITY, const_cast<ALfloat *>(reinterpret_cast<const ALfloat *>(&velocity)));
      Sound_Renderer_AL::alSourcei()(m_source, AL_LOOPING, looping);
    }
#endif
#ifdef ENABLE_SLES
    if(Sound_Renderer_SLES * const sles = dynamic_cast<Sound_Renderer_SLES *>(&sr.get_Renderer())) {
      const SLInterfaceID ids[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
      const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

      // create audio player
      if((*sles->get_engineEngine())->CreateAudioPlayer(sles->get_engineEngine(), &fdPlayerObject, &const_cast<SLDataSource &>(buffer.get_audioSrc()), &audioSnk, 3, ids, req) != SL_RESULT_SUCCESS ||
         (*fdPlayerObject)->Realize(fdPlayerObject, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS ||
         (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_PLAY, &fdPlayerPlay) != SL_RESULT_SUCCESS ||
         (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_SEEK, &fdPlayerSeek) != SL_RESULT_SUCCESS ||
         (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_MUTESOLO, &fdPlayerMuteSolo) != SL_RESULT_SUCCESS ||
         (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_VOLUME, &fdPlayerVolume) != SL_RESULT_SUCCESS ||
         (*fdPlayerSeek)->SetLoop(fdPlayerSeek, looping ? SL_BOOLEAN_TRUE : SL_BOOLEAN_FALSE, 0, SL_TIME_UNKNOWN) != SL_RESULT_SUCCESS ||
         (*fdPlayerVolume)->SetVolumeLevel(fdPlayerVolume, SLmillibel(2000.0f * log10(std::max(1.0f, gain)))) != SL_RESULT_SUCCESS)
      {
        throw Sound_Source_HW_Init_Failure();
      }
    }
#endif

    set_reference_distance();
    set_max_distance();
  }

  void Sound_Source_HW::uninit() {
    stop();

#ifndef DISABLE_AL
    if(m_source != AL_NONE)
      Sound_Renderer_AL::alDeleteSources()(1, &m_source);
#endif
#ifdef ENABLE_SLES
    // destroy file descriptor audio player object, and invalidate all associated interfaces
    if (fdPlayerObject != NULL) {
      (*fdPlayerObject)->Destroy(fdPlayerObject);
      fdPlayerObject = NULL;
      fdPlayerPlay = NULL;
      fdPlayerSeek = NULL;
      fdPlayerMuteSolo = NULL;
      fdPlayerVolume = NULL;
    }
#endif
  }

  Sound_Source::Sound_Source()
    : m_hw(0),
    m_priority(ZENI_DEFAULT_SOUND_PRIORITY),
    m_buffer(&get_Sound().get_Hello_World_Buffer()),
    m_duration(0.0f),
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
    m_duration(0.0f),
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
      const float duration = get_duration();

      const Time_HQ current_time;
      if(m_hw)
        m_play_position = m_hw->get_time();
      else {
        float time_step = float(current_time.get_seconds_since(m_play_time));
        m_play_position += time_step;
      }
      m_play_time = current_time;

      if(duration && m_play_position > duration) {
        if(m_looping)
          m_play_position = float(fmod(m_play_position, duration));
        else {
          m_playing = false;
          m_stopped = true;

          m_play_position = 0.0f;
        }
      }
    }
    else if(m_hw)
      m_play_position = m_hw->get_time();

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
    const float duration = get_duration();

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

    if(m_playing && (!duration || time < duration))
      hw.play();
  }

  Sound_Source_HW * Sound_Source::unassign() {
    Sound_Source_HW * ptr = m_hw;

    if(m_hw) {
      update_state();

      m_hw->stop();

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
      m_playing = true;
      m_paused = m_stopped = false;

      m_play_time.update();

      get_time();
    }
    else if(state == Sound_Source_HW::PAUSED) {
      get_time();

      m_paused = true;
      m_playing = m_stopped = false;
    }
    else if(!m_paused && state == Sound_Source_HW::STOPPED) {
      m_stopped = true;
      m_playing = m_paused = false;

      get_time();
    }
  }

}

#include <Zeni/Undefine.h>
