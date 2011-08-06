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

#include <zeni.h>

#include <cmath>

#include <Zeni/Define.h>

namespace Zeni {

  Camera::Camera(const Point3f &position_, const Quaternion &orientation_, const float &near_clip_, const float &far_clip_, const float &fov_rad_, const float &tunnel_vision_factor_)
    : position(position_),
    orientation(orientation_),
    near_clip(near_clip_),
    far_clip(far_clip_),
    fov_rad(fov_rad_),
    tunnel_vision_factor(tunnel_vision_factor_)
  {
  }

  void Camera::adjust_yaw(const float &theta) {
    orientation = Quaternion::Axis_Angle(get_up(), theta) * orientation;
  }

  void Camera::adjust_pitch(const float &phi) {
    orientation = Quaternion::Axis_Angle(get_left(), phi) * orientation;
  }

  void Camera::adjust_roll(const float &rho) {
    orientation = Quaternion::Axis_Angle(get_forward(), rho) * orientation;
  }

  void Camera::move_forward_xy(const float &distance) {
    position += get_forward().get_ij().normalized() * distance;
  }

  void Camera::move_left_xy(const float &distance) {
    position += get_left().get_ij().normalized() * distance;
  }

  void Camera::turn_left_xy(const float &theta) {
    orientation = Quaternion::Axis_Angle(ZENI_DEFAULT_UP_VECTOR, theta) * orientation;
  }

  void Camera::look_at(const Point3f &world_coord, const Vector3f &horizon_plane_normal) {
    const Vector3f forward = (world_coord - position).normalized();
    const Vector3f left = (horizon_plane_normal % forward).normalized();
    const Vector3f up = (forward % left).normalized(); 

    orientation = Quaternion::Forward_Up(forward, up,
                                         ZENI_DEFAULT_FORWARD_VECTOR,
                                         ZENI_DEFAULT_UP_VECTOR);
  }

  void Camera::look_at(const Point3f &world_coord, const Collision::Plane &horizon_plane) {
    look_at(world_coord, horizon_plane.get_normal());
  }

}

#include <Zeni/Undefine.h>
