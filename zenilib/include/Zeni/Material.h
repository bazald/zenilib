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

#include <string>

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

  class Material {
  public:
    /// An Alternative to the numerous setter functions
    Material(const Color &ambient = ZENI_DEFAULT_MATERIAL_DIFFUSE, 
             const Color &diffuse = ZENI_DEFAULT_MATERIAL_DIFFUSE, 
             const Color &specular = ZENI_DEFAULT_MATERIAL_SPECULAR, 
             const Color &emissive = ZENI_DEFAULT_MATERIAL_EMISSIVE,
             const float &power = ZENI_DEFAULT_MATERIAL_POWER,
             const std::string &texture = "");
    Material(const std::string &texture,
             const Color &ambient_and_diffuse = ZENI_DEFAULT_MATERIAL_DIFFUSE);

    // Accessors
    inline float get_power() const; ///< Get the power of the Material (indicates the focus of the specular highlights)
    float get_shininess() const; ///< Get the shininess of the Material (indicates the focus of the specular highlights - logarithmically tied to power)
    inline const std::string & get_texture() const; ///< Get the texture identifier

    // Modifiers
    inline void set_power(const float &power); ///< Set the power of the Material (indicates the focus of the specular highlights)
    void set_shininess(const float &shininess); ///< Set the shininess of the Material (indicates the focus of the specular highlights - logarithmically tied to power)
    void set_texture(const std::string &texture); ///< Set the texture identifier

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

    std::string m_texture;
    mutable unsigned long m_texture_id;

    unsigned int m_optimization;
  };

}

#include <Zeni/Global_Undef.h>

#endif
