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

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace Zeni {

  Renderable::~Renderable() {
    if(delete_m_material)
      delete m_material;
  }

  void Renderable::give_Material(Material * const &material) {
    if(delete_m_material)
      delete m_material;
    else
      delete_m_material = true;

    m_material = material;
  }

  void Renderable::lend_Material(const Material * const &material) {
    if(delete_m_material) {
      delete m_material;
      delete_m_material = false;
    }

    m_material = const_cast<Material * const &>(material);
  }

  void Renderable::fax_Material(const Material * const &material) {
    give_Material(material ?
                  new Material(*material) :
                  0);
  }

  void Renderable::pre_render() const {
    if(m_material)
      get_Video().set_Material(*m_material);
  }

  void Renderable::post_render() const {
    if(m_material)
      get_Video().unset_Material(*m_material);
  }

}
