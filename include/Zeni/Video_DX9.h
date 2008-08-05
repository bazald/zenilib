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

#include <Zeni/Color.h>
#include <Zeni/Video.h>

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
    inline void render_impl(const Renderable &renderable); ///< Render a Renderable

    // Accessors
    inline int get_maximum_anisotropy_impl() const; ///< Get the current level of anisotrophy
    inline bool has_vertex_buffers_impl() const; ///< Determine whether Vertex_Buffers are supported

    // Modifiers
    inline void set_2d_view_impl(const std::pair<Point2f, Point2f> &camera2d, const std::pair<Point2i, Point2i> &viewport); ///< Set a 2D view for a viewport
    inline void set_3d_view_impl(const Camera &camera, const std::pair<Point2i, Point2i> &viewport); ///< Set a 3D view for a viewport
    inline void set_backface_culling_impl(const bool &on = true); ///< Set backface culling on/off
    inline void set_vertical_sync_impl(const bool &on = true); ///< Set vertical_sync on/off
    inline void set_zwrite_impl(const bool &enabled); ///< Enable or disable writing to the Z-Buffer
    inline void set_ztest_impl(const bool &enabled); ///< Enable or disable testing of the Z-Buffer

    // Color and Texturing
    inline void set_color_impl(const Color & /*color*/); ///< Set the current color
    inline void set_clear_color_impl(const Color & /*color*/) {} ///< Set the blank background color
    inline void apply_texture_impl(const unsigned long &id); ///< Apply a texture by id
    inline void apply_texture_impl(const Texture &texture); ///< Apply a texture by id
    inline void unapply_texture_impl(); ///< Unapply a texture

    // Lighting and Materials
    inline void set_lighting_impl(const bool &on = true); ///< Set lighting on/off
    inline void set_normal_interpolation_impl(const bool &on = true); ///< Set normal interpolation on/off
    inline void set_ambient_lighting_impl(const Color &color); ///< Set ambient lighting on/off
    inline void set_light_impl(const int &number, const Light * const light = 0); ///< Set a particular Light
    inline void set_material_impl(const Material &material, const int &optimization = 0); ///< Set a Material
    inline void unset_material_impl(const Material &material, const int &optimization = 0); ///< Set a Material

    // Fog
    inline void set_fog_impl(const Fog * const fog = 0); ///< Set Fog on/off

    // Model/World Transformation Stack Functions
    inline void select_world_matrix_impl() {} ///< Select the world (model view) matrix; Call before [translate/rotate/scale] scene
    inline void push_world_stack_impl(); ///< Push a model view matrix onto the stack
    inline void pop_world_stack_impl(); ///< Pop a model view matrix off the stack
    inline void translate_scene_impl(const Vector3f &direction); ///< Translate the scene
    inline void rotate_scene_impl(const Vector3f &about, const float &radians); ///< Rotate the scene
    inline void scale_scene_impl(const Vector3f &factor); ///< Scale the scene
    inline void transform_scene_impl(const Matrix4f &transformation); ///< Transform the scene

    // View+Projection Matrix Functions
    inline Point2f get_pixel_offset_impl() const; ///< Get the pixel offset in the 2d view
    inline void set_view_matrix_impl(const Matrix4f &view); ///< Set the view Matrix4f
    inline void set_projection_matrix_impl(const Matrix4f &projection); ///< Set the projection Matrix4f
    inline void set_viewport_impl(const std::pair<Point2i, Point2i> &viewport); ///< Set the viewport

    // Creation Functions
    inline Texture * load_Texture_impl(const std::string &filename, const bool &repeat); ///< Function for loading a Texture; used internally by Textures
    inline Texture * create_Texture_impl(SDL_Surface * const &surface, const bool &repeat); ///< Function for creating a Texture from an SDL_Surface
    inline Font * create_Font_impl(const std::string &filename, const bool &bold, const bool &italic, 
      const int &glyph_height); ///< Function for creating a Font; used internally by Fonts
    inline Vertex_Buffer * create_Vertex_Buffer_impl(); ///< Function for creating a Vertex_Buffer

    // D3D-Related Functions
    inline const D3DCAPS9 & get_d3d_capabilities(); ///< See DirectX Documentation for details
    inline LPDIRECT3D9 & get_d3d(); ///< See DirectX Documentation for details
    inline LPDIRECT3DDEVICE9 & get_d3d_device(); ///< See DirectX Documentation for details
    inline LPD3DXMATRIXSTACK & get_matrix_stack(); ///< Get access to a matrix stack stored by this class
    inline const int & get_dpi(); ///< Get the screen's DPI
    inline float get_dpi_ratio(); ///< Get the ratio of the screen's DPI to the normal DPI

    // Manual 3D Flips
    bool get_3d() const;
    void set_3d(const bool &on);

  protected:
    virtual void init();
    bool init_device();
    void init_context();
    void destroy_device();
    inline void uninit_impl();

  private:
    void set_fvf();

    D3DCAPS9 m_d3d_capabilities;
    D3DPRESENT_PARAMETERS m_d3d_parameters;

    LPDIRECT3D9 m_d3d;
    LPDIRECT3DDEVICE9 m_d3d_device;
    LPD3DXMATRIXSTACK m_matrix_stack;
    int m_dpi;

    Color m_ambient_color;

    bool m_textured, m_3d;
  };

  struct Video_Device_Failure : public Error {
    Video_Device_Failure() : Error("Zeni Video Device Failed") {}
  };

}

#endif

#endif

#ifdef ZENI_INLINES
#include <Zeni/Video_DX9.hxx>
#endif
