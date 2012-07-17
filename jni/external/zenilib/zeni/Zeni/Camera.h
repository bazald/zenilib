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

/**
 * \class Zeni::Camera
 *
 * \ingroup zenilib
 *
 * \brief Camera / Point of View
 *
 * This class describes a viewpoint for a scene.  Its use is similar in 
 * concept to that of an actual camera.  You take time to position a 
 * camera before you shoot (render) a scene.
 *
 * \note Tell the rendering engine to use a camera with a call to get_Video().set_3d(...);
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_CAMERA_H
#define ZENI_CAMERA_H

#include <Zeni/Collision.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Vector3f.h>
#include <Zeni/Matrix4f.h>
#include <Zeni/Quaternion.h>

#include <Zeni/Define.h>

namespace Zeni {

  class ZENI_DLL Camera {
  public:
    /// The Camera constructor is an alternative to using the numerous setter functions.
    Camera(const Point3f &position = ZENI_DEFAULT_CAMERA_POSITION,
           const Quaternion &orientation = ZENI_DEFAULT_CAMERA_ORIENTATION,
           const float &near_clip = ZENI_DEFAULT_CAMERA_NEAR_CLIP,
           const float &far_clip = ZENI_DEFAULT_CAMERA_FAR_CLIP,
           const float &fov_rad_ = ZENI_DEFAULT_CAMERA_FOV,
           const float &tunnel_vision_factor = ZENI_DEFAULT_CAMERA_TUNNEL_VISION);

    // Accessors
    inline Vector3f get_forward() const; ///< Get the vector indicating the direction in which the camera is pointing.
    inline Vector3f get_up() const; ///< Get the vector indicating what is "up" from the perspective of the camera.
    inline Vector3f get_left() const; ///< Get the vector indicating what is "left" from the perspective of the camera.
    inline float get_fov_deg() const; ///< Get the field of view (in the y-axis) in degrees.
    inline float get_tunnel_vision_factor() const; ///< Get the tunnel vision factor a.k.a. how far to pull back the focal point, scaling the near distance and keeping all else equal.

    // Modifiers
    inline void set_fov_deg(const float &degrees); ///< Set the field of view (in the y-axis) in degrees.

    // Convenience Functions

    inline Point3f get_tunneled_position() const; ///< Get the position shifted by tunnel vision
    inline float get_tunneled_near_clip() const; ///< Get the near clip shifted by tunnel vision
    inline float get_tunneled_far_clip() const; ///< Get the far clip shifted by tunnel vision
    inline float get_tunneled_fov_deg() const; ///< Get the field of view (in the y-axis) in degrees, shifted by tunnel vision
    inline float get_tunneled_fov_rad() const; ///< Get the field of view (in the y-axis) in radians, shifted by tunnel vision
    inline Matrix4f get_view_matrix() const; ///< Equivalent to gluLookAt + tunnel_vision_factor
    inline Matrix4f get_projection_matrix(const std::pair<Point2i, Point2i> &viewport) const; ///< Equivalent to gluPerspective + tunnel_vision_factor

    void adjust_yaw(const float &theta); ///< Adjust the orientation of the camera: left == positive;
    void adjust_pitch(const float &phi); ///< Adjust the orientation of the camera: up == positive;
    void adjust_roll(const float &rho);  ///< Adjust the orientation of the camera: spin right == positive;

    void move_forward_xy(const float &distance); ///< Move the camera in the forward direction, projected onto the xy-plane.
    void move_left_xy(const float &distance); ///< Move the camera in the left direction, projected onto the xy-plane.
    void turn_left_xy(const float &theta); ///< Turn the camera left about the z-axis.

    void look_at(const Point3f &world_coord, const Vector3f &horizon_plane_normal = ZENI_DEFAULT_UP_VECTOR); ///< Set the Camera to look at a specific Point3f while maintaining a horizontal horizon for a Plane with the given normal Vector3f.
    void look_at(const Point3f &world_coord, const Collision::Plane &horizon_plane); ///< Set the Camera to look at a specific Point3f while maintaining a horizontal horizon for the given Plane.

    Point3f position;
    Quaternion orientation; // relative to forward(1.0f, 0.0f, 0.0f), up(0.0f, 0.0f, 1.0f)

    float near_clip; // should never be less than 1.0f, and higher values will be better still
    float far_clip; // should always be higher than near_clip
    float fov_rad; // should be in range (0.0f, pi)

    float tunnel_vision_factor; // 1.0f is default, higher values will decrease the field-of-view, while keeping the near clip view the same
  };

}

#include <Zeni/Undefine.h>

#endif
