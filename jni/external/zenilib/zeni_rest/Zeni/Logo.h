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

  class ZENI_REST_DLL Logo {
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
