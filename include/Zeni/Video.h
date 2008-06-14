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
 * \class Zeni::Video
 *
 * \ingroup Zenilib
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
#include <Zeni/Matrix4f.h>

#include <SDL/SDL.h>
#include <string>
#include <cmath>

namespace Zeni {

  enum VIDEO_MODE {ZENI_VIDEO_ANY
#ifndef DISABLE_GL
    , ZENI_VIDEO_GL
#endif
#ifndef DISABLE_DX9
    , ZENI_VIDEO_DX9
#endif
  };

  class Camera;
  class Font;
  class Light;
  class Material;
  class Renderable;
  class Texture;
  class Vertex_Buffer;

  class Video {
  protected:
    Video();
    virtual ~Video();

  private:
    // Undefined
    Video(const Video &);
    Video & operator=(const Video &);

  public:
    // Get reference to only instance; Might throw Video_Init_Failure
    static Video & get_reference(); ///< Get access to the singleton

    // Rendering functions
    virtual void render_all() = 0; ///< Render the scene
    virtual void render(const Renderable &renderable) = 0; ///< Render a Renderable

    // Accessors
    inline static const VIDEO_MODE & get_video_mode(); ///< Get the current VIDEO_MODE
    inline static const int & get_screen_width(); ///< Get the width of the screen
    inline static const int & get_screen_height(); ///< Get the height of the screen
    inline static const bool & is_fullscreen(); ///< Determine whether the screen is windowed or full screen
    inline static const bool & is_frame_visible(); ///< Determine whether the frame is visible (windowed only)
    inline static const bool & get_backface_culling(); ///< Determine whether backface culling is enabled
    inline static const bool & get_lighting(); ///< Determine whether dynamic lighting is enabled
    inline static const bool & get_normal_interpolation(); ///< Determine whether normal interpolation is enabled
    inline static const bool & get_vertical_sync(); ///< Determine whether vertical sync is enabled
    inline static const int & get_multisampling(); ///< Get the current level of multisampling
    virtual int get_maximum_anisotropy() const = 0; ///< Get the current level of anisotrophy
    virtual bool has_vertex_buffers() const = 0; ///< Determine whether Vertex_Buffers are supported
    inline const bool & zwrite_enabled() const; ///< Determine whether writing to Z-Buffer is enabled
    inline const bool & ztest_enabled() const; ///< Determine whether testing the Z-Buffer is enabled

    // Modifiers
    void set_2d(); ///< Set the default 2D view filling the entire display area
    void set_2d(const std::pair<Point2f, Point2f> &camera2d); ///< Set a 2D view for the entire viewing area
    void set_3d(const Camera &camera); ///< Set a 3D view filling the entire display area
    virtual void set_2d_view(const std::pair<Point2f, Point2f> &camera2d, const std::pair<Point2i, Point2i> &viewport); ///< Set a 2D view for a viewport
    virtual void set_3d_view(const Camera &camera, const std::pair<Point2i, Point2i> &viewport); ///< Set a 3D view for a viewport
    virtual void set_backface_culling(const bool &on = true); ///< Set backface culling on/off
    virtual void set_vertical_sync(const bool &on = true); ///< Set vertical_sync on/off
    virtual void set_zwrite(const bool &enabled); ///< Enable or disable writing to the Z-Buffer
    virtual void set_ztest(const bool &enabled); ///< Enable or disable testing of the Z-Buffer

    // Color and Texturing
    inline const Color & get_color() const; ///< Get the current color
    inline const Color & get_clear_color() const; ///< Get the blank background color
    virtual void set_color(const Color &color); ///< Set the current color
    virtual void set_clear_color(const Color &color); ///< Set the blank background color
    virtual void apply_texture(const std::string &name) = 0; ///< Apply a texture by name
    virtual void apply_texture(const unsigned long &id) = 0; ///< Apply a texture by id
    virtual void apply_texture(const Texture &texture) = 0; ///< Apply a texture by id
    virtual void unapply_texture() = 0; ///< Unapply a texture

    // Lighting and Materials
    virtual void set_lighting(const bool &on = true); ///< Set lighting on/off
    virtual void set_normal_interpolation(const bool &on = true); ///< Set normal interpolation on/off
    virtual void set_ambient_lighting(const Color &color) = 0; ///< Set ambient lighting on/off
    virtual void set_light(const int &number, const Light * const light = 0) = 0; ///< Set a particular Light
    virtual void set_material(const Material &material, const int &optimization = 0) = 0; ///< Set a Material
    virtual void unset_material(const Material &material, const int &optimization = 0) = 0; ///< Set a Material

    // Model/World Transformation Stack Functions
    virtual void select_world_matrix() = 0; ///< Select the world (model view) matrix; Call before [translate/rotate/scale] scene
    virtual void push_world_stack() = 0; ///< Push a model view matrix onto the stack
    virtual void pop_world_stack() = 0; ///< Pop a model view matrix off the stack
    virtual void translate_scene(const Vector3f &direction) = 0; ///< Translate the scene
    virtual void rotate_scene(const Vector3f &about, const float &radians) = 0; ///< Rotate the scene
    virtual void scale_scene(const Vector3f &factor) = 0; ///< Scale the scene
    virtual void transform_scene(const Matrix4f &transformation) = 0; ///< Transform the scene

    // View+Projection Matrix Functions
    inline const Matrix4f & get_view_matrix(); ///< Get the view Matrix4f
    inline const Matrix4f & get_projection_matrix(); ///< Get the projection Matrix4f
    inline const std::pair<Point2i, Point2i> & get_viewport() const; ///< Get the viewport
    inline const Matrix4f & get_world_to_screen_matrix() const; ///< For manual projection
    inline const Matrix4f & get_screen_to_world_matrix() const; ///< For picking
    virtual void set_view_matrix(const Matrix4f &view); ///< Set the view Matrix4f
    virtual void set_projection_matrix(const Matrix4f &projection); ///< Set the projection Matrix4f
    virtual void set_viewport(const std::pair<Point2i, Point2i> &viewport); ///< Set the viewport

    // Window Decorations
    void set_tt(const std::string &title, const std::string &taskmsg); ///< Set the window title and taskbar message
    void set_title(const std::string &title); ///< Set the window title
    void set_taskmsg(const std::string &taskmsg); ///< Set the taskbar message
    const bool set_icon(const std::string &filename); ///< Set the window icon

    // Creation Functions
    virtual Texture * load_Texture(const std::string &filename, const bool &repeat) = 0; ///< Function for loading a Texture; used internally by Textures
    virtual Font * create_Font(const std::string &filename, const bool &bold, const bool &italic, 
      const int &glyph_height) = 0; ///< Function for creating a Font; used internally by Fonts
    virtual Vertex_Buffer * create_Vertex_Buffer() = 0; ///< Function for creating a Vertex_Buffer

    // Initialization Checks and Changes
    inline static const bool & is_initialized(); ///< Determine whether Video is already initialized
    // Call before any other Video functions; May throw Video_Initialized
    static void preinit(const VIDEO_MODE &vm = ZENI_VIDEO_ANY, const int &w = 800, 
      const int &h = 600, const bool &full = false, const int &multisampling = 1, 
      const bool &show_frame_ = true); ///< Sets values for Video initialization

  protected:
    inline SDL_Surface * get_display_surface();

    inline const bool & get_opengl_flag() const;
    inline void set_opengl_flag(const bool &on = true);

    virtual void init();
    virtual void uninit();

  private:
    static Video *e_video;

    // Set title and taskmsg texts
    void set_tt();
    // Set icon
    bool set_icon();

    void regenerate_compound_matrices();

    SDL_Surface *m_display_surface, *m_icon_surface;

    static VIDEO_MODE g_video_mode;

    static int g_screen_width;
    static int g_screen_height;
    static bool g_screen_full;
    static bool g_screen_show_frame;
    static bool g_initialized;
    static bool g_backface_culling;
    static bool g_lighting;
    static bool g_normal_interp;
    static bool g_vertical_sync;
    static int g_multisampling;
    bool m_opengl_flag;

    std::string m_title;
    std::string m_taskmsg;
    std::string m_icon;

    Color m_color;
    Color m_clear_color;

    const Matrix4f m_preview;
    Matrix4f m_view;
    Matrix4f m_projection;
    Matrix4f m_world_to_screen;
    Matrix4f m_screen_to_world;
    std::pair<Point2i, Point2i> m_viewport;

    bool m_zwrite;
    bool m_ztest;
  };

  struct Video_Init_Failure : public Error {
    Video_Init_Failure() : Error("Zeni Video Failed to Initialize Correctly") {}
  };

  struct Video_Initialized : public Error {
    Video_Initialized() : Error("Zeni Video Already Initialized") {}
  };

}

#ifdef ZENI_INLINES
#include <Zeni/Video.hxx>
#endif

#endif
