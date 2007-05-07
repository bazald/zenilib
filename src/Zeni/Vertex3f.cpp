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

#include <Zeni/Vertex3f.h>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Video_DX9.hxx>

#include <cassert>

#ifndef DISABLE_GL
#include <GL/gl.h>
#endif

namespace Zeni {

  Vertex3f::Vertex3f(const Point3f &position, const Vector3f &normal)
    : m_position(position),
    m_normal(normal)
  {
  }

  Vertex3f::~Vertex3f() {
  }

  Vertex3f * Vertex3f_Color::interpolate_to(const float &rhs_part, const Vertex3f &rhs) const {
    const Vertex3f_Color &rhs_c = dynamic_cast<const Vertex3f_Color &>(rhs);
    return new Vertex3f_Color(get_position().interpolate_to(rhs_part, rhs_c.get_position()), 
      0.5f*(get_normal() + rhs_c.get_normal()), 
      Color(m_argb).interpolate_to(rhs_part, rhs_c.m_argb).get_argb());
  }

  Vertex3f_Color::Vertex3f_Color(const Point3f &position, const Vector3f &normal, const Color &color)
    : Vertex3f(position, normal),
    m_argb(color.get_argb())
  {
  }

  Vertex3f_Color::Vertex3f_Color(const Point3f &position, const Vector3f &normal, const long &argb)
    : Vertex3f(position, normal),
    m_argb(argb)
  {
  }

  Vertex3f_Color::Vertex3f_Color(const Point3f &position_, const Color &color)
    : Vertex3f(position_),
    m_argb(color.get_argb())
  {
  }

  Vertex3f_Color::Vertex3f_Color(const Point3f &position_, const long &argb_)
    : Vertex3f(position_),
    m_argb(argb_)
  {
  }

  Point3f Vertex3f_Color::get_position() const {
    return Vertex3f::get_position();
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
    glNormal3f(get_normal().i, get_normal().j, get_normal().k);
    glVertex3f(get_position().x, get_position().y, get_position().z);
  }
#endif

#ifndef DISABLE_DX9
  void Vertex3f_Color::render_to(Video_DX9 &screen) const {
    screen.get_d3d_device()->DrawPrimitiveUP(D3DPT_POINTLIST, 1, this, sizeof(Vertex3f_Color));
  }
#endif

  Vertex3f_Texture::Vertex3f_Texture(const Point3f &position_, const Vector3f &normal_, const Point2f &texture_coordinate_)
    : Vertex3f(position_, normal_),
    m_texture_coordinate(texture_coordinate_)
  {
  }

  Vertex3f_Texture::Vertex3f_Texture(const Point3f &position, const Point2f &texture_coordinate)
    : Vertex3f(position),
    m_texture_coordinate(texture_coordinate)
  {
  }

  Vertex3f * Vertex3f_Texture::interpolate_to(const float &rhs_part, const Vertex3f &rhs) const {
    const Vertex3f_Texture &rhs_t = dynamic_cast<const Vertex3f_Texture &>(rhs);
    return new Vertex3f_Texture(get_position().interpolate_to(rhs_part, rhs_t.get_position()), 
      0.5f*(get_normal() + rhs_t.get_normal()), 
      m_texture_coordinate.interpolate_to(rhs_part, rhs_t.m_texture_coordinate));
  }

  Point3f Vertex3f_Texture::get_position() const {
    return Vertex3f::get_position();
  }

#ifndef DISABLE_GL
  void Vertex3f_Texture::render_to(Video_GL &) const {
    assert(false);
  }

  void Vertex3f_Texture::subrender_to(Video_GL &) const {
    glTexCoord2f(m_texture_coordinate.x, m_texture_coordinate.y);
    glNormal3f(get_normal().i, get_normal().j, get_normal().k);
    glVertex3f(get_position().x, get_position().y, get_position().z);
  }
#endif

#ifndef DISABLE_DX9
  void Vertex3f_Texture::render_to(Video_DX9 &) const {
    assert(false);
  }
#endif

}
