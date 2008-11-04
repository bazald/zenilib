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

#ifndef ZENI_Quadrilateral_HXX
#define ZENI_Quadrilateral_HXX

// HXXed below
#include <Zeni/Triangle.h>

#include <Zeni/Quadrilateral.h>

// Not HXXed
#include <GL/gl.h>
#include <cassert>

namespace Zeni {

  template <typename VERTEX>
  Quadrilateral<VERTEX>::Quadrilateral(const VERTEX &vertex0, const VERTEX &vertex1, const VERTEX &vertex2, const VERTEX &vertex3, Render_Wrapper *render_wrapper)
    : a(vertex0),
    b(vertex1),
    c(vertex2),
    d(vertex3),
    m_render_wrapper(render_wrapper)
  {
  }

  template <typename VERTEX>
  Quadrilateral<VERTEX>::~Quadrilateral() {
    delete m_render_wrapper;
  }

  template <typename VERTEX>
  Quadrilateral<VERTEX>::Quadrilateral(const Quadrilateral<VERTEX> &rhs)
    : Renderable(rhs),
    a(rhs.a),
    b(rhs.b),
    c(rhs.c),
    d(rhs.d),
    m_render_wrapper(rhs.m_render_wrapper->get_duplicate())
  {
  }

  template <typename VERTEX>
  Quadrilateral<VERTEX> & Quadrilateral<VERTEX>::operator=(const Quadrilateral<VERTEX> &rhs) {
    if(this != &rhs) {
      delete m_render_wrapper;
      m_render_wrapper = 0;

      a = rhs.a;
      b = rhs.b;
      c = rhs.c;
      d = rhs.d;

      m_render_wrapper = rhs.m_render_wrapper->get_duplicate();
    }

    return *this;
  }

  template <typename VERTEX>
  Point3f Quadrilateral<VERTEX>::get_position() const {
    return Point3f((a.position.x + b.position.x + c.position.x + d.position.x) * 0.25f,
      (a.position.y + b.position.y + c.position.y + d.position.y) * 0.25f,
      (a.position.z + b.position.z + c.position.z + d.position.z) * 0.25f);
  }

#ifndef DISABLE_GL
  template <typename VERTEX>
  void Quadrilateral<VERTEX>::render_to(Video_GL &screen) const {
    m_render_wrapper->prerender();

    glBegin(GL_TRIANGLE_FAN);
    a.subrender_to(screen);
    b.subrender_to(screen);
    c.subrender_to(screen);
    d.subrender_to(screen);
    glEnd();

    m_render_wrapper->postrender();
  }
#endif

#ifndef DISABLE_DX9
  template <typename VERTEX>
  void Quadrilateral<VERTEX>::render_to(Video_DX9 &screen) const {
    m_render_wrapper->prerender();
    screen.get_d3d_device()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, a.get_address(), sizeof(VERTEX));
    m_render_wrapper->postrender();
  }
#endif

  template <typename VERTEX>
  const Render_Wrapper * Quadrilateral<VERTEX>::get_render_wrapper() const {
    return m_render_wrapper;
  }

  template <typename VERTEX>
  void Quadrilateral<VERTEX>::set_render_wrapper(Render_Wrapper * const render_wrapper) {
    delete m_render_wrapper;
    m_render_wrapper = render_wrapper;
  }

  template <typename VERTEX>
  Quadrilateral<VERTEX> * Quadrilateral<VERTEX>::get_duplicate() const {
    return new Quadrilateral<VERTEX>(a, b, c, d, m_render_wrapper->get_duplicate());
  }

  template <typename VERTEX>
  Triangle<VERTEX> * Quadrilateral<VERTEX>::get_duplicate_t0() const {
    return new Triangle<VERTEX>(a, b, c, m_render_wrapper->get_duplicate());
  }

  template <typename VERTEX>
  Triangle<VERTEX> * Quadrilateral<VERTEX>::get_duplicate_t1() const {
    return new Triangle<VERTEX>(a, c, d, m_render_wrapper->get_duplicate());
  }

  template <typename VERTEX>
  const VERTEX & Quadrilateral<VERTEX>::operator[](const int &index) const {
    assert(-1 < index && index < 4);
    const VERTEX * const ptr = &a;
    return ptr[index];
  }

  template <typename VERTEX>
  VERTEX & Quadrilateral<VERTEX>::operator[](const int &index) {
    assert(-1 < index && index < 4);
    VERTEX * const ptr = &a;
    return ptr[index];
  }

}

#include <Zeni/Triangle.hxx>

#endif
