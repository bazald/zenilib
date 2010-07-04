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

/**
 * \class Zeni::Logo
 *
 * \ingroup zenilib
 *
 * \brief Logo
 *
 * This class allows rendering of the Zenipex logo.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_LOGO_H
#define ZENI_LOGO_H

#include <Zeni/Color.h>
#include <Zeni/Coordinate.h>

namespace Zeni {

  class Vertex_Buffer;

  class Logo {
  public:
    Logo(const Point2f &upper_left, const float &height, const Color &logo_color, const Color &text_color);
    ~Logo();

    Logo(const Logo &rhs);
    Logo & operator=(const Logo &rhs);

    void render();

  private:
    Point2f get_p00(const Point2f &upper_left, const float &height);
    Point2f get_p10(const Point2f &upper_left, const float &height);
    Point2f get_p20(const Point2f &upper_left, const float &height);
    Point2f get_p01(const Point2f &upper_left, const float &height);
    Point2f get_p11(const Point2f &upper_left, const float &height);
    Point2f get_p21(const Point2f &upper_left, const float &height);
    Point2f get_p02(const Point2f &upper_left, const float &height);
    Point2f get_p12(const Point2f &upper_left, const float &height);
    Point2f get_p22(const Point2f &upper_left, const float &height);
    float get_thickness(const float &height);

    void create_all();

    void create_logo(const Point2f &upper_left, const float &height);

    void create_z(const Point2f &upper_left, const float &height);
    void create_e(const Point2f &upper_left, const float &height);
    void create_n(const Point2f &upper_left, const float &height);
    void create_i(const Point2f &upper_left, const float &height);
    void create_p(const Point2f &upper_left, const float &height);
    void create_x(const Point2f &upper_left, const float &height);

    void create_thick_line(const Point2f &p0, const Point2f &p1, const float &thickness, const Color &color);
    void create_circle(const Point2f &p, const float &radius, const Color &color, const size_t &segments, const size_t &begin = 0u, const size_t &end = size_t(-1));
    void create_hollow_ellipse(const Point2f &p, const Point2f &inner_radius, const Point2f &outer_radius, const Color &color, const size_t &segments, const size_t &begin = 0u, const size_t &end = size_t(-1));

    Point2f m_upper_left;
    float m_height;
    Color m_logo_color;
    Color m_text_color;

    Vertex_Buffer * m_vbo;
  };

}

#endif
