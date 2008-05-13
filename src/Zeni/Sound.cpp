/* This file is part of the Zenipex Library.
* Copyleft (C) 2007 Mitchell Keith Bloch a.k.a. bazald
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

#include <Zeni/Sound.hxx>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Vector3f.hxx>

#include <SDL/SDL.h>
#include <iostream>
#include <vector>

#ifndef DISABLE_AL
#include <vorbis/vorbisfile.h>
#endif

using std::string;

namespace Zeni {

  Sound_Buffer::Sound_Buffer()
    : m_buffer(AL_NONE),
      m_loader(0),
      m_thread(0)
  {
    Sound::get_reference();

#ifndef DISABLE_AL
    m_buffer = alutCreateBufferHelloWorld();

    if(m_buffer == AL_NONE) {
      std::cerr << "ALUT error on Hello World: " << alutGetErrorString(alutGetError()) << std::endl;
      throw Sound_Buffer_Init_Failure();
    }
#endif
  }

  Sound_Buffer::Sound_Buffer(const Sound_Buffer &rhs)
    : m_buffer(0),
      m_loader(0),
      m_thread(0)
  {
    Sound::get_reference();
    
#ifndef DISABLE_AL
    m_buffer = alutCreateBufferHelloWorld();
    
    if(m_buffer == AL_NONE) {
      std::cerr << "ALUT error on Hello World: " << alutGetErrorString(alutGetError()) << std::endl;
      throw Sound_Buffer_Init_Failure();
    }
#endif

    std::swap(m_buffer, rhs.m_buffer);
    std::swap(m_loader, rhs.m_loader);
    std::swap(m_thread, rhs.m_thread);
  }

  Sound_Buffer::Sound_Buffer(const string &filename)
    : m_buffer(AL_NONE),
      m_loader(0),
      m_thread(0)
  {
    Sound::get_reference();
    
    m_loader = new Loader(filename);
    m_thread = new Thread(*m_loader);
  }

  Sound_Buffer::~Sound_Buffer() {
    delete m_thread;
    delete m_loader;
    
#ifndef DISABLE_AL
    if(m_buffer != AL_NONE)
      alDeleteBuffers(1, &m_buffer);
#endif
  }
  
  Sound_Buffer & Sound_Buffer::operator=(const Sound_Buffer &rhs) {
    Sound_Buffer temp(rhs);
    std::swap(m_buffer, temp.m_buffer);
    std::swap(m_loader, temp.m_loader);
    std::swap(m_thread, temp.m_thread);
    return *this;
  }

  ALuint Sound_Buffer::load_ogg_vorbis(const std::string &
#ifndef DISABLE_AL
    filename
#endif
    ) {

#ifndef DISABLE_AL

    /*** Open VorbisFile ***/

    OggVorbis_File oggFile;
    if(ov_fopen(const_cast<char *>(filename.c_str()), &oggFile))
      return AL_NONE;

    /*** Get Information About the Audio File ***/

    vorbis_info *pInfo = ov_info(&oggFile, -1);
    const ALenum format = pInfo->channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
    const ALsizei freq = pInfo->rate;

    /*** Load the Audio File ***/

    std::vector<char> buffer;
    static const int buffer_size = 4096;
    char array[buffer_size];
    for(;;) {
      const long bytes = ov_read(&oggFile, array, buffer_size, 0, 2, 1, 0);

      if(bytes > 0)
        buffer.insert(buffer.end(), array, array + bytes);
      else {
        ov_clear(&oggFile);
        
        if(!bytes)
          break;
        else {
          ov_clear(&oggFile);
          throw Sound_Buffer_Init_Failure();
        }
      }
    }

    /*** Generate Audio Buffer ***/

    ALuint bufferID = AL_NONE;
    alGenBuffers(1, &bufferID);
    alBufferData(bufferID, format, &buffer[0], static_cast<ALsizei>(buffer.size()), freq);

    return bufferID;
    
#else

    return AL_NONE;

#endif

  }
  
  Sound_Buffer::Loader::Loader(const std::string &filename)
    : m_filename(filename),
      done(false)
  {
  }
  
  int Sound_Buffer::Loader::run() {
#ifndef DISABLE_AL
    m_buffer = load_ogg_vorbis(m_filename);
    if(m_buffer == AL_NONE)
      m_buffer = alutCreateBufferFromFile(m_filename.c_str());

    if(m_buffer == AL_NONE) {
      std::cerr << "ALUT error on '" << m_filename << "': " << alutGetErrorString(alutGetError()) << std::endl;
      return -1;
    }
#endif

    done = true;

    return 0;
  }

  void Sound_Buffer::finish_loading() const {
    delete m_thread;
    m_thread = 0;
    
    const int status = m_loader->status;
    m_buffer = m_loader->m_buffer;
    
    delete m_loader;
    m_loader = 0;
    
    if(status)
      throw Sound_Buffer_Init_Failure();
  }

  static Sound_Buffer g_Hello_World_Buffer;

  Sound_Source::Sound_Source()
    : m_source(AL_NONE)
  {
    init(g_Hello_World_Buffer.get_id());
  }

  Sound_Source::Sound_Source(const Sound_Source &rhs)
    : m_source(rhs.m_source)
  {
    rhs.m_source = AL_NONE;
    const_cast<Sound_Source &>(rhs).init(g_Hello_World_Buffer.get_id());
  }

  Sound_Source::Sound_Source(const Sound_Buffer &buffer, const float &pitch, const float &gain,
    const Point3f &position, const Vector3f &velocity, const bool &looping)
    : m_source(AL_NONE)
  {
    init(buffer.get_id(), pitch, gain, position, velocity, looping);
  }

  Sound_Source::Sound_Source(const ALuint &buffer, const float &pitch, const float &gain,
    const Point3f &position, const Vector3f &velocity, const bool &looping)
    : m_source(AL_NONE)
  {
    init(buffer, pitch, gain, position, velocity, looping);
  }

  Sound_Source::~Sound_Source() {
#ifndef DISABLE_AL
    if(m_source != AL_NONE)
      alDeleteSources(1, &m_source);
#endif
  }

  void Sound_Source::init(const ALuint &
#ifndef DISABLE_AL
    buffer
#endif
    , const float &
#ifndef DISABLE_AL
    pitch
#endif
    , const float &
#ifndef DISABLE_AL
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
#ifndef DISABLE_AL
    looping
#endif
    ) {
    Sound::get_reference();

#ifndef DISABLE_AL
    alGenSources(1, &m_source);

    if(m_source == AL_NONE) {
      std::cerr << "ALUT error" << ": " << alutGetErrorString(alutGetError()) << std::endl;
      throw Sound_Source_Init_Failure();
    }

    alSourcei(m_source, AL_BUFFER, buffer);
    alSourcef(m_source, AL_PITCH, pitch);
    alSourcef(m_source, AL_GAIN, gain);
    alSourcefv(m_source, AL_POSITION, reinterpret_cast<const float *>(&position));
    alSourcefv(m_source, AL_VELOCITY, reinterpret_cast<const float *>(&velocity));
    alSourcei(m_source, AL_LOOPING, looping);
#endif

    set_near_clamp();
    set_far_clamp();
  }

  Sound_Source & Sound_Source::operator=(const Sound_Source &rhs) {
    Sound_Source temp(rhs);
    std::swap(m_source, temp.m_source);
    return *this;
  }

  Sound::Sound()
    : m_bgm(0),
    m_bgm_source(0)
  {
    // Ensure Core is initialized
    Core::get_reference();

#ifndef DISABLE_AL
    if(!alutInit(0, 0))
      throw Sound_Init_Failure();

    // Check for Vorbis extension functionality; seems to always fail :(
    alIsExtensionPresent("AL_EXT_vorbis");
    std::cerr << "Valid Audio Formats: " << alutGetMIMETypes(ALUT_LOADER_BUFFER) << std::endl;

    ALfloat listener_position[] = {0.0f, 0.0f, 0.0f};
    ALfloat listener_velocity[] = {0.0f, 0.0f, 0.0f};
    ALfloat listener_forward_and_up[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    alListenerfv(AL_POSITION, listener_position);
    alListenerfv(AL_VELOCITY, listener_velocity);
    alListenerfv(AL_ORIENTATION, listener_forward_and_up);
#endif
  }

  Sound::~Sound() {
#ifndef DISABLE_AL
    delete m_bgm_source;
    delete m_bgm;
    alutExit();
#endif
  }

  Sound & Sound::get_reference() {
    static Sound e_sound;
    return e_sound;
  }

  void Sound::set_BGM(const string &
#ifndef DISABLE_AL
    filename
#endif
    ) {
#ifndef DISABLE_AL
    assert_m_bgm();

    bool playing = m_bgm_source->is_playing() ? true : false;
    float pitch = m_bgm_source->get_pitch();
    float gain = m_bgm_source->get_gain();
    Point3f position = m_bgm_source->get_position();
    Vector3f velocity = m_bgm_source->get_velocity();
    bool looping = m_bgm_source->is_looping();

    m_bgm_source->stop();
    delete m_bgm_source;
    m_bgm_source = 0;

    m_bgmusic = filename;
    *m_bgm = Sound_Buffer(filename);

    m_bgm_source = new Sound_Source(*m_bgm, pitch, gain, position, velocity, looping);

    if(playing)
      m_bgm_source->play();
#endif
  }

  void Sound::assert_m_bgm() {
#ifndef DISABLE_AL
    if(!m_bgm) {
      m_bgm = new Sound_Buffer();
      if(!m_bgm) {
        alutExit();
        throw Sound_Init_Failure();
      }
    }

    if(!m_bgm_source) {
      m_bgm_source = new Sound_Source(*m_bgm);
      if(!m_bgm_source) {
        delete m_bgm;
        alutExit();
        throw Sound_Init_Failure();
      }
    }
#endif
  }

}
