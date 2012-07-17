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

  namespace Global {
    const float pi = float(acos(-1.0f));
    const float pi_over_two = float(acos(-1.0f)) / 2.0f;
    const float three_pi_over_two = 3.0f * float(acos(-1.0f)) / 2.0f;
    const float over_three = 1.0f / 3.0f;
    const float sqrt_two = float(sqrt(2.0f));
    const float sqrt_three = float(sqrt(3.0f));

    const Vector3f vector_i(1, 0, 0);
    const Vector3f vector_j(0, 1, 0);
    const Vector3f vector_k(0, 0, 1);
  }

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
      return float(atan(j/i));
    else if(i < 0)
      return float(atan(j/i)) + Global::pi;
    else if(j > 0)
      return Global::pi_over_two;
    else if(j < 0)
      return Global::three_pi_over_two;
    return 0;
  }

  float Vector3f::phi() const {
    const float xy_mag = float(sqrt(pow(i, 2) + pow(j, 2)));

    if(xy_mag > 0.0f)
      return Global::pi_over_two + float(atan(-k / xy_mag));

    if(k < 0.0f)
      return Global::pi;

    return 0.0f;
  }

  void Vector3f::set_spherical(const float &theta, const float &phi, const float &magnitude) {
    i = float(sin(phi)) * magnitude;
    j = float(sin(theta)) * i;
    i *= float(cos(theta));
    k = float(cos(phi)) * magnitude;
    degenerate = false;
  }
  
  std::ostream & serialize(std::ostream &os, const Vector3f &value) {
    return serialize(serialize(serialize(os, value.i), value.j), value.k);
  }
  
  std::istream & unserialize(std::istream &is, Vector3f &value) {
    return unserialize(unserialize(unserialize(is, value.i), value.j), value.k);
  }

}

#include <Zeni/Undefine.h>
