/* This file is part of the Zenipex Library.
* Copyleft (C) 2010 Mitchell Keith Bloch a.k.a. bazald
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
 * \class Zeni::Sound_Source_HW
 *
 * \ingroup zenilib
 *
 * \brief Plays Sound Data
 *
 * This Sound_Source is directly tied to the OpenAL driver "hardware".
 * This is the original Zeni::Sound_Source, which has been replaced
 * by a much saner frontend to Sound_Source_Pool (which in turn manages
 * the Sound_Source_HW objects).
 *
 * \warning Don't use this unless you find that you need to.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Sound_Source
 *
 * \ingroup zenilib
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

#ifndef ZENI_SOUND_SOURCE_H
#define ZENI_SOUND_SOURCE_H

#include <Zeni/Core.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Sound.h>
#include <Zeni/Timer.h>
#include <Zeni/Vector3f.h>

#include <string>

#include <Zeni/Global.h>

namespace Zeni {

  class Sound_Source_Pool;

  const Sound_Buffer & get_Hello_World_Buffer();

  class Sound_Source_HW {
    friend class Sound_Source_Pool;

    Sound_Source_HW(const Sound_Source_HW &rhs);
    Sound_Source_HW & operator=(const Sound_Source_HW &rhs);

    Sound_Source_HW(const ALuint &source = AL_NONE);
    Sound_Source_HW(const Sound_Buffer &buffer,
                    const float &pitch = ZENI_DEFAULT_PITCH,
                    const float &gain = ZENI_DEFAULT_GAIN,
                    const Point3f &position = Point3f(),
                    const Vector3f &velocity = Vector3f(),
                    const bool &looping = false);
    ~Sound_Source_HW();

    static Sound_Source_HW * Try_Construct();

  public:
    enum STATE {STOPPED, PAUSED, PLAYING};

    inline void set_buffer(const Sound_Buffer &buffer); ///< Set the Sound_Buffer to be played.
    inline void set_pitch(const float &pitch = ZENI_DEFAULT_PITCH); ///< Set the pitch.
    inline void set_gain(const float &gain = ZENI_DEFAULT_GAIN); ///< Set the gain.
    inline void set_position(const Point3f &position); ///< Set the position of the Sound_Source_HW.
    inline void set_velocity(const Vector3f &velocity); ///< Set the velocity of the Sound_Source_HW for the doppler effect.
    inline void set_looping(const bool &looping); ///< Set whether the Sound_Buffer should loop back to the start once it is done playing.
    inline void set_time(const float &time); ///< Set the current position in the Sound_Buffer, offset in seconds.
    inline void set_reference_distance(const float &reference_distance = ZENI_DEFAULT_REFERENCE_DISTANCE); // Set the near clamping distance
    inline void set_max_distance(const float &max_distance = ZENI_DEFAULT_MAX_SOUND_DISTANCE); // Set the far clamping distance
    inline void set_rolloff(const float &rolloff = ZENI_DEFAULT_ROLLOFF); // Set the maximum reduction in volume due to distance

    inline const Sound_Buffer & get_buffer() const; ///< Get the Sound_Buffer
    inline float get_pitch() const; ///< Get the pitch.
    inline float get_gain() const; ///< Get the gain.
    inline Point3f get_position() const; ///< Get the position of the Sound_Buffer.
    inline Vector3f get_velocity() const; ///< Get the velocity of the Sound_Buffer.
    inline bool is_looping() const; ///< Check to see if the Sound_Buffer is set to loop back to the start once it is done playing.
    inline float get_time() const; ///< Get the current position in the Sound_Buffer, offset in seconds.
    inline float get_reference_distance() const; // Get the near clamping distance
    inline float get_max_distance() const; // Get the far clamping distance
    inline float get_rolloff() const; // Get the maximum reduction in volume due to distance

    inline void play(); ///< Begin playing or unpause the Sound_Source_HW.
    inline void pause(); ///< Pause the Sound_Source_HW.
    inline void stop(); ///< Stop the Sound_Source_HW.  (Essentially the same as pause but resets the current time.)

    inline STATE get_state() const; ///< Get PLAYING/PAUSED/STOPPED
    inline bool is_playing() const; ///< Check to see if the Sound_Source_HW is playing.
    inline bool is_paused() const; ///< Check to see if the Sound_Source_HW is paused.
    inline bool is_stopped() const; ///< Check to see if the Sound_Source_HW is stopped.

  private:
    void init(const Sound_Buffer &buffer,
              const float &pitch = ZENI_DEFAULT_PITCH,
              const float &gain = ZENI_DEFAULT_GAIN,
              const Point3f &position = Point3f(),
              const Vector3f &velocity = Vector3f(),
              const bool &looping = false);

    mutable ALuint m_source;
    mutable const Sound_Buffer * m_buffer;
  };

  class Sound_Source {
    friend class Sound_Source_Pool;

    Sound_Source(const Sound_Source &rhs);
    Sound_Source & operator=(const Sound_Source &rhs);

  public:
    Sound_Source();
    Sound_Source(const Sound_Buffer &buffer,
                 const float &pitch = ZENI_DEFAULT_PITCH,
                 const float &gain = ZENI_DEFAULT_GAIN,
                 const Point3f &position = Point3f(),
                 const Vector3f &velocity = Vector3f(),
                 const bool &looping = false);
    ~Sound_Source();

    inline void set_priority(const int &priority = ZENI_DEFAULT_SOUND_PRIORITY); ///< Set the priority that this Sound_Source should have. Higher numbers are more likely to be selected for play.
    inline void set_buffer(const Sound_Buffer &buffer); ///< Set the Sound_Buffer to be played.
    inline void set_pitch(const float &pitch = ZENI_DEFAULT_PITCH); ///< Set the pitch.
    inline void set_gain(const float &gain = ZENI_DEFAULT_GAIN); ///< Set the gain.
    inline void set_position(const Point3f &position); ///< Set the position of the Sound_Source.
    inline void set_velocity(const Vector3f &velocity); ///< Set the velocity of the Sound_Source for the doppler effect.
    inline void set_looping(const bool &looping); ///< Set whether the Sound_Buffer should loop back to the start once it is done playing.
    inline void set_time(const float &time); ///< Set the current position in the Sound_Buffer, offset in seconds.
    inline void set_reference_distance(const float &reference_distance = ZENI_DEFAULT_REFERENCE_DISTANCE); // Set the near clamping distance
    inline void set_max_distance(const float &max_distance = ZENI_DEFAULT_MAX_SOUND_DISTANCE); // Set the far clamping distance
    inline void set_rolloff(const float &rolloff = ZENI_DEFAULT_ROLLOFF); // Set the maximum reduction in volume due to distance

    inline int get_priority() const; ///< Get the Sound_Source's priority.
    inline Time get_unstop_time() const; ///< Get the Time at which the Sound_Source was most recently played or unpaused.
    inline const Sound_Buffer & get_buffer() const; ///< Get the Sound_Buffer's OpenAL id.
    inline float get_pitch() const; ///< Get the pitch.
    inline float get_gain() const; ///< Get the gain.
    inline Point3f get_position() const; ///< Get the position of the Sound_Buffer.
    inline Vector3f get_velocity() const; ///< Get the velocity of the Sound_Buffer.
    inline bool is_looping() const; ///< Check to see if the Sound_Buffer is set to loop back to the start once it is done playing.
    float get_time() const; ///< Get the current position in the Sound_Buffer, offset in seconds.
    inline float get_reference_distance() const; // Get the near clamping distance
    inline float get_max_distance() const; // Get the far clamping distance
    inline float get_rolloff() const; // Get the maximum reduction in volume due to distance

    inline void play(); ///< Begin playing or unpause the Sound_Source.
    inline void pause(); ///< Pause the Sound_Source.
    inline void stop(); ///< Stop the Sound_Source.  (Essentially the same as pause but resets the current time.)

    inline bool is_playing() const; ///< Check to see if the Sound_Source is playing.
    inline bool is_paused() const; ///< Check to see if the Sound_Source is paused.
    inline bool is_stopped() const; ///< Check to see if the Sound_Source is stopped.

    float calculate_gain(const Point3f &listener_position) const; ///< Get the actual volume gain, given gain+position+near_clamp+far_clamp+rolloff

    inline bool is_assigned() const; ///< Check to see if the Sound_Source is assigned to actual hardware.

  private:
    void assign(Sound_Source_HW &hw);
    Sound_Source_HW * unassign();

    void update_state() const;
    void update_state(const Sound_Source_HW::STATE &state) const;

    Sound_Source_HW * m_hw;
    int m_priority;
    Time m_unstop_time;

    const Sound_Buffer * m_buffer;
    float m_pitch;
    float m_gain;
    Point3f m_position;
    Vector3f m_velocity;
    bool m_looping;
    float m_reference_distance;
    float m_max_distance;
    float m_rolloff;

    mutable Time m_play_time;
    mutable float m_play_position;

    mutable bool m_playing;
    mutable bool m_paused;
    mutable bool m_stopped;

    bool m_remove_from_Pool_on_destruction;
  };

  struct Sound_Source_HW_Init_Failure : public Error {
    Sound_Source_HW_Init_Failure() : Error("Zeni Sound Source Hardware Failed to Initialize Correctly") {}
  };

}

#include <Zeni/Global_Undef.h>

#endif
