/* This file is part of the Zenipex Library.
* Copyleft (C) 2007 Mitchell Keith Bloch a.k.a. bazald
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

#include <Zeni/Quaternion.h>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Vector3f.hxx>

#include <cmath>

namespace Zeni {

  Quaternion Quaternion::operator+(const Quaternion &rhs) const {
    return Quaternion(time + rhs.time, space + rhs.space);
	}

	Quaternion Quaternion::operator-(const Quaternion &rhs) const {
    return Quaternion(time - rhs.time, space - rhs.space);
	}

	Quaternion & Quaternion::operator+=(const Quaternion &rhs) {
    time += rhs.time;
    space += rhs.space;
    return *this;
	}

	Quaternion & Quaternion::operator-=(const Quaternion &rhs) {
    time -= rhs.time;
    space -= rhs.space;
    return *this;
	}

  Quaternion Quaternion::operator*(const Quaternion &rhs) const {
    return grassman_product(rhs);
  }

  Quaternion Quaternion::operator*=(const Quaternion &rhs) {
    return *this = grassman_product(rhs);
  }

  Quaternion Quaternion::grassman_product(const Quaternion &rhs) const {
    return Quaternion(time * rhs.time - space * rhs.space, time * rhs.space + rhs.time * space + space % rhs.space);
  }

  Quaternion Quaternion::grassman_even_product(const Quaternion &rhs) const {
    return Quaternion(time * rhs.time - space * rhs.space, time * rhs.space + space * rhs.time);
  }

  Quaternion Quaternion::grassman_odd_product(const Quaternion &rhs) const {
    return Quaternion(0.0f, space % rhs.space);
  }

  Quaternion Quaternion::euclidean_product(const Quaternion &rhs) const {
    return Quaternion(time * rhs.time + space * rhs.space, time * rhs.space - space * rhs.time - space % rhs.space);
  }

  Quaternion Quaternion::euclidean_even_product(const Quaternion &rhs) const {
    return Quaternion(time * rhs.time + space * rhs.space, Vector3f());
  }

  Quaternion Quaternion::euclidean_odd_product(const Quaternion &rhs) const {
    return Quaternion(0.0f, time * rhs.space - space * rhs.time - space % rhs.space);
  }

  Quaternion Quaternion::operator*(const float &rhs) const {
    return Quaternion(time * rhs, space * rhs);
  }

  Quaternion Quaternion::operator/(const float &rhs) const {
    return Quaternion(time / rhs, space / rhs);
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
    return Quaternion(time * -1, space * -1);
  }

  Quaternion Quaternion::conjugate() const {
    return Quaternion(time, -space);
  }

  Quaternion Quaternion::reciprocal() const {
    Quaternion star = conjugate();
    return star / (*this * star).time;
  }

  Quaternion Quaternion::absolute_value() const {
    return Quaternion(sqrt(time * time + space * space));
  }

  Quaternion Quaternion::norm() const {
    return Quaternion(time * time + space * space);
  }

  Quaternion Quaternion::determinant() const {
    float temp = time * time + space * space;
    return Quaternion(temp * temp);
  }

  Quaternion Quaternion::adjoint() const {
    return conjugate() * norm();
  }

  Vector3f Quaternion::operator*(const Vector3f &rhs) const {
    return (*this * (Quaternion(0.0f, rhs.normalized()) * conjugate())).space.normalized() * rhs.magnitude();
  }

  std::pair<Vector3f, float> Quaternion::get_rotation() const {
    float macos = acos(time);
    if(!(macos < 32.0f)) /// Hack & imperfect
      macos = 0.0f;
    return std::make_pair(space.normalized(), 2.0f * macos);
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
		  2.0f * (xy + wz), 
		  2.0f * (xz - wy), 
      0.0f,

      2.0f * (xy - wz), 
      1.0f - 2.0f * (x2 + z2), 
      2.0f * (yz + wx), 
      0.0f,

      2.0f * (xz + wy), 
      2.0f * (yz - wx), 
      1.0f - 2.0f * (x2 + y2), 
      0.0f,

		  0.0f, 0.0f, 0.0f, 1.0f);
  }

}

#endif
