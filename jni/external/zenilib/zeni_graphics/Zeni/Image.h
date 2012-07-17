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
#ifndef ANDROID
#ifdef _MACOSX
#include <SDL/SDL.h>
#else
#include <SDL/SDL.h>
#endif
#endif
/* \endcond */

namespace Zeni {

  class ZENI_GRAPHICS_DLL Image {
  public:
    enum Color_Space {Luminance, Luminance_Alpha, RGB, RGBA};

    Image();
    Image(const String &filename, const bool &tileable_ = false);
    Image(const Point2i &size_, const Color_Space &color_space_, const bool &tileable_ = false);

    inline Color_Space color_space() const; ///< Determine the Color_Space of the raw image data.
    inline const Uint8 * get_data() const; ///< Get access to the raw image data.
    inline Uint8 * get_data(); ///< Get access to the raw image data.
    inline const Point2i & size() const; ///< Get the dimensions of the image.
    inline int width() const; ///< Get the number of pixels in the image in the x-direction.
    inline int height() const; ///< Get the number of pixels in the image in the y-direction.
    inline bool tileable() const; ///< Determine if the given Image is tileable.

    void set_Color(const Point2i &pixel, const Color &color); ///< Set the Color value of the pixel specified [0, 0] being the upper left, (width, height) being the lower right.
    void set_RGBA(const Point2i &pixel, const Uint32 &rgba); ///< Set the Color value of the pixel specified [0, 0] being the upper left, (width, height) being the lower right.
    Color extract_Color(const Point2i &pixel) const; ///< Get the Color value of the pixel specified [0, 0] being the upper left, (width, height) being the lower right.
    Uint32 extract_RGBA(const Point2i &pixel) const; ///< Get the Color value of the pixel specified [0, 0] being the upper left, (width, height) being the lower right.

    Color extract_Color(const Point2f &coordinate) const; ///< Get the Color value of a given coordinate, [0.0f, 0.0f] to (1.0f, 1.0f), with wrapping if (tileable == true).

    void resize(const int &width, const int &height);
    bool blit(const Point2i &upper_left, const Image &source); ///< Copy a different Image in the same color-space into this Image. Returns true if blits successfully, false otherwise.

  private:
    const Uint8 * get_pixel(const Point2i &pixel) const;
    Uint8 * get_pixel(const Point2i &pixel);

    Point2i m_size;
    Color_Space m_color_space;

    int m_bytes_per_pixel;
    int m_row_size;

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::vector<Uint8> m_data;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

    bool m_tileable;
  };

  struct ZENI_GRAPHICS_DLL Image_Init_Failure : public Error {
    Image_Init_Failure() : Error("Zeni Image Failed to Initialize Correctly") {}
  };

  struct ZENI_GRAPHICS_DLL Image_Index_Error : public Error {
    Image_Index_Error() : Error("Zeni Image Indexed Out of Bounds") {}
  };

}

#endif
