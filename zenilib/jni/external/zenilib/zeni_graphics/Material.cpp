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

#include <zeni_graphics.h>

#include <cmath>

#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif

#ifndef DISABLE_GL
#if defined(REQUIRE_GL_ES)
#include <GLES/gl.h>
#else
#include <GL/glew.h>
#endif
#endif

#include <Zeni/Define.h>

namespace Zeni {

  Material::Material(const Color &ambient_, const Color &diffuse_, const Color &specular_, const Color &emissive_, const float &power, const String &texture)
    : diffuse(diffuse_),
    ambient(ambient_),
    specular(specular_), 
    emissive(emissive_),
    m_power(power),
    m_texture(""),
    m_texture_id(0),
    m_optimization(0)
  {
    set_Texture(texture);
  }

  Material::Material(const String &texture, const Color &ambient_and_diffuse)
    : diffuse(ambient_and_diffuse), 
    ambient(ambient_and_diffuse), 
    specular(ZENI_DIFFUSE_TO_SPECULAR(ambient_and_diffuse)), 
    emissive(ZENI_DEFAULT_MATERIAL_EMISSIVE),
    m_power(ZENI_DEFAULT_MATERIAL_POWER),
    m_texture(""),
    m_texture_id(0),
    m_optimization(0)
  {
    set_Texture(texture);
  }

  float Material::get_shininess() const {
    return 0.1f * float(log(m_power)/log(2.0f));
  }

  void Material::set_shininess(const float &shininess) {
    m_power = float(pow(2.0f, 10.0f * shininess));
    if(m_power > 128.0f)
      m_power = 128.0f;
  }

  void Material::set_Texture(const String &texture) {
    m_texture = texture;
    if(texture.empty())
      m_texture_id = 0;
    else
      m_texture_id = get_Textures().get_id(texture);
  }

#ifndef DISABLE_GL
  void Material::set(Video_GL &vgl) const {
    if(vgl.get_lighting()) {
#ifdef REQUIRE_GL_ES
      const GLenum face = GL_FRONT_AND_BACK;
#else
      const GLenum face = GLenum(vgl.get_backface_culling() ? GL_FRONT : GL_FRONT_AND_BACK);
#endif

      if(!(m_optimization & (1 << 0)))
        glMaterialfv(face, GL_AMBIENT, reinterpret_cast<const GLfloat *>(&ambient));
      if(!(m_optimization & (1 << 1)))
        glMaterialfv(face, GL_DIFFUSE, reinterpret_cast<const GLfloat *>(&diffuse));
      if(!(m_optimization & (1 << 2)))
        glMaterialfv(face, GL_SPECULAR, reinterpret_cast<const GLfloat *>(&specular));
      if(!(m_optimization & (1 << 3)))
        glMaterialfv(face, GL_EMISSION, reinterpret_cast<const GLfloat *>(&emissive));
      if(!(m_optimization & (1 << 4)))
        glMaterialfv(face, GL_SHININESS, &m_power);
    }
    else
      vgl.set_Color(diffuse);

    if(!(m_optimization & (1 << 5)) &&
       !m_texture.empty()) {
      try {
        vgl.apply_Texture(m_texture_id);
      }
      catch(Database_Entry_Not_Found &) {
        m_texture_id = get_Textures().get_id(m_texture);
        if(!m_texture_id)
          throw;
        vgl.apply_Texture(m_texture_id);
      }
    }
  }

  void Material::unset(Video_GL &vgl) const {
    if(!(m_optimization & (1 << 11)) &&
       !m_texture.empty())
      vgl.unapply_Texture();
  }
#endif

#ifndef DISABLE_DX9
  void Material::set(Video_DX9 &vdx) const {
    if(vdx.get_lighting()) {
      if((m_optimization & ((1 << 5) - 1)) != 0x1F)
        vdx.get_d3d_device()->SetMaterial(reinterpret_cast<const D3DMATERIAL9 *>(this));
    }
    else
      vdx.set_Color(diffuse);

    if(!(m_optimization & (1 << 5)) &&
       !m_texture.empty())
      vdx.apply_Texture(m_texture);
  }

  void Material::unset(Video_DX9 &vdx) const {
    if(!(m_optimization & (1 << 11)) &&
       !m_texture.empty())
      vdx.unapply_Texture();
  }
#endif

  bool Material::operator<(const Material &rhs) const {
    return m_texture < rhs.m_texture || (m_texture == rhs.m_texture &&
      (diffuse < rhs.diffuse || (diffuse == rhs.diffuse &&
      (ambient < rhs.ambient || (ambient == rhs.ambient &&
      (specular < rhs.specular || (specular == rhs.specular &&
      (emissive < rhs.emissive || (emissive == rhs.emissive &&
      m_power < rhs.m_power)))))))));
  }

  bool Material::operator==(const Material &rhs) const {
    return m_texture == rhs.m_texture &&
      diffuse == rhs.diffuse &&
      ambient == rhs.ambient &&
      specular == rhs.specular &&
      emissive == rhs.emissive &&
      m_power == rhs.m_power;
  }

  void Material::optimize_to_follow(const Material &rhs) {
    m_optimization &= 0xFFFFFFC0;

    if(ambient == rhs.ambient)
      m_optimization |= (1 << 0);
    if(diffuse == rhs.diffuse)
      m_optimization |= (1 << 1);
    if(specular == rhs.specular)
      m_optimization |= (1 << 2);
    if(emissive == rhs.emissive)
      m_optimization |= (1 << 3);
    if(m_power == rhs.m_power)
      m_optimization |= (1 << 4);
    if(m_texture == rhs.m_texture)
      m_optimization |= (1 << 5);
  }

  void Material::optimize_to_precede(const Material &rhs) {
    m_optimization &= 0xFFFFF03F;

    if(ambient == rhs.ambient)
      m_optimization |= (1 << 6);
    if(diffuse == rhs.diffuse)
      m_optimization |= (1 << 7);
    if(specular == rhs.specular)
      m_optimization |= (1 << 8);
    if(emissive == rhs.emissive)
      m_optimization |= (1 << 9);
    if(m_power == rhs.m_power)
      m_optimization |= (1 << 10);
    if(m_texture == rhs.m_texture)
      m_optimization |= (1 << 11);
  }

  void Material::clear_optimization() {
    m_optimization = 0;
  }

}

#include <Zeni/Undefine.h>
