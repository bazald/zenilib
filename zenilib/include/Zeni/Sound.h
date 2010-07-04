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

#include <Zeni/Core.h>
#include <Zeni/IV.h>
#include <Zeni/Sound_Buffer.h>

namespace Zeni {

  class Sound_Base {
  public:
    enum SOUND_MODE {ZENI_SOUND_ANY = 0
      , ZENI_SOUND_NULL = 1
#ifndef DISABLE_AL
      , ZENI_SOUND_AL = 2
#endif
    };

    typedef ::IV<Sound_Base, SOUND_MODE> IV;
  };

  class Sound : public Sound_Base::IV {
    // Get reference to only instance;
    friend Sound & get_Sound(); ///< Get access to the singleton.

    // Undefined
    Sound(const Sound &);
    Sound & operator=(const Sound &);

  protected:
    Sound(const Sound_Base::SOUND_MODE &vtype_);
    virtual ~Sound();

  public:
    // Listener Functions
    inline void set_listener_position(const Point3f &position); ///< Set the position of the listener and BGM.
    inline void set_listener_velocity(const Vector3f &velocity); ///< Set the velocity of the listener and BGM for the doppler effect.
    inline void set_listener_forward_and_up(const Vector3f &forward, const Vector3f &up); ///< Set the orientation of the listener

    inline Point3f get_listener_position() const; ///< Get the position of the listener and BGM.
    inline Vector3f get_listener_velocity() const; ///< Get the velocity of the listener and BGM.
    inline std::pair<Vector3f, Vector3f> get_listener_forward_and_up() const; ///< Set the orientation of the listener

    // BackGround Music Functions
    void set_BGM(const std::string &filename); ///< Set BackGround Music

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

  private:
    void assert_m_bgm(); ///< Initialize m_bgm and m_bgm_source if not already done.

    static Sound *e_sound;

    std::string m_bgmusic;
    Sound_Buffer *m_bgm;
    Sound_Source *m_bgm_source;
  };

  Sound & get_Sound(); ///< Get access to the singleton.

  struct Sound_Init_Failure {
    Sound_Init_Failure() : msg("Zeni Sound Failed to Initialize Correctly") {}
    std::string msg;
  };

}

#endif
