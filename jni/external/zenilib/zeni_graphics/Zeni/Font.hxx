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

#ifndef ZENI_FONT_HXX
#define ZENI_FONT_HXX

#include <Zeni/Font.h>

namespace Zeni {
  
  const String & Font::get_name() const {
    return m_font_name;
  }

  float Font::get_text_height() const {
    return m_glyph_height;
  }

  float Font::get_virtual_screen_height() const {
    return m_virtual_screen_height;
  }

  float Font_FT::Glyph::get_glyph_width() const {
    return m_glyph_width;
  }

}

#endif
