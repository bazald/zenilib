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
#include <vector>
#include <iomanip>

#ifndef DISABLE_AL
#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>
#undef OV_EXCLUDE_STATIC_CALLBACKS
#endif

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace Zeni {

  Sound_Buffer::Sound_Buffer()
    : 
#ifndef DISABLE_AL
    m_buffer(AL_NONE),
#endif
    m_duration(float())
    //, m_loader(0)
    //, m_thread(0)
  {
    Sound &sr = get_Sound();

    const String &filename = "sfx/104469__dkmedic__world";

#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&sr.get_Renderer())) {
      //m_loader = new Loader(filename);
      //m_thread = new Thread(*m_loader);

      std::pair<ALuint, float> loaded_ogg = load_ogg_vorbis(filename);

      if(loaded_ogg.first == AL_NONE) {
        std::cerr << "OpenAL error on '" << filename << "': " << Sound_Renderer_AL::errorString() << std::endl;
        throw Sound_Buffer_Init_Failure();
      }

      m_buffer = loaded_ogg.first;
      m_duration = loaded_ogg.second;
    }
#endif
#ifdef ENABLE_SLES
    init_SLES(filename);
#endif
  }

  Sound_Buffer::Sound_Buffer(const String &filename)
    :
#ifndef DISABLE_AL
    m_buffer(AL_NONE),
#endif
    m_duration(float())
    //, m_loader(0)
    //, m_thread(0)
  {
    Sound &sr = get_Sound();

#ifndef DISABLE_AL
    if(dynamic_cast<Sound_Renderer_AL *>(&sr.get_Renderer())) {
      //m_loader = new Loader(filename);
      //m_thread = new Thread(*m_loader);

      std::pair<ALuint, float> loaded_ogg = load_ogg_vorbis(filename);

      if(loaded_ogg.first == AL_NONE) {
        std::cerr << "OpenAL error on '" << filename << "': " << Sound_Renderer_AL::errorString() << std::endl;
        throw Sound_Buffer_Init_Failure();
      }

      m_buffer = loaded_ogg.first;
      m_duration = loaded_ogg.second;
    }
#endif
#ifdef ENABLE_SLES
    init_SLES(filename);
#endif
  }

  Sound_Buffer::~Sound_Buffer() {
    //delete m_thread;
    //delete m_loader;

#ifndef DISABLE_AL
    if(m_buffer != AL_NONE && !Quit_Event::has_fired())
      Sound_Renderer_AL::alDeleteBuffers()(1, &m_buffer);
#endif
  }

  std::pair<ALuint, float> Sound_Buffer::load_ogg_vorbis(const String &
#ifndef DISABLE_AL
    filename
#endif
    ) {

#ifndef DISABLE_AL
    /*** Open VorbisFile ***/

    OggVorbis_File oggFile;
    if(ov_fopen(const_cast<char *>((filename + ".ogg").c_str()), &oggFile))
      return std::make_pair(AL_NONE, 0.0f);

    /*** Get Information About the Audio File ***/

    vorbis_info *pInfo = ov_info(&oggFile, -1);
    const ALenum format = pInfo->channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
    const ALsizei freq = ALsizei(pInfo->rate);
    const ALsizei bytes_per_sample = format == AL_FORMAT_STEREO16 ? 4 : 2;
    const ogg_int64_t num_samples = ov_pcm_total(&oggFile, -1);
    const ogg_int64_t pcm_size = num_samples * bytes_per_sample;
    const float duration = float(num_samples) / freq;

#ifndef NDEBUG
    if(format == AL_FORMAT_STEREO16)
      std::cerr << "WARNING: '" << filename << "' is stereo and will be unaffected by the OpenAL positional audio system." << std::endl;
#endif

    /*** Load the Audio File ***/

    int bytes = 0;
    int buffer_size = int(pcm_size);
    std::vector<char> buffer( static_cast<size_t>(buffer_size) );
    for(char *begin = &buffer[0], *end = begin + pcm_size;
        begin != end;
        begin += bytes, buffer_size -= bytes) {
      bytes = int(ov_read(&oggFile, begin, buffer_size, 0, 2, 1, 0));

      if(!bytes) {
        ov_clear(&oggFile);
        throw Sound_Buffer_Init_Failure();
      }
    }

    /*** Generate Audio Buffer ***/

    ALuint bufferID = AL_NONE;
    Sound_Renderer_AL::alGenBuffers()(1, &bufferID);
    Sound_Renderer_AL::alBufferData()(bufferID, format, &buffer[0], static_cast<ALsizei>(buffer.size()), freq);

    return std::make_pair(bufferID, duration);

#else

    return std::make_pair(AL_NONE, 0.0f);

#endif

  }

#ifdef ENABLE_SLES
  void Sound_Buffer::init_SLES(const String &filename) {
    if(dynamic_cast<Sound_Renderer_SLES *>(&get_Sound().get_Renderer())) {
      // use asset manager to open asset by filename
      AAsset* asset = AAssetManager_open(File_Ops::get_AAssetManager(), (filename + ".wav").c_str(), AASSET_MODE_UNKNOWN);
      if(!asset)
        throw Sound_Buffer_Init_Failure();

      // open asset as file descriptor
      off_t start, length;
      int fd = AAsset_openFileDescriptor(asset, &start, &length);
      assert(0 <= fd);
      AAsset_close(asset);

      // configure audio source
      loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
      format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
      audioSrc = {&loc_fd, &format_mime};
    }
  }
#endif

//  Sound_Buffer::Loader::Loader(const String &filename)
//    : m_duration(0.0f),
//    m_filename(filename)
//  {
//  }
//
//  int Sound_Buffer::Loader::function() {
//#ifndef DISABLE_AL
//    std::pair<ALuint, float> loaded_ogg = load_ogg_vorbis(m_filename);
//
//    if(loaded_ogg.first == AL_NONE) {
//      std::cerr << "OpenAL error on '" << m_filename << "': " << Sound_Renderer_AL::errorString() << std::endl;
//      return -1;
//    }
//
//    m_buffer = loaded_ogg.first;
//    m_duration = loaded_ogg.second;
//#endif
//
//    return 0;
//  }
//
//  void Sound_Buffer::finish_loading() const {
//    delete m_thread;
//    m_thread = 0;
//
//    const int status = m_loader->status;
//    m_buffer = m_loader->m_buffer;
//    m_duration = m_loader->m_duration;
//
//    delete m_loader;
//    m_loader = 0;
//
//    if(status)
//      throw Sound_Buffer_Init_Failure();
//  }

}
