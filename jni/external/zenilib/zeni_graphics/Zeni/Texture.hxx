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

#ifndef ZENI_TEXTURE_HXX
#define ZENI_TEXTURE_HXX

// HXXed below
#include <Zeni/Textures.h>

#include <Zeni/Texture.h>

// Not HXXed
#ifndef DISABLE_GL
#if defined(REQUIRE_GL_ES)
#include <GLES/gl.h>
#else
#include <GL/glew.h>
#endif
#endif

namespace Zeni {

  const Point2i & Sprite::get_size() const {
    try {
      return get_Textures()[m_frames[size_t(m_frame)].second].get_size();
    }
    catch(Database_Entry_Not_Found &) {
      m_frames[size_t(m_frame)].second = get_Textures().get_id(m_frames[size_t(m_frame)].first);
      return get_Textures()[m_frames[size_t(m_frame)].second].get_size();
    }
  }

#ifndef DISABLE_GL
  const Point2i & Texture_GL::get_size() const {
    return m_size;
  }
#endif

#ifndef DISABLE_DX9
  const Point2i & Texture_DX9::get_size() const {
    return m_size;
  }

  ID3DXRenderToSurface * Texture_DX9::render_to_surface() const {
    return m_render_to_surface;
  }
#endif

}

#include <Zeni/Textures.hxx>

#endif
