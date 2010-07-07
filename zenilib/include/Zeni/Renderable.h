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
 * \class Zeni::Renderable
 *
 * \ingroup zenilib
 *
 * \brief A Renderable Interface
 *
 * This class provides an interface for anything that "knows how to render itself" 
 * it all rendering engines supported by zenilib.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_RENDERABLE_H
#define ZENI_RENDERABLE_H

#include <Zeni/Core.h>

namespace Zeni {

  class Material;

  class Video_GL;
  class Video_DX9;

  // For any object that knows how to render itself
  class Renderable {
  public:
    inline Renderable();
    virtual ~Renderable();

    inline Renderable(const Renderable &rhs);
    inline Renderable & operator=(const Renderable &rhs);

    /// Tell the rendering system if we're using 3D coordinates
    virtual bool is_3d() const = 0;

#ifndef DISABLE_GL
    virtual void render_to(Video_GL &screen) const = 0; ///< Overridden for OpenGL rendering
#endif

#ifndef DISABLE_DX9
    virtual void render_to(Video_DX9 &screen) const = 0; ///< Overridden for Direct3D9 rendering
#endif

    virtual void pre_render() const;
    virtual void post_render() const;

    /** Manage Render_Wrappers **/

    inline const Material * get_Material() const; ///< Get the Material
    void give_Material(Material * const &material); ///< Set the Material, giving the Renderable ownership
    void lend_Material(const Material * const &material); ///< Set the Material, giving the Renderable no ownership
    void fax_Material(const Material * const &material); ///< Set the Material, giving the Renderable a copy

  private:
    Material * m_material;
    bool delete_m_material;
  };


  struct Invalid_Vertex_Index : public Error {
    Invalid_Vertex_Index() : Error("Invalid Vertex Index") {}
  };

}

#endif
