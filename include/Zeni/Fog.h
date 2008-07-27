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

/**
 * \class Zeni::Fog
 *
 * \ingroup Zenilib
 *
 * \brief An Abstraction of Fog
 *
 * The Fog class simply provides Fog data to both Direct3D9 and OpenGL.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_FOG_H
#define ZENI_FOG_H

#include <Zeni/Color.h>

#ifndef DISABLE_GL
#include <GL/gl.h>
#endif
#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif

namespace Zeni {

  class Video_GL;
  class Video_DX9;

  enum FOG_TYPE {FOG_LINEAR = 1, FOG_EXP = 2, FOG_EXP2 = 3};

  class Fog {
  public:
    /// An Alternative to the numerous setter functions
    Fog(const Color &color_ = Color(1.0f, 1.0f, 1.0f, 1.0f), 
        const float &density_ = 0.01f,
        const FOG_TYPE &type_ = FOG_EXP,
        const float &start_ = 10.0f,
        const float &end_ = 1000.0f);

    // Accessors
    inline FOG_TYPE get_type() const; ///< Get the type of the Fog
    inline const Color & get_color() const; ///< Get the Color
    inline const float & get_density() const; ///< Get the fog density
    inline const float & get_start() const; ///< Get the start of the Fog
    inline const float & get_end() const; ///< Get the end of the Fog

    // Modifiers
    inline void set_type(const FOG_TYPE &type_); ///< Set the type of the Fog
    inline void set_color(const Color &color_); ///< Set the Color
    inline void set_density(const float &density_); ///< Set the fog density
    inline void set_start(const float &start_); ///< Set the start of the Fog
    inline void set_end(const float &end_); ///< Set the end of the Fog

#ifndef DISABLE_GL
    void set(Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    void set(Video_DX9 &screen) const;
#endif

  private:
    FOG_TYPE m_type;
    Color m_color;
    float m_density; // used when (FOGTYPE == FOG_EXP || FOGTYPE == FOG_EXP2)
    float m_start;   // used when FOGTYPE == FOG_LINEAR
    float m_end;     // used when FOGTYPE == FOG_LINEAR
  };

}

#endif

#ifdef ZENI_INLINES
#include <Zeni/Fog.hxx>
#endif
