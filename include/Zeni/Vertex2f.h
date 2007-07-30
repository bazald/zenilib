/* This file is part of the Zenipex Library.
* Copyleft (C) 2007 Mitchell Keith Bloch a.k.a. bazald
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
 * \class Zeni::Vertex2f_Color
 *
 * \ingroup Zenilib
 *
 * \brief An Abstraction of a Vertex in 2-space, colored
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Vertex2f_Texture
 *
 * \ingroup Zenilib
 *
 * \brief An Abstraction of a Vertex in 2-space, textured
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_VERTEX2F_H
#define ZENI_VERTEX2F_H

#include <Zeni/Color.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Render_Wrapper.h>

#include <string>

namespace Zeni {

  class Vertex2f {
  public:
    Vertex2f(const Point2f &position);
    Vertex2f(const Point3f &position = Point3f());
    virtual ~Vertex2f();

    virtual Point3f get_position() const;
    inline void set_position(const Point3f &position);
    inline void set_position(const Point2f &position);

    inline void * get_address() const; ///< A bit of a hack, necessary to memcpy data into Vertex_Buffers
    inline unsigned long get_offset() const; ///< A bit of a hack, necessary to memcpy data into Vertex_Buffers

  private:
    Point3f m_position;
  };

  class Vertex2f_Color : public Renderable, public Vertex2f {
  public:
    /// Initialize the Vertex2f_Color
    Vertex2f_Color(const Point2f &position = Point2f(), const Color &color = Color());
    Vertex2f_Color(const Point2f &position, const long &argb);

    Vertex2f * interpolate_to(const float &rhs_part, const Vertex2f_Color &rhs) const; ///< Get a Vertex2f_Color between two vertices; rhs must be a Vertex2f_Color

    inline const unsigned long & get_color() const; ///< Get the current Color
    inline void set_color(const Color &color); ///< Set the current Color
    inline void set_color(const unsigned long &argb); ///< Set the current Color

    // Begin rendering functions

    virtual Point3f get_position() const; ///< Get the current position

#ifndef DISABLE_GL
    virtual void render_to(Video_GL &screen) const;
    virtual void subrender_to(Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    virtual void render_to(Video_DX9 &screen) const;
#endif

  private:
    unsigned long m_argb;
  };

  class Vertex2f_Texture : public Renderable, public Vertex2f {
  public:
    /// Initialize the Vertex2f_Texture
    Vertex2f_Texture(const Point2f &position = Point2f(), 
      const Point2f &texture_coordinate = Point2f());

    Vertex2f * interpolate_to(const float &rhs_part, const Vertex2f_Texture &rhs) const; ///< Get a Vertex2f_Texture between two vertices; rhs must be a Vertex2f_Texture

    inline Point2f get_texture_coordinate() const; ///< Get the current Texture coordinate
    inline void set_texture_coordinate(const Point2f &texture_coordinate); ///< Set the current Texture coordinate

    // Begin rendering functions

    virtual Point3f get_position() const; ///< Get the current position

    // NOTE: Not really meaningful to render a textured point in isolation;
    //       assert(false) for all but subrender_to(Video_GL &)

#ifndef DISABLE_GL
    virtual void render_to(Video_GL &screen) const;
    virtual void subrender_to(Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    virtual void render_to(Video_DX9 &screen) const;
#endif

  private:
    Point2f m_texture_coordinate;
  };

}

#ifdef ZENI_INLINES
#include <Zeni/Vertex2f.hxx>
#endif

#endif
