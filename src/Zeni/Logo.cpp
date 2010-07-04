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

#include <Zeni/Logo.h>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Material.h>
#include <Zeni/Triangle.hxx>
#include <Zeni/Quadrilateral.hxx>
#include <Zeni/Vertex_Buffer.h>
#include <Zeni/Video.h>

namespace Zeni {

  Logo::Logo(const Point2f &upper_left, const float &height, const Color &logo_color, const Color &text_color)
    : m_upper_left(upper_left),
    m_height(height),
    m_logo_color(logo_color),
    m_text_color(text_color),
    m_vbo(new Vertex_Buffer())
  {
    create_all();
  }

  Logo::~Logo() {
    delete m_vbo;
  }

  Logo::Logo(const Logo &rhs)
    : m_upper_left(rhs.m_upper_left),
    m_height(rhs.m_height),
    m_logo_color(rhs.m_logo_color),
    m_text_color(rhs.m_text_color),
    m_vbo(new Vertex_Buffer())
  {
    create_all();
  }

  Logo & Logo::operator=(const Logo &rhs) {
    Logo temp(rhs.m_upper_left, rhs.m_height, rhs.m_logo_color, rhs.m_text_color);

    std::swap(m_upper_left, temp.m_upper_left);
    std::swap(m_height, temp.m_height);
    std::swap(m_logo_color, temp.m_logo_color);
    std::swap(m_text_color, temp.m_text_color);
    std::swap(m_vbo, temp.m_vbo);

    return *this;
  }

  void Logo::render() {
    m_vbo->render();
  }

  Point2f Logo::get_p00(const Point2f &upper_left, const float &/*height*/) {
    return Point2f(upper_left.x                 , upper_left.y                );
  }
  Point2f Logo::get_p10(const Point2f &upper_left, const float &height) {
    return Point2f(upper_left.x + 0.34f * height, upper_left.y                );
  }
  Point2f Logo::get_p20(const Point2f &upper_left, const float &height) {
    return Point2f(upper_left.x + 0.68f * height, upper_left.y                );
  }
  Point2f Logo::get_p01(const Point2f &upper_left, const float &height) {
    return Point2f(upper_left.x                 , upper_left.y + 0.5f * height);
  }
  Point2f Logo::get_p11(const Point2f &upper_left, const float &height) {
    return Point2f(upper_left.x + 0.34f * height, upper_left.y + 0.5f * height);
  }
  Point2f Logo::get_p21(const Point2f &upper_left, const float &height) {
    return Point2f(upper_left.x + 0.68f * height, upper_left.y + 0.5f * height);
  }
  Point2f Logo::get_p02(const Point2f &upper_left, const float &height) {
    return Point2f(upper_left.x                 , upper_left.y + 1.0f * height);
  }
  Point2f Logo::get_p12(const Point2f &upper_left, const float &height) {
    return Point2f(upper_left.x + 0.34f * height, upper_left.y + 1.0f * height);
  }
  Point2f Logo::get_p22(const Point2f &upper_left, const float &height) {
    return Point2f(upper_left.x + 0.68f * height, upper_left.y + 1.0f * height);
  }
  float Logo::get_thickness(const float &height) {
    return 0.68f / 12.0f * height;
  }

  void Logo::create_all() {
    create_z(Point2f(m_upper_left.x - 0.99f * m_height, m_upper_left.y + 0.25f * m_height), 0.5f * m_height);
    create_e(Point2f(m_upper_left.x - 0.55f * m_height, m_upper_left.y + 0.25f * m_height), 0.5f * m_height);
    create_n(Point2f(m_upper_left.x - 0.11f * m_height, m_upper_left.y + 0.25f * m_height), 0.5f * m_height);
    create_i(Point2f(m_upper_left.x + 0.33f * m_height, m_upper_left.y + 0.25f * m_height), 0.5f * m_height);
    create_p(Point2f(m_upper_left.x + 0.77f * m_height, m_upper_left.y + 0.25f * m_height), 0.5f * m_height);
    create_e(Point2f(m_upper_left.x + 1.21f * m_height, m_upper_left.y + 0.25f * m_height), 0.5f * m_height);
    create_x(Point2f(m_upper_left.x + 1.65f * m_height, m_upper_left.y + 0.25f * m_height), 0.5f * m_height);
    create_logo(m_upper_left, m_height);
  }

  void Logo::create_logo(const Point2f &upper_left, const float &height) {
    const Point2f p30(upper_left.x + 0.500f * height, upper_left.y                  );
    const Point2f p02(upper_left.x                  , upper_left.y + 0.333f * height);
    const Point2f p42(upper_left.x + 0.666f * height, upper_left.y + 0.333f * height);
    const Point2f p06(upper_left.x                  , upper_left.y +          height);
    const Point2f p26(upper_left.x + 0.333f * height, upper_left.y +          height);
    const Point2f p66(upper_left.x +          height, upper_left.y +          height);
    const float thickness = 0.68f / 30.0f * height;

    create_thick_line(p30, p06, thickness, m_logo_color);
    create_thick_line(p30, p66, thickness, m_logo_color);
    create_thick_line(p02, p42, thickness, m_logo_color);
    create_thick_line(p06, p26, thickness, m_logo_color);
    create_circle(p30, thickness, m_logo_color, 18);
    create_circle(p02, thickness, m_logo_color, 18);
    create_circle(p06, thickness, m_logo_color, 18);
    create_circle(p26, thickness, m_logo_color, 18);
    create_circle(p66, thickness, m_logo_color, 18);
  }

  void Logo::create_z(const Point2f &upper_left, const float &height) {
    const Point2f p00 = get_p00(upper_left, height);
    const Point2f p20 = get_p20(upper_left, height);
    const Point2f p02 = get_p02(upper_left, height);
    const Point2f p22 = get_p22(upper_left, height);
    const float thickness = get_thickness(height);

    create_thick_line(p00, p20, thickness, m_text_color);
    create_thick_line(p20, p02, thickness, m_text_color);
    create_thick_line(p02, p22, thickness, m_text_color);
    create_circle(p00, thickness, m_text_color, 12);
    create_circle(p20, thickness, m_text_color, 12);
    create_circle(p02, thickness, m_text_color, 12);
    create_circle(p22, thickness, m_text_color, 12);
  }

  void Logo::create_e(const Point2f &upper_left, const float &height) {
    const Point2f p00 = get_p00(upper_left, height);
    const Point2f p20 = get_p20(upper_left, height);
    const Point2f p01 = get_p01(upper_left, height);
    const Point2f p21 = get_p21(upper_left, height);
    const Point2f p02 = get_p02(upper_left, height);
    const Point2f p22 = get_p22(upper_left, height);
    const float thickness = get_thickness(height);

    create_thick_line(p00, p20, thickness, m_text_color);
    create_thick_line(p00, p02, thickness, m_text_color);
    create_thick_line(p01, p21, thickness, m_text_color);
    create_thick_line(p02, p22, thickness, m_text_color);
    create_circle(p00, thickness, m_text_color, 12);
    create_circle(p20, thickness, m_text_color, 12);
    create_circle(p21, thickness, m_text_color, 12);
    create_circle(p02, thickness, m_text_color, 12);
    create_circle(p22, thickness, m_text_color, 12);
  }

  void Logo::create_n(const Point2f &upper_left, const float &height) {
    const Point2f p00 = get_p00(upper_left, height);
    const Point2f p20 = get_p20(upper_left, height);
    const Point2f p02 = get_p02(upper_left, height);
    const Point2f p22 = get_p22(upper_left, height);
    const float thickness = get_thickness(height);

    create_thick_line(p00, p02, thickness, m_text_color);
    create_thick_line(p00, p22, thickness, m_text_color);
    create_thick_line(p20, p22, thickness, m_text_color);
    create_circle(p00, thickness, m_text_color, 12);
    create_circle(p20, thickness, m_text_color, 12);
    create_circle(p02, thickness, m_text_color, 12);
    create_circle(p22, thickness, m_text_color, 12);
  }

  void Logo::create_i(const Point2f &upper_left, const float &height) {
    const Point2f p00 = get_p00(upper_left, height);
    const Point2f p10 = get_p10(upper_left, height);
    const Point2f p20 = get_p20(upper_left, height);
    const Point2f p02 = get_p02(upper_left, height);
    const Point2f p12 = get_p12(upper_left, height);
    const Point2f p22 = get_p22(upper_left, height);
    const float thickness = get_thickness(height);

    create_thick_line(p00, p20, thickness, m_text_color);
    create_thick_line(p10, p12, thickness, m_text_color);
    create_thick_line(p02, p22, thickness, m_text_color);
    create_circle(p00, thickness, m_text_color, 12);
    create_circle(p20, thickness, m_text_color, 12);
    create_circle(p02, thickness, m_text_color, 12);
    create_circle(p22, thickness, m_text_color, 12);
  }

  void Logo::create_p(const Point2f &upper_left, const float &height) {
    const Point2f p00 = get_p00(upper_left, height);
    const Point2f p10 = get_p10(upper_left, height);
    const Point2f p01 = get_p01(upper_left, height);
    const Point2f p11 = get_p11(upper_left, height);
    const Point2f p02 = get_p02(upper_left, height);
    const Point2f phc(p10.x, 0.5f * (p10.y + p11.y));
    const float thickness = get_thickness(height);

    create_thick_line(p00, p10, thickness, m_text_color);
    create_thick_line(p00, p02, thickness, m_text_color);
    create_thick_line(p01, p11, thickness, m_text_color);
    create_circle(p00, thickness, m_text_color, 12);
    create_circle(p02, thickness, m_text_color, 12);
    create_hollow_ellipse(phc, Point2f(0.167f * m_height - thickness, 0.125f * m_height - thickness), Point2f(0.167f * m_height + thickness, 0.125f * m_height + thickness), m_text_color, 36, 18);
  }

  void Logo::create_x(const Point2f &upper_left, const float &height) {
    const Point2f p00 = get_p00(upper_left, height);
    const Point2f p20 = get_p20(upper_left, height);
    const Point2f p02 = get_p02(upper_left, height);
    const Point2f p22 = get_p22(upper_left, height);
    const float thickness = get_thickness(height);

    create_thick_line(p00, p22, thickness, m_text_color);
    create_thick_line(p02, p20, thickness, m_text_color);
    create_circle(p00, thickness, m_text_color, 12);
    create_circle(p20, thickness, m_text_color, 12);
    create_circle(p02, thickness, m_text_color, 12);
    create_circle(p22, thickness, m_text_color, 12);
  }

  void Logo::create_thick_line(const Point2f &p0, const Point2f &p1, const float &thickness, const Color &color) {
    const Vector3f n = (vector_k % (Vector3f(p1.x, p1.y, 0.0f) - Vector3f(p0.x, p0.y, 0.0f))).normalized() * thickness;
    Quadrilateral<Vertex2f_Color> q( (Vertex2f_Color(Point2f(p0.x - n.x, p0.y - n.y), color)) ,
                                     (Vertex2f_Color(Point2f(p0.x + n.x, p0.y + n.y), color)) ,
                                     (Vertex2f_Color(Point2f(p1.x + n.x, p1.y + n.y), color)) ,
                                     (Vertex2f_Color(Point2f(p1.x - n.x, p1.y - n.y), color)) );
    Material m("", color);
    q.fax_Material(&m);
    m_vbo->fax_quadrilateral(&q);
  }

  void Logo::create_circle(const Point2f &p, const float &radius, const Color &color, const size_t &segments, const size_t &begin, const size_t &end) {
    const float arc = 2.0f * pi / segments;
    Triangle<Vertex2f_Color> t(Vertex2f_Color(p, color),
                               Vertex2f_Color(Point2f(), color),
                               Vertex2f_Color(Point2f(), color));
    Material m("", color);
    t.fax_Material(&m);

    for(size_t i = begin; i != segments && i != end; ++i) {
      t.b.position.x = p.x - radius * sin(arc * i);
      t.b.position.y = p.y - radius * cos(arc * i);
      t.c.position.x = p.x - radius * sin(arc * (i + 1));
      t.c.position.y = p.y - radius * cos(arc * (i + 1));
      m_vbo->fax_triangle(&t);
    }
  }

  void Logo::create_hollow_ellipse(const Point2f &p, const Point2f &inner_radius, const Point2f &outer_radius, const Color &color, const size_t &segments, const size_t &begin, const size_t &end) {
    const float arc = 2.0f * pi / segments;
    Quadrilateral<Vertex2f_Color> q(Vertex2f_Color(p, color),
                                    Vertex2f_Color(p, color),
                                    Vertex2f_Color(p, color),
                                    Vertex2f_Color(p, color));
    Material m("", color);
    q.fax_Material(&m);

    for(size_t i = begin; i != segments && i != end; ++i) {
      q.a.position.x = p.x - inner_radius.x * sin(arc * i);
      q.a.position.y = p.y - inner_radius.y * cos(arc * i);
      q.b.position.x = p.x - outer_radius.x * sin(arc * i);
      q.b.position.y = p.y - outer_radius.y * cos(arc * i);
      q.c.position.x = p.x - outer_radius.x * sin(arc * (i + 1));
      q.c.position.y = p.y - outer_radius.y * cos(arc * (i + 1));
      q.d.position.x = p.x - inner_radius.x * sin(arc * (i + 1));
      q.d.position.y = p.y - inner_radius.y * cos(arc * (i + 1));
      m_vbo->fax_quadrilateral(&q);
    }
  }

}
