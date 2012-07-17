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

#ifndef ZENI_QUADRILATERAL_HXX
#define ZENI_QUADRILATERAL_HXX

// HXXed below
#include <Zeni/Triangle.h>
#include <Zeni/Vertex3f.h>

#include <Zeni/Quadrilateral.h>

// Not HXXed
#if defined(REQUIRE_GL_ES)
#include <GLES/gl.h>
#else
#include <GL/glew.h>
#endif

#include <cassert>

namespace Zeni {

  template <typename VERTEX>
  Quadrilateral<VERTEX>::Quadrilateral()
    : a(VERTEX()),
    b(VERTEX()),
    c(VERTEX()),
    d(VERTEX())
  {
  }

  template <typename VERTEX>
  Quadrilateral<VERTEX>::Quadrilateral(const VERTEX &vertex0, const VERTEX &vertex1, const VERTEX &vertex2, const VERTEX &vertex3)
    : a(vertex0),
    b(vertex1),
    c(vertex2),
    d(vertex3)
  {
  }

  template <typename VERTEX>
  Quadrilateral<VERTEX>::Quadrilateral(const Quadrilateral<VERTEX> &rhs)
    : Renderable(rhs),
    a(rhs.a),
    b(rhs.b),
    c(rhs.c),
    d(rhs.d)
  {
  }

  template <typename VERTEX>
  Quadrilateral<VERTEX> & Quadrilateral<VERTEX>::operator=(const Quadrilateral<VERTEX> &rhs) {
    static_cast<Renderable &>(*this) = rhs;

    a = rhs.a;
    b = rhs.b;
    c = rhs.c;
    d = rhs.d;

    return *this;
  }

  template <typename VERTEX>
  bool Quadrilateral<VERTEX>::is_3d() const {
    return a.is_3d();
  }

#if !defined(DISABLE_GL) && !defined(REQUIRE_GL_ES)
  template <typename VERTEX>
  void Quadrilateral<VERTEX>::render_to(Video_GL &screen) const {
    glBegin(GL_TRIANGLE_FAN);
    a.subrender_to(screen);
    b.subrender_to(screen);
    c.subrender_to(screen);
    d.subrender_to(screen);
    glEnd();
  }
#endif

#ifndef DISABLE_DX9
  template <typename VERTEX>
  void Quadrilateral<VERTEX>::render_to(Video_DX9 &screen) const {
    screen.get_d3d_device()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, a.get_address(), sizeof(VERTEX));
  }
#endif

  template <typename VERTEX>
  Quadrilateral<VERTEX> * Quadrilateral<VERTEX>::get_duplicate() const {
    return new Quadrilateral<VERTEX>(*this);
  }

  template <typename VERTEX>
  Triangle<VERTEX> * Quadrilateral<VERTEX>::get_duplicate_t0() const {
    Triangle<VERTEX> * const t0 = new Triangle<VERTEX>(a, b, c);
    t0->fax_Material(get_Material());
    return t0;
  }

  template <typename VERTEX>
  Triangle<VERTEX> * Quadrilateral<VERTEX>::get_duplicate_t1() const {
    Triangle<VERTEX> * const t1 = new Triangle<VERTEX>(a, c, d);
    t1->fax_Material(get_Material());
    return t1;
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
#include <Zeni/Vertex3f.hxx>

#endif
