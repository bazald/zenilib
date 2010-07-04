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
 * \struct Zeni::Fog
 *
 * \ingroup zenilib
 *
 * \brief An Abstraction of Fog
 *
 * The Fog struct simply provides Fog data to both Direct3D9 and OpenGL.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_FOG_H
#define ZENI_FOG_H

#include <Zeni/Color.h>

#ifndef DISABLE_GL
#ifdef _MACOSX
#include <GLEW/glew.h>
#else
#include <GL/glew.h>
#endif
#endif

#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif

#include <Zeni/Global.h>

namespace Zeni {

  class Video_GL;
  class Video_DX9;

  enum FOG_TYPE {FOG_LINEAR = 1, FOG_EXP = 2, FOG_EXP2 = 3};

  struct Fog {
    Fog(const Color &color_ = ZENI_DEFAULT_FOG_COLOR, 
        const float &density_ = ZENI_DEFAULT_FOG_DENSITY,
        const FOG_TYPE &type_ = ZENI_DEFAULT_FOG_TYPE,
        const float &start_ = ZENI_DEFAULT_FOG_START,
        const float &end_ = ZENI_DEFAULT_FOG_END);

#ifndef DISABLE_GL
    void set(Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    void set(Video_DX9 &screen) const;
#endif

    FOG_TYPE type; ///< The type of the Fog
    Color color;   ///< Color muxed with the rendering
    float density; ///< used when (FOGTYPE == FOG_EXP || FOGTYPE == FOG_EXP2)
    float start;   ///< used when FOGTYPE == FOG_LINEAR
    float end;     ///< used when FOGTYPE == FOG_LINEAR
  };

}

#include <Zeni/Global_Undef.h>

#endif
