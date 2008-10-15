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

#include <Zeni/Color.h>

namespace Zeni {

  Color::Color() {
    rgba[0] = 1.0f;
    rgba[1] = 1.0f;
    rgba[2] = 1.0f;
    rgba[3] = 1.0f;
  }

  Color::Color(const float &a, const float &r, const float &g, const float &b) {
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
  }

  Color::Color(const unsigned long &argb) {
    rgba[0] = ((argb >> 16) & 0xFF) / 256.0f;
    rgba[1] = ((argb >> 8) & 0xFF) / 256.0f;
    rgba[2] = (argb & 0xFF) / 256.0f;
    rgba[3] = ((argb >> 24) & 0xFF) / 256.0f;
  }

  Color Color::interpolate_to(const float &rhs_part, const Color &rhs) const {
    float lhs_part = 1.0f - rhs_part;
    return Color(lhs_part * rgba[1] + rhs_part * rhs.rgba[1], 
                 lhs_part * rgba[2] + rhs_part * rhs.rgba[2], 
                 lhs_part * rgba[3] + rhs_part * rhs.rgba[3],
                 lhs_part * rgba[0] + rhs_part * rhs.rgba[0]);
  }

  bool Color::operator<(const Color &rhs) const {
    return rgba[0] < rhs.rgba[0] || rgba[0] == rhs.rgba[0] &&
      (rgba[1] < rhs.rgba[1] || rgba[1] == rhs.rgba[1] &&
      (rgba[2] < rhs.rgba[2] || rgba[2] == rhs.rgba[2] &&
      rgba[3] < rhs.rgba[3]));
  }

  bool Color::operator==(const Color &rhs) const {
    return rgba[0] == rhs.rgba[0] &&
      rgba[1] == rhs.rgba[1] &&
      rgba[2] == rhs.rgba[2] &&
      rgba[3] == rhs.rgba[3];
  }

}
