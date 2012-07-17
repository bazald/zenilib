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
 * \class Zeni::Video_DX9
 *
 * \ingroup zenilib
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

typedef enum _D3DFORMAT D3DFORMAT;
typedef enum _D3DPOOL D3DPOOL;
typedef struct _D3DCAPS9 D3DCAPS9, *LPD3DCAPS9;
typedef struct _D3DPRESENT_PARAMETERS_ D3DPRESENT_PARAMETERS, *LPD3DPRESENT_PARAMETERS;
typedef struct ID3DXMatrixStack *LPD3DXMATRIXSTACK;
typedef struct ID3DXRenderToSurface *LPD3DXRENDERTOSURFACE;
typedef struct IDirect3D9 *LPDIRECT3D9;
typedef struct IDirect3DBaseTexture9 *LPDIRECT3DBASETEXTURE9;
typedef struct IDirect3DDevice9 *LPDIRECT3DDEVICE9;
typedef struct IDirect3DSurface9 *LPDIRECT3DSURFACE9;
typedef struct IDirect3DTexture9 *LPDIRECT3DTEXTURE9;
typedef struct tagPALETTEENTRY PALETTEENTRY;

namespace Zeni {

  struct Color;
  class Texture_DX9;

  class ZENI_GRAPHICS_DLL Video_DX9 : public Video {
    friend class Video;
    friend ZENI_GRAPHICS_DLL Video & get_Video();

    Video_DX9();
    ~Video_DX9();

    // Undefined
    Video_DX9(const Video_DX9 &);
    Video_DX9 & operator=(const Video_DX9 &);

  public:
    typedef LPDIRECT3D9 (WINAPI *Direct3DCreate9_fcn)(UINT SDKVersion);
    typedef HRESULT (WINAPI *D3DXCreateRenderToSurface_fcn)(
      LPDIRECT3DDEVICE9       pDevice,
      UINT                    Width,
      UINT                    Height,
      D3DFORMAT               Format,
      BOOL                    DepthStencil,
      D3DFORMAT               DepthStencilFormat,
      LPD3DXRENDERTOSURFACE*  ppRenderToSurface);
    typedef HRESULT (WINAPI *D3DXCreateTexture_fcn)(
      LPDIRECT3DDEVICE9         pDevice,
      UINT                      Width,
      UINT                      Height,
      UINT                      MipLevels,
      DWORD                     Usage,
      D3DFORMAT                 Format,
      D3DPOOL                   Pool,
      LPDIRECT3DTEXTURE9*       ppTexture);
    typedef HRESULT (WINAPI *D3DXFilterTexture_fcn)(
      LPDIRECT3DBASETEXTURE9    pBaseTexture,
      CONST PALETTEENTRY*       pPalette,
      UINT                      SrcLevel,
      DWORD                     Filter);
    typedef HRESULT (WINAPI *D3DXCreateMatrixStack_fcn)(
      DWORD               Flags, 
      LPD3DXMATRIXSTACK*  ppStack);

    // DLL Functions
    static Direct3DCreate9_fcn Direct3DCreate9() {return g_Direct3DCreate9;}
    static D3DXCreateRenderToSurface_fcn D3DXCreateRenderToSurface() {return g_D3DXCreateRenderToSurface;}
    static D3DXCreateTexture_fcn D3DXCreateTexture() {return g_D3DXCreateTexture;}
    static D3DXFilterTexture_fcn D3DXFilterTexture() {return g_D3DXFilterTexture;}
    static D3DXCreateMatrixStack_fcn D3DXCreateMatrixStack() {return g_D3DXCreateMatrixStack;}

    // Rendering functions
    bool begin_prerender(); ///< Must be called before begin_render
    bool begin_render(); ///< Must be called before all rendering functions; Returns true if rendering can proceed
    void end_render(); ///< Must be called after all rendering functions
    void render(const Renderable &renderable); ///< Render a Renderable

    // Accessors
    int get_maximum_anisotropy() const; ///< Get the current level of anisotrophy
    bool has_vertex_buffers() const; ///< Determine whether Vertex_Buffers are supported

    // Modifiers
    void set_2d_view(const std::pair<Point2f, Point2f> &camera2d,
      const std::pair<Point2i, Point2i> &viewport = std::make_pair(Point2i(), get_Video().get_render_target_size()),
      const bool &fix_aspect_ratio = false); ///< Set a 2D view for a viewport
    void set_3d_view(const Camera &camera,
      const std::pair<Point2i, Point2i> &viewport = std::make_pair(Point2i(), get_Video().get_render_target_size())); ///< Set a 3D view for a viewport
    void set_backface_culling(const bool &on); ///< Set backface culling on/off
    void set_vertical_sync(const bool &on); ///< Set vertical_sync on/off
    void set_zwrite(const bool &enabled); ///< Enable or disable writing to the Z-Buffer
    void set_ztest(const bool &enabled); ///< Enable or disable testing of the Z-Buffer
    void set_alpha_test(const bool &enabled, const TEST &test, const float &value); ///< Set the alpha test

    // Color and Texturing
    void set_Color(const Color & /*color*/); ///< Set the current color
    void set_clear_Color(const Color &color) {Video::set_clear_Color(color);} ///< Set the blank background color
    inline void apply_Texture(const String &name) {return Video::apply_Texture(name);} ///< Apply a texture by name
    void apply_Texture(const unsigned long &id); ///< Apply a texture by id
    void apply_Texture(const Texture &texture); ///< Apply a texture by id
    void unapply_Texture(); ///< Unapply a texture

    // Lighting and Materials
    void set_lighting(const bool &on = true); ///< Set lighting on/off
    void set_ambient_lighting(const Color &color); ///< Set ambient lighting on/off
    void set_Light(const int &number, const Light &light); ///< Set a particular Light
    void unset_Light(const int &number); ///< Unset a particular Light
    void set_Material(const Material &material); ///< Set a Material
    void unset_Material(const Material &material); ///< Unset a Material

    // Fog
    void set_Fog(const Fog &fog); ///< Set Fog
    void unset_Fog(); ///< Unset Fog

#ifndef DISABLE_CG
    // Shaders
    void set_vertex_shader(const Vertex_Shader &shader); ///< Enable a Vertex_Shader
    void set_fragment_shader(const Fragment_Shader &shader); ///< Enable a Vertex_Shader
    void unset_vertex_shader(const Vertex_Shader &shader); ///< Enable a Vertex_Shader
    void unset_fragment_shader(const Fragment_Shader &shader); ///< Enable a Vertex_Shader
#endif

    // Render-to-texture
    void set_render_target(Texture &texture); ///< Set a render target
    void unset_render_target(); ///< Unset a render target
    void clear_render_target(const Color &color = Color(0.0f, 0.0f, 0.0f, 0.0f)); ///< Clear the viewport
    const Point2i & get_render_target_size() const; ///< Get the dimensions of the render target

    // Model/World Transformation Stack Functions
    void select_world_matrix() {} ///< Select the world (model view) matrix; Call before [translate/rotate/scale] scene
    void push_world_stack(); ///< Push a model view matrix onto the stack
    void pop_world_stack(); ///< Pop a model view matrix off the stack
    void translate_scene(const Vector3f &direction); ///< Translate the scene
    void rotate_scene(const Vector3f &about, const float &radians); ///< Rotate the scene
    void scale_scene(const Vector3f &factor); ///< Scale the scene
    void transform_scene(const Matrix4f &transformation); ///< Transform the scene

    // View+Projection Matrix Functions
    Point2f get_pixel_offset() const; ///< Get the pixel offset in the 2d view
    void set_view_matrix(const Matrix4f &view); ///< Set the view Matrix4f
    void set_projection_matrix(const Matrix4f &projection); ///< Set the projection Matrix4f
    void set_viewport(const std::pair<Point2i, Point2i> &viewport =
      std::make_pair(Point2i(), get_Video().get_render_target_size())); ///< Set the viewport

    // Creation Functions
    Texture * load_Texture(const String &filename, const bool &repeat, const bool &lazy_loading = false); ///< Function for loading a Texture; used internally by Textures
    Texture * create_Texture(const Image &image); ///< Function for creating a Texture from an Image
    Texture * create_Texture(const Point2i &size, const bool &repeat); ///< Function for creating a Texture for render-to-texture
    Font * create_Font(const String &filename, 
      const float &glyph_height, const float &virtual_screen_height); ///< Function for creating a Font; used internally by Fonts
    Vertex_Buffer_Renderer * create_Vertex_Buffer_Renderer(Vertex_Buffer &vertex_buffer); ///< Function for creating a Vertex_Buffer_Renderer

#ifndef DISABLE_CG
    // Initialization Functions
    void initialize(Shader_System &shader_system); ///< Initialize a Shader_System; Used by the Shader_System's constructor
    void initialize(Vertex_Shader &shader, const String &filename, const String &entry_function); ///< Function for initializing a Vertex_Shader
    void initialize(Fragment_Shader &shader, const String &filename, const String &entry_function); ///< Function for initializing a Fragment_Shader
#endif

    // D3D-Related Functions
    const D3DCAPS9 & get_d3d_capabilities(); ///< See DirectX Documentation for details
    const D3DPRESENT_PARAMETERS & get_d3d_parameters(); ///< See DirectX Documentation for details
    inline LPDIRECT3D9 & get_d3d(); ///< See DirectX Documentation for details
    inline LPDIRECT3DDEVICE9 & get_d3d_device(); ///< See DirectX Documentation for details
    inline LPD3DXMATRIXSTACK & get_matrix_stack(); ///< Get access to a matrix stack stored by this class
    inline const int & get_dpi(); ///< Get the screen's DPI
    inline float get_dpi_ratio(); ///< Get the ratio of the screen's DPI to the normal DPI

    // Custom D3D-Related Functions
    bool is_fvf_3d() const;
    void set_fvf_3d(const bool &on);
    void set_fvf(const bool &is_3d);

  protected:
    void init();
    bool init_device();
    void init_context();
    void destroy_device();

  private:
    void zero_handles();

    HMODULE m_d3d9;
    static Direct3DCreate9_fcn g_Direct3DCreate9;

    HMODULE m_d3dx9;
    static D3DXCreateRenderToSurface_fcn g_D3DXCreateRenderToSurface;
    static D3DXCreateTexture_fcn g_D3DXCreateTexture;
    static D3DXFilterTexture_fcn g_D3DXFilterTexture;
    static D3DXCreateMatrixStack_fcn g_D3DXCreateMatrixStack;

    void set_fvf();

    LPD3DCAPS9 m_d3d_capabilities;
    LPD3DPRESENT_PARAMETERS m_d3d_parameters;

    LPDIRECT3D9 m_d3d;
    LPDIRECT3DDEVICE9 m_d3d_device;
    LPD3DXMATRIXSTACK m_matrix_stack;
    int m_dpi;
    bool m_begun_render;

    bool m_textured, m_fvf_3d;

    Texture_DX9 * m_render_target;
    ID3DXRenderToSurface * m_render_to_surface;
    LPDIRECT3DSURFACE9 m_back_buffer;
  };

  struct ZENI_GRAPHICS_DLL Video_Device_Failure : public std::exception {
    Video_Device_Failure() : std::exception("Zeni Video Device Failed") {}
  };

}

#endif

#endif
