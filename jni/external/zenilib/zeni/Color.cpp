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

  Color::Color()
    : r(1.0f),
      g(1.0f),
      b(1.0f),
      a(1.0f)
  {
  }

  Color::Color(const float &a_, const float &r_, const float &g_, const float &b_)
    : r(r_),
      g(g_),
      b(b_),
      a(a_)
  {
  }

  Color::Color(const Uint32 &argb)
    : r(((argb >> 16) & 0xFF) / 256.0f),
      g(((argb >> 8) & 0xFF) / 256.0f),
      b((argb & 0xFF) / 256.0f),
      a(((argb >> 24) & 0xFF) / 256.0f)
  {
  }

  Color Color::interpolate_to(const float &rhs_part, const Color &rhs) const {
    float lhs_part = 1.0f - rhs_part;
    return Color(lhs_part * a + rhs_part * rhs.a, 
                 lhs_part * r + rhs_part * rhs.r, 
                 lhs_part * g + rhs_part * rhs.g,
                 lhs_part * b + rhs_part * rhs.b);
  }

  bool Color::operator<(const Color &rhs) const {
    return a < rhs.a || (a == rhs.a &&
          (r < rhs.r || (r == rhs.r &&
          (g < rhs.g || (g == rhs.g &&
           b < rhs.b)))));
  }

  bool Color::operator==(const Color &rhs) const {
    return a == rhs.a &&
           r == rhs.r &&
           g == rhs.g &&
           b == rhs.b;
  }

  std::ostream & serialize(std::ostream &os, const Color &value) {
    return serialize(serialize(serialize(serialize(os, value.a), value.r), value.g), value.b);
  }

  std::istream & unserialize(std::istream &is, Color &value) {
    return unserialize(unserialize(unserialize(unserialize(is, value.a), value.r), value.g), value.b);
  }

}
