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
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Sound_Source
 *
 * \ingroup Zenilib
 *
 * \brief Plays Sound Data
 *
 * A Sound_Source plays sound data from a Sound_Buffer.  The term comes
 * from the idea that OpenAL is built for 3D positional sound.  A Sound_Source
 * is defined by the Sound_Buffer it is playing as well as its position, 
 * velocity, and several other factors.
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

  class Sound_Buffer {
  public:
    Sound_Buffer();
    Sound_Buffer(const Sound_Buffer &rhs); ///< Transfers the buffer rather than copying it (auto_ptr semantics)
    Sound_Buffer(const std::string &filename); ///< Load a Sound_Buffer from a file.  Only wav is guaranteed to be supported.
    ~Sound_Buffer();

    inline const ALuint & get_id() const; ///< Get the OpenAL id of the Sound_Buffer

    /// Transfers the buffer rather than copying it (auto_ptr semantics)
    Sound_Buffer & operator=(const Sound_Buffer &rhs);

    /// Ogg Vorbis Loader
    static ALuint load_ogg_vorbis(const std::string &filename);

  private:
    mutable ALuint m_buffer;
    
    class Loader : public Task {
      Loader(const Loader &);
      Loader & operator=(const Loader &);
    
    public:
      Loader(const std::string &filename);
      
      virtual int function();
      
      ALuint m_buffer;
      const std::string m_filename;
    };
    
    void finish_loading() const;
    
    mutable Loader *m_loader;
    mutable Thread *m_thread;
  };

  class Sound_Source {
  public:
    Sound_Source();
    Sound_Source(const Sound_Source &rhs);
    Sound_Source(const Sound_Buffer &buffer,
                 const float &pitch = 1.0f,
                 const float &gain = 1.0f,
                 const Point3f &position = Point3f(),
                 const Vector3f &velocity = Vector3f(),
                 const bool &looping = false);
    Sound_Source(const ALuint &buffer,
                 const float &pitch = 1.0f,
                 const float &gain = 1.0f,
                 const Point3f &position = Point3f(),
                 const Vector3f &velocity = Vector3f(),
                 const bool &looping = false);
    ~Sound_Source();

    inline void set_buffer(const Sound_Buffer &buffer); ///< Set the Sound_Buffer to be played.
    inline void set_buffer(const ALuint &buffer); ///< Set the Sound_Buffer to be played.
    inline void set_pitch(const float &pitch = 1.0f); ///< Set the pitch.
    inline void set_gain(const float &gain = 1.0f); ///< Set the gain.
    inline void set_position(const Point3f &position); ///< Set the position of the Sound_Source.
    inline void set_velocity(const Vector3f &velocity); ///< Set the velocity of the Sound_Source for the doppler effect.
    inline void set_looping(const bool &looping); ///< Set whether the Sound_Buffer should loop back to the start once it is done playing.
    inline void set_time(const float &time); ///< Set the current position in the Sound_Buffer, offset in seconds.
    inline void set_near_clamp(const float &near_clip = 10.0f); // Set the near clipping distance
    inline void set_far_clamp(const float &far_clip = 1000.0f); // Set the far clipping distance
    inline void set_rolloff(const float &rolloff = 1.0f); // Set the maximum reduction in volume due to distance

    inline ALuint get_buffer() const; ///< Get the Sound_Buffer's OpenAL id
    inline float get_pitch() const; ///< Get the pitch.
    inline float get_gain() const; ///< Get the gain.
    inline Point3f get_position() const; ///< Get the position of the Sound_Buffer.
    inline Vector3f get_velocity() const; ///< Get the velocity of the Sound_Buffer.
    inline bool is_looping() const; ///< Check to see if the Sound_Buffer is set to loop back to the start once it is done playing.
    inline float get_time() const; ///< Get the current position in the Sound_Buffer, offset in seconds.
    inline float get_near_clamp() const; // Get the near clipping distance
    inline float get_far_clamp() const; // Get the far clipping distance
    inline float get_rolloff() const; // Get the maximum reduction in volume due to distance

    inline void play(); ///< Begin playing or unpause the Sound_Source.
    inline void pause(); ///< Pause the Sound_Source.
    inline void stop(); ///< Stop the Sound_Source.  (Essentially the same as pause but resets the current time.)

    inline bool is_playing(); ///< Check to see if the Sound_Source is playing.
    inline bool is_paused(); ///< Check to see if the Sound_Source is paused.
    inline bool is_stopped(); ///< Check to see if the Sound_Source is stopped.

    /// Transfers the buffer rather than copying it (auto_ptr semantics)
    Sound_Source & operator=(const Sound_Source &rhs);

  private:
    void init(const ALuint &buffer,
              const float &pitch = 1.0f,
              const float &gain = 1.0f,
              const Point3f &position = Point3f(),
              const Vector3f &velocity = Vector3f(),
              const bool &looping = false);

    mutable ALuint m_source;
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

  struct Sound_Source_Init_Failure : public Error {
    Sound_Source_Init_Failure() : Error("Zeni Sound Source Failed to Initialize Correctly") {}
  };

  struct Sound_Init_Failure : public Error {
    Sound_Init_Failure() : Error("Zeni Sound Failed to Initialize Correctly") {}
  };

}

#endif
