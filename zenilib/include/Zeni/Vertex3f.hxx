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

#ifndef ZENI_VERTEX3F_HXX
#define ZENI_VERTEX3F_HXX

#include <Zeni/Vertex3f.h>

namespace Zeni {

  void * Vertex3f::get_address() const {
    return const_cast<Point3f *>(&position);
  }

#if defined(_WINDOWS) && defined(X64)
  unsigned long long Vertex3f::get_offset() const {
    return reinterpret_cast<unsigned long long>(&position) - reinterpret_cast<unsigned long long>(this);
  }
#else
  unsigned long Vertex3f::get_offset() const {
    return reinterpret_cast<unsigned long>(&position) - reinterpret_cast<unsigned long>(this);
  }
#endif

  const Uint32 & Vertex3f_Color::get_Color() const {
    return m_argb;
  }

  void Vertex3f_Color::set_Color(const Color &color) {
    m_argb = color.get_argb();
  }

  void Vertex3f_Color::set_Color(const Uint32 &argb) {
    m_argb = argb;
  }

}

#endif
