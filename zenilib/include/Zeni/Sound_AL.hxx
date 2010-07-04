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

#ifndef ZENI_SOUND_AL_HXX
#define ZENI_SOUND_AL_HXX

// HXXed below
#include <Zeni/Sound_Source.h>

#include <Zeni/Sound_AL.h>

#ifndef DISABLE_AL

namespace Zeni {

  void Sound_AL::set_listener_position_impl(const Point3f &position) {
    ALfloat listener_position[3] = {position.x, position.y, position.z};
    alListenerfv(AL_POSITION, listener_position);
  }

  void Sound_AL::set_listener_velocity_impl(const Vector3f &velocity) {
    ALfloat listener_velocity[3] = {velocity.i, velocity.j, velocity.k};
    alListenerfv(AL_VELOCITY, listener_velocity);
  }

  void Sound_AL::set_listener_forward_and_up_impl(const Vector3f &forward, const Vector3f &up) {
    ALfloat listener_forward_and_up[6] = {forward.i, forward.j, forward.k, up.i, up.j, up.k};
    alListenerfv(AL_ORIENTATION, listener_forward_and_up);
  }

  Point3f Sound_AL::get_listener_position_impl() const {
    ALfloat listener_position[3] = {0.0f, 0.0f, 0.0f};
    alGetListenerfv(AL_POSITION, listener_position);
    return Point3f(listener_position[0], listener_position[1], listener_position[2]);
  }

  Vector3f Sound_AL::get_listener_velocity_impl() const {
    ALfloat listener_velocity[3] = {0.0f, 0.0f, 0.0f};
    alGetListenerfv(AL_VELOCITY, listener_velocity);
    return Vector3f(listener_velocity[0], listener_velocity[1], listener_velocity[2]);
  }

  std::pair<Vector3f, Vector3f> Sound_AL::get_listener_forward_and_up_impl() const {
    ALfloat lfau[6] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    alGetListenerfv(AL_ORIENTATION, lfau);
    return std::make_pair(Vector3f(lfau[0], lfau[1], lfau[2]), Vector3f(lfau[3], lfau[4], lfau[5]));
  }

  std::string alErrorString(const ALenum &err) {
    switch(err) {
      case AL_NO_ERROR:          return "AL_NO_ERROR";          break;
      case AL_INVALID_NAME:      return "AL_INVALID_NAME";      break;
      case AL_INVALID_ENUM:      return "AL_INVALID_ENUM";      break;
      case AL_INVALID_VALUE:     return "AL_INVALID_VALUE";     break;
      case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION"; break;
      case AL_OUT_OF_MEMORY:     return "AL_OUT_OF_MEMORY";     break;
      default:                   return "AL_UNKNOWN_ERROR";     break;
    }
  }

}

#endif

#include <Zeni/Sound_Source.hxx>

#endif
