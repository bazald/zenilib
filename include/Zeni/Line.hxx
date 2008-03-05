/* This file is part of the Zenipex Library.
* Copyleft (C) 2007 Mitchell Keith Bloch a.k.a. bazald
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

#ifndef ZENI_LINE_HXX
#define ZENI_LINE_HXX

#include <Zeni/Line.h>
#include <Zeni/Vector3f.h>

#include <Zeni/Video_DX9.hxx>

#include <GL/gl.h>

namespace Zeni {

  template <typename VERTEX>
  Line<VERTEX>::Line(const VERTEX &vertex0, const VERTEX &vertex1, Render_Wrapper *render_wrapper)
    : m_render_wrapper(render_wrapper)
  {
    m_vertex[0] = vertex0;
    m_vertex[1] = vertex1;
  }

  template <typename VERTEX>
  const VERTEX & Line<VERTEX>::get_vertex(const int &index) const {
    if(index < 0 || index > 1)
      throw Invalid_Vertex_Index();
    return m_vertex[index];
  }

  template <typename VERTEX>
  void Line<VERTEX>::set_vertex(const int &index, const VERTEX &vertex) {
    if(index < 0 || index > 1)
      throw Invalid_Vertex_Index();
    m_vertex[index] = vertex;
  }

  template <typename VERTEX>
  Point3f Line<VERTEX>::get_position() const {
    return Point3f((m_vertex[0].get_position().x + m_vertex[1].get_position().x) * 0.5f,
      (m_vertex[0].get_position().y + m_vertex[1].get_position().y) * 0.5f,
      (m_vertex[0].get_position().z + m_vertex[1].get_position().z) * 0.5f);
  }

#ifndef DISABLE_GL
  template <typename VERTEX>
  void Line<VERTEX>::render_to(Video_GL &screen) const {
    m_render_wrapper->prerender();

    glBegin(GL_LINES);
    m_vertex[0].subrender_to(screen);
    m_vertex[1].subrender_to(screen);
    glEnd();

    m_render_wrapper->postrender();
  }
#endif

#ifndef DISABLE_DX9
  template <typename VERTEX>
  void Line<VERTEX>::render_to(Video_DX9 &screen) const {
    m_render_wrapper->prerender();
    screen.get_d3d_device()->DrawPrimitiveUP(D3DPT_LINELIST, 1, m_vertex[0].get_address(), sizeof(VERTEX));
    m_render_wrapper->postrender();
  }
#endif

  template <typename VERTEX>
  const Render_Wrapper * const Line<VERTEX>::get_render_wrapper() const {
    return m_render_wrapper.get();
  }

  template <typename VERTEX>
  Line<VERTEX> * Line<VERTEX>::get_duplicate() const {
    return new Line<VERTEX>(m_vertex[0], m_vertex[1], m_render_wrapper->get_duplicate());
  }

}

#endif
