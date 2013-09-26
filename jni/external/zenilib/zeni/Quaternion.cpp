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

  Quaternion::Quaternion(const float &t, const Vector3f &s, const bool &degenerate_)
    : time(t),
    space(s),
    degenerate(s.degenerate || degenerate_)
  {
  }

  Quaternion Quaternion::Vector3f_to_Vector3f(const Vector3f &destination, const Vector3f &source) {
    const Vector3f axis = source % destination;
    const float angle = source.angle_between(destination);
    Quaternion rotation = Quaternion::Axis_Angle(axis, angle);

    if(rotation.degenerate) {
      if(INFINTESSIMAL(angle))
        return Quaternion();
      else {
        const Vector3f pos(fabsf(source.i), fabsf(source.j), fabsf(source.k));

        if(pos.i < pos.j && pos.i < pos.k)
          rotation = Quaternion::Axis_Angle(source % Vector3f(1.0f, 0.0f, 0.0f), Global::pi);
        else if(pos.j < pos.k)
          rotation = Quaternion::Axis_Angle(source % Vector3f(0.0f, 1.0f, 0.0f), Global::pi);
        else
          rotation = Quaternion::Axis_Angle(source % Vector3f(0.0f, 0.0f, 1.0f), Global::pi);
      }
    }

    return rotation.normalized();
  }

  Quaternion Quaternion::Axis_Angle(const Vector3f &v, const float &theta) {
    const float half_theta = 0.5f * theta;

    const float time = float(cos(half_theta));
    const float space_coeff = float(sin(half_theta));
    const Vector3f space_vec = v.normalized();

    return Quaternion(time, space_coeff * space_vec, space_vec.degenerate).normalized();
  }
  
  Quaternion Quaternion::Forward_Up(const Vector3f &destination_forward,
                                    const Vector3f &destination_up,
                                    const Vector3f &default_forward,
                                    const Vector3f &default_up)
  {
    const Vector3f axis0 = default_forward % destination_forward;
    const float angle0 = default_forward.angle_between(destination_forward);

    Quaternion rotation0 = Quaternion::Axis_Angle(axis0, angle0);
    if(rotation0.degenerate) {
      if(INFINTESSIMAL(angle0))
        rotation0 = Quaternion();
      else
        rotation0 = Quaternion::Axis_Angle(default_up, Global::pi);
    }

    const Vector3f intermediate_up = rotation0 * default_up;
    const Vector3f axis1 = intermediate_up % destination_up;
    const float angle1 = intermediate_up.angle_between(destination_up);

    Quaternion rotation1 = Quaternion::Axis_Angle(axis1, angle1);
    if(rotation1.degenerate) {
      if(INFINTESSIMAL(angle1))
        rotation1 = Quaternion();
      else
        rotation1 = Quaternion::Axis_Angle(destination_forward, Global::pi);
    }

    return (rotation1 * rotation0).normalized();
  }

  Quaternion::Quaternion(const float &yaw, const float &pitch, const float &roll, const bool &degenerate_)
    : degenerate(degenerate_)
  {
    const float half_yaw = 0.5f * yaw;
    const float half_pitch = 0.5f * pitch;
    const float half_roll = 0.5f * roll;
   
	  const float shy = float(sin(half_yaw));
	  const float shp = float(sin(half_pitch));
	  const float shr = float(sin(half_roll));
	  const float chy = float(cos(half_yaw));
	  const float chp = float(cos(half_pitch));
	  const float chr = float(cos(half_roll));
   
	  time = chr * chp * chy + shr * shp * shy;
	  space.i = shr * chp * chy - chr * shp * shy;
	  space.j = chr * shp * chy + shr * chp * shy;
	  space.k = chr * chp * shy - shr * shp * chy;
  }

  Quaternion::Quaternion(const Quaternion &rhs, const bool &degenerate_)
    : time(rhs.time),
    space(rhs.space),
    degenerate(rhs.degenerate || degenerate_)
  {
  }

  Quaternion & Quaternion::normalize() {
    float mplier = magnitude();

    if(INFINTESSIMAL(mplier)) {
      degenerate = true;
      return *this;
    }

    mplier = 1.0f / mplier;

    time *= mplier;
    space *= mplier;

    return *this;
  }

  Quaternion Quaternion::normalized() const {
    float mplier = magnitude();

    if(INFINTESSIMAL(mplier))
      return Quaternion(*this, true);

    mplier = 1.0f / mplier;

    return Quaternion(time * mplier, space * mplier);
  }

  std::ostream & serialize(std::ostream &os, const Quaternion &value) {
    return serialize(serialize(os, value.time), value.space);
  }

  std::istream & unserialize(std::istream &is, Quaternion &value) {
    return unserialize(unserialize(is, value.time), value.space);
  }

}

#include <Zeni/Undefine.h>
