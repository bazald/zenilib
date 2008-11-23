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

#ifndef ZENI_TRIANGLE_HXX
#define ZENI_TRIANGLE_HXX

// HXXed below
#include <Zeni/Vertex3f.h>
#include <Zeni/Video_DX9.h>

#include <Zeni/Triangle.h>

// Not HXXed
#include <Zeni/Vector3f.h>
#include <GL/glew.h>
#include <cassert>

namespace Zeni {

  template <typename VERTEX>
  Triangle<VERTEX>::Triangle()
    : a(VERTEX()),
    b(VERTEX()),
    c(VERTEX()),
    m_render_wrapper(new Render_Wrapper())
  {
  }

  template <typename VERTEX>
  Triangle<VERTEX>::Triangle(const VERTEX &vertex0, const VERTEX &vertex1, const VERTEX &vertex2, Render_Wrapper * const &render_wrapper)
    : a(vertex0),
    b(vertex1),
    c(vertex2),
    m_render_wrapper(render_wrapper)
  {
  }

  template <>
  Triangle<Vertex3f_Color>::Triangle(const Vertex3f_Color &vertex0, const Vertex3f_Color &vertex1, const Vertex3f_Color &vertex2, Render_Wrapper * const &render_wrapper)
    : m_render_wrapper(render_wrapper)
  {
    const Vector3f normal = ((vertex1.position - vertex0.position) %
                             (vertex2.position - vertex0.position)).normalized();

    if(vertex0.normal.magnitude2() < 0.0001f)
      a = Vertex3f_Color(vertex0.position, normal, vertex0.get_color());
    else
      a = vertex0;

    if(vertex1.normal.magnitude2() < 0.0001f)
      b = Vertex3f_Color(vertex1.position, normal, vertex1.get_color());
    else
      b = vertex1;

    if(vertex2.normal.magnitude2() < 0.0001f)
      c = Vertex3f_Color(vertex2.position, normal, vertex2.get_color());
    else
      c = vertex2;
  }

  template <>
  Triangle<Vertex3f_Texture>::Triangle(const Vertex3f_Texture &vertex0, const Vertex3f_Texture &vertex1, const Vertex3f_Texture &vertex2, Render_Wrapper * const &render_wrapper)
    : m_render_wrapper(render_wrapper)
  {
    const Vector3f normal = ((vertex1.position - vertex0.position) %
                             (vertex2.position - vertex0.position)).normalized();

    if(vertex0.normal.magnitude2() < 0.0001f)
      a = Vertex3f_Texture(vertex0.position, normal, vertex0.texture_coordinate);
    else
      a = vertex0;

    if(vertex1.normal.magnitude2() < 0.0001f)
      b = Vertex3f_Texture(vertex1.position, normal, vertex1.texture_coordinate);
    else
      b = vertex1;

    if(vertex2.normal.magnitude2() < 0.0001f)
      c = Vertex3f_Texture(vertex2.position, normal, vertex2.texture_coordinate);
    else
      c = vertex2;
  }

  template <typename VERTEX>
  Triangle<VERTEX>::~Triangle() {
    delete m_render_wrapper;
  }

  template <typename VERTEX>
  Triangle<VERTEX>::Triangle(const Triangle<VERTEX> &rhs)
    : Renderable(rhs),
    a(rhs.a),
    b(rhs.b),
    c(rhs.c),
    m_render_wrapper(rhs.m_render_wrapper->get_duplicate())
  {
  }

  template <typename VERTEX>
  Triangle<VERTEX> & Triangle<VERTEX>::operator=(const Triangle<VERTEX> &rhs) {
    if(this != &rhs) {
      delete m_render_wrapper;
      m_render_wrapper = 0;

      a = rhs.a;
      b = rhs.b;
      c = rhs.c;

      m_render_wrapper = rhs.m_render_wrapper->get_duplicate();
    }

    return *this;
  }

  template <typename VERTEX>
  Point3f Triangle<VERTEX>::get_position() const {
    return Point3f((a.position.x + b.position.x + c.position.x) * over_three,
      (a.position.y + b.position.y + c.position.y) * over_three,
      (a.position.z + b.position.z + c.position.z) * over_three);
  }

#ifndef DISABLE_GL
  template <typename VERTEX>
  void Triangle<VERTEX>::render_to(Video_GL &screen) const {
    m_render_wrapper->prerender();

    glBegin(GL_TRIANGLES);
    a.subrender_to(screen);
    b.subrender_to(screen);
    c.subrender_to(screen);
    glEnd();

    m_render_wrapper->postrender();
  }
#endif

#ifndef DISABLE_DX9
  template <typename VERTEX>
  void Triangle<VERTEX>::render_to(Video_DX9 &screen) const {
    m_render_wrapper->prerender();
    screen.get_d3d_device()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, a.get_address(), sizeof(VERTEX));
    m_render_wrapper->postrender();
  }
#endif

  template <typename VERTEX>
  const Render_Wrapper * Triangle<VERTEX>::get_render_wrapper() const {
    return m_render_wrapper;
  }

  template <typename VERTEX>
  void Triangle<VERTEX>::set_render_wrapper(Render_Wrapper * const &render_wrapper) {
    delete m_render_wrapper;
    m_render_wrapper = render_wrapper;
  }

  template <typename VERTEX>
  Triangle<VERTEX> * Triangle<VERTEX>::get_duplicate() const {
    return new Triangle<VERTEX>(a, b, c, m_render_wrapper->get_duplicate());
  }

  template <typename VERTEX>
  Triangle<VERTEX> * Triangle<VERTEX>::get_duplicate_subt0() const {
    return new Triangle<VERTEX>(a, b.interpolate_to(0.5f, a), c.interpolate_to(0.5f, a), m_render_wrapper->get_duplicate());
  }

  template <typename VERTEX>
  Triangle<VERTEX> * Triangle<VERTEX>::get_duplicate_subt1() const {
    return new Triangle<VERTEX>(a.interpolate_to(0.5f, b), b, c.interpolate_to(0.5f, b), m_render_wrapper->get_duplicate());
  }

  template <typename VERTEX>
  Triangle<VERTEX> * Triangle<VERTEX>::get_duplicate_subt2() const {
    return new Triangle<VERTEX>(a.interpolate_to(0.5f, c), b.interpolate_to(0.5f, c), c, m_render_wrapper->get_duplicate());
  }

  template <typename VERTEX>
  Triangle<VERTEX> * Triangle<VERTEX>::get_duplicate_subt3() const {
    return new Triangle<VERTEX>(a.interpolate_to(0.5f, b), b.interpolate_to(0.5f, c), c.interpolate_to(0.5f, a), m_render_wrapper->get_duplicate());
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

#include <Zeni/Video_DX9.hxx>
#include <Zeni/Vertex3f.hxx>

#endif
