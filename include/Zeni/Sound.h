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

/**
 * \class Zeni::Sound
 *
 * \ingroup Zenilib
 *
 * \brief The Sound Singleton
 *
 * The Sound Singleton is responsible for playing music and Sound_Effects.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_SOUND_H
#define ZENI_SOUND_H

#include "Core.h"
#include "Coordinate.h"

#include <AL/alut.h>
#include <string>

namespace Zeni {

  class Sound_Buffer {
  public:
    Sound_Buffer();
    Sound_Buffer(const Sound_Buffer &rhs); ///< Transfers the buffer rather than copying it (auto_ptr semantics)
    Sound_Buffer(const std::string &filename);
    ~Sound_Buffer();

    inline const ALuint & get_id() const;

    /// Transfers the buffer rather than copying it (auto_ptr semantics)
    Sound_Buffer & operator=(const Sound_Buffer &rhs);

  private:
    mutable ALuint m_buffer;
  };

  class Sound_Source {
  public:
    Sound_Source();
    Sound_Source(const Sound_Source &rhs);
    Sound_Source(const Sound_Buffer &buffer,
                 const float &pitch = 1.0f,
                 const float &gain = 1.0f,
                 const Point3f &position = Point3f(),
                 const Point3f &velocity = Point3f(),
                 const bool &looping = false);
    Sound_Source(const ALuint &buffer,
                 const float &pitch = 1.0f,
                 const float &gain = 1.0f,
                 const Point3f &position = Point3f(),
                 const Point3f &velocity = Point3f(),
                 const bool &looping = false);
    ~Sound_Source();

    inline void set_buffer(const Sound_Buffer &buffer);
    inline void set_buffer(const ALuint &buffer);
    inline void set_pitch(const float &pitch);
    inline void set_gain(const float &gain);
    inline void set_position(const Point3f &position);
    inline void set_velocity(const Point3f &velocity);
    inline void set_looping(const bool &looping);
    inline void set_time(const float &time);

    inline ALuint get_buffer() const;
    inline float get_pitch() const;
    inline float get_gain() const;
    inline Point3f get_position() const;
    inline Point3f get_velocity() const;
    inline bool is_looping() const;
    inline float get_time() const;

    inline void play();
    inline void pause();
    inline void stop();

    inline bool is_playing();
    inline bool is_paused();
    inline bool is_stopped();

    Sound_Source & operator=(const Sound_Source &rhs);

  private:
    void init(const ALuint &buffer,
              const float &pitch = 1.0f,
              const float &gain = 1.0f,
              const Point3f &position = Point3f(),
              const Point3f &velocity = Point3f(),
              const bool &looping = false) const;

    mutable ALuint m_source;
  };

  class Sound {
    Sound();
    ~Sound();

    // Undefined
    Sound(const Sound &);
    Sound & operator=(const Sound &);

  public:
    // Get reference to only instance; Might throw Sound_Init_Failure
    static Sound & get_reference();

    // Accessors
    inline bool is_enabled() const; ///< Check Sound is currently enabled

    // BackGround Music Functions
    void set_BGM(const std::string &filename); ///< Set BackGround Music

    inline bool playing_BGM(); ///< Check to see if BackGround Music is playing
    inline bool paused_BGM(); ///< Check to see if BackGround Music is paused
    inline bool stopped_BGM(); ///< Check to see if BackGround Music is stopped

    inline void play_BGM(); ///< Begin Playing BackGround Music
    inline void pause_BGM(); ///< Pause BackGround Music
    inline void stop_BGM(); ///< Stop BackGround Music
    inline void loop_BGM(const bool &looping = true); ///< Loop BackGround Music

  private:
    void assert_m_bgm();

    std::string m_bgmusic;
    Sound_Buffer *m_bgm;
    Sound_Source *m_bgm_source;
  };

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

#ifdef ZENI_INLINES
#include "Sound.hxx"
#endif

#endif
