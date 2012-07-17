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

#include <Zeni/Define.h>

namespace Zeni {

  class ZENI_GRAPHICS_DLL Video_GL;
  class ZENI_GRAPHICS_DLL Video_DX9;

  enum FOG_TYPE {FOG_LINEAR = 1, FOG_EXP = 2, FOG_EXP2 = 3};

  struct ZENI_GRAPHICS_DLL Fog {
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

#include <Zeni/Undefine.h>

#endif
