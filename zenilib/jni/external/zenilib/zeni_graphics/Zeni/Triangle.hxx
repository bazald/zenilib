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

#ifndef ZENI_TRIANGLE_HXX
#define ZENI_TRIANGLE_HXX

// HXXed below
#include <Zeni/Vertex3f.h>
#include <Zeni/Video_DX9.h>

#include <Zeni/Triangle.h>

// Not HXXed
#include <Zeni/Vector3f.h>

#if defined(REQUIRE_GL_ES)
#include <GLES/gl.h>
#else
#include <GL/glew.h>
#endif

#include <cassert>

#include <Zeni/Define.h>

namespace Zeni {

  template <typename VERTEX>
  Triangle<VERTEX>::Triangle()
    : a(VERTEX()),
    b(VERTEX()),
    c(VERTEX())
  {
  }

  template <typename VERTEX>
  Triangle<VERTEX>::Triangle(const VERTEX &vertex0, const VERTEX &vertex1, const VERTEX &vertex2)
    : a(vertex0),
    b(vertex1),
    c(vertex2)
  {
  }

  template <typename VERTEX>
  Triangle<VERTEX>::Triangle(const Triangle<VERTEX> &rhs)
    : Renderable(rhs),
    a(rhs.a),
    b(rhs.b),
    c(rhs.c)
  {
  }

  template <typename VERTEX>
  Triangle<VERTEX> & Triangle<VERTEX>::operator=(const Triangle<VERTEX> &rhs) {
    static_cast<Renderable &>(*this) = rhs;

    a = rhs.a;
    b = rhs.b;
    c = rhs.c;

    return *this;
  }

  template <typename VERTEX>
  bool Triangle<VERTEX>::is_3d() const {
    return a.is_3d();
  }

#if !defined(DISABLE_GL) && !defined(REQUIRE_GL_ES)
  template <typename VERTEX>
  void Triangle<VERTEX>::render_to(Video_GL &screen) const {
    glBegin(GL_TRIANGLES);
    a.subrender_to(screen);
    b.subrender_to(screen);
    c.subrender_to(screen);
    glEnd();
  }
#endif

#ifndef DISABLE_DX9
  template <typename VERTEX>
  void Triangle<VERTEX>::render_to(Video_DX9 &screen) const {
    screen.get_d3d_device()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, a.get_address(), sizeof(VERTEX));
  }
#endif

  template <typename VERTEX>
  Triangle<VERTEX> * Triangle<VERTEX>::get_duplicate() const {
    return new Triangle<VERTEX>(*this);
  }

  template <typename VERTEX>
  Triangle<VERTEX> * Triangle<VERTEX>::get_duplicate_subt0() const {
    std::auto_ptr<VERTEX> b2(b.interpolate_to(0.5f, a));
    std::auto_ptr<VERTEX> c2(c.interpolate_to(0.5f, a));
    Triangle<VERTEX> * triangle = new Triangle<VERTEX>(a, *b2, *c2);
    triangle->fax_Material(get_Material());
    return triangle;
  }

  template <typename VERTEX>
  Triangle<VERTEX> * Triangle<VERTEX>::get_duplicate_subt1() const {
    std::auto_ptr<VERTEX> a2(a.interpolate_to(0.5f, b));
    std::auto_ptr<VERTEX> c2(c.interpolate_to(0.5f, b));
    Triangle<VERTEX> * triangle = new Triangle<VERTEX>(*a2, b, *c2);
    triangle->fax_Material(get_Material());
    return triangle;
  }

  template <typename VERTEX>
  Triangle<VERTEX> * Triangle<VERTEX>::get_duplicate_subt2() const {
    std::auto_ptr<VERTEX> a2(a.interpolate_to(0.5f, c));
    std::auto_ptr<VERTEX> b2(b.interpolate_to(0.5f, c));
    Triangle<VERTEX> * triangle = new Triangle<VERTEX>(*a2, *b2, c);
    triangle->fax_Material(get_Material());
    return triangle;
  }

  template <typename VERTEX>
  Triangle<VERTEX> * Triangle<VERTEX>::get_duplicate_subt3() const {
    std::auto_ptr<VERTEX> a2(a.interpolate_to(0.5f, b));
    std::auto_ptr<VERTEX> b2(b.interpolate_to(0.5f, c));
    std::auto_ptr<VERTEX> c2(c.interpolate_to(0.5f, a));
    Triangle<VERTEX> * triangle = new Triangle<VERTEX>(*a2, *b2, *c2);
    triangle->fax_Material(get_Material());
    return triangle;
  }

  template <typename VERTEX>
  const VERTEX & Triangle<VERTEX>::operator[](const int &index) const {
    assert(-1 < index && index < 3);
    const VERTEX * const ptr = &a;
    return ptr[index];
  }

  template <typename VERTEX>
  VERTEX & Triangle<VERTEX>::operator[](const int &index) {
    assert(-1 < index && index < 3);
    VERTEX * const ptr = &a;
    return ptr[index];
  }

}

#include <Zeni/Undefine.h>

#include <Zeni/Video_DX9.hxx>
#include <Zeni/Vertex3f.hxx>

#endif
