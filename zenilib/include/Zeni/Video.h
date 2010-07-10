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
 * \note Initial attempts at making reinitializations possible proved too spotty.  This singleton requires program restarts for changes to take effect.
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
#include <Zeni/IV.h>
#include <Zeni/Matrix4f.h>

#include <SDL/SDL.h>
#include <string>
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

  class Video_Base {
  public:
    enum VIDEO_MODE {ZENI_VIDEO_ANY = 0
#ifndef DISABLE_GL
      , ZENI_VIDEO_GL = 1
#endif
#ifndef DISABLE_DX9
      , ZENI_VIDEO_DX9 = 2
#endif
    };

    typedef ::IV<Video_Base, VIDEO_MODE> IV;
  };

  Video & get_Video(); ///< Get access to the singleton.

  class Video : public Video_Base::IV {
    // Get reference to only instance;
    friend Video & get_Video(); ///< Get access to the singleton.
    friend class Video_Destroyer; ///< Destroy the Video object as needed

  protected:
    Video(const Video_Base::VIDEO_MODE &vtype_);
    virtual ~Video() = 0;

  private:
    // Undefined
    Video(const Video &);
    Video & operator=(const Video &);

  public:
    enum TEST {ZENI_NEVER = 0,
               ZENI_LESS = 1,
               ZENI_EQUAL = 2,
               ZENI_GREATER = 4,
               ZENI_NOT_EQUAL = 8,
               ZENI_LESS_OR_EQUAL = 3,
               ZENI_GREATER_OR_EQUAL = 6,
               ZENI_ALWAYS = 15};

    // Rendering functions
    virtual void render_all() = 0; ///< Render the scene
    inline void render(const Renderable &renderable); ///< Render a Renderable

    // Accessors
    inline static const Video_Base::VIDEO_MODE & get_video_mode(); ///< Get the current VIDEO_MODE
    inline static const bool & is_enabled(); ///< Determine whether the use of rendering is desired
    inline static const Point2i & get_screen_size(); /// Get the size of the screen
    inline static const int & get_screen_width(); ///< Get the width of the screen
    inline static const int & get_screen_height(); ///< Get the height of the screen
    inline static const bool & is_fullscreen(); ///< Determine whether the screen is windowed or full screen
    inline static const bool & is_frame_visible(); ///< Determine whether the frame is visible (windowed only)
    inline static const bool & is_resizable(); ///< Determine whether the frame is resizable (windowed only)
    inline static const bool & get_backface_culling(); ///< Determine whether backface culling is enabled
    inline static const bool & get_lighting(); ///< Determine whether dynamic lighting is enabled
    inline static const Color & get_ambient_lighting(); ///< Get the current ambient lighting Color
    inline static const bool & get_normal_interpolation(); ///< Determine whether normal interpolation is enabled
    inline static const bool & get_vertical_sync(); ///< Determine whether vertical sync is enabled
    inline static const int & get_multisampling(); ///< Get the current level of multisampling
    inline int get_maximum_anisotropy() const; ///< Get the current level of anisotrophy
    inline bool has_vertex_buffers() const; ///< Determine whether Vertex_Buffers are supported
    inline const bool & is_zwrite_enabled() const; ///< Determine whether writing to Z-Buffer is enabled
    inline const bool & is_ztest_enabled() const; ///< Determine whether testing the Z-Buffer is enabled
    inline const bool & is_alpha_test_enabled() const; ///< Determine whether alpha testing is enabled
    inline const TEST & get_alpha_test_function() const; ///< Determine which alpha test is in use
    inline const float & get_alpha_test_value() const; ///< Determine what value the alpha test is comparing against
    inline const bool & is_3d() const; ///< Determine whether currently rendering in 3D
    inline const std::vector<Point2i> & get_resolutions() const; ///< Get available full screen resolutions

    // Modifiers
    inline void set_2d(); ///< Set the default 2D view filling the entire display area
    inline void set_2d(const std::pair<Point2f, Point2f> &camera2d,
                       const bool &fix_aspect_ratio = false); ///< Set a 2D view for the entire viewing area
    inline void set_3d(const Camera &camera); ///< Set a 3D view filling the entire display area
    inline void set_2d_view(const std::pair<Point2f, Point2f> &camera2d,
      const std::pair<Point2i, Point2i> &viewport = std::make_pair(Point2i(), get_Video().get_render_target_size()),
      const bool &fix_aspect_ratio = false); ///< Set a 2D view for a viewport
    inline void set_3d_view(const Camera &camera, const std::pair<Point2i, Point2i> &viewport =
      std::make_pair(Point2i(), get_Video().get_render_target_size())); ///< Set a 3D view for a viewport
    inline void set_backface_culling(const bool &on); ///< Set backface culling on/off
    inline void set_vertical_sync(const bool &on); ///< Set vertical_sync on/off
    inline void set_zwrite(const bool &enabled); ///< Enable or disable writing to the Z-Buffer
    inline void set_ztest(const bool &enabled); ///< Enable or disable testing of the Z-Buffer
    inline void set_alpha_test(const bool &enabled, const TEST &test = ZENI_ALWAYS, const float &value = 0.0f); ///< Set the alpha test

    // Color and Texturing
    inline const Color & get_color() const; ///< Get the current color
    inline const Color & get_clear_color() const; ///< Get the blank background color
    inline void set_color(const Color &color); ///< Set the current color
    inline void set_clear_color(const Color &color); ///< Set the blank background color
    inline void apply_texture(const std::string &name); ///< Apply a texture by name
    inline void apply_texture(const unsigned long &id); ///< Apply a texture by id
    inline void apply_texture(const Texture &texture); ///< Apply a texture by id
    inline void unapply_texture(); ///< Unapply a texture

    // Lighting and Materials
    inline void set_lighting(const bool &on = true); ///< Set lighting on/off
    inline void set_ambient_lighting(const Color &color); ///< Set ambient lighting on/off
    inline void set_light(const int &number, const Light &light); ///< Set a particular Light
    inline void unset_light(const int &number); ///< Unset a particular Light
    inline void set_material(const Material &material); ///< Set a Material
    inline void unset_material(const Material &material); ///< Unset a Material

    // Fog
    inline void set_fog(const Fog &fog); ///< Set Fog
    inline void unset_fog(); ///< Unset Fog

#ifndef DISABLE_CG
    // Shaders
    inline void set_vertex_shader(const Vertex_Shader &shader); ///< Enable a Vertex_Shader
    inline void set_fragment_shader(const Fragment_Shader &shader); ///< Enable a Vertex_Shader
    inline void unset_vertex_shader(const Vertex_Shader &shader); ///< Enable a Vertex_Shader
    inline void unset_fragment_shader(const Fragment_Shader &shader); ///< Enable a Vertex_Shader
#endif

    // Render-to-texture
    inline void set_render_target(Texture &texture); ///< Set a render target
    inline void unset_render_target(); ///< Unset a render target
    inline void clear_render_target(const Color &color = Color(0.0f, 0.0f, 0.0f, 0.0f)); ///< Clear the viewport
    inline const Point2i & get_render_target_size() const; ///< Get the dimensions of the render target

    // Model/World Transformation Stack Functions
    inline void select_world_matrix(); ///< Select the world (model view) matrix; Call before [translate/rotate/scale] scene
    inline void push_world_stack(); ///< Push a model view matrix onto the stack
    inline void pop_world_stack(); ///< Pop a model view matrix off the stack
    inline void translate_scene(const Vector3f &direction); ///< Translate the scene
    inline void rotate_scene(const Vector3f &about, const float &radians); ///< Rotate the scene
    inline void rotate_scene(const Quaternion &rotation); ///< Rotate the scene
    inline void scale_scene(const Vector3f &factor); ///< Scale the scene
    inline void transform_scene(const Matrix4f &transformation); ///< Transform the scene

    // View+Projection Matrix Functions
    inline Point2f get_pixel_offset() const; ///< Get the pixel offset in the 2d view
    inline const Matrix4f & get_view_matrix() const; ///< Get the view Matrix4f
    inline const Matrix4f & get_projection_matrix() const; ///< Get the projection Matrix4f
    inline const std::pair<Point2i, Point2i> & get_viewport() const; ///< Get the viewport
    inline void set_view_matrix(const Matrix4f &view); ///< Set the view Matrix4f
    inline void set_projection_matrix(const Matrix4f &projection); ///< Set the projection Matrix4f
    inline void set_viewport(const std::pair<Point2i, Point2i> &viewport =
      std::make_pair(Point2i(), get_Video().get_render_target_size())); ///< Set the viewport
    inline void set_viewport(const float &aspect_ratio,
      const std::pair<Point2i, Point2i> &viewport =
      std::make_pair(Point2i(), get_Video().get_render_target_size())); ///< Set the viewport

    // Window Decorations
    inline const std::string & get_title() const; ///< Get the window title
    inline const std::string & get_taskmsg() const; ///< Get the taskbar message
    static void set_tt(const std::string &title, const std::string &taskmsg); ///< Set the window title and taskbar message
    static void set_title(const std::string &title); ///< Set the window title
    static void set_taskmsg(const std::string &taskmsg); ///< Set the taskbar message
    static bool set_icon(const std::string &filename); ///< Set the window icon

    // Mouse Functions
    inline bool is_mouse_grabbed() const; ///< Find out if the (primary) Window has grabbed the mouse
    inline bool is_mouse_hidden() const; ///< Find out if the mouse cursor is hidden
    inline void mouse_grab(const bool &grab); ///< Tell the (primary) Window to grab/ungrab the mouse
    inline void mouse_hide(const bool &hide); ///< Hide/Unhide the mouse

    // Creation Functions
    inline Texture * load_Texture(const std::string &filename, const bool &repeat, const bool &lazy_loading = false); ///< Function for loading a Texture; used internally by Textures
    inline Texture * create_Texture(SDL_Surface * const &surface, const bool &repeat); ///< Function for creating a Texture from an SDL_Surface
    inline Texture * create_Texture(const Point2i &size, const bool &repeat); ///< Function for creating a Texture for render-to-texture
    inline Font * create_Font(const std::string &filename, const bool &bold, const bool &italic, 
      const float &glyph_height, const float &virtual_screen_height); ///< Function for creating a Font; used internally by Fonts
    inline Vertex_Buffer_Renderer * create_Vertex_Buffer_Renderer(Vertex_Buffer &vertex_buffer); ///< Function for creating a Vertex_Buffer_Renderer

    // Initialization Functions
    inline void initialize(Shader_System &shader_system); ///< Initialize a Shader_System; Used by the Shader_System's constructor
    inline void initialize(Vertex_Shader &shader, const std::string &filename, const std::string &entry_function); ///< Function for initializing a Vertex_Shader
    inline void initialize(Fragment_Shader &shader, const std::string &filename, const std::string &entry_function); ///< Function for initializing a Fragment_Shader

    // Initialization Checks and Changes
    inline static const bool & is_initialized(); ///< Determine whether Video is already initialized
    // Call before any other Video functions; May throw Video_Initialized
    static void preinit_video_mode(const Video_Base::VIDEO_MODE &vm = Video_Base::ZENI_VIDEO_ANY); ///< Set which rendering engine to use
    static void preinit_screen_resolution(const Point2i &resolution = Point2i(800, 600)); ///< Set the rendering window to have a given resolution
    static void preinit_full_screen(const bool &full_screen = false); ///< Set the rendering window to be full screen at launch
    static void preinit_multisampling(const int &multisampling = 0); ///< Set a multisampling value
    static void preinit_vertical_sync(const bool &vertical_sync = false); ///< Set vertical_sync to true
    static void preinit_show_frame(const bool &show_frame_ = true); ///< Show a frame around the rendering window when in windowed mode
    static void preinit_resizable(const bool &resizable_ = true); ///< Allow the frame to be dynamically resized when in windows mode
    static void preinit_from_file(const std::string &filename); ///< Set rendering options from a file
    static void preinit_from_file(const XML_Document &file); ///< Set rendering options from a file

    // Disable use of the Rendering Device (until Video is instantiated for the first time)
    static void set_enabled(const bool &enabled); ///< Enable/Disable the use of rendering; This will not close the rendering window once it is open

    // Re/uninitialize the Rendering Device
    static void destroy(); ///< Completely uninitialize the rendering device, if it is initialized
    static void reinit(const Point2i * const &resolution = 0); ///< Destroy, reload options, and reinitialize
    static void save(); ///< Save options
    static void set_failsafe_defaults(); ///< Set failsafe default options

#if SDL_VERSION_ATLEAST(1,3,0)
    inline SDL_Window * get_window(); ///< Get the (primary) window for your application
    virtual void alert_window_destroyed(); ///< Tell Video that its SDL_Window has been destroyed
#endif

  protected:
#if !SDL_VERSION_ATLEAST(1,3,0)
    inline SDL_Surface * get_display_surface();
#endif

    inline const bool & get_opengl_flag() const;
    inline void set_opengl_flag(const bool &on = true);

    virtual void init();
    inline void uninit();

  private:
    static Video *e_video;

    // Set title and taskmsg texts
    void set_tt();
    // Set icon
    bool set_icon();

#if SDL_VERSION_ATLEAST(1,3,0)
    SDL_Window *m_window;
#else
    SDL_Surface *m_display_surface;
#endif
    SDL_Surface *m_icon_surface;

    static Video_Base::VIDEO_MODE g_video_mode;
    static bool g_enabled;

    static Point2i g_screen_size;
    static bool g_screen_full;
    static bool g_screen_show_frame;
    static bool g_screen_resizable;
    static bool g_initialized;
    static bool g_backface_culling;
    static bool g_lighting;
    static Color g_ambient_lighting;
    static Color g_clear_color;
    static bool g_normal_interp;
    static bool g_vertical_sync;
    static int g_multisampling;
    bool m_opengl_flag;

    static std::string & get_m_title();
    static std::string & get_m_taskmsg();
    static std::string & get_m_icon();

    Color m_color;

    const Matrix4f m_preview;
    Matrix4f m_view;
    Matrix4f m_projection;
    std::pair<Point2i, Point2i> m_viewport;

    static bool g_zwrite;
    static bool g_ztest;

    bool m_alpha_test;
    TEST m_alpha_function;
    float m_alpha_value;

    bool m_3d;

    std::vector<Point2i> m_modes;
  };

  struct Video_Init_Failure : public Error {
    Video_Init_Failure() : Error("Zeni Video Failed to Initialize Correctly") {}
  };

  struct Light_Out_of_Range : public Error {
    Light_Out_of_Range() : Error("Light Set Out of Range [0,7]") {}
  };

  struct Video_Render_To_Texture_Error : public Error {
    Video_Render_To_Texture_Error() : Error("Zeni Video Render To Texture Encountered An Error") {}
  };

}

#endif
