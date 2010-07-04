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

#include <Zeni/Quaternion.hxx>

#include <cmath>

#include <Zeni/Global.h>

namespace Zeni {

  Quaternion::Quaternion(const float &t, const Vector3f &s, const bool &degenerate_)
    : time(t),
    space(s),
    degenerate(s.degenerate || degenerate_)
  {
  }

  Quaternion Quaternion::Axis_Angle(const Vector3f &v, const float &theta) {
    const float half_theta = 0.5f * theta;

    const float time = cos(half_theta);
    const float space_coeff = sin(half_theta);
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
        rotation0 = Quaternion::Axis_Angle(default_up, pi);
    }

    const Vector3f intermediate_up = rotation0 * default_up;
    const Vector3f axis1 = intermediate_up % destination_up;
    const float angle1 = intermediate_up.angle_between(destination_up);

    Quaternion rotation1 = Quaternion::Axis_Angle(axis1, angle1);
    if(rotation1.degenerate) {
      if(INFINTESSIMAL(angle1))
        rotation1 = Quaternion();
      else
        rotation1 = Quaternion::Axis_Angle(destination_forward, pi);
    }

    return (rotation1 * rotation0).normalized();
  }

  Quaternion::Quaternion(const float &yaw, const float &pitch, const float &roll, const bool &degenerate_)
    : degenerate(degenerate_)
  {
    const float half_yaw = 0.5f * yaw;
    const float half_pitch = 0.5f * pitch;
    const float half_roll = 0.5f * roll;
   
	  const float shy = sin(half_yaw);
	  const float shp = sin(half_pitch);
	  const float shr = sin(half_roll);
	  const float chy = cos(half_yaw);
	  const float chp = cos(half_pitch);
	  const float chr = cos(half_roll);
   
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

}

#include <Zeni/Global_Undef.h>
