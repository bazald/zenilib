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

#include <Zeni/Vertex3f.h>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Video_DX9.hxx>

#include <cassert>

#ifndef DISABLE_GL
#ifdef _MACOSX
#include <GLEW/glew.h>
#else
#include <GL/glew.h>
#endif
#endif

namespace Zeni {

  Vertex3f::Vertex3f()
  {
  }

  Vertex3f::Vertex3f(const Point3f &position, const Point3f &normal)
    : position(position),
    normal(normal)
  {
  }

  Vertex3f::~Vertex3f() {
  }

  Point3f Vertex3f::get_position() const {
    return position;
  }

  Vertex3f * Vertex3f_Color::interpolate_to(const float &rhs_part, const Vertex3f_Color &rhs) const {
    return new Vertex3f_Color(position.interpolate_to(rhs_part, rhs.position), 
      0.5f*(normal + rhs.normal), 
      Color(m_argb).interpolate_to(rhs_part, rhs.m_argb).get_argb());
  }

  Vertex3f_Color::Vertex3f_Color()
  {
  }

  Vertex3f_Color::Vertex3f_Color(const Point3f &position, const Point3f &normal, const Color &color)
    : Vertex3f(position, normal),
    m_argb(color.get_argb())
  {
  }

  Vertex3f_Color::Vertex3f_Color(const Point3f &position, const Point3f &normal, const Uint32 &argb)
    : Vertex3f(position, normal),
    m_argb(argb)
  {
  }

  Vertex3f_Color::Vertex3f_Color(const Point3f &position_, const Color &color)
    : Vertex3f(position_),
    m_argb(color.get_argb())
  {
  }

  Vertex3f_Color::Vertex3f_Color(const Point3f &position_, const Uint32 &argb_)
    : Vertex3f(position_),
    m_argb(argb_)
  {
  }

  bool Vertex3f_Color::is_3d() const {
    return true;
  }

#ifndef DISABLE_GL
  void Vertex3f_Color::render_to(Video_GL &screen) const {
    glBegin(GL_POINTS);
    subrender_to(screen);
    glEnd();
  }

  void Vertex3f_Color::subrender_to(Video_GL &) const {
    glColor4ub(GLubyte((m_argb >> 16) & 0xFF), 
      GLubyte((m_argb >> 8) & 0xFF), 
      GLubyte(m_argb & 0xFF), 
      GLubyte((m_argb >> 24) & 0xFF));
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(position.x, position.y, position.z);
  }
#endif

#ifndef DISABLE_DX9
  void Vertex3f_Color::render_to(Video_DX9 &screen) const {
    screen.get_d3d_device()->DrawPrimitiveUP(D3DPT_POINTLIST, 1, this, sizeof(Vertex3f_Color));
  }
#endif

  Vertex3f_Texture::Vertex3f_Texture()
  {
  }

  Vertex3f_Texture::Vertex3f_Texture(const Point3f &position_, const Point3f &normal_, const Point2f &texture_coordinate_)
    : Vertex3f(position_, normal_),
    texture_coordinate(texture_coordinate_)
  {
  }

  Vertex3f_Texture::Vertex3f_Texture(const Point3f &position, const Point2f &texture_coordinate_)
    : Vertex3f(position),
    texture_coordinate(texture_coordinate_)
  {
  }

  Vertex3f * Vertex3f_Texture::interpolate_to(const float &rhs_part, const Vertex3f_Texture &rhs) const {
    return new Vertex3f_Texture(position.interpolate_to(rhs_part, rhs.position), 
      0.5f*(normal + rhs.normal), 
      texture_coordinate.interpolate_to(rhs_part, rhs.texture_coordinate));
  }

  bool Vertex3f_Texture::is_3d() const {
    return true;
  }

#ifndef DISABLE_GL
  void Vertex3f_Texture::render_to(Video_GL &) const {
    assert(false);
  }

  void Vertex3f_Texture::subrender_to(Video_GL &) const {
    glTexCoord2f(texture_coordinate.x, texture_coordinate.y);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(position.x, position.y, position.z);
  }
#endif

#ifndef DISABLE_DX9
  void Vertex3f_Texture::render_to(Video_DX9 &) const {
    assert(false);
  }
#endif

}
