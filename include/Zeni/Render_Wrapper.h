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

/**
 * \class Zeni::Renderable
 *
 * \ingroup Zenilib
 *
 * \brief A Renderable Interface
 *
 * This class provides an interface for anything that "knows how to render itself" 
 * it all rendering engines supported by Zenilib.
 *
 * \note Having a position in 3-space for each renderable can be useful for simple approximations of BSP-tree algorithms, but it may not suit your purposes to provide a meaningful result for the function call.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Render_Wrapper
 *
 * \ingroup Zenilib
 *
 * \brief A Render_Wrapper
 *
 * This base class acts as an empty wrapper for a Renderable.  It is useful 
 * to provide wrappers for renderables so that one need not remember to set and 
 * unset Textures, Materials, etc...
 *
 * \warning Comparisons are dangerous between different types of Render_Wrappers.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Material_Render_Wrapper
 *
 * \ingroup Zenilib
 *
 * \brief A Render_Wrapper for Materials
 *
 * This Render_Wrapper supports automatic setting of Materials.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_RENDER_WRAPPER_H
#define ZENI_RENDER_WRAPPER_H

#include <Zeni/Core.h>
#include <Zeni/Material.h>

#include <string>

namespace Zeni {

  class Video_GL;
  class Video_DX9;
  struct Point3f;

  // For any object that knows how to render itself
  class Renderable {
  public:
    virtual ~Renderable() {}

    // Returns the approximate distance between two Renderables
    float get_distance(const Renderable &renderable) const; ///< Get an approximate distance measurement to another Renderable
    float get_distance(const Point3f &position) const; ///< Get an approximate distance measurement to another Renderable

    // The "position" is the average of all vertices
    virtual Point3f get_position() const = 0; ///< Get an approximate position of this Renderable

#ifndef DISABLE_GL
    virtual void render_to(Video_GL &screen) const = 0; ///< Overridden for OpenGL rendering
#endif

#ifndef DISABLE_DX9
    virtual void render_to(Video_DX9 &screen) const = 0; ///< Overridden for Direct3D9 rendering
#endif
  };

  // For vertices to provide pre/postrendering functions to objects
  class Render_Wrapper {
  public:
    virtual ~Render_Wrapper() {};

    virtual void prerender() const {}; ///< To be called automatically before a Renderable is rendered
    virtual void postrender() const {}; ///< To be called automatically after a Renderable is rendered

    inline bool operator<(const Render_Wrapper &rhs) const; ///< Provide a total order
    inline bool operator==(const Render_Wrapper &rhs) const; ///< Provide an equality test

    virtual Render_Wrapper * get_duplicate() const; ///< Get a duplicate Render_Wrapper

    void optimize_to_follow(const Render_Wrapper &) {} ///< Optimize for application directly after rhs
    void optimize_to_precede(const Render_Wrapper &) {} ///< Optimize for application directly before rhs
    void clear_optimization() {} ///< Clear optimization
  };

  class Material_Render_Wrapper : public Render_Wrapper {
  public:
    Material_Render_Wrapper(const Material &material);

    inline const Material & get_material() const; ///< Get the wrapped Material

    virtual void prerender() const;
    virtual void postrender() const;

    inline bool operator<(const Material_Render_Wrapper &rhs) const;
    inline bool operator==(const Material_Render_Wrapper &rhs) const;

    virtual Render_Wrapper * get_duplicate() const;

    void optimize_to_follow(const Material_Render_Wrapper &rhs); ///< Optimize for application directly after rhs
    void optimize_to_precede(const Material_Render_Wrapper &rhs); ///< Optimize for application directly before rhs
    void clear_optimization(); ///< Clear optimization

    Material m_material;
    int optimization;
  };

  struct Invalid_Vertex_Index : public Error {
    Invalid_Vertex_Index() : Error("Invalid Vertex Index") {}
  };

}

#endif

#ifdef ZENI_INLINES
#include <Zeni/Render_Wrapper.hxx>
#endif
