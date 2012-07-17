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

#ifndef ZENI_CAMERA_HXX
#define ZENI_CAMERA_HXX

// HXXed below
#include <Zeni/Coordinate.h>
#include <Zeni/Matrix4f.h>
#include <Zeni/Quaternion.h>

#include <Zeni/Camera.h>
#include <cmath>

#include <Zeni/Define.h>

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
    return fov_rad * 180.0f / Global::pi;
  }

  void Camera::set_fov_deg(const float &degrees) {
    fov_rad = degrees * Global::pi / 180.0f;
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
    return get_tunneled_fov_rad() * 180.0f / Global::pi;
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

#include <Zeni/Undefine.h>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Matrix4f.hxx>
#include <Zeni/Quaternion.hxx>

#endif
