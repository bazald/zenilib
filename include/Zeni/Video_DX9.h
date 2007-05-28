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
 * \class Zeni::Video_DX9
 *
 * \ingroup Zenilib
 *
 * \brief The Direct3D9 Rendering System
 *
 * This class supports Direct3D9-specific rendering.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_VIDEO_DX9_H
#define ZENI_VIDEO_DX9_H

#include "Color.h"
#include "Video.h"

#ifndef DISABLE_DX9

#include <d3d9.h>
#include <d3dx9.h>

namespace Zeni {

  class Color;

  class Video_DX9 : public Video {
    friend class Video;

    Video_DX9();
    ~Video_DX9();

    // Undefined
    Video_DX9(const Video_DX9 &);
    Video_DX9 & operator=(const Video_DX9 &);

  public:
    // Rendering functions
    virtual void render_all(); ///< Render the scene
    virtual void render(const Renderable &renderable); ///< Render a Renderable

    // Accessors
    virtual int get_maximum_anisotropy() const; ///< Get the current level of anisotrophy
    virtual bool has_vertex_buffers() const; ///< Determine whether Vertex_Buffers are supported
    virtual bool zwrite_enabled() const; ///< Determine whether writing to Z-Buffer is enabled

    // Modifiers
    virtual void set_3d_view(const Camera &camera, const bool &on, 
      const std::pair<Point2i, Point2i> &viewport); ///< Set a 3D view for a viewport
    virtual void set_backface_culling(const bool &on = true); ///< Set backface culling on/off
    virtual void set_vertical_sync(const bool &on = true); ///< Set vertical_sync on/off
    virtual void set_zwrite(const bool &enabled); ///< Enable or disable writing to the Z-Buffer

    // Color and Texturing
    virtual void set_color_to(const Color &color); ///< Set the current color
    virtual void set_clear_color_to(const Color &color); ///< Set the blank background color
    virtual void apply_texture(const std::string &name); ///< Apply a texture by name
    virtual void apply_texture(const unsigned long &id); ///< Apply a texture by id
    virtual void unapply_texture(); ///< Unapply a texture

    // Lighting and Materials
    virtual void set_lighting(const bool &on = true); ///< Set lighting on/off
    virtual void set_normal_interpolation(const bool &on = true); ///< Set normal interpolation on/off
    virtual void set_ambient_lighting(const Color &color); ///< Set ambient lighting on/off
    virtual void set_light(const int &number, const Light * const light = 0); ///< Set a particular Light
    virtual void set_material(const Material &material, const int &optimization = 0); ///< Set a Material
    virtual void unset_material(const Material &material, const int &optimization = 0); ///< Set a Material

    // Model Stack Functions
    virtual void select_world_matrix(); ///< Select the world (model view) matrix; Call before [translate/rotate/scale] scene
    virtual void push_world_stack(); ///< Push a model view matrix onto the stack
    virtual void pop_world_stack(); ///< Pop a model view matrix off the stack
    virtual void translate_scene(const Vector3f &direction); ///< Translate the scene
    virtual void rotate_scene(const Vector3f &about, const float &radians); ///< Rotate the scene
    virtual void scale_scene(const Vector3f &factor); ///< Scale the scene

    // Creation Functions
    virtual Texture * load_Texture(const std::string &filename); ///< Function for loading a Texture; used internally by Textures
    virtual Font * create_Font(const std::string &filename, const bool &bold, const bool &italic, 
      const int &glyph_height); ///< Function for creating a Font; used internally by Fonts
    virtual Vertex_Buffer * create_Vertex_Buffer(); ///< Function for creating a Vertex_Buffer

    // D3D-Related Functions
    inline const D3DCAPS9 & get_d3d_capabilities(); ///< See DirectX Documentation for details
    inline LPDIRECT3D9 & get_d3d(); ///< See DirectX Documentation for details
    inline LPDIRECT3DDEVICE9 & get_d3d_device(); ///< See DirectX Documentation for details
    inline LPD3DXMATRIXSTACK & get_matrix_stack(); ///< Get access to a matrix stack stored by this class

  protected:
    virtual void init();
    virtual void uninit();

  private:
    void set_fvf();

    D3DCAPS9 m_d3d_capabilities;
    LPDIRECT3D9 m_d3d;
    LPDIRECT3DDEVICE9 m_d3d_device;
    LPD3DXMATRIXSTACK m_matrix_stack;

    Color m_color;
    Color m_clear_color;

    bool m_textured, m_3d, m_zwrite;
  };

}

#endif

#ifdef ZENI_INLINES
#include "Video_DX9.hxx"
#endif

#endif
