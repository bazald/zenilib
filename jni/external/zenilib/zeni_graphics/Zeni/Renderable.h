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

  class ZENI_GRAPHICS_DLL Material;

  class ZENI_GRAPHICS_DLL Video_GL;
  class ZENI_GRAPHICS_DLL Video_DX9;

  // For any object that knows how to render itself
  class ZENI_GRAPHICS_DLL Renderable {
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

  struct ZENI_GRAPHICS_DLL Invalid_Vertex_Index : public Error {
    Invalid_Vertex_Index() : Error("Invalid Vertex Index") {}
  };

}

#endif
