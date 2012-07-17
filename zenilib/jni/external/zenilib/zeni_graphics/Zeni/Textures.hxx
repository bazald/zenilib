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

#ifndef ZENI_TEXTURES_HXX
#define ZENI_TEXTURES_HXX

// HXXed below
#include <Zeni/Texture.h>

#include <Zeni/Textures.h>

namespace Zeni {

  bool Textures::get_bilinear_filtering() {
    return m_bilinear_filtering;
  }

  bool Textures::get_mipmapping() {
    return m_mipmapping;
  }

  bool Textures::get_trilinear_filtering() {
    return m_bilinear_filtering && m_mipmapping;
  }

  int Textures::get_anisotropic_filtering() {
    return m_anisotropic_filtering;
  }

  bool Textures::get_lazy_loading() {
    return m_lazy_loading;
  }

  void Textures::set_lazy_loading(const bool &lazy_loading) {
    m_lazy_loading = lazy_loading;
  }

}

#include <Zeni/Texture.hxx>

#endif
