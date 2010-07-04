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

#ifndef ZENI_COORDINATE_HXX
#define ZENI_COORDINATE_HXX

#include <Zeni/Coordinate.h>

// HXXed below
#include <Zeni/Vector3f.h>

namespace Zeni {

  Point2i::Point2i()
    : x(0), y(0)
  {
  }

  Point2i::Point2i(const int &x_, const int &y_)
    : x(x_), y(y_)
  {
  }

  Point2i::Point2i(const Point2f &rhs)
    : x(int(rhs.x)), y(int(rhs.y))
  {
  }

  Point2i::Point2i(const Point3i &rhs)
    : x(rhs.x), y(rhs.y)
  {
  }

  Point2i::Point2i(const Point3f &rhs)
    : x(int(rhs.x)), y(int(rhs.y))
  {
  }

  Point2f::Point2f()
    : x(0.0f), y(0.0f)
  {
  }

  Point2f::Point2f(const float &x_, const float &y_)
    : x(x_), y(y_)
  {
  }

  Point2f::Point2f(const Point2i &rhs)
    : x(float(rhs.x)), y(float(rhs.y))
  {
  }

  Point2f::Point2f(const Point3i &rhs)
    : x(float(rhs.x)), y(float(rhs.y))
  {
  }

  Point2f::Point2f(const Point3f &rhs)
    : x(rhs.x), y(rhs.y)
  {
  }

  Point3i::Point3i()
    : x(0), y(0), z(0)
  {
  }

  Point3i::Point3i(const int &x_, const int &y_, const int &z_)
    : x(x_), y(y_), z(z_)
  {
  }

  Point3i::Point3i(const Point2i &rhs)
    : x(rhs.x), y(rhs.y), z(0)
  {
  }

  Point3i::Point3i(const Point2f &rhs)
    : x(int(rhs.x)), y(int(rhs.y)), z(0)
  {
  }

  Point3i::Point3i(const Point3f &rhs)
    : x(int(rhs.x)), y(int(rhs.y)), z(int(rhs.z))
  {
  }

  Point3f::Point3f()
    : x(0.0f), y(0.0f), z(0.0f)
  {
  }

  Point3f::Point3f(const float &x_, const float &y_, const float &z_)
    : x(x_), y(y_), z(z_)
  {
  }

  Point3f::Point3f(const Point2i &rhs)
    : x(float(rhs.x)), y(float(rhs.y)), z(0.0f)
  {
  }

  Point3f::Point3f(const Point2f &rhs)
    : x(rhs.x), y(rhs.y), z(0.0f)
  {
  }

  Point3f::Point3f(const Point3i &rhs)
    : x(float(rhs.x)), y(float(rhs.y)), z(float(rhs.z))
  {
  }

  Point3f::Point3f(const Vector3f &rhs)
    : x(rhs.i), y(rhs.j), z(rhs.k)
  {
  }

  Vector3f Point3f::operator-(const Point3f &rhs) const {
    return Vector3f(x - rhs.x, y - rhs.y, z - rhs.z);
  }

  Point3f Point3f::operator+(const Vector3f &rhs) const {
    return Point3f(x + rhs.i, y + rhs.j, z + rhs.k);
  }

  Point3f Point3f::operator-(const Vector3f &rhs) const {
    return Point3f(x - rhs.i, y - rhs.j, z - rhs.k);
  }

  Point3f & Point3f::operator+=(const Vector3f &rhs) {
    x += rhs.i;
    y += rhs.j;
    z += rhs.k;
    return *this;
  }

  Point3f & Point3f::operator-=(const Vector3f &rhs) {
    x -= rhs.i;
    y -= rhs.j;
    z -= rhs.k;
    return *this;
  }

}

#include <Zeni/Vector3f.hxx>

#endif
