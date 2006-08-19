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

/**
 * \class Zeni::Camera
 *
 * \ingroup Zenilib
 *
 * \brief Camera / Point of View
 *
 * This class describes a viewpoint for a scene.  Its use is similar in 
 * concept to that of an actual camera.  You take time to position a 
 * camera before you shoot (render) a scene.
 *
 * \note Tell the rendering engine to use a camera with a call to Video::get_reference().set_3d();
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_CAMERA_H
#define ZENI_CAMERA_H

#include "Coordinate.hxx"
#include "Vector3f.h"

namespace Zeni {

  class Camera {
  public:
    /// The Camera constructor is an alternative to using the numerous setter functions. 
    Camera(const Point3f &position = Point3f(0, 0, 0), const Vector3f &forward = Vector3f(1, 0, 0), 
      const Vector3f &up = Vector3f(0, 0, 1), const float &near_clip = 10.0f, 
      const float &far_clip = 1000.0f, const float &fov_rad_ = pi/2);

    // Accessor
    inline const Point3f & get_position() const; ///< Get the current position of the camera.
    inline const Vector3f & get_forward() const; ///< Get the vector indicating the direction in which the camera is pointing.
    inline const Vector3f & get_up() const; ///< Get the vector indicating what is "up" from the perspective of the camera.
    inline Vector3f get_left() const; ///< Get the vector indicating what is "left" from the perspective of the camera.
    inline float get_near_clip() const; ///< Get the near clipping plane.
    inline float get_far_clip() const; ///< Get the far clipping plane.
    inline float get_fov_deg() const; ///< Get the field of view (in the y-axis) in degrees.
    inline float get_fov_rad() const; ///< Get the field of view (in the y-axis) in radians.

    // Modifiers
    inline void set_position(const Point3f &point); ///< Set the current position of the camera.
    inline void set_position(const float &x, const float &y, const float &z); ///< Set the vector indicating the direction in which the camera is pointing.
    inline void set_forward(const Vector3f &forward); ///< Set the vector indicating what is "forward" from the perspective of the camera.
    inline void set_forward(const float &i, const float &j, const float &k); ///< Set the vector indicating what is "forward" from the perspective of the camera.
    inline void set_up(const Vector3f &up); ///< Set the vector indicating what is "up" from the perspective of the camera.
    inline void set_up(const float &i, const float &j, const float &k); ///< Set the vector indicating what is "up" from the perspective of the camera.
    inline void set_near_clip(const float &distance); ///< Set the near clipping plane.  Must be non-zero.  Should be greater than or equal to 1.0f
    inline void set_far_clip(const float &distance); ///< Set the far clipping plane.  Must not equal the near-clipping value.
    inline void set_fov_deg(const float &degrees); ///< Set the field of view (in the y-axis) in degrees.
    inline void set_fov_rad(const float &radians); ///< Set the field of view (in the y-axis) in radians.

    inline void adjust_position(const Vector3f &by); ///< Adjust the position of the camera using a vector.
    inline void adjust_position(const float &i, const float &j, const float &k); ///< Adjust the position of the camera using ijk values.

    void adjust_yaw(const float &theta); ///< Adjust the orientation of the camera: left == positive;
    void adjust_pitch(const float &phi); ///< Adjust the orientation of the camera: down == positive;
    void adjust_roll(const float &rho);  ///< Adjust the orientation of the camera: spin left == positive;

    void move_forward_xy(const float &distance); ///< Move the camera in the forward direction, projected onto the xy-plane.
    void move_left_xy(const float &distance); ///< Move the camera in the left direction, projected onto the xy-plane.
    void turn_left_xy(const float &theta); ///< Turn the camera left about the z-axis.

  private:
    Point3f m_position;
    Vector3f m_forward, m_up;
    float m_near_clip, m_far_clip, m_fov_rad;
  };

}

#ifdef ZENI_INLINES
#include "Camera.hxx"
#endif

#endif
