/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
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

namespace Zeni {

  Camera::Camera(const Point3f &position, const Vector3f &forward, const Vector3f &up, const float &near_clip, const float &far_clip, const float &fov_rad_, const float &tunnel_vision_factor)
    : m_position(position),
    m_forward(forward.normalized()),
    m_up(up.normalized()),
    m_near_clip(near_clip),
    m_far_clip(far_clip),
    m_fov_rad(fov_rad_),
    m_tunnel_vision_factor(tunnel_vision_factor)
  {
  }

  void Camera::adjust_yaw(const float &theta) {
    m_forward = (Quaternion(get_up(), theta) * m_forward).normalized();
    // left changes implicitly

	  // anti-parallelism correction
	  m_up = (m_up - (m_forward * m_up) * m_up).normalized();
  }

  void Camera::adjust_pitch(const float &phi) {
    const Quaternion rot(get_left().normalized(), phi);
    m_forward = (rot * m_forward).normalized();
    m_up = (rot * m_up).normalized();

	  // anti-parallelism correction
	  m_up = (m_up - (m_forward * m_up) * m_up).normalized();
  }

  void Camera::adjust_roll(const float &rho) {
    m_up = (Quaternion(get_forward(), rho) * m_up).normalized();
    // left changes implicitly

	  // anti-parallelism correction
	  m_up = (m_up - (m_forward * m_up) * m_up).normalized();
  }

  void Camera::move_forward_xy(const float &distance) {
    m_position += Vector3f(m_up.k * m_forward.i, m_up.k * m_forward.j, 0.0f).normalized() * distance;
  }

  void Camera::move_left_xy(const float &distance) {
    Vector3f xy_left = get_left();
    m_position += Vector3f(xy_left.i, xy_left.j, 0.0f).normalized() * distance;
  }

  void Camera::turn_left_xy(const float &theta) {
    const Quaternion rot(Vector3f(0.0f, 0.0f, 1.0f), theta);
    m_forward = (rot * m_forward).normalized();
    m_up = (rot * m_up).normalized();

	  // anti-parallelism correction
	  m_up = (m_up - (m_forward * m_up) * m_up).normalized();
  }

  void Camera::look_at(const Point3f &world_coord, const Vector3f &horizon_plane_normal) {
    m_forward = (world_coord - m_position).normalized();
    const Vector3f left = (horizon_plane_normal % m_forward).normalized();
    m_up = (m_forward % left).normalized(); 
  }

  void Camera::look_at(const Point3f &world_coord, const Zeni_Collision::Plane &horizon_plane) {
    look_at(world_coord, horizon_plane.get_normal());
  }

}
