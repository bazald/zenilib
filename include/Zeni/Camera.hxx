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

#ifndef ZENI_CAMERA_HXX
#define ZENI_CAMERA_HXX

// HXXed below
#include <Zeni/Coordinate.h>
#include <Zeni/Matrix4f.h>

#include <Zeni/Camera.h>
#include <cmath>

namespace Zeni {

  const Point3f & Camera::get_position() const {
    return m_position;
  }

  const Vector3f & Camera::get_forward() const {
    return m_forward;
  }

  const Vector3f & Camera::get_up() const {
    return m_up;
  }

  Vector3f Camera::get_left() const {
    return m_up % m_forward;
  }

  float Camera::get_near_clip() const {
    return m_near_clip;
  }

  float Camera::get_far_clip() const {
    return m_far_clip;
  }

  float Camera::get_fov_deg() const {
    return m_fov_rad * 180.0f / pi;
  }

  float Camera::get_fov_rad() const {
    return m_fov_rad;
  }

  float Camera::get_tunnel_vision_factor() const {
    return m_tunnel_vision_factor;
  }

  void Camera::set_position(const Point3f &point) {
    m_position = point;
  }

  void Camera::set_forward(const Vector3f &forward) {
    m_forward = forward.normalized();
  }

  void Camera::set_up(const Vector3f &up) {
    m_up = up.normalized();
  }

  void Camera::set_near_clip(const float &distance) {
    m_near_clip = distance;
  }

  void Camera::set_far_clip(const float &distance) {
    m_far_clip = distance;
  }

  void Camera::set_fov_deg(const float &degrees) {
    m_fov_rad = degrees * pi / 180.0f;
  }

  void Camera::set_fov_rad(const float &radians) {
    m_fov_rad = radians;
  }

  void Camera::set_tunnel_vision_factor(const float &tunnel_vision_factor) {
    m_tunnel_vision_factor = tunnel_vision_factor;
  }

  Point3f Camera::get_tunneled_position() const {
    return m_position + (1.0f - m_tunnel_vision_factor) * m_near_clip * m_forward;
  }

  float Camera::get_tunneled_near_clip() const {
    return m_near_clip * m_tunnel_vision_factor;
  }

  float Camera::get_tunneled_far_clip() const {
    return m_far_clip - m_near_clip + get_tunneled_near_clip();
  }

  float Camera::get_tunneled_fov_deg() const {
    return get_tunneled_fov_rad() * 180.0f / pi;
  }

  float Camera::get_tunneled_fov_rad() const {
    return 2.0f * std::atan((m_near_clip * std::tan(0.5f * m_fov_rad)) / get_tunneled_near_clip());
  }

  Matrix4f Camera::get_view_matrix() const {
    return Matrix4f::View(get_tunneled_position(), m_forward, m_up);
  }

  Matrix4f Camera::get_projection_matrix(const std::pair<Point2i, Point2i> &viewport) const {
    return Matrix4f::Perspective(get_tunneled_fov_rad(),
      float(viewport.second.x - viewport.first.x) / (viewport.second.y - viewport.first.y),
      get_tunneled_near_clip(), get_tunneled_far_clip());
  }

  void Camera::adjust_position(const Vector3f &by) {
    m_position += by;
  }

}

#include <Zeni/Coordinate.hxx>
#include <Zeni/Matrix4f.hxx>

#endif
