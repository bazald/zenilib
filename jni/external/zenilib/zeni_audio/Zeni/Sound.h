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

/**
 * \class Zeni::Sound
 *
 * \ingroup zenilib
 *
 * \brief The Sound Singleton
 *
 * The Sound Singleton is responsible for initializing and uninitializing OpenAL.
 * It also controls a single Sound_Source that is positioned at the listener's 
 * current location so as to allow the playing of background music.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_SOUND_H
#define ZENI_SOUND_H

#include <Zeni/Error.h>
#include <Zeni/Singleton.h>
#include <Zeni/Sound_Buffer.h>
#include <Zeni/Sound_Renderer.h>

#include <Zeni/Define.h>

namespace Zeni {

  class ZENI_AUDIO_DLL Sound;

#ifdef _WINDOWS
  ZENI_AUDIO_EXT template class ZENI_AUDIO_DLL Singleton<Sound>;
#endif

  class ZENI_AUDIO_DLL Sound : public Singleton<Sound> {
    friend class Singleton<Sound>;

    static Sound * create();

    // Undefined
    Sound(const Sound &);
    Sound & operator=(const Sound &);

    Sound();
    virtual ~Sound();

  public:
    inline Sound_Renderer & get_Renderer(); ///< Get the current Sound_Renderer

    // Listener Functions
    void set_listener_position(const Point3f &position); ///< Set the position of the listener and BGM.
    void set_listener_velocity(const Vector3f &velocity); ///< Set the velocity of the listener and BGM for the doppler effect.
    void set_listener_forward_and_up(const Vector3f &forward, const Vector3f &up); ///< Set the orientation of the listener
    void set_listener_gain(const float &gain); ///< Set the listener gain
    void set_listener_muted(const bool &muted); ///< Set whether the listener is muted

    Point3f get_listener_position() const; ///< Get the position of the listener and BGM.
    Vector3f get_listener_velocity() const; ///< Get the velocity of the listener and BGM.
    std::pair<Vector3f, Vector3f> get_listener_forward_and_up() const; ///< Set the orientation of the listener
    inline float get_listener_gain() const; ///< Get the listener gain
    inline bool is_listener_muted() const; ///< Is the listener muted

    // BackGround Music Functions
    void set_BGM(const String &filename); ///< Set BackGround Music

    // BackGround Music Getter Functions
    inline bool playing_BGM(); ///< Check to see if BackGround Music is playing
    inline bool paused_BGM(); ///< Check to see if BackGround Music is paused
    inline bool stopped_BGM(); ///< Check to see if BackGround Music is stopped
    inline float get_BGM_pitch(); ///< Get the pitch.
    inline float get_BGM_gain(); ///< Get the gain.
    inline bool is_BGM_looping(); ///< Check to see if the BGM is set to loop back to the start once it is done playing.
    inline float get_BGM_time(); ///< Get the current position in the BGM, offset in seconds.

    // BackGround Music Setter Functions
    inline void play_BGM(); ///< Begin Playing BackGround Music
    inline void pause_BGM(); ///< Pause BackGround Music
    inline void stop_BGM(); ///< Stop BackGround Music
    inline void set_BGM_pitch(const float &pitch); ///< Set the pitch.
    inline void set_BGM_gain(const float &gain); ///< Set the gain.
    inline void set_BGM_looping(const bool &looping); ///< Set whether the BGM should loop back to the start once it is done playing.
    inline void set_BGM_time(const float &time); ///< Set the current position in the BGM, offset in seconds.

    Sound_Buffer & get_Hello_World_Buffer() const; ///< Initialize m_bgm if needed and return a corresponding Sound_Source

  private:
    Sound_Source & get_BGM_Source() const; ///< Initialize m_bgm if needed and return a corresponding Sound_Source

    Sound_Renderer *m_sound_renderer;
    mutable Sound_Buffer *m_hello_world_buffer;

    String m_bgmusic;
    mutable Sound_Buffer *m_bgm;

    float m_listener_gain;
    bool m_listener_muted;
  };

  ZENI_AUDIO_DLL Sound & get_Sound(); ///< Get access to the singleton.

  struct ZENI_AUDIO_DLL Sound_Init_Failure {
    Sound_Init_Failure() : msg("Zeni Sound Failed to Initialize Correctly") {}
    String msg;
  };

}

#include <Zeni/Undefine.h>

#endif
