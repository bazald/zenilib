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

#ifndef ZENI_VERTEX_BUFFER_HXX
#define ZENI_VERTEX_BUFFER_HXX

// HXXed below
#include <Zeni/Triangle.h>

#include <Zeni/Vertex_Buffer.h>

namespace Zeni {

  void Vertex_Buffer::do_normal_alignment(const bool align_normals_) {
    m_align_normals = align_normals_;
  }

  bool Vertex_Buffer::will_do_normal_alignment() const {
    return m_align_normals;
  }

  int Vertex_Buffer::num_vertices_cm() const {
    return int(3 * m_triangles_cm.size());
  }

  int Vertex_Buffer::num_vertices_t() const {
    return int(3 * m_triangles_t.size());
  }

#ifndef DISABLE_GL

  int Vertex_Buffer_GL::vertex_size() const {
    return 3 * sizeof(float);
  }

  int Vertex_Buffer_GL::normal_size() const {
    return 3 * sizeof(float);
  }

  int Vertex_Buffer_GL::color_size() const {
    return 4 * sizeof(unsigned char);
  }

  int Vertex_Buffer_GL::texel_size() const {
    return 2 * sizeof(float);
  }

#endif
#ifndef DISABLE_DX9

  int Vertex_Buffer_DX9::vertex_c_size() const {
    static Vertex3f_Color vert;
    return int(sizeof(Vertex3f_Color) - vert.get_offset());
  }

  int Vertex_Buffer_DX9::vertex_t_size() const {
    static Vertex3f_Texture vert;
    return int(sizeof(Vertex3f_Texture) - vert.get_offset());
  }

#endif

}

#include <Zeni/Triangle.hxx>

#endif
