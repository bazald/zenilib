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

#include <Zeni/Quaternion.hxx>

#include <cmath>

#include <Zeni/Global.h>

namespace Zeni {

  Quaternion::Quaternion(const float &t, const Vector3f &s)
    : time(t),
    space(s)
  {
  }

  Quaternion Quaternion::Axis_Angle(const Vector3f &v, const float &theta) {
    const float half_theta = 0.5f * theta;

    const float time = cos(half_theta);
    const float space_coeff = sin(half_theta);
    const Vector3f space_vec = v.normalized();

    return Quaternion(time, space_coeff * space_vec).normalized();
  }
  
  bool IsNormal(const Vector3f& v) {
    static const float kNormalTolerance = 1e-05f;
    return fabsf(1.0f - v.magnitude2()) < kNormalTolerance;
  }

  bool IsOrthogonal(const Vector3f& v1, const Vector3f& v2) {
    static const float kOrthogonalTolerance = 1e-05f;
    assert(IsNormal(v1));
    assert(IsNormal(v2));
    return fabsf(v1 * v2) < kOrthogonalTolerance;
  }

  Quaternion Quaternion::Forward_Up(const Vector3f &destination_forward,
                                    const Vector3f &destination_up,
                                    const Vector3f &default_forward,
                                    const Vector3f &default_up)
  {
    assert(IsOrthogonal(destination_forward, destination_up));

    const float kTolerance = 1e-05f;
    Quaternion rotation0;
    // if default and destination are (nearly) identical, don't bother with rot0
    const float dot0 = default_forward * destination_forward;
    if(dot0 < 1.0f - kTolerance)
    {
      Vector3f axis0 = default_forward % destination_forward;
      const float angle0 = default_forward.angle_between(destination_forward);

      // if default and destination are (nearly) opposite, use up vector as axis 
      // (as any orthogonal axis will work)
      if(angle0 > pi - kTolerance)
      {
        axis0 = destination_up;
      }
      rotation0 = Quaternion::Axis_Angle(axis0, angle0);
    }

    const Vector3f intermediate_up = rotation0 * default_up;
    // same as above, this version has fewer conditionals, but does the work even 
    // if the two up vectors are identical.
    Vector3f axis1 = intermediate_up % destination_up;
    const float angle1 = intermediate_up.angle_between(destination_up);
    const float dot1 = intermediate_up * destination_up;
    if(1.0f - fabsf(dot1) < kTolerance) // co-linear
    {
      // this works because destination_up is orthogonal to destination_forward (by assert at top)
      // and intermediate_up is co-linear to destination_up (by dot product test)
      // so destination_forward must be orthogonal to both
      //assert( IsOrthogonal( destination_forward, intermediate_up ) );
      //assert( IsOrthogonal( destination_forward, destination_up ) );
      axis1 = destination_forward;
    }
    const Quaternion rotation1 = Quaternion::Axis_Angle(axis1, angle1);

    return (rotation1 * rotation0).normalized();
  }

  Quaternion::Quaternion(const float &yaw, const float &pitch, const float &roll)
  {
    const float half_yaw = 0.5f * yaw;    const float half_pitch = 0.5f * pitch;
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

  Quaternion::Quaternion(const Quaternion &rhs)
    : time(rhs.time),
    space(rhs.space)
  {
  }

  Quaternion & Quaternion::normalize() {
    float mplier = magnitude();

    if(INFINTESSIMAL(mplier)) {
      assert(false);
      return *this;
    }

    mplier = 1.0f / mplier;

    time *= mplier;
    space *= mplier;

    return *this;
  }

  Quaternion Quaternion::normalized() const {
    float mplier = magnitude();

	if(INFINTESSIMAL(mplier)) {
      assert(false);
      return Quaternion(*this);
    }

    mplier = 1.0f / mplier;

    return Quaternion(time * mplier, space * mplier);
  }

}

#include <Zeni/Global_Undef.h>
