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

#ifndef ZENI_VERTEX_BUFFER_HXX
#define ZENI_VERTEX_BUFFER_HXX

#include <Zeni/Vertex_Buffer.h>

namespace Zeni {

  void Vertex_Buffer::do_normal_alignment(const bool align_normals_) {
    m_align_normals = align_normals_;
  }

  bool Vertex_Buffer::will_do_normal_alignment() const {
    return m_align_normals;
  }

  size_t Vertex_Buffer::num_vertices_cm() const {
    return 3u * m_triangles_cm.size();
  }

  size_t Vertex_Buffer::num_vertices_t() const {
    return 3u * m_triangles_t.size();
  }

  void Vertex_Buffer::unprerender() {
    lose();
    m_descriptors_cm.clear();
    m_descriptors_t.clear();
    m_prerendered = false;
  }

#ifndef DISABLE_GL

  size_t Vertex_Buffer_Renderer_GL::vertex_size() const {
    return 3u * sizeof(float);
  }

  size_t Vertex_Buffer_Renderer_GL::normal_size() const {
    return 3u * sizeof(float);
  }

  size_t Vertex_Buffer_Renderer_GL::color_size() const {
    return 4u * sizeof(unsigned char);
  }

  size_t Vertex_Buffer_Renderer_GL::texel_size() const {
    return 2u * sizeof(float);
  }
  
  bool Vertex_Buffer_Renderer_GL::buffers_supported(Video_GL &
#if !defined(REQUIRE_GL_ES) && !defined(DISABLE_VBO)
    vgl
#endif
    ) const {
#if !defined(REQUIRE_GL_ES) && !defined(DISABLE_VBO)
    return vgl.get_pglDeleteBuffersARB() != 0;
#else
    return true;
#endif
  }

#endif
#ifndef DISABLE_DX9

  size_t Vertex_Buffer_Renderer_DX9::vertex_c_size() const {
    static Vertex3f_Color vert;
    return sizeof(Vertex3f_Color) - vert.get_offset();
  }

  size_t Vertex_Buffer_Renderer_DX9::vertex_t_size() const {
    static Vertex3f_Texture vert;
    return sizeof(Vertex3f_Texture) - vert.get_offset();
  }

#endif

}

#endif
