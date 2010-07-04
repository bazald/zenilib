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

#ifndef ZENI_CAMERA_HXX
#define ZENI_CAMERA_HXX

// HXXed below
#include <Zeni/Coordinate.h>
#include <Zeni/Matrix4f.h>
#include <Zeni/Quaternion.h>

#include <Zeni/Camera.h>
#include <cmath>

#include <Zeni/Global.h>

namespace Zeni {

  Vector3f Camera::get_forward() const {
    return orientation * ZENI_DEFAULT_FORWARD_VECTOR;
  }

  Vector3f Camera::get_up() const {
    return orientation * ZENI_DEFAULT_UP_VECTOR;
  }

  Vector3f Camera::get_left() const {
    return orientation * ZENI_DEFAULT_LEFT_VECTOR;
  }
  float Camera::get_fov_deg() const {
    return fov_rad * 180.0f / pi;
  }

  void Camera::set_fov_deg(const float &degrees) {
    fov_rad = degrees * pi / 180.0f;
  }

  Point3f Camera::get_tunneled_position() const {
    return position + (1.0f - tunnel_vision_factor) * near_clip * get_forward();
  }

  float Camera::get_tunneled_near_clip() const {
    return near_clip * tunnel_vision_factor;
  }

  float Camera::get_tunneled_far_clip() const {
    return far_clip - near_clip + get_tunneled_near_clip();
  }

  float Camera::get_tunneled_fov_deg() const {
    return get_tunneled_fov_rad() * 180.0f / pi;
  }

  float Camera::get_tunneled_fov_rad() const {
    return 2.0f * std::atan((near_clip * std::tan(0.5f * fov_rad)) / get_tunneled_near_clip());
  }

  Matrix4f Camera::get_view_matrix() const {
    return Matrix4f::View(get_tunneled_position(), get_forward(), get_up());
  }

  Matrix4f Camera::get_projection_matrix(const std::pair<Point2i, Point2i> &viewport) const {
    return Matrix4f::Perspective(get_tunneled_fov_rad(),
      float(viewport.second.x - viewport.first.x) / (viewport.second.y - viewport.first.y),
      get_tunneled_near_clip(), get_tunneled_far_clip());
  }

}

#include <Zeni/Global_Undef.h>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Matrix4f.hxx>
#include <Zeni/Quaternion.hxx>

#endif
