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

#ifndef ZENI_VECTOR2F_HXX
#define ZENI_VECTOR2F_HXX

#include <Zeni/Vector2f.h>

// HXXed Below
#include <Zeni/Vector3f.h>

// Not HXXed
#include <cassert>
#include <cmath>

namespace Zeni {

  Vector2f::Vector2f()
    : i(0.0f), j(0.0f)
  {
  }

  Vector2f::Vector2f(const float &i_, const float &j_)
    : i(i_), j(j_)
  {
  }

  Vector2f::Vector2f(const Vector2f &rhs)
    : i(rhs.i), j(rhs.j)
  {
  }

  Vector2f::Vector2f(const Vector3f &rhs)    
    : i(rhs.i), j(rhs.j)
  {  
  }  

  bool Vector2f::operator==(const Vector2f &rhs) const {
    return i == rhs.i &&
           j == rhs.j;
  }

  bool Vector2f::operator!=(const Vector2f &rhs) const {
    return !(*this == rhs);
  }

  Vector2f Vector2f::operator+(const Vector2f &rhs) const {
    return Vector2f(i + rhs.i,
      j + rhs.j);
  }

  Vector2f Vector2f::operator-(const Vector2f &rhs) const {
    return Vector2f(i - rhs.i,
      j - rhs.j);
  }

  Vector2f & Vector2f::operator+=(const Vector2f &rhs) {
    i += rhs.i;
    j += rhs.j;
    return *this;
  }

  Vector2f & Vector2f::operator-=(const Vector2f &rhs) {
    i -= rhs.i;
    j -= rhs.j;
    return *this;
  }

  Vector2f Vector2f::operator*(const float &rhs) const {
    return Vector2f(i * rhs, j * rhs);
  }

  Vector2f Vector2f::operator/(const float &rhs) const {
    return Vector2f(i / rhs, j / rhs);
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
    return *this * -1.0f;
  }

  // Vector dot product
  float Vector2f::operator*(const Vector2f &rhs) const {
    return
      i * rhs.i +
      j * rhs.j;
  }

  // Vector Scalar Multiplication Part II of II
  Vector2f operator*(const float &lhs, const Vector2f &rhs) {
    return rhs * lhs;
  }

  float Vector2f::magnitude2() const {
    return i * i + j * j;
  }

  float Vector2f::magnitude() const {
    return sqrt(magnitude2());
  }

  Vector2f Vector2f::get_i() const {
    return Vector2f(i, 0.0f);
  }

  Vector2f Vector2f::get_j() const {
    return Vector2f(0.0f, j);
  }

  Vector2f Vector2f::multiply_by(const Vector2f &rhs) const {
    return Vector2f(i*rhs.i, j*rhs.j);
  }

  Vector2f Vector2f::divide_by(const Vector2f &rhs) const {
    return Vector2f(i/rhs.i, j/rhs.j);
  }

  Vector2f Vector2f::interpolate_to(const float &rhs_part, const Vector2f &rhs) const {
    const float lhs_part = 1.0f - rhs_part;
    return lhs_part * *this + rhs_part * rhs;
  }

  float Vector2f::angle_between(const Vector2f &rhs) const {
    float val = normalized() * rhs.normalized(); // 2D dot product a*a + b*b
    val = std::min( val, 1.0f );
    val = std::max( val, -1.0f );
    return acos( val );
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

#include <Zeni/Vector3f.hxx>

#endif
