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

#ifndef ZENI_SOUND_HXX
#define ZENI_SOUND_HXX

// HXXed below
#include <Zeni/Sound_Source.h>
#include <Zeni/Sound_AL.h>
#include <Zeni/Sound_NULL.h>

#include <Zeni/Sound.h>

#include <Zeni/Global.h>

namespace Zeni {

  /* Note that variadic macros *may* not be supported in some pre-C99 compilers.
   * It is not hard to code around, but they are convenient.
   */

#ifndef DISABLE_AL
#ifndef DISABLE_NULL

#define \
  SOUND_IV_FCN_CALL(member_function, ...) { \
    switch(vtype()) { \
      case Sound_Base::ZENI_SOUND_NULL: return static_cast<Sound_NULL *>(this)->member_function(__VA_ARGS__); \
      case Sound_Base::ZENI_SOUND_AL: return static_cast<Sound_AL *>(this)->member_function(__VA_ARGS__); \
      default: abort(); /* implies ZENI_SOUND_ANY, which *should* be impossible */ \
    } \
    exit(END_OF_TIME); /* cannot be called, but suppresses a warning */ \
  }

#define \
  SOUND_IV_FCN_CALL_CONST(member_function, ...) { \
    switch(vtype()) { \
      case Sound_Base::ZENI_SOUND_NULL: return static_cast<const Sound_NULL *>(this)->member_function(__VA_ARGS__); \
      case Sound_Base::ZENI_SOUND_AL: return static_cast<const Sound_AL *>(this)->member_function(__VA_ARGS__); \
      default: abort(); /* implies ZENI_SOUND_ANY, which *should* be impossible */ \
    } \
    exit(END_OF_TIME); /* cannot be called, but suppresses a warning */ \
  }

#else

#define \
  SOUND_IV_FCN_CALL(member_function, ...) { \
    return static_cast<Sound_AL *>(this)->member_function(__VA_ARGS__); \
  }

#define \
  SOUND_IV_FCN_CALL_CONST(member_function, ...) { \
    return static_cast<const Sound_AL *>(this)->member_function(__VA_ARGS__); \
  }

#endif
#else

#define \
  SOUND_IV_FCN_CALL(member_function, ...) { \
    return static_cast<Sound_NULL *>(this)->member_function(__VA_ARGS__); \
  }

#define \
  SOUND_IV_FCN_CALL_CONST(member_function, ...) { \
    return static_cast<const Sound_NULL *>(this)->member_function(__VA_ARGS__); \
  }

#endif

#define EMPTY()

  void Sound::set_listener_position(const Point3f &position) {
    assert_m_bgm();

    m_bgm_source->set_position(position);

    SOUND_IV_FCN_CALL(set_listener_position_impl, position);
  }

  void Sound::set_listener_velocity(const Vector3f &velocity) {
    assert_m_bgm();

    m_bgm_source->set_velocity(velocity);

    SOUND_IV_FCN_CALL(set_listener_velocity_impl, velocity);
  }

  void Sound::set_listener_forward_and_up(const Vector3f &forward, const Vector3f &up) {
    SOUND_IV_FCN_CALL(set_listener_forward_and_up_impl, forward, up);
  }

  Point3f Sound::get_listener_position() const {
    SOUND_IV_FCN_CALL_CONST(get_listener_position_impl, EMPTY());
  }

  Vector3f Sound::get_listener_velocity() const {
    SOUND_IV_FCN_CALL_CONST(get_listener_velocity_impl, EMPTY());
  }

  std::pair<Vector3f, Vector3f> Sound::get_listener_forward_and_up() const {
    SOUND_IV_FCN_CALL_CONST(get_listener_forward_and_up_impl, EMPTY());
  }

#undef SOUND_IV_FCN_CALL
#undef SOUND_IV_FCN_CALL_CONST
#undef EMPTY

  void Sound::set_BGM_pitch(const float & pitch) {
    assert_m_bgm();

    m_bgm_source->set_pitch(pitch);
  }

  void Sound::set_BGM_gain(const float &gain) {
    assert_m_bgm();

    m_bgm_source->set_gain(gain);
  }

  void Sound::set_BGM_looping(const bool &looping) {
    assert_m_bgm();

    m_bgm_source->set_looping(looping);
  }

  void Sound::set_BGM_time(const float &time) {
    assert_m_bgm();

    m_bgm_source->set_time(time);
  }

  float Sound::get_BGM_pitch() {
    assert_m_bgm();

    return m_bgm_source->get_pitch();
  }

  float Sound::get_BGM_gain() {
    assert_m_bgm();

    return m_bgm_source->get_gain();
  }

  bool Sound::is_BGM_looping() {
    assert_m_bgm();

    return m_bgm_source->is_looping();
  }

  float Sound::get_BGM_time() {
    assert_m_bgm();

    return m_bgm_source->get_time();
  }

  bool Sound::playing_BGM() {
    assert_m_bgm();

    return m_bgm_source->is_playing();
  }

  bool Sound::paused_BGM() {
    assert_m_bgm();

    return m_bgm_source->is_paused();
  }

  bool Sound::stopped_BGM() {
    assert_m_bgm();

    return m_bgm_source->is_stopped();
  }

  void Sound::play_BGM() {
    assert_m_bgm();

    return m_bgm_source->play();
  }

  void Sound::pause_BGM() {
    assert_m_bgm();

    return m_bgm_source->pause();
  }
  
  void Sound::stop_BGM() {
    assert_m_bgm();

    return m_bgm_source->stop();
  }

}

#include <Zeni/Global_Undef.h>

#include <Zeni/Sound_Source.hxx>
#include <Zeni/Sound_AL.hxx>
#include <Zeni/Sound_NULL.hxx>

#endif
