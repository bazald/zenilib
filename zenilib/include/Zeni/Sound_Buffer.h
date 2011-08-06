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
 * \class Zeni::Sound_Buffer
 *
 * \ingroup zenilib
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

#ifndef ZENI_SOUND_BUFFER_H
#define ZENI_SOUND_BUFFER_H

#include <Zeni/Coordinate.h>
#include <Zeni/Database.h>
#include <Zeni/Vector3f.h>

#ifndef DISABLE_AL
#ifdef _MACOSX
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif
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

  class ZENI_AUDIO_DLL Sound_Source;

  class ZENI_AUDIO_DLL Sound_Buffer {
    friend class Sound;
    friend class Database<Sound_Buffer>;
    friend class Sounds;

    Sound_Buffer(const Sound_Buffer &rhs);
    Sound_Buffer & operator=(const Sound_Buffer &rhs);

    Sound_Buffer();
    Sound_Buffer(const String &filename); ///< Load a Sound_Buffer from a file.  Only wav is guaranteed to be supported.
    ~Sound_Buffer();

  public:
    const ALuint & get_id() const {return m_buffer;} ///< Get the OpenAL id of the Sound_Buffer
    const float & get_duration() const {return m_duration;} ///< Get the duration of the Sound_Buffer in seconds

    /// Ogg Vorbis Loader
    static std::pair<ALuint, float> load_ogg_vorbis(const String &filename);

  private:
    mutable ALuint m_buffer;
    mutable float m_duration;

  //  class ZENI_AUDIO_DLL Loader : public Task {
  //    Loader(const Loader &);
  //    Loader & operator=(const Loader &);
  //  
  //  public:
  //    Loader(const String &filename);
  //    
  //    virtual int function();
  //    
  //    ALuint m_buffer;
  //    float m_duration;
  //    const String m_filename;
  //  };
  //  
  //  void finish_loading() const;
  //  
  //  mutable Loader *m_loader;
  //  mutable Thread *m_thread;
  };

  struct ZENI_AUDIO_DLL Sound_Buffer_Init_Failure : public Error {
    Sound_Buffer_Init_Failure() : Error("Zeni Sound Buffer Failed to Initialize Correctly") {}
  };

}

#endif
