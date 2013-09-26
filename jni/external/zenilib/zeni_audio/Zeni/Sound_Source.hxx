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

#ifndef ZENI_SOUND_SOURCE_HXX
#define ZENI_SOUND_SOURCE_HXX

// HXXed below
#include <Zeni/Timer_HQ.h>

#include <Zeni/Sound_Source.h>

#include <cassert>
#include <cmath>

#ifndef DISABLE_AL
#ifndef AL_SEC_OFFSET
#define AL_SEC_OFFSET 0x1024
#endif
#endif

#include <Zeni/Define.h>

namespace Zeni {

  void Sound_Source::set_priority(const int &priority)       {m_priority = priority;}
  void Sound_Source::set_buffer(const Sound_Buffer &buffer)  {m_buffer = &buffer;    if(m_hw) m_hw->set_buffer(buffer);}
  void Sound_Source::set_pitch(const float &pitch)           {m_pitch = pitch;       if(m_hw) m_hw->set_pitch(pitch);}
  void Sound_Source::set_gain(const float &gain)             {m_gain = gain;         if(m_hw) m_hw->set_gain(gain);}
  void Sound_Source::set_position(const Point3f &position)   {m_position = position; if(m_hw) m_hw->set_position(position);}
  void Sound_Source::set_velocity(const Vector3f &velocity)  {m_velocity = velocity; if(m_hw) m_hw->set_velocity(velocity);}
  void Sound_Source::set_looping(const bool &looping)        {m_looping = looping;   if(m_hw) m_hw->set_looping(looping);}

  void Sound_Source::set_time(const float &time) {
    m_play_position = time;
    if(m_playing)
      m_play_time.update();

    if(m_hw)
      m_hw->set_time(time);
  }

  void Sound_Source::set_reference_distance(const float &reference_distance) {m_reference_distance = reference_distance; if(m_hw) m_hw->set_reference_distance(reference_distance);}
  void Sound_Source::set_max_distance(const float &max_distance)             {m_max_distance = max_distance;             if(m_hw) m_hw->set_max_distance(max_distance);}
  void Sound_Source::set_rolloff(const float &rolloff)                       {m_rolloff = rolloff;                       if(m_hw) m_hw->set_rolloff(rolloff);}

  int Sound_Source::get_priority() const {return m_priority;}
  Time_HQ Sound_Source::get_unstop_time() const {return m_unstop_time;}
  const Sound_Buffer & Sound_Source::get_buffer() const {assert(m_buffer); return *m_buffer;}
  float Sound_Source::get_duration() const {if(m_hw) m_duration = m_hw->get_duration(); return m_duration;}
  float Sound_Source::get_pitch() const {return m_pitch;}
  float Sound_Source::get_gain() const {return m_gain;}
  Point3f Sound_Source::get_position() const {return m_position;}
  Vector3f Sound_Source::get_velocity() const {return m_velocity;}
  bool Sound_Source::is_looping() const {return m_looping;}
  float Sound_Source::get_reference_distance() const {return m_reference_distance;}
  float Sound_Source::get_max_distance() const {return m_max_distance;}
  float Sound_Source::get_rolloff() const {return m_rolloff;}

  void Sound_Source::play()  {update_state(Sound_Source_HW::PLAYING); if(m_hw) m_hw->play(); m_unstop_time.update();}
  void Sound_Source::pause() {update_state(Sound_Source_HW::PAUSED); if(m_hw) m_hw->pause(); m_unstop_time.update();}
  void Sound_Source::stop()  {update_state(Sound_Source_HW::STOPPED); if(m_hw) m_hw->stop();}

  bool Sound_Source::is_playing() const {update_state(); return m_playing;}
  bool Sound_Source::is_paused() const  {update_state(); return m_paused;}
  bool Sound_Source::is_stopped() const {update_state(); return m_stopped;}

  bool Sound_Source::is_assigned() const {return m_hw != 0;}

}

#include <Zeni/Undefine.h>

#include <Zeni/Timer_HQ.hxx>

#endif
