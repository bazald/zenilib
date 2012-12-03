#include <zeni_audio.h>

#include <iostream>

#ifndef DISABLE_AL

#include <Zeni/Define.h>

namespace Zeni {

  Sound_Stream_AL::Sound_Stream_AL(const String &path, const bool &looping_, const float &time_)
    : buffers_used(0),
    looping(looping_)
  {
    if(!dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer()))
      throw Sound_Stream_Init_Failure();

    const int result = ov_fopen((path + ".ogg").c_str(), &oggStream);
    if(result < 0)
      throw Sound_Stream_Ogg_Read_Failure();

    vorbisInfo = ov_info(&oggStream, -1);
    vorbisComment = ov_comment(&oggStream, -1);
 
    if(vorbisInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;

    ov_time_seek(&oggStream, time_);

    Sound_Renderer_AL::alGenBuffers()(NUM_BUFFERS, buffers);
    Sound_Renderer_AL::alGenSources()(1, &source);

    ALfloat pos[3] = {0.0f, 0.0f, 0.0f};
    Sound_Renderer_AL::alSourcefv()(source, AL_POSITION,        pos    );
    Sound_Renderer_AL::alSourcefv()(source, AL_VELOCITY,        pos    );
    Sound_Renderer_AL::alSourcefv()(source, AL_DIRECTION,       pos    );
    Sound_Renderer_AL::alSourcef() (source, AL_ROLLOFF_FACTOR,  0.0    );
    Sound_Renderer_AL::alSourcei() (source, AL_SOURCE_RELATIVE, AL_TRUE);

    if(Sound_Renderer_AL::alGetError()() != AL_NO_ERROR) {
      destroy();
      throw Sound_Stream_Init_Failure();
    }
  }

  Sound_Stream_AL::~Sound_Stream_AL() {
    destroy();
  }

  void Sound_Stream_AL::set_pitch(const float &pitch) {
    Sound_Renderer_AL::alSourcef()(source, AL_PITCH, pitch);
  }

  void Sound_Stream_AL::set_gain(const float &gain) {
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer()))
      Sound_Renderer_AL::alSourcef()(source, AL_GAIN, gain);
  }

  void Sound_Stream_AL::set_position(const Point3f &position) {
    Sound_Renderer_AL::alSourcefv()(source, AL_POSITION, const_cast<ALfloat *>(reinterpret_cast<const ALfloat *>(&position)));
  }

  void Sound_Stream_AL::set_velocity(const Vector3f &velocity) {
    Sound_Renderer_AL::alSourcefv()(source, AL_VELOCITY, const_cast<ALfloat *>(reinterpret_cast<const ALfloat *>(&velocity)));
  }

  void Sound_Stream_AL::set_looping(const bool &looping_) {
    looping = looping_;
  }

  void Sound_Stream_AL::set_time(const float &time) {
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer()))
      ov_time_seek(&oggStream, time);
  }

  void Sound_Stream_AL::set_reference_distance(const float &reference_distance) {
    Sound_Renderer_AL::alSourcef()(source, AL_REFERENCE_DISTANCE, reference_distance);
  }

  void Sound_Stream_AL::set_max_distance(const float &max_distance) {
    Sound_Renderer_AL::alSourcef()(source, AL_MAX_DISTANCE, max_distance);
  }

  void Sound_Stream_AL::set_rolloff(const float &rolloff) {
    Sound_Renderer_AL::alSourcef()(source, AL_ROLLOFF_FACTOR, rolloff);
  }

  float Sound_Stream_AL::get_duration() const {
    return float(ov_time_total(const_cast<OggVorbis_File *>(&oggStream), -1));
  }

  float Sound_Stream_AL::get_pitch() const {
    float pitch = ZENI_DEFAULT_PITCH;
    Sound_Renderer_AL::alGetSourcef()(source, AL_PITCH, &pitch);
    return pitch;
  }

  float Sound_Stream_AL::get_gain() const {
    float gain = ZENI_DEFAULT_GAIN;
    if(dynamic_cast<Sound_Renderer_AL *>(&get_Sound().get_Renderer())) {
      Sound_Renderer_AL::alGetSourcef()(source, AL_GAIN, &gain);
    }
    return gain;
  }

  Point3f Sound_Stream_AL::get_position() const {
    Point3f position;
    Sound_Renderer_AL::alGetSourcefv()(source, AL_POSITION, reinterpret_cast<ALfloat *>(&position));
    return position;
  }

  Vector3f Sound_Stream_AL::get_velocity() const {
    Vector3f velocity;
    Sound_Renderer_AL::alGetSourcefv()(source, AL_VELOCITY, reinterpret_cast<ALfloat *>(&velocity));
    return velocity;
  }

  bool Sound_Stream_AL::is_looping() const {
    return looping;
  }

  float Sound_Stream_AL::get_time() const {
    return float(ov_time_tell(const_cast<OggVorbis_File *>(&oggStream)));
  }
  
  float Sound_Stream_AL::get_reference_distance() const {
    float reference_distance = ZENI_DEFAULT_REFERENCE_DISTANCE;
    Sound_Renderer_AL::alGetSourcef()(source, AL_REFERENCE_DISTANCE, &reference_distance);
    return reference_distance;
  }

  float Sound_Stream_AL::get_max_distance() const {
    float max_distance = ZENI_DEFAULT_MAX_SOUND_DISTANCE;
    Sound_Renderer_AL::alGetSourcef()(source, AL_MAX_DISTANCE, &max_distance);
    return max_distance;
  }

  float Sound_Stream_AL::get_rolloff() const {
    float rolloff = ZENI_DEFAULT_ROLLOFF;
    Sound_Renderer_AL::alGetSourcef()(source, AL_ROLLOFF_FACTOR, &rolloff);
    return rolloff;
  }

  void Sound_Stream_AL::play() {
    if(oggStream.offset == oggStream.end)
      ov_raw_seek(&oggStream, 0);

    update();

    Sound_Renderer_AL::alSourcePlay()(source);
  }

  void Sound_Stream_AL::pause() {
    Sound_Renderer_AL::alSourcePause()(source);
  }

  void Sound_Stream_AL::stop() {
    Sound_Renderer_AL::alSourceStop()(source);
  }

  bool Sound_Stream_AL::is_playing() const {
    ALenum state;
    Sound_Renderer_AL::alGetSourcei()(source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
  }

  bool Sound_Stream_AL::is_paused() const {
    ALenum state;
    Sound_Renderer_AL::alGetSourcei()(source, AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
  }

  bool Sound_Stream_AL::is_stopped() const {
    ALenum state;
    Sound_Renderer_AL::alGetSourcei()(source, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
  }

  void Sound_Stream_AL::update() {
    if(oggStream.offset == oggStream.end && !looping)
      return;

    int processed;
    Sound_Renderer_AL::alGetSourcei()(source, AL_BUFFERS_PROCESSED, &processed);
    processed += NUM_BUFFERS - buffers_used;
 
    while(processed--) {
      ALuint buffer;
      if(buffers_used == NUM_BUFFERS)
        Sound_Renderer_AL::alSourceUnqueueBuffers()(source, 1, &buffer);
      else
        buffer = buffers[buffers_used++];
 
      stream(buffer);

      Sound_Renderer_AL::alSourceQueueBuffers()(source, 1, &buffer);

      if(oggStream.offset == oggStream.end && !looping)
        break;
    }
  }

  void Sound_Stream_AL::destroy() {
    Sound_Renderer_AL::alSourceStop()(source);
    Sound_Renderer_AL::alDeleteSources()(1, &source);
    Sound_Renderer_AL::alDeleteBuffers()(NUM_BUFFERS, buffers);
 
    ov_clear(&oggStream);
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
          throw Sound_Stream_Ogg_Read_Failure();
        else if(looping)
          ov_raw_seek(&oggStream, 0);
        else
          break;
      }
    }
    
    if(size == 0)
      throw Error("Failed to stream data from .ogg file.");
    //else if(size < BUFFER_SIZE)
    //  memset(data + size, 0, BUFFER_SIZE - size);
 
    Sound_Renderer_AL::alBufferData()(buffer, format, data, size, vorbisInfo->rate);

    if(Sound_Renderer_AL::alGetError()() != AL_NO_ERROR)
      throw Sound_Stream_Update_Failure();
  }

  //String Sound_Stream_AL::errorString(int code) {
  //  switch(code) {
  //    case OV_EREAD:
  //      return "Read from media.";
  //    case OV_ENOTVORBIS:
  //      return "Not Vorbis data.";
  //    case OV_EVERSION:
  //      return "Vorbis version mismatch.";
  //    case OV_EBADHEADER:
  //      return "Invalid Vorbis header.";
  //    case OV_EFAULT:
  //      return "Internal logic fault (bug or heap/stack corruption.";
  //    default:
  //      return "Unknown Ogg error.";
  //  }
  //}

}

namespace Zeni {
  void * this_pointer_is_silent_sound_two = (void *)0xDEADBEEF;
}

#include <Zeni/Undefine.h>

#endif
