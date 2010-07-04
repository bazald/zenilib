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

#ifndef ZENI_QUATERNION_HXX
#define ZENI_QUATERNION_HXX

// HXXed below
#include <Zeni/Coordinate.h>
#include <Zeni/Vector3f.h>

#include <Zeni/Quaternion.h>

// Not HXXed
#include <cassert>
#include <cmath>

namespace Zeni {

  Quaternion::Quaternion(const bool &degenerate_)
    : time(1.0f),
    space(0.0f, 0.0f, 0.0f),
    degenerate(degenerate_)
  {
  }

  Quaternion Quaternion::operator+(const Quaternion &rhs) const {
    return Quaternion(time + rhs.time, space + rhs.space, degenerate || rhs.degenerate);
	}

	Quaternion Quaternion::operator-(const Quaternion &rhs) const {
    return Quaternion(time - rhs.time, space - rhs.space, degenerate || rhs.degenerate);
	}

	Quaternion & Quaternion::operator+=(const Quaternion &rhs) {
    time += rhs.time;
    space += rhs.space;
    degenerate |= rhs.degenerate;
    return *this;
	}

	Quaternion & Quaternion::operator-=(const Quaternion &rhs) {
    time -= rhs.time;
    space -= rhs.space;
    degenerate |= rhs.degenerate;
    return *this;
	}

  Quaternion Quaternion::operator*(const Quaternion &rhs) const {
    return grassman_product(rhs);
  }

  Quaternion Quaternion::operator*=(const Quaternion &rhs) {
    return *this = grassman_product(rhs);
  }

  Quaternion Quaternion::grassman_product(const Quaternion &rhs) const {
    return Quaternion(time * rhs.time - space * rhs.space, time * rhs.space + rhs.time * space + space % rhs.space, degenerate || rhs.degenerate);
  }

  Quaternion Quaternion::grassman_even_product(const Quaternion &rhs) const {
    return Quaternion(time * rhs.time - space * rhs.space, time * rhs.space + space * rhs.time, degenerate || rhs.degenerate);
  }

  Quaternion Quaternion::grassman_odd_product(const Quaternion &rhs) const {
    return Quaternion(0.0f, space % rhs.space, degenerate || rhs.degenerate);
  }

  Quaternion Quaternion::euclidean_product(const Quaternion &rhs) const {
    return Quaternion(time * rhs.time + space * rhs.space, time * rhs.space - space * rhs.time - space % rhs.space, degenerate || rhs.degenerate);
  }

  Quaternion Quaternion::euclidean_even_product(const Quaternion &rhs) const {
    return Quaternion(time * rhs.time + space * rhs.space, Vector3f(), degenerate || rhs.degenerate);
  }

  Quaternion Quaternion::euclidean_odd_product(const Quaternion &rhs) const {
    return Quaternion(0.0f, time * rhs.space - space * rhs.time - space % rhs.space, degenerate || rhs.degenerate);
  }

  Quaternion Quaternion::operator*(const float &rhs) const {
    return Quaternion(time * rhs, space * rhs, degenerate);
  }

  Quaternion Quaternion::operator/(const float &rhs) const {
    return Quaternion(time / rhs, space / rhs, degenerate);
  }

  Quaternion & Quaternion::operator*=(const float &rhs) {
    time *= rhs;
    space *= rhs;
    return *this;
  }

  Quaternion & Quaternion::operator/=(const float &rhs) {
    time /= rhs;
    space /= rhs;
    return *this;
  }

  Quaternion Quaternion::operator-() const {
    return Quaternion(time * -1, space * -1, degenerate);
  }

  float Quaternion::magnitude2() const {
    return time * time + space * space;
  }

  float Quaternion::magnitude() const {
    return sqrt(magnitude2());
  }

  Quaternion Quaternion::conjugate() const {
    return Quaternion(time, -space, degenerate);
  }

  Quaternion Quaternion::reciprocal() const {
    Quaternion star = conjugate();
    return star / (*this * star).time;
  }

  Quaternion Quaternion::absolute_value() const {
    return Quaternion(magnitude(), Vector3f(), degenerate);
  }

  Quaternion Quaternion::norm() const {
    return Quaternion(magnitude2(), Vector3f(), degenerate);
  }

  Quaternion Quaternion::determinant() const {
    const float temp = magnitude2();
    return Quaternion(temp * temp, Vector3f(), degenerate);
  }

  Quaternion Quaternion::adjoint() const {
    return conjugate() * norm();
  }

  Vector3f Quaternion::operator*(const Vector3f &rhs) const {
    const float &a = time;
    const float &b = space.i;
    const float &c = space.j;
    const float &d = space.k;
    const float &v1 = rhs.i;
    const float &v2 = rhs.j;
    const float &v3 = rhs.k;
    
    // http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
    // dependent on Quaternion being pre-normalized
    
    const float t2 = a * b;
    const float t3 = a * c;
    const float t4 = a * d;
    const float t5 = -b * b;
    const float t6 = b * c;
    const float t7 = b * d;
    const float t8 = -c * c;
    const float t9 = c * d;
    const float t10 = -d * d;
    
    const float v1new = 2.0f * ((t8 + t10) * v1 + (t6 -  t4) * v2 + (t3 + t7) * v3) + v1;
    const float v2new = 2.0f * ((t4 + t6) * v1 + (t5 + t10) * v2 + (t9 - t2) * v3) + v2;
    const float v3new = 2.0f * ((t7 - t3) * v1 + (t2 +  t9) * v2 + (t5 + t8) * v3) + v3;
    
    return Vector3f(v1new, v2new, v3new, degenerate || rhs.degenerate);
  }

  std::pair<Vector3f, float> Quaternion::get_rotation() const {
    Quaternion q = normalized();

    if(q.time < -1.0f)
      q.time = -1.0f;
    else if(q.time > 1.0f)
      q.time = 1.0f;

    const float angle = 2.0f * acos(q.time);
    float s = sqrt(1.0f - q.time * q.time);

    if(fabs(s) < 0.001f)
      return std::make_pair(q.space, angle);
    
    return std::make_pair(q.space / s, angle);
  }

  Matrix4f Quaternion::get_matrix() const {
    const float x2 = space.i * space.i;
    const float y2 = space.j * space.j;
    const float z2 = space.k * space.k;
    const float xy = space.i * space.j;
    const float xz = space.i * space.k;
    const float yz = space.j * space.k;
    const float wx = time * space.i;
    const float wy = time * space.j;
    const float wz = time * space.k;
   
    return Matrix4f(
      1.0f - 2.0f * (y2 + z2), 
      2.0f * (xy - wz), 
      2.0f * (xz + wy), 
      0.0f,

      2.0f * (xy + wz), 
      1.0f - 2.0f * (x2 + z2), 
      2.0f * (yz - wx), 
      0.0f,

      2.0f * (xz - wy), 
      2.0f * (yz + wx), 
      1.0f - 2.0f * (x2 + y2), 
      0.0f,

      0.0f, 0.0f, 0.0f, 1.0f);
  }

  const float & Quaternion::operator[](const int &index) const {
    assert(-1 < index && index < 4);
    const float * const ptr = &time;
    return ptr[index];
  }

  float & Quaternion::operator[](const int &index) {
    assert(-1 < index && index < 4);
    float * const ptr = &time;
    return ptr[index];
  }

}

#include <Zeni/Coordinate.hxx>
#include <Zeni/Vector3f.hxx>

#endif
