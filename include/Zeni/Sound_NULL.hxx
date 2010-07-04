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

#ifndef ZENI_SOUND_NULL_HXX
#define ZENI_SOUND_NULL_HXX

// HXXed below
#include <Zeni/Sound_Source.h>

#include <Zeni/Sound_NULL.h>

namespace Zeni {

  void Sound_NULL::set_listener_position_impl(const Point3f &)
  {
  }

  void Sound_NULL::set_listener_velocity_impl(const Vector3f &)
  {
  }

  void Sound_NULL::set_listener_forward_and_up_impl(const Vector3f &, const Vector3f &)
  {
  }

  Point3f Sound_NULL::get_listener_position_impl() const {
    return Point3f();
  }

  Vector3f Sound_NULL::get_listener_velocity_impl() const {
    return Vector3f();
  }

  std::pair<Vector3f, Vector3f> Sound_NULL::get_listener_forward_and_up_impl() const {
    return std::make_pair(Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f));
  }

}

#include <Zeni/Sound_Source.hxx>

#endif
