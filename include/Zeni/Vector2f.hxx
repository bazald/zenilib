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

#ifndef ZENI_VECTOR2F_HXX
#define ZENI_VECTOR2F_HXX

#include <Zeni/Vector2f.h>

// HXXed below
#include <Zeni/Coordinate.h>
#include <Zeni/Vector3f.h>

// Not HXXed
#include <cassert>
#include <cmath>

namespace Zeni {

  Vector2f::Vector2f(const bool &degenerate_)
    : i(0.0f), j(0.0f), degenerate(degenerate_)
  {
  }

  Vector2f::Vector2f(const float &i_, const float &j_, const bool &degenerate_)
    : i(i_), j(j_), degenerate(degenerate_)
  {
  }

  Vector2f::Vector2f(const Vector2f &rhs, const bool &degenerate_)
    : i(rhs.i), j(rhs.j), degenerate(rhs.degenerate || degenerate_)
  {
  }
  
  Vector2f::Vector2f(const Point2f &rhs)
  : i(rhs.x), j(rhs.y)
  {
  }
  
  Vector2f::Vector2f(const Vector3f &rhs)
  : i(rhs.i), j(rhs.j)
  {
  }

  Vector2f Vector2f::operator+(const Vector2f &rhs) const {
    return Vector2f(i + rhs.i,
      j + rhs.j,
      degenerate || rhs.degenerate);
  }

  Vector2f Vector2f::operator-(const Vector2f &rhs) const {
    return Vector2f(i - rhs.i,
      j - rhs.j,
      degenerate || rhs.degenerate);
  }

  Vector2f & Vector2f::operator+=(const Vector2f &rhs) {
    i += rhs.i;
    j += rhs.j;
    degenerate |= rhs.degenerate;
    return *this;
  }

  Vector2f & Vector2f::operator-=(const Vector2f &rhs) {
    i -= rhs.i;
    j -= rhs.j;
    degenerate |= rhs.degenerate;
    return *this;
  }

  float Vector2f::operator*(const Vector2f &rhs) const {
    return
      i * rhs.i +
      j * rhs.j;
  }

  Vector2f Vector2f::operator*(const float &rhs) const {
    return Vector2f(i * rhs, j * rhs, degenerate);
  }

  Vector2f Vector2f::operator/(const float &rhs) const {
    return Vector2f(i / rhs, j / rhs, degenerate);
  }

  Vector2f & Vector2f::operator*=(const float &rhs) {
    i *= rhs;
    j *= rhs;
    return *this;
  }

  Vector2f & Vector2f::operator/=(const float &rhs) {
    i /= rhs;
    j /= rhs;
    return *this;
  }

  Vector2f Vector2f::operator-() const {
    return *this * -1;
  }

  // Vector Scalar Multiplication Part II of II
  Vector2f operator*(const float &lhs, const Vector2f &rhs) {
    return rhs * lhs;
  }

  float Vector2f::magnitude2() const {
    return i * i + j * j;
  }

  float Vector2f::magnitude() const {
    return float(sqrt(magnitude2()));
  }

  Vector2f Vector2f::get_i() const {
    return Vector2f(i, 0.0f);
  }

  Vector2f Vector2f::get_j() const {
    return Vector2f(0.0f, j);
  }

  Vector2f Vector2f::multiply_by(const Vector2f &rhs) const {
    return Vector2f(i*rhs.i, j*rhs.j, degenerate || rhs.degenerate);
  }

  Vector2f Vector2f::divide_by(const Vector2f &rhs) const {
    return Vector2f(i/rhs.i, j/rhs.j, degenerate || rhs.degenerate);
  }

  float Vector2f::angle_between(const Vector2f &rhs) const {
    const float a = magnitude();
    const float b = rhs.magnitude();
    const float c = (rhs - *this).magnitude();

    return float(acos((a * a + b * b - c * c) / (2 * a * b)));
  }
  
  const float & Vector2f::operator[](const int &index) const {
    assert(-1 < index && index < 2);
    const float * const ptr = &i;
    return ptr[index];
  }
   
  float & Vector2f::operator[](const int &index) {
    assert(-1 < index && index < 2);
    float * const ptr = &i;
    return ptr[index];
  }

}

#include <Zeni/Coordinate.hxx>
#include <Zeni/Vector3f.hxx>

#endif
