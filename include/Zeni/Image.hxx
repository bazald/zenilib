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

  Image::Image(const String &filename, const bool &tileable_)
    : m_surface_ptr(IMG_Load(filename.c_str())),
    m_tileable(tileable_)
  {
    if(!m_surface_ptr ||
      !m_surface_ptr->format)
      throw Image_Init_Failure();

    init();
  }

  Image::Image(SDL_Surface * surface_ptr, const bool &tileable_)
    : m_surface_ptr(surface_ptr),
    m_tileable(tileable_)
  {
    if(!m_surface_ptr ||
      !m_surface_ptr->format)
      throw Image_Init_Failure();

    init();
  }

  Image::~Image() {
    SDL_FreeSurface(m_surface_ptr);
  }

  Image::Image(const Image &rhs)
    : m_surface_ptr(SDL_ConvertSurface(rhs.m_surface_ptr,
                                       rhs.m_surface_ptr->format,
                                       rhs.m_surface_ptr->flags)),
      m_tileable(rhs.m_tileable),
      m_max_alpha(rhs.m_max_alpha),
      m_max_red(rhs.m_max_red),
      m_max_green(rhs.m_max_green),
      m_max_blue(rhs.m_max_blue)
  {
    if(!m_surface_ptr ||
       !m_surface_ptr->format)
      throw Image_Init_Failure();
  }

  Image & Image::operator=(const Image &rhs) {
    Image temp(rhs);

    std::swap(m_surface_ptr, temp.m_surface_ptr);
    std::swap(m_tileable, temp.m_tileable);
    std::swap(m_max_alpha, temp.m_max_alpha);
    std::swap(m_max_red, temp.m_max_red);
    std::swap(m_max_green, temp.m_max_green);
    std::swap(m_max_blue, temp.m_max_blue);

    return *this;
  }

  SDL_Surface & Image::get_surface() {
    return *m_surface_ptr;
  }

  const int & Image::width() const {
    return m_surface_ptr->w;
  }

  const int & Image::height() const {
    return m_surface_ptr->h;
  }

  const bool & Image::tileable() const {
    return m_tileable;
  }

}

#endif
