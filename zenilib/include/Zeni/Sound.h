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

/**
 * \class Zeni::Sound_Buffer
 *
 * \ingroup Zenilib
 *
 * \brief Stores Sound Data
 *
 * A Sound_Buffer stores sound data to be played from Sound_Sources.
 *
 * \note Mono sounds work best.
 *
 * \warning Stereo sounds will not be affected by the positional audio system.  Furthermore, in a surround sound speaker setup, they may play only through the left and right channels.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Sound
 *
 * \ingroup Zenilib
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

#ifdef ZENI_INLINES
#include <Zeni/Sound.hxx>
#endif

#ifndef ZENI_SOUND_H
#define ZENI_SOUND_H

#include <Zeni/Core.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Vector3f.h>
#include <Zeni/Thread.h>

#include <string>

#ifndef DISABLE_AL
#include <AL/alut.h>
#else

#define ALint int
#define ALuint unsigned int
#define ALfloat float
#define ALenum int

#define AL_NONE 0
#define AL_TRUE true
#define AL_FALSE false

#endif

namespace Zeni {

  class Sound_Source;

  class Sound_Buffer {
    Sound_Buffer(const Sound_Buffer &rhs);
    Sound_Buffer & operator=(const Sound_Buffer &rhs);

  public:
    Sound_Buffer();
    Sound_Buffer(const std::string &filename); ///< Load a Sound_Buffer from a file.  Only wav is guaranteed to be supported.
    ~Sound_Buffer();

    inline const ALuint & get_id() const; ///< Get the OpenAL id of the Sound_Buffer
    inline const float & get_duration() const; ///< Get the duration of the Sound_Buffer in seconds

    /// Ogg Vorbis Loader
    static std::pair<ALuint, float> load_ogg_vorbis(const std::string &filename);

  private:
    mutable ALuint m_buffer;
    mutable float m_duration;
    
    class Loader : public Task {
      Loader(const Loader &);
      Loader & operator=(const Loader &);
    
    public:
      Loader(const std::string &filename);
      
      virtual int function();
      
      ALuint m_buffer;
      float m_duration;
      const std::string m_filename;
    };
    
    void finish_loading() const;
    
    mutable Loader *m_loader;
    mutable Thread *m_thread;
  };

  class Sound {
    // Get reference to only instance;
    friend Sound & get_Sound(); ///< Get access to the singleton.

    Sound();
    ~Sound();

    // Undefined
    Sound(const Sound &);
    Sound & operator=(const Sound &);

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
    std::string m_bgmusic;
    Sound_Buffer *m_bgm;
    Sound_Source *m_bgm_source;
  };

  Sound & get_Sound(); ///< Get access to the singleton.

  struct Sound_Buffer_Init_Failure : public Error {
    Sound_Buffer_Init_Failure() : Error("Zeni Sound Buffer Failed to Initialize Correctly") {}
  };

  struct Sound_Init_Failure : public Error {
    Sound_Init_Failure() : Error("Zeni Sound Failed to Initialize Correctly") {}
  };

}

#endif
