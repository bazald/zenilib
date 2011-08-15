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
 * \class Zeni::Vertex3f_Color
 *
 * \ingroup zenilib
 *
 * \brief An Abstraction of a Vertex in 3-space, colored
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Vertex3f_Texture
 *
 * \ingroup zenilib
 *
 * \brief An Abstraction of a Vertex in 3-space, textured
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_VERTEX3F_H
#define ZENI_VERTEX3F_H

#include <Zeni/Color.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Line_Segment.h>
#include <Zeni/Quadrilateral.h>
#include <Zeni/Renderable.h>
#include <Zeni/String.h>
#include <Zeni/Triangle.h>
#include <Zeni/Vector3f.h>

namespace Zeni {

  class ZENI_GRAPHICS_DLL Vertex3f {
  protected:
    Vertex3f();
    Vertex3f(const Point3f &position, const Point3f &normal = Point3f());
  public:
    virtual ~Vertex3f();

    virtual Point3f get_position() const;

    inline void * get_address() const; ///< A bit of a hack, necessary to memcpy data into Vertex_Buffers
#if defined(_WINDOWS) && defined(X64)
    inline unsigned long long get_offset() const; ///< A bit of a hack, necessary to memcpy data into Vertex_Buffers
#else
    inline unsigned long get_offset() const; ///< A bit of a hack, necessary to memcpy data into Vertex_Buffers
#endif

    Point3f position;
    Point3f normal;
  };

  class ZENI_GRAPHICS_DLL Vertex3f_Color : public Renderable, public Vertex3f {
  public:
    /// Initialize the Vertex3f_Color
    Vertex3f_Color();
    Vertex3f_Color(const Point3f &position, const Point3f &normal, const Color &color);
    Vertex3f_Color(const Point3f &position, const Point3f &normal, const Uint32 &argb);
    Vertex3f_Color(const Point3f &position, const Color &color);
    Vertex3f_Color(const Point3f &position, const Uint32 &argb);

    Vertex3f_Color * interpolate_to(const float &rhs_part, const Vertex3f_Color &rhs) const; ///< Get a Vertex3f_Color between two vertices; rhs must be a Vertex3f_Color

    inline const Uint32 & get_Color() const; ///< Get the current Color
    inline void set_Color(const Color &color); ///< Set the current Color
    inline void set_Color(const Uint32 &argb); ///< Set the current Color

    // Begin rendering functions

    virtual bool is_3d() const; ///< Tell the rendering system if we're using 3D coordinates

#ifndef DISABLE_GL
    virtual void render_to(Video_GL &screen) const;
    virtual void subrender_to(Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    virtual void render_to(Video_DX9 &screen) const;
#endif

  private:
    Uint32 m_argb;
  };

  class ZENI_GRAPHICS_DLL Vertex3f_Texture : public Renderable, public Vertex3f {
  public:
    /// Initialize the Vertex3f_Texture
    Vertex3f_Texture();
    Vertex3f_Texture(const Point3f &position, const Point3f &normal, const Point2f &texture_coordinate);
    Vertex3f_Texture(const Point3f &position, const Point2f &texture_coordinate);

    Vertex3f_Texture * interpolate_to(const float &rhs_part, const Vertex3f_Texture &rhs) const; ///< Get a Vertex2f_Texture between two vertices; rhs must be a Vertex2f_Texture

    // Begin rendering functions

    virtual bool is_3d() const; ///< Tell the rendering system if we're using 3D coordinates

    // NOTE: Not really meaningful to render a textured point in isolation;
    //       assert(false) for all but subrender_to(Video_GL &)

#ifndef DISABLE_GL
    virtual void render_to(Video_GL &screen) const;
    virtual void subrender_to(Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    virtual void render_to(Video_DX9 &screen) const;
#endif

    Point2f texture_coordinate;
  };

#ifdef _WINDOWS
  ZENI_GRAPHICS_EXT template class ZENI_GRAPHICS_DLL Line_Segment<Vertex3f_Color>;
  ZENI_GRAPHICS_EXT template class ZENI_GRAPHICS_DLL Line_Segment<Vertex3f_Texture>;
  ZENI_GRAPHICS_EXT template class ZENI_GRAPHICS_DLL Quadrilateral<Vertex3f_Color>;
  ZENI_GRAPHICS_EXT template class ZENI_GRAPHICS_DLL Quadrilateral<Vertex3f_Texture>;
  ZENI_GRAPHICS_EXT template class ZENI_GRAPHICS_DLL Triangle<Vertex3f_Color>;
  ZENI_GRAPHICS_EXT template class ZENI_GRAPHICS_DLL Triangle<Vertex3f_Texture>;
#endif

}

#endif
