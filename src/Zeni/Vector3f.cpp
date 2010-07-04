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

#include <Zeni/Vector3f.hxx>

#include <cmath>

#include <Zeni/Global.h>

namespace Zeni {

  const float pi = acos(-1.0f);
  const float pi_over_two = acos(-1.0f) / 2.0f;
  const float three_pi_over_two = 3.0f * acos(-1.0f) / 2.0f;
  const float over_three = 1.0f / 3.0f;
  const float sqrt_two = sqrt(2.0f);
  const float sqrt_three = sqrt(3.0f);

  const Vector3f vector_i(1, 0, 0);
  const Vector3f vector_j(0, 1, 0);
  const Vector3f vector_k(0, 0, 1);

  Vector3f & Vector3f::normalize() {
    float mplier = magnitude();

    if(INFINTESSIMAL(mplier)) {
      degenerate = true;
      return *this;
    }
    else
      degenerate = false;

    mplier = 1.0f / mplier;

    i *= mplier;
    j *= mplier;
    k *= mplier;

    return *this;
  }

  Vector3f Vector3f::normalized() const {
    float mplier = magnitude();

    if(INFINTESSIMAL(mplier))
      return Vector3f(*this, true);

    mplier = 1.0f / mplier;

    return Vector3f(i * mplier, j * mplier, k * mplier);
  }

  float Vector3f::theta() const {
    if(i > 0)
      return atan(j/i);
    else if(i < 0)
      return atan(j/i) + pi;
    else if(j > 0)
      return pi_over_two;
    else if(j < 0)
      return three_pi_over_two;
    return 0;
  }

  float Vector3f::phi() const {
    const float xy_mag = sqrt(pow(i, 2) + pow(j, 2));

    if(xy_mag > 0.0f)
      return pi_over_two + atan(-k / xy_mag);

    if(k < 0.0f)
      return pi;

    return 0.0f;
  }

  void Vector3f::set_spherical(const float &theta, const float &phi, const float &magnitude) {
    i = sin(phi) * magnitude;
    j = sin(theta) * i;
    i *= cos(theta);
    k = cos(phi) * magnitude;
    degenerate = false;
  }
}

#include <Zeni/Global_Undef.h>
