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
 * \class Zeni::Video
 *
 * \ingroup zenilib
 *
 * \brief The Video Rendering Singleton
 *
 * Not literally related to video, as in movies, this base class supports 
 * graphics rendering.  If you do not intend to write your own Renderables, 
 * this might be all you need.  If you do intend to write your own Renderables, 
 * expect to use the derived Video classes as well.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_VIDEO_H
#define ZENI_VIDEO_H

#include <Zeni/Core.h>
#include <Zeni/Color.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Matrix4f.h>
#include <Zeni/Singleton.h>
#include <Zeni/String.h>

#include <SDL/SDL.h>
#include <cmath>
#include <vector>

namespace Zeni {

  class Camera;
  struct Fog;
  class Font;
  class Fragment_Shader;
  struct Light;
  class Material;
  class Renderable;
  class Shader_System;
  class Texture;
  class Vertex_Buffer;
  class Vertex_Buffer_Renderer;
  class Vertex_Shader;
  class Video;
  class XML_Document;

#ifdef _WINDOWS
  ZENI_GRAPHICS_EXT template class ZENI_GRAPHICS_DLL Singleton<Video>;
#endif

  class ZENI_GRAPHICS_DLL Video : public Singleton<Video> {
    friend class Singleton<Video>;

    static Video * create();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    static Uninit g_uninit;
    static Reinit g_reinit;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

  protected:
    Video();
    virtual ~Video() = 0;

  private:
    // Undefined
    Video(const Video &);
    Video & operator=(const Video &);

  public:
    enum VIDEO_MODE {ZENI_VIDEO_ANY, ZENI_VIDEO_GL, ZENI_VIDEO_DX9};

    enum TEST {ZENI_NEVER = 0,
               ZENI_LESS = 1,
               ZENI_EQUAL = 2,
               ZENI_GREATER = 4,
               ZENI_NOT_EQUAL = 8,
               ZENI_LESS_OR_EQUAL = 3,
               ZENI_GREATER_OR_EQUAL = 6,
               ZENI_ALWAYS = 15};

    // Rendering functions
    virtual bool begin_prerender() = 0; ///< Must be called before begin_render
    virtual bool begin_render() = 0; ///< Must be called before all rendering functions; Returns true if rendering can proceed
    virtual void end_render() = 0; ///< Must be called after all rendering functions
    virtual void render(const Renderable &renderable) = 0; ///< Render a Renderable

    // Accessors
    inline static const VIDEO_MODE & get_video_mode(); ///< Get the currently selected video mode
    inline static const bool & get_backface_culling(); ///< Determine whether backface culling is enabled
    inline static const bool & get_lighting(); ///< Determine whether dynamic lighting is enabled
    inline static const Color & get_ambient_lighting(); ///< Get the current ambient lighting Color
    inline static const bool & get_normal_interpolation(); ///< Determine whether normal interpolation is enabled
    inline static const bool & get_vertical_sync(); ///< Determine whether vertical sync is enabled
    inline static const int & get_multisampling(); ///< Get the current level of multisampling
    virtual int get_maximum_anisotropy() const = 0; ///< Get the current level of anisotrophy
    virtual bool has_vertex_buffers() const = 0; ///< Determine whether Vertex_Buffers are supported
    inline const bool & is_zwrite_enabled() const; ///< Determine whether writing to Z-Buffer is enabled
    inline const bool & is_ztest_enabled() const; ///< Determine whether testing the Z-Buffer is enabled
    inline const bool & is_alpha_test_enabled() const; ///< Determine whether alpha testing is enabled
    inline const TEST & get_alpha_test_function() const; ///< Determine which alpha test is in use
    inline const float & get_alpha_test_value() const; ///< Determine what value the alpha test is comparing against
    inline const bool & is_3d() const; ///< Determine whether currently rendering in 3D

    // Modifiers
    inline void set_2d(); ///< Set the default 2D view filling the entire display area
    inline void set_2d(const std::pair<Point2f, Point2f> &camera2d,
                       const bool &fix_aspect_ratio = false); ///< Set a 2D view for the entire viewing area
    inline void set_3d(const Camera &camera); ///< Set a 3D view filling the entire display area
    virtual void set_2d_view(const std::pair<Point2f, Point2f> &camera2d,
      const std::pair<Point2i, Point2i> &viewport = std::make_pair(Point2i(), get().get_render_target_size()),
      const bool &fix_aspect_ratio = false) = 0; ///< Set a 2D view for a viewport
    virtual void set_3d_view(const Camera &camera, const std::pair<Point2i, Point2i> &viewport =
      std::make_pair(Point2i(), get().get_render_target_size())) = 0; ///< Set a 3D view for a viewport
    virtual void set_backface_culling(const bool &on) = 0; ///< Set backface culling on/off
    virtual void set_vertical_sync(const bool &on) = 0; ///< Set vertical_sync on/off
    virtual void set_zwrite(const bool &enabled) = 0; ///< Enable or disable writing to the Z-Buffer
    virtual void set_ztest(const bool &enabled) = 0; ///< Enable or disable testing of the Z-Buffer
    virtual void set_alpha_test(const bool &enabled, const TEST &test = ZENI_ALWAYS, const float &value = 0.0f) = 0; ///< Set the alpha test

    // Color and Texturing
    inline const Color & get_Color() const; ///< Get the current color
    inline const Color & get_clear_Color() const; ///< Get the blank background color
    virtual void set_Color(const Color &color) = 0; ///< Set the current color
    virtual void set_clear_Color(const Color &color) = 0; ///< Set the blank background color
    inline void apply_Texture(const String &name); ///< Apply a texture by name
    virtual void apply_Texture(const unsigned long &id) = 0; ///< Apply a texture by id
    virtual void apply_Texture(const Texture &texture) = 0; ///< Apply a texture by id
    virtual void unapply_Texture() = 0; ///< Unapply a texture

    // Lighting and Materials
    virtual void set_lighting(const bool &on = true) = 0; ///< Set lighting on/off
    virtual void set_ambient_lighting(const Color &color) = 0; ///< Set ambient lighting on/off
    const Light * const get_Light(const int &number) const; ///< Get pointer to Light, or 0 if no such Light
    virtual void set_Light(const int &number, const Light &light) = 0; ///< Set a particular Light
    virtual void unset_Light(const int &number) = 0; ///< Unset a particular Light
    virtual void set_Material(const Material &material) = 0; ///< Set a Material
    virtual void unset_Material(const Material &material) = 0; ///< Unset a Material

    // Fog
    const Fog * const get_Fog() const; ///< Get pointer to current Fog, or 0 if no Fog
    virtual void set_Fog(const Fog &fog) = 0; ///< Set Fog
    virtual void unset_Fog() = 0; ///< Unset Fog

#ifndef DISABLE_CG
    // Shaders
    virtual void set_vertex_shader(const Vertex_Shader &shader) = 0; ///< Enable a Vertex_Shader
    virtual void set_fragment_shader(const Fragment_Shader &shader) = 0; ///< Enable a Vertex_Shader
    virtual void unset_vertex_shader(const Vertex_Shader &shader) = 0; ///< Enable a Vertex_Shader
    virtual void unset_fragment_shader(const Fragment_Shader &shader) = 0; ///< Enable a Vertex_Shader
#endif

    // Render-to-texture
    virtual void set_render_target(Texture &texture) = 0; ///< Set a render target
    virtual void unset_render_target() = 0; ///< Unset a render target
    virtual void clear_render_target(const Color &color = Color(0.0f, 0.0f, 0.0f, 0.0f)) = 0; ///< Clear the viewport
    virtual const Point2i & get_render_target_size() const = 0; ///< Get the dimensions of the render target

    // Model/World Transformation Stack Functions
    virtual void select_world_matrix() = 0; ///< Select the world (model view) matrix; Call before [translate/rotate/scale] scene
    virtual void push_world_stack() = 0; ///< Push a model view matrix onto the stack
    virtual void pop_world_stack() = 0; ///< Pop a model view matrix off the stack
    virtual void translate_scene(const Vector3f &direction) = 0; ///< Translate the scene
    virtual void rotate_scene(const Vector3f &about, const float &radians) = 0; ///< Rotate the scene
    inline void rotate_scene(const Quaternion &rotation); ///< Rotate the scene
    virtual void scale_scene(const Vector3f &factor) = 0; ///< Scale the scene
    virtual void transform_scene(const Matrix4f &transformation) = 0; ///< Transform the scene

    // View+Projection Matrix Functions
    virtual Point2f get_pixel_offset() const = 0; ///< Get the pixel offset in the 2d view
    inline const Matrix4f & get_view_matrix() const; ///< Get the view Matrix4f
    inline const Matrix4f & get_projection_matrix() const; ///< Get the projection Matrix4f
    inline const std::pair<Point2i, Point2i> & get_viewport() const; ///< Get the viewport
    virtual void set_view_matrix(const Matrix4f &view) = 0; ///< Set the view Matrix4f
    virtual void set_projection_matrix(const Matrix4f &projection) = 0; ///< Set the projection Matrix4f
    virtual void set_viewport(const std::pair<Point2i, Point2i> &viewport =
      std::make_pair(Point2i(), get().get_render_target_size())) = 0; ///< Set the viewport
    void set_viewport(const float &aspect_ratio,
      const std::pair<Point2i, Point2i> &viewport =
      std::make_pair(Point2i(), get().get_render_target_size())); ///< Set the viewport
    std::pair<Point2i, Point2i> calculate_viewport(const std::pair<Point2f, Point2f> &camera2d,
      const std::pair<Point2i, Point2i> &viewport,
      const bool &fix_aspect_ratio); ///< Calculate the viewport
    std::pair<Point2i, Point2i> calculate_viewport(const float &aspect_ratio,
      const std::pair<Point2i, Point2i> &viewport =
      std::make_pair(Point2i(), get().get_render_target_size())) const; ///< Calculate the viewport

    // Creation Functions
    virtual Texture * load_Texture(const String &filename, const bool &repeat, const bool &lazy_loading = false) = 0; ///< Function for loading a Texture; used internally by Textures
    virtual Texture * create_Texture(const Image &image) = 0; ///< Function for creating a Texture from an Image
    virtual Texture * create_Texture(const Point2i &size, const bool &repeat) = 0; ///< Function for creating a Texture for render-to-texture
    virtual Font * create_Font(const String &filename, 
      const float &glyph_height, const float &virtual_screen_height) = 0; ///< Function for creating a Font; used internally by Fonts
    virtual Vertex_Buffer_Renderer * create_Vertex_Buffer_Renderer(Vertex_Buffer &vertex_buffer) = 0; ///< Function for creating a Vertex_Buffer_Renderer

#ifndef DISABLE_CG
    // Initialization Functions
    virtual void initialize(Shader_System &shader_system) = 0; ///< Initialize a Shader_System; Used by the Shader_System's constructor
    virtual void initialize(Vertex_Shader &shader, const String &filename, const String &entry_function) = 0; ///< Function for initializing a Vertex_Shader
    virtual void initialize(Fragment_Shader &shader, const String &filename, const String &entry_function) = 0; ///< Function for initializing a Fragment_Shader
#endif

    // Call before any other Video functions; May throw Video_Initialized
    static void preinit_video_mode(const VIDEO_MODE &vm = ZENI_VIDEO_ANY); ///< Set which rendering engine to use
    static void preinit_multisampling(const int &multisampling = 0); ///< Set a multisampling value
    static void preinit_vertical_sync(const bool &vertical_sync = false); ///< Set vertical_sync to true
    static void preinit_from_file(const String &filename); ///< Set rendering options from a file
    static void preinit_from_file(const XML_Document &file); ///< Set rendering options from a file

    // Re/uninitialize the Rendering Device
    static void change_resolution(const Point2i &resolution);
#ifndef ANDROID
    static void save(const bool &backup = true); ///< Save options
    static bool revert(); ///< Restore previously saved options; Returns true if successful
    static void set_failsafe_defaults(); ///< Set failsafe default options

    static void print_errors(); ///< Print any errors that may have occurred
#endif

  private:
    static VIDEO_MODE g_video_mode;

    static bool g_backface_culling;
    static bool g_lighting;
    static Color g_ambient_lighting;
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    static Unordered_Map<int, Light> g_lights;
    static Fog g_fog;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
    static bool g_fog_enabled;
    static Color g_clear_color;
    static bool g_normal_interp;
    static bool g_vertical_sync;
    static int g_multisampling;

    Color m_color;

    const Matrix4f m_preview;
    Matrix4f m_view;
    Matrix4f m_projection;
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::pair<Point2i, Point2i> m_viewport;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

    static bool g_zwrite;
    static bool g_ztest;

    bool m_alpha_test;
    TEST m_alpha_function;
    float m_alpha_value;

    bool m_3d;
  };

  ZENI_GRAPHICS_DLL Video & get_Video(); ///< Get access to the singleton.

  struct ZENI_GRAPHICS_DLL Video_Init_Failure : public Error {
    Video_Init_Failure() : Error("Zeni Video Failed to Initialize Correctly") {}
  };

  struct ZENI_GRAPHICS_DLL Light_Out_of_Range : public Error {
    Light_Out_of_Range() : Error("Light Set Out of Range [0,7]") {}
  };

  struct ZENI_GRAPHICS_DLL Video_Render_To_Texture_Error : public Error {
    Video_Render_To_Texture_Error() : Error("Zeni Video Render To Texture Encountered An Error") {}
  };

}

#endif
