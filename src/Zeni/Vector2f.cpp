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

  Vector2f & Vector2f::normalize() {
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

    return *this;
  }

  Vector2f Vector2f::normalized() const {
    float mplier = magnitude();

    if(INFINTESSIMAL(mplier))
      return Vector2f(*this, true);

    mplier = 1.0f / mplier;

    return Vector2f(i * mplier, j * mplier);
  }

  float Vector2f::theta() const {
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

  void Vector2f::set_spherical(const float &theta, const float &magnitude) {
    i = magnitude;
    j = float(sin(theta)) * i;
    i *= float(cos(theta));
    degenerate = false;
  }

  std::ostream & serialize(std::ostream &os, const Vector2f &value) {
    return serialize(serialize(os, value.i), value.j);
  }
  
  std::istream & unserialize(std::istream &is, Vector2f &value) {
    return unserialize(unserialize(is, value.i), value.j);
  }

}

#include <Zeni/Undefine.h>
