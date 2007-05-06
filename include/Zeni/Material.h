/* This file is part of the Zenipex Library.
* Copyleft (C) 2006 Mitchell Keith Bloch a.k.a. bazald
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
 * \ingroup Zenilib
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

#include "Color.h"
#include "Texture.h"

#include <string>

#ifndef DISABLE_GL
#include <GL/gl.h>
#endif
#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif

namespace Zeni {

  class Video_GL;
  class Video_DX9;

  class Material {
  public:
    /// An Alternative to the numerous setter functions
    Material(const Color &ambient = Color(1.0f, 1.0f, 1.0f, 1.0f), 
      const Color &diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f), 
      const Color &specular = Color(0.2f, 0.2f, 0.2f, 1.0f), 
      const Color &emissive = Color(0.0f, 0.0f, 0.0f, 1.0f), const float &power = 1.0f,
      const std::string &texture = "");
    Material(const std::string &texture);

    // Accessors
    inline const Color & get_ambient() const; ///< Get the ambient Color
    inline const Color & get_diffuse() const; ///< Get the diffuse Color
    inline const Color & get_specular() const; ///< Get the specular Color
    inline const Color & get_emissive() const; ///< Get the emissive Color
    inline float get_power() const; ///< Get the power of the Material (indicates the focus of the specular highlights)
    float get_shininess() const; ///< Get the shininess of the Material (indicates the focus of the specular highlights - logarithmically tied to power)
    inline const std::string & get_texture() const; ///< Get the texture identifier

    // Modifiers
    inline void set_ambient(const Color &ambient); ///< Set the ambient Color
    inline void set_diffuse(const Color &diffuse); ///< Set the diffuse Color
    inline void set_specular(const Color &specular); ///< Set the specular Color
    inline void set_emissive(const Color &emissive); ///< Set the emissive Color
    inline void set_power(const float &power); ///< Set the power of the Material (indicates the focus of the specular highlights)
    void set_shininess(const float &shininess); ///< Set the shininess of the Material (indicates the focus of the specular highlights - logarithmically tied to power)
    inline void set_texture(const std::string &texture); ///< Set the texture identifier

#ifndef DISABLE_GL
    void set(Video_GL &screen, const int &optimization) const;
    void unset(Video_GL &screen, const int &optimization) const;
#endif

#ifndef DISABLE_DX9
    void set(Video_DX9 &screen, const int &optimization) const;
    void unset(Video_DX9 &screen, const int &optimization) const;
#endif

    bool operator<(const Material &rhs) const; ///< To provide an arbitrary total ordering. Do not depend on it remaining the same in the future.

    bool operator==(const Material &rhs) const; ///< A simple equality test. Close hits are misses.

  private:
    Color m_diffuse;
    Color m_ambient;
    Color m_specular;
    Color m_emissive;
    float m_power;
    std::string m_texture;
  };

}

#ifdef ZENI_INLINES
#include "Material.hxx"
#endif

#endif
