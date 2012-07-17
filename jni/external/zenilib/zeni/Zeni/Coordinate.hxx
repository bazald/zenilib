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

#ifndef ZENI_COORDINATE_HXX
#define ZENI_COORDINATE_HXX

#include <Zeni/Coordinate.h>

// HXXed below
#include <Zeni/Vector2f.h>
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

  Point2f::Point2f(const Vector2f &rhs)
  : x(rhs.i), y(rhs.j)
  {
  }

  Vector2f Point2f::operator-(const Point2f &rhs) const {
    return Vector2f(x - rhs.x, y - rhs.y);
  }

  Point2f Point2f::operator+(const Vector2f &rhs) const {
    return Point2f(x + rhs.i, y + rhs.j);
  }

  Point2f Point2f::operator-(const Vector2f &rhs) const {
    return Point2f(x - rhs.i, y - rhs.j);
  }

  Point2f & Point2f::operator+=(const Vector2f &rhs) {
    x += rhs.i;
    y += rhs.j;
    return *this;
  }

  Point2f & Point2f::operator-=(const Vector2f &rhs) {
    x -= rhs.i;
    y -= rhs.j;
    return *this;
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

#include <Zeni/Vector2f.hxx>
#include <Zeni/Vector3f.hxx>

#endif
