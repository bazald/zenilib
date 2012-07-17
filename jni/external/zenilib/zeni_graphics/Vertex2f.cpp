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

#include <zeni_graphics.h>

#include <cassert>

#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif

#ifndef DISABLE_GL
#if defined(REQUIRE_GL_ES)
#include <GLES/gl.h>
#else
#include <GL/glew.h>
#endif
#endif

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Line_Segment.hxx>
#include <Zeni/Quadrilateral.hxx>
#include <Zeni/Triangle.hxx>

namespace Zeni {

  template class Line_Segment<Vertex2f_Color>;
  template class Line_Segment<Vertex2f_Texture>;
  template class Quadrilateral<Vertex2f_Color>;
  template class Quadrilateral<Vertex2f_Texture>;
  template class Triangle<Vertex2f_Color>;
  template class Triangle<Vertex2f_Texture>;

  Vertex2f::Vertex2f()
  {
  }

  Vertex2f::Vertex2f(const Point2f &position)
    : position(position.x, position.y, position.z)
  {
  }

  Vertex2f::~Vertex2f() {
  }

  Point3f Vertex2f::get_position() const {
    return position;
  }

  Vertex2f_Color * Vertex2f_Color::interpolate_to(const float &rhs_part, const Vertex2f_Color &rhs) const {
    const Point3f midpt = position.interpolate_to(rhs_part, rhs.position);
    return new Vertex2f_Color(Point2f(midpt.x, midpt.y), 
      Color(m_argb).interpolate_to(rhs_part, rhs.m_argb).get_argb());
  }

  Vertex2f_Color::Vertex2f_Color()
  {
  }

  Vertex2f_Color::Vertex2f_Color(const Point2f &position, const Color &color)
    : Vertex2f(position),
    m_argb(color.get_argb())
  {
  }

  Vertex2f_Color::Vertex2f_Color(const Point2f &position, const Uint32 &argb)
    : Vertex2f(position),
    m_argb(argb)
  {
  }

  bool Vertex2f_Color::is_3d() const {
    return false;
  }

#ifdef REQUIRE_GL_ES
  template <>
  void Line_Segment<Vertex2f_Color>::render_to(Video_GL &) const {
    Uint32 c4ub[] = {((a.get_Color() & 0x000000FF) << 16) | ((a.get_Color() & 0x00FF0000) >> 16) | ((a.get_Color() & 0xFF00FF00)),
                     ((b.get_Color() & 0x000000FF) << 16) | ((b.get_Color() & 0x00FF0000) >> 16) | ((b.get_Color() & 0xFF00FF00))};

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(a), a.get_address());
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, c4ub);

    glDrawArrays(GL_LINES, 0, 2);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }

  template <>
  void Triangle<Vertex2f_Color>::render_to(Video_GL &) const {
    Uint32 c4ub[] = {((a.get_Color() & 0x000000FF) << 16) | ((a.get_Color() & 0x00FF0000) >> 16) | ((a.get_Color() & 0xFF00FF00)),
                     ((b.get_Color() & 0x000000FF) << 16) | ((b.get_Color() & 0x00FF0000) >> 16) | ((b.get_Color() & 0xFF00FF00)),
                     ((c.get_Color() & 0x000000FF) << 16) | ((c.get_Color() & 0x00FF0000) >> 16) | ((c.get_Color() & 0xFF00FF00))};

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(a), a.get_address());
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, c4ub);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }

  template <>
  void Quadrilateral<Vertex2f_Color>::render_to(Video_GL &) const {
    Uint32 c4ub[] = {((a.get_Color() & 0x000000FF) << 16) | ((a.get_Color() & 0x00FF0000) >> 16) | ((a.get_Color() & 0xFF00FF00)),
                     ((b.get_Color() & 0x000000FF) << 16) | ((b.get_Color() & 0x00FF0000) >> 16) | ((b.get_Color() & 0xFF00FF00)),
                     ((c.get_Color() & 0x000000FF) << 16) | ((c.get_Color() & 0x00FF0000) >> 16) | ((c.get_Color() & 0xFF00FF00)),
                     ((d.get_Color() & 0x000000FF) << 16) | ((d.get_Color() & 0x00FF0000) >> 16) | ((d.get_Color() & 0xFF00FF00))};

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(a), a.get_address());
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, c4ub);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }
#endif

#ifndef DISABLE_GL
  void Vertex2f_Color::render_to(Video_GL &
#ifndef REQUIRE_GL_ES
    screen
#endif
    ) const
  {
#ifdef REQUIRE_GL_ES
    Uint32 c4ub = ((m_argb & 0x000000FF) << 16) | ((m_argb & 0x00FF0000) >> 16) | ((m_argb & 0xFF00FF00));

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, get_address());
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, &c4ub);

    glDrawArrays(GL_POINTS, 0, 1);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
#else
    glBegin(GL_POINTS);
    subrender_to(screen);
    glEnd();
#endif
  }

  void Vertex2f_Color::subrender_to(Video_GL &) const {
#ifndef REQUIRE_GL_ES
    glColor4ub(GLubyte((m_argb >> 16) & 0xFF), 
      GLubyte((m_argb >> 8) & 0xFF), 
      GLubyte(m_argb & 0xFF), 
      GLubyte((m_argb >> 24) & 0xFF));
    glVertex2f(position.x, position.y);
#endif
  }
#endif

#ifndef DISABLE_DX9
  void Vertex2f_Color::render_to(Video_DX9 &screen) const {
    screen.get_d3d_device()->DrawPrimitiveUP(D3DPT_POINTLIST, 1, this, sizeof(Vertex2f_Color));
  }
#endif

  Vertex2f_Texture::Vertex2f_Texture()
  {
  }

  Vertex2f_Texture::Vertex2f_Texture(const Point2f &position, const Point2f &texture_coordinate_)
    : Vertex2f(position),
    texture_coordinate(texture_coordinate_)
  {
  }

  Vertex2f_Texture * Vertex2f_Texture::interpolate_to(const float &rhs_part, const Vertex2f_Texture &rhs) const {
    const Point3f midpt = position.interpolate_to(rhs_part, rhs.position);
    return new Vertex2f_Texture(Point2f(midpt.x, midpt.y), 
      texture_coordinate.interpolate_to(rhs_part, rhs.texture_coordinate));
  }

  bool Vertex2f_Texture::is_3d() const {
    return false;
  }

#ifdef REQUIRE_GL_ES
  template <>
  void Line_Segment<Vertex2f_Texture>::render_to(Video_GL &) const {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(a), a.get_address());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(a), &a.texture_coordinate);

    glDrawArrays(GL_LINES, 0, 2);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }

  template <>
  void Triangle<Vertex2f_Texture>::render_to(Video_GL &) const {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(a), a.get_address());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(a), &a.texture_coordinate);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }

  template <>
  void Quadrilateral<Vertex2f_Texture>::render_to(Video_GL &) const {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(a), a.get_address());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(a), &a.texture_coordinate);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }
#endif

#ifndef DISABLE_GL
  void Vertex2f_Texture::render_to(Video_GL &) const {
    assert(false);
  }

  void Vertex2f_Texture::subrender_to(Video_GL &) const {
#ifndef REQUIRE_GL_ES
    glTexCoord2f(texture_coordinate.x, texture_coordinate.y);
    glVertex2f(position.x, position.y);
#endif
  }
#endif

#ifndef DISABLE_DX9
  void Vertex2f_Texture::render_to(Video_DX9 &) const {
    assert(false);
  }
#endif

}
