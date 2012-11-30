#ifndef ZENI_SOUND_STREAM_AL_H
#define ZENI_SOUND_STREAM_AL_H

#include <Zeni/Sound_Renderer_AL.h>

#ifndef DISABLE_AL

#include <Zeni/Define.h>

namespace Zeni {

  class ZENI_AUDIO_DLL Sound_Stream_AL {
    Sound_Stream_AL(const Sound_Stream_AL &);
    Sound_Stream_AL & operator=(const Sound_Stream_AL &);

    // stereo 44kHz = 2 * 88000 bytes per second
    static const size_t NUM_BUFFERS = 60;
    static const size_t BUFFER_SIZE = 4096;

  public:
    Sound_Stream_AL(const String &path, const bool &looping_ = false, const float &time_ = 0.0f);
    ~Sound_Stream_AL();

    void set_pitch(const float &pitch = ZENI_DEFAULT_PITCH); ///< Set the pitch.
    void set_gain(const float &gain = ZENI_DEFAULT_GAIN); ///< Set the gain.
    void set_position(const Point3f &position); ///< Set the position of the Sound_Source_HW.
    void set_velocity(const Vector3f &velocity); ///< Set the velocity of the Sound_Source_HW for the doppler effect.
    void set_looping(const bool &looping); ///< Set whether the Sound_Buffer should loop back to the start once it is done playing.
    void set_time(const float &time); ///< Set the current position in the Sound_Buffer, offset in seconds.
    void set_reference_distance(const float &reference_distance = ZENI_DEFAULT_REFERENCE_DISTANCE); // Set the near clamping distance
    void set_max_distance(const float &max_distance = ZENI_DEFAULT_MAX_SOUND_DISTANCE); // Set the far clamping distance
    void set_rolloff(const float &rolloff = ZENI_DEFAULT_ROLLOFF); // Set the maximum reduction in volume due to distance

    float get_duration() const; ///< Get the duration of the Sound_Buffer in seconds
    float get_pitch() const; ///< Get the pitch.
    float get_gain() const; ///< Get the gain.
    Point3f get_position() const; ///< Get the position of the Sound_Buffer.
    Vector3f get_velocity() const; ///< Get the velocity of the Sound_Buffer.
    bool is_looping() const; ///< Check to see if the Sound_Buffer is set to loop back to the start once it is done playing.
    float get_time() const; ///< Get the current position in the Sound_Buffer, offset in seconds.
    float get_reference_distance() const; // Get the near clamping distance
    float get_max_distance() const; // Get the far clamping distance
    float get_rolloff() const; // Get the maximum reduction in volume due to distance
    
    void play(); ///< Begin playing or unpause the Sound_Source.
    void pause(); ///< Pause the Sound_Source.
    void stop(); ///< Stop the Sound_Source.  (Essentially the same as pause but resets the current time.)

    bool is_playing() const; ///< Check to see if the Sound_Source is playing.
    bool is_paused() const; ///< Check to see if the Sound_Source is paused.
    bool is_stopped() const; ///< Check to see if the Sound_Source is stopped.

    void update();
        
  private:
    void destroy();
    void stream(ALuint buffer);   // reloads a buffer

    OggVorbis_File  oggStream;     // stream handle
    vorbis_info*    vorbisInfo;    // some formatting data
    vorbis_comment* vorbisComment; // user comments
 
    ALuint buffers[NUM_BUFFERS]; // front and back buffers
    ALuint buffers_used;         // number of buffers in the queue
    ALuint source;               // audio source
    ALenum format;               // internal format

    bool looping;
  };

  struct ZENI_AUDIO_DLL Sound_Stream_Init_Failure : public Error {
    Sound_Stream_Init_Failure() : Error("Zeni Sound Stream Failed to Initialize Correctly") {}
  };

  struct ZENI_AUDIO_DLL Sound_Stream_Update_Failure : public Error {
    Sound_Stream_Update_Failure() : Error("Zeni Sound Stream Failed to Update Correctly") {}
  };

  struct ZENI_AUDIO_DLL Sound_Stream_Ogg_Read_Failure : public Error {
    Sound_Stream_Ogg_Read_Failure() : Error("Zeni Sound Stream Failed to Read .ogg File Correctly") {}
  };

}

#include <Zeni/Undefine.h>

#endif

#endif
