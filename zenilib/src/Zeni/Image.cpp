/* This file is part of the Zenipex Library.
* Copyleft (C) 2009 Mitchell Keith Bloch a.k.a. bazald
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

#include <Zeni/Image.h>

#include <Zeni/Color.hxx>
#include <Zeni/Coordinate.hxx>

#include <cassert>
#include <cmath>

namespace Zeni {

  Color Image::extract_Color(const Point2i &pixel) const
  {
    assert(0 <= pixel.x && pixel.x < m_surface_ptr->w);
    assert(0 <= pixel.y && pixel.y < m_surface_ptr->h);

    const SDL_PixelFormat * const &format = m_surface_ptr->format;
    assert(format);

    const Uint8 &BytesPerPixel = format->BytesPerPixel;

    unsigned char *row_ptr =
      reinterpret_cast<unsigned char *>(m_surface_ptr->pixels) +
      m_surface_ptr->pitch * pixel.y;
    unsigned char *pixel_ptr = row_ptr + BytesPerPixel * pixel.x;
    const Uint32 pixel_value = *reinterpret_cast<Uint32 *>(pixel_ptr);

    const Uint32 alpha = ((pixel_value & format->Amask) >> format->Ashift) << format->Aloss;
    const Uint32 red   = ((pixel_value & format->Rmask) >> format->Rshift) << format->Rloss;
    const Uint32 green = ((pixel_value & format->Gmask) >> format->Gshift) << format->Gloss;
    const Uint32 blue  = ((pixel_value & format->Bmask) >> format->Bshift) << format->Bloss;

    return Color(m_max_alpha ? (float(alpha) / m_max_alpha) : 1.0f,
                 float(red) / m_max_red,
                 float(green) / m_max_green,
                 float(blue) / m_max_blue);
  }

  Color Image::extract_Color(const Point2f &coordinate) const
  {
    const Point2f scaled(coordinate.x * m_surface_ptr->w,
                         coordinate.y * m_surface_ptr->h);

    Point2i ul(int(scaled.x),
               int(scaled.y));

    if(m_tileable) {
      if(ul.x < 0)
        ul.x = m_surface_ptr->w - ((-ul.x) % m_surface_ptr->w);
      if(ul.x >= m_surface_ptr->w)
        ul.x = ul.x % m_surface_ptr->w;
      
      if(ul.y < 0)
        ul.y = m_surface_ptr->h - ((-ul.y) % m_surface_ptr->h);
      if(ul.y >= m_surface_ptr->h)
        ul.y = ul.y % m_surface_ptr->h;
    }
    else {
      if(ul.x < 0)
        ul.x = 0;
      else if(ul.x >= m_surface_ptr->w)
        ul.x = m_surface_ptr->w - 1;

      if(ul.y < 0)
        ul.y = 0;
      else if(ul.y >= m_surface_ptr->h)
        ul.y = m_surface_ptr->h - 1;
    }

    Point2i lr(ul.x + 1,
               ul.y + 1);

    if(m_tileable) {
      lr.x = lr.x % m_surface_ptr->w;
      lr.y = lr.y % m_surface_ptr->h;
    }
    else {
      if(lr.x == m_surface_ptr->w)
        --lr.x;
      if(lr.y == m_surface_ptr->h)
        --lr.y;
    }

    const Color ulc = extract_Color(ul);
    const Color llc = extract_Color(Point2i(ul.x, lr.y));
    const Color lrc = extract_Color(lr);
    const Color urc = extract_Color(Point2i(lr.x, ul.y));

    const float x_rhs_part = scaled.x - floor(scaled.x);
    const float y_rhs_part = scaled.y - floor(scaled.y);

    const Color uc = ulc.interpolate_to(x_rhs_part, urc);
    const Color lc = llc.interpolate_to(x_rhs_part, lrc);

    return uc.interpolate_to(y_rhs_part, lc);
  }

}
