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

#include <Zeni/Color.h>

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

}
