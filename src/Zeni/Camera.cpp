/* This file is part of the Zenipex Library.
* Copyleft (C) 2006 Mitchell Keith Bloch a.k.a. bazald
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
#include <Zeni/Coordinate.hxx>
#include <Zeni/Vector3f.hxx>

#include <cmath>

namespace Zeni {

  Camera::Camera(const Point3f &position, const Vector3f &forward, const Vector3f &up, const float &near_clip, const float &far_clip, const float &fov_rad_)
    : m_position(position),
    m_forward(forward),
    m_up(up),
    m_near_clip(near_clip),
    m_far_clip(far_clip),
    m_fov_rad(fov_rad_)
  {
  }

  void Camera::adjust_yaw(const float &theta) {
    m_forward = cos(theta) * m_forward + sin(theta) * get_left();
  }

  void Camera::adjust_pitch(const float &phi) {
    float c = cos(phi), s = sin(phi);
    Vector3f up = m_up;
    m_up = c * m_up - s * m_forward;
    m_forward = c * m_forward + s * up;
  }

  void Camera::adjust_roll(const float &rho) {
    m_up = cos(rho) * m_up + sin(rho) * get_left();
  }

  void Camera::move_forward_xy(const float &distance) {
    m_position += Vector3f(m_up.k * m_forward.i, m_up.k * m_forward.j, 0.0f).normalized() * distance;
  }

  void Camera::move_left_xy(const float &distance) {
    Vector3f xy_left = get_left();
    m_position += Vector3f(xy_left.i, xy_left.j, 0.0f).normalized() * distance;
  }

  void Camera::turn_left_xy(const float &theta) {
    float c = cos(theta), s = sin(theta);

    Vector3f
      xy_forward(m_forward.i, m_forward.j, 0.0f),
      xy_up(m_up.i, m_up.j, 0.0f);

    xy_forward = c * xy_forward + s * (vector_k % xy_forward);
    xy_up = c * xy_up + s * (vector_k % xy_up);

    m_forward = Vector3f(xy_forward.i, xy_forward.j, m_forward.k);
    m_up = Vector3f(xy_up.i, xy_up.j, m_up.k);
  }

}
