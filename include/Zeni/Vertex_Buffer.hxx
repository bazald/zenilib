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

#ifndef ZENI_VERTEX_BUFFER_HXX
#define ZENI_VERTEX_BUFFER_HXX

#include "Vertex_Buffer.h"

namespace Zeni {

  int Vertex_Buffer::get_begin_end() const {
    return m_begin_end;
  }

  int Vertex_Buffer_3FC::num_triangles() const {
    return int(m_triangles_no_mat.size() + m_triangles_mat.size());
  }

  int Vertex_Buffer_3FC::num_vertices() const {
    return 3 * num_triangles();
  }

  int Vertex_Buffer_3FT::num_triangles() const {
    return int(m_triangles_no_mat.size() + m_triangles_mat.size());
  }

  int Vertex_Buffer_3FT::num_vertices() const {
    return 3 * num_triangles();
  }

#ifndef DISABLE_GL

  int Vertex_Buffer_3FC_GL::vertex_size() const {
    return 3 * sizeof(float);
  }

  int Vertex_Buffer_3FC_GL::buffer_size() const {
    return vertex_size() * num_vertices();
  }

  int Vertex_Buffer_3FC_GL::normal_size() const {
    return 3 * sizeof(float);
  }

  int Vertex_Buffer_3FC_GL::normbuf_size() const {
    return normal_size() * num_vertices();
  }

  int Vertex_Buffer_3FC_GL::color_size() const {
    return 4 * sizeof(unsigned char);
  }
  int Vertex_Buffer_3FC_GL::colorbuf_size() const {
    return color_size() * num_vertices();
  }

  int Vertex_Buffer_3FT_GL::vertex_size() const {
    return 3 * sizeof(float);
  }

  int Vertex_Buffer_3FT_GL::buffer_size() const {
    return vertex_size() * num_vertices();
  }

  int Vertex_Buffer_3FT_GL::normal_size() const {
    return 3 * sizeof(float);
  }

  int Vertex_Buffer_3FT_GL::normbuf_size() const {
    return normal_size() * num_vertices();
  }

  int Vertex_Buffer_3FT_GL::texel_size() const {
    return 2 * sizeof(float);
  }

  int Vertex_Buffer_3FT_GL::texbuf_size() const {
    return texel_size() * num_vertices();
  }

#endif
#ifndef DISABLE_DX9

  int Vertex_Buffer_3FC_DX9::vertex_size() const {
    static Vertex3f_Color vert;
    return sizeof(Vertex3f_Color) - vert.get_offset();
  }

  int Vertex_Buffer_3FC_DX9::buffer_size() const {
    return vertex_size() * num_vertices();
  }

  int Vertex_Buffer_3FT_DX9::vertex_size() const {
    static Vertex3f_Texture vert;
    return sizeof(Vertex3f_Texture) - vert.get_offset();
  }

  int Vertex_Buffer_3FT_DX9::buffer_size() const {
    return vertex_size() * num_vertices();
  }

#endif

}

#endif
