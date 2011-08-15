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

#ifndef ZENI_RENDERABLE_HXX
#define ZENI_RENDERABLE_HXX

// HXXed below
#include <Zeni/Material.h>

#include <Zeni/Renderable.h>

namespace Zeni {

  Renderable::Renderable()
    : m_material(0),
    delete_m_material(false)
  {
  }

  Renderable::Renderable(const Renderable &rhs)
    : m_material(0),
    delete_m_material(false)
  {
    fax_Material(rhs.m_material);
  }

  Renderable & Renderable::operator=(const Renderable &rhs) {
    fax_Material(rhs.m_material);

    return *this;
  }

  const Material * Renderable::get_Material() const {
    return m_material;
  }

}

#include <Zeni/Material.hxx>

#endif
