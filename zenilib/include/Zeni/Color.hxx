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

#ifndef ZENI_COLOR_HXX
#define ZENI_COLOR_HXX

#include <Zeni/Color.h>

#include <cassert>

namespace Zeni {

  unsigned char Color::r_ub() const {
    return static_cast<unsigned char>(r*0xFF);
  }

  unsigned char Color::g_ub() const {
    return static_cast<unsigned char>(g*0xFF);
  }

  unsigned char Color::b_ub() const {
    return static_cast<unsigned char>(b*0xFF);
  }

  unsigned char Color::a_ub() const {
    return static_cast<unsigned char>(a*0xFF);
  }

  Uint32 Color::get_rgba() const {
    return Uint32(r_ub() << 24) | Uint32(g_ub() << 16) | Uint32(b_ub() << 8) | Uint32(a_ub());
  }

  Uint32 Color::get_argb() const {
    return Uint32(a_ub() << 24) | Uint32(r_ub() << 16) | Uint32(g_ub() << 8) | Uint32(b_ub());
  }

  Uint32 Color::get_bgra() const {
    return Uint32(b_ub() << 24) | Uint32(g_ub() << 16) | Uint32(r_ub() << 8) | Uint32(a_ub());
  }

  const float & Color::operator[](const int &index) const {
    assert(-1 < index && index < 4);
    const float * const ptr = &r;
    return ptr[index];
  }

  float & Color::operator[](const int &index) {
    assert(-1 < index && index < 4);
    float * const ptr = &r;
    return ptr[index];
  }

}

#endif
