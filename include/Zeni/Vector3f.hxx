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

#ifndef ZENI_VECTOR3F_HXX
#define ZENI_VECTOR3F_HXX

#include "Vector3f.h"

#include "Coordinate.hxx"

#include <cmath>

namespace Zeni {

  Vector3f Vector3f::operator+(const Vector3f &rhs) const {
    return Vector3f(i + rhs.i,
      j + rhs.j,
      k + rhs.k);
  }

  Vector3f Vector3f::operator-(const Vector3f &rhs) const {
    return Vector3f(i - rhs.i,
      j - rhs.j,
      k - rhs.k);
  }

  Vector3f & Vector3f::operator+=(const Vector3f &rhs) {
    i += rhs.i;
    j += rhs.j;
    k += rhs.k;
    return *this;
  }

  Vector3f & Vector3f::operator-=(const Vector3f &rhs) {
    i -= rhs.i;
    j -= rhs.j;
    k -= rhs.k;
    return *this;
  }

  float Vector3f::operator*(const Vector3f &rhs) const {
    return
      i * rhs.i +
      j * rhs.j +
      k * rhs.k;
  }

  Vector3f Vector3f::operator%(const Vector3f &rhs) const {
    return Vector3f(j * rhs.k - rhs.j *k,
      rhs.i *k - i * rhs.k,
      i * rhs.j - rhs.i *j);
  }

  Vector3f & Vector3f::operator%=(const Vector3f &rhs) {
    return *this = *this % rhs;
  }

  Vector3f Vector3f::operator*(const float &rhs) const {
    return Vector3f(i * rhs, j * rhs, k * rhs);
  }

  Vector3f Vector3f::operator/(const float &rhs) const {
    return Vector3f(i / rhs, j / rhs, k / rhs);
  }

  Vector3f & Vector3f::operator*=(const float &rhs) {
    i *= rhs;
    j *= rhs;
    k *= rhs;
    return *this;
  }

  Vector3f & Vector3f::operator/=(const float &rhs) {
    i /= rhs;
    j /= rhs;
    k /= rhs;
    return *this;
  }

  Vector3f Vector3f::operator-() const {
    return *this * -1;
  }

  // Vector Scalar Multiplication Part II of II
  Vector3f operator*(const float &lhs, const Vector3f &rhs) {
    return rhs * lhs;
  }

  // Vector Addition / Subtraction to a Point
  Point3f operator+(const Point3f &lhs, const Vector3f &rhs) {
    return Point3f(lhs.x + rhs.i, lhs.y + rhs.j, lhs.z + rhs.k);
  }

  Point3f & operator+=(Point3f &lhs, const Vector3f &rhs) {
    lhs.x += rhs.i;
    lhs.y += rhs.j;
    lhs.z += rhs.k;
    return lhs;
  }

  Point3f operator-(const Point3f &lhs, const Vector3f &rhs) {
    return Point3f(lhs.x - rhs.i, lhs.y - rhs.j, lhs.z - rhs.k);
  }

  Point3f & operator-=(Point3f &lhs, const Vector3f &rhs) {
    lhs.x -= rhs.i;
    lhs.y -= rhs.j;
    lhs.z -= rhs.k;
    return lhs;
  }

  float Vector3f::magnitude() const {
    return sqrt(pow(i, 2) + pow(j, 2) + pow(k, 2));
  }

}

#endif
