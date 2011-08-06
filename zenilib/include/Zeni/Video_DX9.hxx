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

#ifndef ZENI_VIDEO_DX9_HXX
#define ZENI_VIDEO_DX9_HXX

// HXXed below
#include <Zeni/Color.h>
#include <Zeni/Light.h>
#include <Zeni/Material.h>
#include <Zeni/Renderable.h>
#include <Zeni/Shader.h>
#include <Zeni/Texture.h>
#include <Zeni/Textures.h>
#include <Zeni/Video.h>

#include <Zeni/Video_DX9.h>

// Not HXXed
#include <Zeni/Fog.h>
#include <Zeni/Fonts.h>
#include <Zeni/Vertex_Buffer.h>

#include <Zeni/Define.h>

#ifndef DISABLE_DX9

namespace Zeni {

  LPDIRECT3D9 & Video_DX9::get_d3d() {
    return m_d3d;
  }

  LPDIRECT3DDEVICE9 & Video_DX9::get_d3d_device() {
    return m_d3d_device;
  }

  LPD3DXMATRIXSTACK & Video_DX9::get_matrix_stack() {
    return m_matrix_stack;
  }
  
  const int & Video_DX9::get_dpi() {
    return m_dpi;
  }

  float Video_DX9::get_dpi_ratio() {
    return m_dpi / ZENI_STANDARD_DPI;
  }

}

#endif

#include <Zeni/Undefine.h>

#include <Zeni/Color.hxx>
#include <Zeni/Light.hxx>
#include <Zeni/Material.hxx>
#include <Zeni/Renderable.hxx>
#include <Zeni/Shader.hxx>
#include <Zeni/Texture.hxx>
#include <Zeni/Textures.hxx>
#include <Zeni/Video.hxx>

#endif
