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

#include <Zeni/Camera.hxx>
#include <Zeni/Collision.hxx>
#include <Zeni/Coordinate.hxx>
#include <Zeni/Quaternion.hxx>
#include <Zeni/Vector3f.hxx>

#include <cmath>

#include <Zeni/Global.h>

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

#include <Zeni/Global_Undef.h>
