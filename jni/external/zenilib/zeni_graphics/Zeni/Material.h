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
 * \class Zeni::Material
 *
 * \ingroup zenilib
 *
 * \brief An Abstraction of a Material
 *
 * The Light class simply provides Material data to both Direct3D9 and OpenGL.
 *
 * \note Materials are relevant only when dynamic lighting is used.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_MATERIAL_H
#define ZENI_MATERIAL_H

#include <Zeni/Color.h>
#include <Zeni/String.h>

#ifndef DISABLE_GL
#if defined(REQUIRE_GL_ES)
#include <GLES/gl.h>
#else
#include <GL/glew.h>
#endif
#endif

#include <Zeni/Define.h>

namespace Zeni {

  class ZENI_GRAPHICS_DLL Video_GL;
  class ZENI_GRAPHICS_DLL Video_DX9;

  class ZENI_GRAPHICS_DLL Material {
  public:
    /// An Alternative to the numerous setter functions
    Material(const Color &ambient = ZENI_DEFAULT_MATERIAL_DIFFUSE, 
             const Color &diffuse = ZENI_DEFAULT_MATERIAL_DIFFUSE, 
             const Color &specular = ZENI_DEFAULT_MATERIAL_SPECULAR, 
             const Color &emissive = ZENI_DEFAULT_MATERIAL_EMISSIVE,
             const float &power = ZENI_DEFAULT_MATERIAL_POWER,
             const String &texture = "");
    Material(const String &texture,
             const Color &ambient_and_diffuse = ZENI_DEFAULT_MATERIAL_DIFFUSE);

    // Accessors
    inline float get_power() const; ///< Get the power of the Material (indicates the focus of the specular highlights)
    float get_shininess() const; ///< Get the shininess of the Material (indicates the focus of the specular highlights - logarithmically tied to power)
    inline const String & get_Texture() const; ///< Get the texture identifier

    // Modifiers
    inline void set_power(const float &power); ///< Set the power of the Material (indicates the focus of the specular highlights)
    void set_shininess(const float &shininess); ///< Set the shininess of the Material (indicates the focus of the specular highlights - logarithmically tied to power)
    void set_Texture(const String &texture); ///< Set the texture identifier

#ifndef DISABLE_GL
    void set(Video_GL &screen) const;
    void unset(Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    void set(Video_DX9 &screen) const;
    void unset(Video_DX9 &screen) const;
#endif

    bool operator<(const Material &rhs) const; ///< To provide an arbitrary total ordering. Do not depend on it remaining the same in the future.

    bool operator==(const Material &rhs) const; ///< A simple equality test. Close hits are misses.

    // Optimizers
    void optimize_to_follow(const Material &rhs); ///< If this Material will be set regularly after another Material, and only after that other Material, the set function can be optimized.
    void optimize_to_precede(const Material &rhs); ///< If this Material will be set regularly before another Material, and only before that other Material, the unset function can be optimized.
    void clear_optimization(); ///< Simply undo any previous optimizations.

    Color diffuse; ///< The diffuse Color
    Color ambient; ///< The ambient Color
    Color specular; ///< The specular Color
    Color emissive; ///< The emissive Color

  private:
    float m_power;

    String m_texture;
    mutable unsigned long m_texture_id;

    unsigned int m_optimization;
  };

}

#include <Zeni/Undefine.h>

#endif
