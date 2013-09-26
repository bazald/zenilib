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

#ifndef ZENI_IMAGE_HXX
#define ZENI_IMAGE_HXX

#include <Zeni/Image.h>

namespace Zeni {

  Image::Color_Space Image::color_space() const {
    return m_color_space;
  }

  const Uint8 * Image::get_data() const {
    return m_data.data();
  }

  Uint8 * Image::get_data() {
    return m_data.data();
  }

  const Point2i & Image::size() const {
    return m_size;
  }

  int Image::width() const {
    return m_size.x;
  }

  int Image::height() const {
    return m_size.y;
  }

  bool Image::tileable() const {
    return m_tileable;
  }

}

#endif
