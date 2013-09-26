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

namespace Zeni {

  Point2i Point2i::interpolate_to(const float &rhs_part, const Point2i &rhs) const {
    const float lhs_part = 1.0f - rhs_part;
    return Point2i(int(lhs_part * x + rhs_part * rhs.x), 
                   int(lhs_part * y + rhs_part * rhs.y));
  }

  Point2f Point2f::interpolate_to(const float &rhs_part, const Point2f &rhs) const {
    const float lhs_part = 1.0f - rhs_part;
    return Point2f(lhs_part * x + rhs_part * rhs.x, 
                   lhs_part * y + rhs_part * rhs.y);
  }

  Point3i Point3i::interpolate_to(const float &rhs_part, const Point3i &rhs) const {
    const float lhs_part = 1.0f - rhs_part;
    return Point3i(int(lhs_part * x + rhs_part * rhs.x), 
                   int(lhs_part * y + rhs_part * rhs.y), 
                   int(lhs_part * z + rhs_part * rhs.z));
  }

  Point3f Point3f::interpolate_to(const float &rhs_part, const Point3f &rhs) const {
    const float lhs_part = 1.0f - rhs_part;
    return Point3f(lhs_part * x + rhs_part * rhs.x,
                   lhs_part * y + rhs_part * rhs.y,
                   lhs_part * z + rhs_part * rhs.z);
  }

  const int Point2i::z = 0;
  const float Point2f::z = 0;

  std::ostream & serialize(std::ostream &os, const Point2i &value) {
    return serialize(serialize(os, value.x), value.y);
  }
  
  std::ostream & serialize(std::ostream &os, const Point2f &value) {
    return serialize(serialize(os, value.x), value.y);
  }
  
  std::ostream & serialize(std::ostream &os, const Point3i &value) {
    return serialize(serialize(serialize(os, value.x), value.y), value.z);
  }
  
  std::ostream & serialize(std::ostream &os, const Point3f &value) {
    return serialize(serialize(serialize(os, value.x), value.y), value.z);
  }
  
  std::istream & unserialize(std::istream &is, Point2i &value) {
    return unserialize(unserialize(is, value.x), value.y);
  }
  
  std::istream & unserialize(std::istream &is, Point2f &value) {
    return unserialize(unserialize(is, value.x), value.y);
  }
  
  std::istream & unserialize(std::istream &is, Point3i &value) {
    return unserialize(unserialize(unserialize(is, value.x), value.y), value.z);
  }
  
  std::istream & unserialize(std::istream &is, Point3f &value) {
    return unserialize(unserialize(unserialize(is, value.x), value.y), value.z);
  }

}
