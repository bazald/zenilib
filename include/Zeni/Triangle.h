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

/**
 * \class Zeni::Triangle<VERTEX>
 *
 * \ingroup Zenilib
 *
 * \brief An Abstraction of a Triangle
 *
 * \note Use a Texture_Render_Wrapper to avoid having to manually set and unset a texture each time the Triangle is rendered.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_TRIANGLE_H
#define ZENI_TRIANGLE_H

#include "Render_Wrapper.h"

#include <memory>

namespace Zeni {

  template <typename VERTEX>
  class Triangle : public Renderable {
  public:
    /// The best way to create a Triangle
    Triangle(const VERTEX &vertex0 = VERTEX(), const VERTEX &vertex1 = VERTEX(), 
      const VERTEX &vertex2 = VERTEX(), Render_Wrapper *render_wrapper = new Render_Wrapper());

    const VERTEX & get_vertex(const int &index) const; ///< Get a vertex
    void set_vertex(const int &index, const VERTEX &vertex); ///< Set a vertex

    // The "position" is the average of the three vertices
    virtual Point3f get_position() const; ///< Get the aveage of all vertices

#ifndef DISABLE_GL
    virtual void render_to(Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    virtual void render_to(Video_DX9 &screen) const;
#endif

    const Render_Wrapper * const get_render_wrapper() const; ///< Get the current Render_Wrapper
    Triangle<VERTEX> * get_duplicate() const; ///< Get a duplicate of the Triangle

    Triangle<VERTEX> * get_duplicate_subt0() const; ///< Get quarter 0 of the Triangle; Can be used for software LOD increase
    Triangle<VERTEX> * get_duplicate_subt1() const; ///< Get quarter 1 of the Triangle; Can be used for software LOD increase
    Triangle<VERTEX> * get_duplicate_subt2() const; ///< Get quarter 2 of the Triangle; Can be used for software LOD increase
    Triangle<VERTEX> * get_duplicate_subt3() const; ///< Get quarter 3 of the Triangle; Can be used for software LOD increase

  private:
    VERTEX m_vertex[3];
    std::auto_ptr<Render_Wrapper> m_render_wrapper;
  };

}

#ifdef ZENI_INLINES
#include "Triangle.hxx"
#endif

#endif
