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

/**
 * \class Zeni::Image
 *
 * \ingroup zenilib
 *
 * \brief Image
 *
 * This class describes a image, loaded from a file.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_IMAGE_H
#define ZENI_IMAGE_H

#include <Zeni/Color.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Core.h>
#include <Zeni/String.h>

/* \cond */
#ifdef _MACOSX
#include <SDL_image/SDL_image.h>
#else
#include <SDL/SDL_image.h>
#endif
/* \endcond */

namespace Zeni {

  class ZENI_GRAPHICS_DLL Image {
  public:
    inline Image(const String &filename, const bool &tileable_ = false);
    inline Image(SDL_Surface * surface_ptr, const bool &tileable_ = false); ///< Create an Image from surface_ptr, giving the Image ownership of surface_ptr
    inline ~Image();
    
    inline Image(const Image &rhs);
    inline Image & operator=(const Image &rhs);

    inline SDL_Surface & get_surface(); ///< Get access to the SDL_Surface.
    inline const int & width() const; ///< Get the number of pixels in the image in the x-direction.
    inline const int & height() const; ///< Get the number of pixels in the image in the y-direction.
    inline const bool & tileable() const; ///< Determine if the given Image is tileable.

    void set_Color(const Point2i &pixel, const Color &color); ///< Set the Color value of the pixel specified [0, 0] being the upper left, (width, height) being the lower right.
    Color extract_Color(const Point2i &pixel) const; ///< Get the Color value of the pixel specified [0, 0] being the upper left, (width, height) being the lower right.

    Color extract_Color(const Point2f &coordinate) const; ///< Get the Color value of a given coordinate, [0.0f, 0.0f] to (1.0f, 1.0f), with wrapping if (tileable == true).

    void resize(const int &width, const int &height);

    SDL_Surface * get_copy() const; ///< Get a copy of the SDL_Surface

  private:
    void init();
    Uint32 & get_pixel(const Point2i &pixel) const;

    SDL_Surface * m_surface_ptr;
    bool m_tileable;

    Uint32 m_max_alpha;
    Uint32 m_max_red;
    Uint32 m_max_green;
    Uint32 m_max_blue;
  };

  struct ZENI_GRAPHICS_DLL Image_Init_Failure : public Error {
    Image_Init_Failure() : Error("Zeni Image Failed to Initialize Correctly") {}
  };

  struct ZENI_GRAPHICS_DLL Image_Index_Error : public Error {
    Image_Index_Error() : Error("Zeni Image Indexed Out of Bounds") {}
  };

}

#endif
