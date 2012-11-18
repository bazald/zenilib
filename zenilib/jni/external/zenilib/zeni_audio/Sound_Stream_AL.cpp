#include <zeni_audio.h>

#include <iostream>

#ifndef DISABLE_AL

namespace Zeni {

  Sound_Stream_AL::Sound_Stream_AL(const Zeni::String &path, const bool &looping_)
    : looping(looping_)
  {
    const int result = ov_fopen(path.c_str(), &oggStream);
    if(result < 0) {
      String error = String("Could not open Ogg stream. ") + errorString(result);
      std::cerr << error << std::endl;
      throw error;
    }

    vorbisInfo = ov_info(&oggStream, -1);
    vorbisComment = ov_comment(&oggStream, -1);
 
    if(vorbisInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;

    Zeni::Sound_Renderer_AL::alGenBuffers()(NUM_BUFFERS, buffers);
    check();
    Zeni::Sound_Renderer_AL::alGenSources()(1, &source);
    check();

    ALfloat pos[3] = {0.0f, 0.0f, 0.0f};
    Zeni::Sound_Renderer_AL::alSourcefv()(source, AL_POSITION,        pos    );
    check();
    Zeni::Sound_Renderer_AL::alSourcefv()(source, AL_VELOCITY,        pos    );
    check();
    Zeni::Sound_Renderer_AL::alSourcefv()(source, AL_DIRECTION,       pos    );
    check();
    Zeni::Sound_Renderer_AL::alSourcef() (source, AL_ROLLOFF_FACTOR,  0.0    );
    check();
    Zeni::Sound_Renderer_AL::alSourcei() (source, AL_SOURCE_RELATIVE, AL_TRUE);
    check();

    int buffers_queued;
    for(buffers_queued = 0; buffers_queued != NUM_BUFFERS; ++buffers_queued) {
      if(oggStream.offset == oggStream.end && !looping)
        break;
      stream(buffers[buffers_queued]);
    }

    Zeni::Sound_Renderer_AL::alSourceQueueBuffers()(source, buffers_queued, buffers);
    check();
  }

  Sound_Stream_AL::~Sound_Stream_AL() {
    Zeni::Sound_Renderer_AL::alSourceStop()(source);
    check();
    Zeni::Sound_Renderer_AL::alDeleteSources()(1, &source);
    check();
    Zeni::Sound_Renderer_AL::alDeleteBuffers()(NUM_BUFFERS, buffers);
    check();
 
    ov_clear(&oggStream);
  }

  bool Sound_Stream_AL::is_playing() {
    ALenum state;
    Zeni::Sound_Renderer_AL::alGetSourcei()(source, AL_SOURCE_STATE, &state);
    check();
    return state == AL_PLAYING;
  }

  bool Sound_Stream_AL::is_paused() {
    ALenum state;
    Zeni::Sound_Renderer_AL::alGetSourcei()(source, AL_SOURCE_STATE, &state);
    check();
    return state == AL_PAUSED;
  }

  bool Sound_Stream_AL::is_stopped() {
    ALenum state;
    Zeni::Sound_Renderer_AL::alGetSourcei()(source, AL_SOURCE_STATE, &state);
    check();
    return state == AL_STOPPED;
  }

  void Sound_Stream_AL::play() {
    Zeni::Sound_Renderer_AL::alSourcePlay()(source);
    check();
  }

  void Sound_Stream_AL::pause() {
    Zeni::Sound_Renderer_AL::alSourcePause()(source);
    check();
  }

  void Sound_Stream_AL::stop() {
    Zeni::Sound_Renderer_AL::alSourceStop()(source);
    check();
  }

  void Sound_Stream_AL::update() {
    if(oggStream.offset == oggStream.end && !looping)
      return;

    int processed;
    Zeni::Sound_Renderer_AL::alGetSourcei()(source, AL_BUFFERS_PROCESSED, &processed);
    check();
 
    while(processed--) {
      ALuint buffer;
      Zeni::Sound_Renderer_AL::alSourceUnqueueBuffers()(source, 1, &buffer);
      check();
 
      stream(buffer);

      Zeni::Sound_Renderer_AL::alSourceQueueBuffers()(source, 1, &buffer);
      check();

      if(oggStream.offset == oggStream.end && !looping)
        break;
    }
  }

  void Sound_Stream_AL::stream(ALuint buffer) {
    char data[BUFFER_SIZE];
    int  size = 0;
    int  section;
    int  result;
 
    while(size < BUFFER_SIZE) {
      result = ov_read(&oggStream, data + size, BUFFER_SIZE - size, 0, 2, 1, & section);
    
      if(result > 0)
        size += result;
      else {
        if(result < 0)
          throw Zeni::Error(errorString(result));
        else if(looping) {
          ov_time_seek(&oggStream, 0.0);
          check();
        }
        else
          break;
      }
    }
    
    if(size == 0)
      throw Zeni::Error("Failed to stream data from .ogg file.");
 
    Zeni::Sound_Renderer_AL::alBufferData()(buffer, format, data, size, vorbisInfo->rate);
    check();
  }

  void Sound_Stream_AL::check() {
    int error = Zeni::Sound_Renderer_AL::alGetError()();
 
    if(error != AL_NO_ERROR)
      throw Zeni::Error("OpenAL error was raised.");
  }

  String Sound_Stream_AL::errorString(int code) {
    switch(code) {
      case OV_EREAD:
        return String("Read from media.");
      case OV_ENOTVORBIS:
        return String("Not Vorbis data.");
      case OV_EVERSION:
        return String("Vorbis version mismatch.");
      case OV_EBADHEADER:
        return String("Invalid Vorbis header.");
      case OV_EFAULT:
        return String("Internal logic fault (bug or heap/stack corruption.");
      default:
        return String("Unknown Ogg error.");
    }
  }

}

namespace Zeni {
  void * this_pointer_is_silent_sound_two = (void *)0xDEADBEEF;
}

#endif
