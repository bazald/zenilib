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

/**
 * \class Zeni::Color
 *
 * \ingroup zenilib
 *
 * \brief Color
 *
 * This class describes a color in ARGB mode.
 *
 * \note Different functions prefer receiving color channels in different types and orders.  Be careful.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_COLOR_H
#define ZENI_COLOR_H

#include <SDL/SDL_stdinc.h>

namespace Zeni {

  struct Color {
    /// Set the red, green, blue, and alpha channels using the stored type, float
    Color(); ///< Default value is full white (opaque)
    Color(const float &a, const float &r, const float &g, const float &b); ///< ARGB order is used for consistency despite interal RGBA ordering
    Color(const Uint32 &argb);

    inline unsigned char a_ub() const; ///< Get the alpha channel [0x00, 0xFF]
    inline unsigned char r_ub() const; ///< Get the red channel [0x00, 0xFF]
    inline unsigned char g_ub() const; ///< Get the green channel [0x00, 0xFF]
    inline unsigned char b_ub() const; ///< Get the blue channel [0x00, 0xFF]

    inline Uint32 get_rgba() const; ///< Get a Uint32 representation of 0xRRGGBBAA
    inline Uint32 get_argb() const; ///< Get a Uint32 representation of 0xAARRGGBB
    inline Uint32 get_bgra() const; ///< Get a Uint32 representation of 0xBBGGRRAA - Endianness swap of rgba

    Color interpolate_to(const float &rhs_part, const Color &rhs) const; ///< Get a color that is inbetween this color and another color.

    bool operator<(const Color &rhs) const; ///< To provide an arbitrary total ordering. Do not depend on it remaining the same in the future.

    bool operator==(const Color &rhs) const; ///< A simple equality test. Close hits are misses.

    // Indexing
    inline const float & operator[](const int &index) const; ///< Get 'index'
    inline float & operator[](const int &index); ///< Get 'index'

    float r;
    float g;
    float b;
    float a;
  };

}

#endif
