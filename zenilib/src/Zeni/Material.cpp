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

#include <Zeni/Material.h>

#include <Zeni/Textures.hxx>
#include <Zeni/Video.hxx>
#include <Zeni/Video_DX9.hxx>
#include <Zeni/Video_GL.h>

#include <cmath>

#ifndef DISABLE_GL
#include <GL/glew.h>
#endif

#include <Zeni/Global.h>

using namespace std;

namespace Zeni {

  Material::Material(const Color &ambient_, const Color &diffuse_, const Color &specular_, const Color &emissive_, const float &power, const string &texture)
    : diffuse(diffuse_),
    ambient(ambient_),
    specular(specular_), 
    emissive(emissive_),
    m_power(power),
    m_texture(""),
    m_texture_id(0)
  {
    set_texture(texture);
  }

  Material::Material(const string &texture, const Color &ambient_and_diffuse)
    : diffuse(ambient_and_diffuse), 
    ambient(ambient_and_diffuse), 
    specular(ZENI_DIFFUSE_TO_SPECULAR(ambient_and_diffuse)), 
    emissive(ZENI_DEFAULT_MATERIAL_EMISSIVE),
    m_power(ZENI_DEFAULT_MATERIAL_POWER),
    m_texture(""),
    m_texture_id(0)
  {
    set_texture(texture);
  }

  float Material::get_shininess() const {
    return 0.1f * log(m_power)/log(2.0f);
  }

  void Material::set_shininess(const float &shininess) {
    m_power = pow(2.0f, 10.0f * shininess);
    if(m_power > 128.0f)
      m_power = 128.0f;
  }

  void Material::set_texture(const std::string &texture) {
    m_texture = texture;
    if(texture.empty())
      m_texture_id = 0;
    else
      m_texture_id = get_Textures().get_texture_id(texture);
  }

#ifndef DISABLE_GL
  void Material::set(Video_GL &vgl, const int &optimization) const {
    if(vgl.get_lighting()) {
      GLenum face = vgl.get_backface_culling() ? GL_FRONT : GL_FRONT_AND_BACK;

      if(!(optimization & (1 << 0)))
        glMaterialfv(face, GL_AMBIENT, reinterpret_cast<const GLfloat *>(&ambient));
      if(!(optimization & (1 << 1)))
        glMaterialfv(face, GL_DIFFUSE, reinterpret_cast<const GLfloat *>(&diffuse));
      if(!(optimization & (1 << 2)))
        glMaterialfv(face, GL_SPECULAR, reinterpret_cast<const GLfloat *>(&specular));
      if(!(optimization & (1 << 3)))
        glMaterialfv(face, GL_EMISSION, reinterpret_cast<const GLfloat *>(&emissive));
      if(!(optimization & (1 << 4)))
        glMaterialfv(face, GL_SHININESS, &m_power);
    }
    else
      vgl.set_color(diffuse);

    if(!(optimization & (1 << 5)) &&
       !m_texture.empty()) {
      try {
        vgl.apply_texture(m_texture_id);
      }
      catch(Texture_Not_Found &) {
        m_texture_id = get_Textures().get_texture_id(m_texture);
        if(!m_texture_id)
          throw;
        vgl.apply_texture(m_texture_id);
      }
    }
  }

  void Material::unset(Video_GL &vgl, const int &optimization) const {
    if(!(optimization & (1 << 11)) &&
       !m_texture.empty())
      vgl.unapply_texture();
  }
#endif

#ifndef DISABLE_DX9
  void Material::set(Video_DX9 &vdx, const int &optimization) const {
    if(vdx.get_lighting()) {
      if(!(optimization & ((1 << 5) - 1)))
        vdx.get_d3d_device()->SetMaterial(reinterpret_cast<const D3DMATERIAL9 *>(this));
    }
    else
      vdx.set_color(diffuse);

    if(!(optimization & (1 << 5)) &&
       !m_texture.empty())
      vdx.apply_texture(m_texture);
  }

  void Material::unset(Video_DX9 &vdx, const int &optimization) const {
    if(!(optimization & (1 << 11)) &&
       !m_texture.empty())
      vdx.unapply_texture();
  }
#endif

  bool Material::operator<(const Material &rhs) const {
    return m_texture < rhs.m_texture || m_texture == rhs.m_texture &&
      (diffuse < rhs.diffuse || diffuse == rhs.diffuse &&
      (ambient < rhs.ambient || ambient == rhs.ambient &&
      (specular < rhs.specular || specular == rhs.specular &&
      (emissive < rhs.emissive || emissive == rhs.emissive &&
      m_power < rhs.m_power))));
  }

  bool Material::operator==(const Material &rhs) const {
    return m_texture == rhs.m_texture &&
      diffuse == rhs.diffuse &&
      ambient == rhs.ambient &&
      specular == rhs.specular &&
      emissive == rhs.emissive &&
      m_power == rhs.m_power;
  }

}
