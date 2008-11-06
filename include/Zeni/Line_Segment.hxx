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

#ifndef ZENI_LINE_SEGMENT_HXX
#define ZENI_LINE_SEGMENT_HXX

// HXXed below
#include <Zeni/Video_DX9.h>

#include <Zeni/Line_Segment.h>

// Not HXXed
#include <Zeni/Vector3f.h>
#include <GL/gl.h>

namespace Zeni {

  template <typename VERTEX>
  Line_Segment<VERTEX>::Line_Segment()
    : a(VERTEX()),
    b(VERTEX()),
    m_render_wrapper(new Render_Wrapper())
  {
  }

  template <typename VERTEX>
  Line_Segment<VERTEX>::Line_Segment(const VERTEX &vertex0, const VERTEX &vertex1, Render_Wrapper * const &render_wrapper)
    : a(vertex0),
    b(vertex1),
    m_render_wrapper(render_wrapper)
  {
  }

  template <typename VERTEX>
  Line_Segment<VERTEX>::~Line_Segment() {
    delete m_render_wrapper;
  }

  template <typename VERTEX>
  Line_Segment<VERTEX>::Line_Segment(const Line_Segment<VERTEX> &rhs)
    : Renderable(rhs),
    a(rhs.a),
    b(rhs.b),
    m_render_wrapper(rhs.m_render_wrapper->get_duplicate())
  {
  }

  template <typename VERTEX>
  Line_Segment<VERTEX> & Line_Segment<VERTEX>::operator=(const Line_Segment<VERTEX> &rhs) {
    if(this != &rhs) {
      delete m_render_wrapper;
      m_render_wrapper = 0;

      a = rhs.a;
      b = rhs.b;

      m_render_wrapper = rhs.m_render_wrapper->get_duplicate();
    }

    return *this;
  }

  template <typename VERTEX>
  Point3f Line_Segment<VERTEX>::get_position() const {
    return Point3f((a.position.x + b.position.x) * 0.5f,
      (a.position.y + b.position.y) * 0.5f,
      (a.position.z + b.position.z) * 0.5f);
  }

#ifndef DISABLE_GL
  template <typename VERTEX>
  void Line_Segment<VERTEX>::render_to(Video_GL &screen) const {
    m_render_wrapper->prerender();

    glBegin(GL_LINES);
    a.subrender_to(screen);
    b.subrender_to(screen);
    glEnd();

    m_render_wrapper->postrender();
  }
#endif

#ifndef DISABLE_DX9
  template <typename VERTEX>
  void Line_Segment<VERTEX>::render_to(Video_DX9 &screen) const {
    m_render_wrapper->prerender();
    screen.get_d3d_device()->DrawPrimitiveUP(D3DPT_LINELIST, 1, a.get_address(), sizeof(VERTEX));
    m_render_wrapper->postrender();
  }
#endif

  template <typename VERTEX>
  const Render_Wrapper * Line_Segment<VERTEX>::get_render_wrapper() const {
    return m_render_wrapper;
  }

  template <typename VERTEX>
  void Line_Segment<VERTEX>::set_render_wrapper(Render_Wrapper * const &render_wrapper) {
    delete m_render_wrapper;
    m_render_wrapper = render_wrapper;
  }

  template <typename VERTEX>
  Line_Segment<VERTEX> * Line_Segment<VERTEX>::get_duplicate() const {
    return new Line_Segment<VERTEX>(a, b, m_render_wrapper->get_duplicate());
  }

  template <typename VERTEX>
  const VERTEX & Line_Segment<VERTEX>::operator[](const int &index) const {
    assert(-1 < index && index < 2);
    const VERTEX * const ptr = &a;
    return ptr[index];
  }

  template <typename VERTEX>
  VERTEX & Line_Segment<VERTEX>::operator[](const int &index) {
    assert(-1 < index && index < 2);
    VERTEX * const ptr = &a;
    return ptr[index];
  }

}

#include <Zeni/Video_DX9.hxx>

#endif
