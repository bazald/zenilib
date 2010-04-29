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

#include <Zeni/Sound.hxx>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Mutex.hxx>
#include <Zeni/Vector3f.hxx>

#include <SDL/SDL.h>
#include <iostream>
#include <vector>
#include <iomanip>

#ifndef DISABLE_AL
#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>
#undef OV_EXCLUDE_STATIC_CALLBACKS
#endif

using namespace std;

namespace Zeni {

  Sound_Buffer::Sound_Buffer()
    : m_buffer(AL_NONE),
      m_loader(0),
      m_thread(0)
  {
    get_Sound();

#ifndef DISABLE_AL
    m_buffer = alutCreateBufferHelloWorld();

    if(m_buffer == AL_NONE) {
      cerr << "ALUT error on Hello World: " << alutGetErrorString(alutGetError()) << endl;
      throw Sound_Buffer_Init_Failure();
    }
#endif
  }

//  Sound_Buffer::Sound_Buffer(const Sound_Buffer &rhs)
//    : m_buffer(0),
//      m_loader(0),
//      m_thread(0)
//  {
//#ifndef DISABLE_AL
//    m_buffer = alutCreateBufferHelloWorld();
//
//    if(m_buffer == AL_NONE) {
//      cerr << "ALUT error on Hello World: " << alutGetErrorString(alutGetError()) << endl;
//      throw Sound_Buffer_Init_Failure();
//    }
//#endif
//
//    swap(m_buffer, rhs.m_buffer);
//    swap(m_loader, rhs.m_loader);
//    swap(m_thread, rhs.m_thread);
//  }

  Sound_Buffer::Sound_Buffer(const string &filename)
    : m_buffer(AL_NONE),
      m_loader(0),
      m_thread(0)
  {
    get_Sound();

    m_loader = new Loader(filename);
    m_thread = new Thread(*m_loader);
  }

  Sound_Buffer::~Sound_Buffer() {
    delete m_thread;
    delete m_loader;

#ifndef DISABLE_AL
    if(m_buffer != AL_NONE && !Quit_Event::has_fired())
      alDeleteBuffers(1, &m_buffer);
#endif
  }

  //Sound_Buffer & Sound_Buffer::operator=(const Sound_Buffer &rhs) {
  //  Sound_Buffer temp(rhs);
  //  swap(m_buffer, temp.m_buffer);
  //  swap(m_loader, temp.m_loader);
  //  swap(m_thread, temp.m_thread);
  //  return *this;
  //}

  pair<ALuint, float> Sound_Buffer::load_ogg_vorbis(const string &
#ifndef DISABLE_AL
    filename
#endif
    ) {

#ifndef DISABLE_AL

    /*** Open VorbisFile ***/

    OggVorbis_File oggFile;
    if(ov_fopen(const_cast<char *>(filename.c_str()), &oggFile))
      return make_pair(AL_NONE, 0.0f);

    /*** Get Information About the Audio File ***/

    vorbis_info *pInfo = ov_info(&oggFile, -1);
    const ALenum format = pInfo->channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
    const ALsizei freq = pInfo->rate;
    const ALsizei bytes_per_sample = format == AL_FORMAT_STEREO16 ? 4 : 2;
    const ogg_int64_t num_samples = ov_pcm_total(&oggFile, -1);
    const ogg_int64_t pcm_size = num_samples * bytes_per_sample;
    const float duration = float(num_samples) / freq;

#ifndef NDEBUG
    if(format == AL_FORMAT_STEREO16)
      cerr << "WARNING: '" << filename << "' is stereo and will be unaffected by the OpenAL positional audio system." << endl;
#endif

    /*** Load the Audio File ***/

    int bytes = 0;
    int buffer_size = int(pcm_size);
    vector<char> buffer( static_cast<size_t>(buffer_size) );
    for(char *begin = &buffer[0], *end = begin + pcm_size;
        begin != end;
        begin += bytes, buffer_size -= bytes) {
      bytes = ov_read(&oggFile, begin, buffer_size, 0, 2, 1, 0);

      if(!bytes) {
        ov_clear(&oggFile);
        throw Sound_Buffer_Init_Failure();
      }
    }

    /*** Generate Audio Buffer ***/

    ALuint bufferID = AL_NONE;
    alGenBuffers(1, &bufferID);
    alBufferData(bufferID, format, &buffer[0], static_cast<ALsizei>(buffer.size()), freq);

    return make_pair(bufferID, duration);

#else

    return make_pair(AL_NONE, 0.0f);

#endif

  }

  Sound_Buffer::Loader::Loader(const string &filename)
    : m_duration(0.0f),
    m_filename(filename)
  {
  }

  int Sound_Buffer::Loader::function() {
#ifndef DISABLE_AL
    pair<ALuint, float> loaded_ogg = load_ogg_vorbis(m_filename);
    //if(loaded_ogg.first == AL_NONE)
    //  loaded_ogg.first = alutCreateBufferFromFile(m_filename.c_str());

    if(loaded_ogg.first == AL_NONE) {
      cerr << "ALUT error on '" << m_filename << "': " << alutGetErrorString(alutGetError()) << endl;
      return -1;
    }

    m_buffer = loaded_ogg.first;
    m_duration = loaded_ogg.second;
#endif

    return 0;
  }

  void Sound_Buffer::finish_loading() const {
    delete m_thread;
    m_thread = 0;

    const int status = m_loader->status;
    m_buffer = m_loader->m_buffer;
    m_duration = m_loader->m_duration;

    delete m_loader;
    m_loader = 0;

    if(status)
      throw Sound_Buffer_Init_Failure();
  }

  Sound::Sound()
    : m_bgm(0),
    m_bgm_source(0)
  {
    // Ensure Core is initialized
    get_Core();

#ifndef DISABLE_AL
    if(!alutInit(0, 0))
      throw Sound_Init_Failure();

    // Check for Vorbis extension functionality; seems to always fail :(
#ifdef _MACOSX
    alIsExtensionPresent(const_cast<ALubyte *>(reinterpret_cast<const ALubyte *>("AL_EXT_vorbis")));
#else
    alIsExtensionPresent("AL_EXT_vorbis");
#endif
    //cerr << "Valid Audio Formats: " << alutGetMIMETypes(ALUT_LOADER_BUFFER) << endl;

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

  Sound & get_Sound() {
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

    m_bgm_source->stop();
    m_bgm_source->set_buffer(get_Hello_World_Buffer());
    delete m_bgm;
    m_bgm = 0;

    m_bgmusic = filename;
    m_bgm = new Sound_Buffer(m_bgmusic);
    m_bgm_source->set_buffer(*m_bgm);
    m_bgm_source->set_time(0.0f);

    if(playing)
      m_bgm_source->play();
#endif
  }

  void Sound::assert_m_bgm() {
    if(!m_bgm)
      m_bgm = new Sound_Buffer();

    if(!m_bgm_source)
      m_bgm_source = new Sound_Source(*m_bgm);
  }

}
