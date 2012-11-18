#ifndef ZENI_SOUND_STREAM_AL_H
#define ZENI_SOUND_STREAM_AL_H

#include <Zeni/Sound_Renderer_AL.h>

#ifndef DISABLE_AL

namespace Zeni {

  class ZENI_AUDIO_DLL Sound_Stream_AL {
    Sound_Stream_AL(const Sound_Stream_AL &);
    Sound_Stream_AL & operator=(const Sound_Stream_AL &);

    // stereo 44kHz = 2 * 88000 bytes per second
    static const size_t NUM_BUFFERS = 60;
    static const size_t BUFFER_SIZE = 4096;

  public:
    Sound_Stream_AL(const Zeni::String &path, const bool &looping_ = false);
    ~Sound_Stream_AL();
  
    bool is_playing();
    bool is_paused();
    bool is_stopped();

    void play();
    void pause();
    void stop();

    void update();
        
  private:
    void stream(ALuint buffer);   // reloads a buffer
    void check();                 // checks OpenAL error state
    Zeni::String errorString(int code); // stringify an error code

    FILE*           oggFile;       // file handle
    OggVorbis_File  oggStream;     // stream handle
    vorbis_info*    vorbisInfo;    // some formatting data
    vorbis_comment* vorbisComment; // user comments
 
    ALuint buffers[NUM_BUFFERS]; // front and back buffers
    ALuint source;               // audio source
    ALenum format;               // internal format

    bool looping;
  };

}

#endif

#endif
