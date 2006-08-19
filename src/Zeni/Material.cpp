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

#include <Zeni/Material.h>

#include <Zeni/Video.hxx>
#include <Zeni/Video_DX9.hxx>

#include <cmath>

#ifndef DISABLE_GL
#include <GL/gl.h>
#endif

namespace Zeni {

  Material::Material(const Color &ambient, const Color &diffuse, const Color &specular, const Color &emissive, const float &power)
    : m_diffuse(diffuse),
    m_ambient(ambient),
    m_specular(specular), 
    m_emissive(emissive),
    m_power(power)
  {
  }

  float Material::get_shininess() const {
    return 0.1f * log(m_power)/log(2.0f);
  }

  void Material::set_shininess(const float &shininess) {
    m_power = pow(2, 10.0f * shininess);
    if(m_power > 128.0f)
      m_power = 128.0f;
  }

#ifndef DISABLE_GL
  void Material::set(Video_GL &) const {
    GLenum face = Video::get_reference().get_backface_culling() ? GL_FRONT : GL_FRONT_AND_BACK;

    glMaterialfv(face, GL_AMBIENT, reinterpret_cast<const GLfloat *>(&m_ambient));
    glMaterialfv(face, GL_DIFFUSE, reinterpret_cast<const GLfloat *>(&m_diffuse));
    glMaterialfv(face, GL_SPECULAR, reinterpret_cast<const GLfloat *>(&m_specular));
    glMaterialfv(face, GL_EMISSION, reinterpret_cast<const GLfloat *>(&m_emissive));
    glMaterialfv(face, GL_SHININESS, &m_power);
  }
#endif

#ifndef DISABLE_DX9
  void Material::set(Video_DX9 &screen) const {
    screen.get_d3d_device()->SetMaterial(reinterpret_cast<const D3DMATERIAL9 *>(this));
  }
#endif

  bool Material::operator<(const Material &rhs) const {
    return m_diffuse < rhs.m_diffuse ||
      m_ambient < rhs.m_ambient ||
      m_specular < rhs.m_specular ||
      m_emissive < rhs.m_emissive ||
      m_power < rhs.m_power;
  }

  bool Material::operator==(const Material &rhs) const {
    return m_diffuse == rhs.m_diffuse &&
      m_ambient == rhs.m_ambient &&
      m_specular == rhs.m_specular &&
      m_emissive == rhs.m_emissive &&
      m_power == rhs.m_power;
  }

}
