/* This file is part of the Zenipex Library.
* Copyleft (C) 2010 Mitchell Keith Bloch a.k.a. bazald
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

#ifdef _MACOSX
#include <GLEW/glew.h>
#else
#include <GL/glew.h>
#endif

namespace Zeni {

  template <typename VERTEX>
  Line_Segment<VERTEX>::Line_Segment()
    : a(VERTEX()),
    b(VERTEX())
  {
  }

  template <typename VERTEX>
  Line_Segment<VERTEX>::Line_Segment(const VERTEX &vertex0, const VERTEX &vertex1)
    : a(vertex0),
    b(vertex1)
  {
  }

  template <typename VERTEX>
  Line_Segment<VERTEX>::Line_Segment(const Line_Segment<VERTEX> &rhs)
    : Renderable(rhs),
    a(rhs.a),
    b(rhs.b)
  {
  }

  template <typename VERTEX>
  Line_Segment<VERTEX> & Line_Segment<VERTEX>::operator=(const Line_Segment<VERTEX> &rhs) {
    static_cast<Renderable &>(*this) = rhs;

    a = rhs.a;
    b = rhs.b;

    return *this;
  }

  template <typename VERTEX>
  bool Line_Segment<VERTEX>::is_3d() const {
    return a.is_3d();
  }

#ifndef DISABLE_GL
  template <typename VERTEX>
  void Line_Segment<VERTEX>::render_to(Video_GL &screen) const {
    glBegin(GL_LINES);
    a.subrender_to(screen);
    b.subrender_to(screen);
    glEnd();
  }
#endif

#ifndef DISABLE_DX9
  template <typename VERTEX>
  void Line_Segment<VERTEX>::render_to(Video_DX9 &screen) const {
    screen.get_d3d_device()->DrawPrimitiveUP(D3DPT_LINELIST, 1, a.get_address(), sizeof(VERTEX));
  }
#endif

  template <typename VERTEX>
  Line_Segment<VERTEX> * Line_Segment<VERTEX>::get_duplicate() const {
    return new Line_Segment<VERTEX>(*this);
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
