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
 * \class Zeni::Video_GL
 *
 * \ingroup zenilib
 *
 * \brief The OpenGL Rendering System
 *
 * This class supports OpenGL-specific rendering.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_VIDEO_GL_H
#define ZENI_VIDEO_GL_H

#include <Zeni/Timer.h>
#include <Zeni/Video.h>

#ifndef DISABLE_GL

#if defined(REQUIRE_GL_ES)
#include <GLES/gl.h>
#define GL_ARRAY_BUFFER_ARB GL_ARRAY_BUFFER
#define GL_STATIC_DRAW_ARB GL_STATIC_DRAW
#define glBindFramebufferEXT glBindFramebuffer
#define glBindRenderbufferEXT glBindRenderbuffer
#define glFogi glFogx
#define glFramebufferRenderbufferEXT glFramebufferRenderbuffer
#define glFramebufferTexture2DEXT glFramebufferTexture2D
#define glGenFramebuffersEXT glGenFramebuffers
#define glGenRenderbuffersEXT glGenRenderbuffers
#define glRenderbufferStorageEXT glRenderbufferStorage
#else
#include <GL/glew.h>
#ifdef _LINUX
namespace GLXEW {
struct _XDisplay;
typedef _XDisplay Display;
typedef long unsigned int XID;
typedef XID GLXDrawable;
typedef void ( * PFNGLXSWAPINTERVALEXTPROC) (Display* dpy, GLXDrawable drawable, int interval);
typedef int ( * PFNGLXSWAPINTERVALSGIPROC) (int interval);
}
#endif
#endif

namespace Zeni {

  class Texture_GL;

  class ZENI_GRAPHICS_DLL Video_GL : public Video {
    friend class Video;
    friend ZENI_GRAPHICS_DLL Video & get_Video();

    Video_GL();
    ~Video_GL();

    // Undefined
    Video_GL(const Video_GL &);
    Video_GL & operator=(const Video_GL &);

  public:
    // Rendering functions
    bool begin_prerender(); ///< Must be called before begin_render
    bool begin_render(); ///< Must be called before all rendering functions; Returns true if rendering can proceed
    void end_render(); ///< Must be called after all rendering functions
    void render(const Renderable &renderable); ///< Render a Renderable

    // Accessors
    int get_maximum_anisotropy() const; ///< Get the current level of anisotrophy
    bool has_vertex_buffers() const; ///< Determine whether Vertex_Buffers are supported

    // Modifiers
    void set_2d_view(const std::pair<Point2f, Point2f> & /*camera2d*/,
      const std::pair<Point2i, Point2i> & /*viewport*/ = std::make_pair(Point2i(), get_Video().get_render_target_size()),
      const bool &fix_aspect_ratio = false); ///< Set a 2D view for a viewport
    void set_3d_view(const Camera & /*camera*/,
      const std::pair<Point2i, Point2i> & /*viewport*/ = std::make_pair(Point2i(), get_Video().get_render_target_size())); ///< Set a 3D view for a viewport
    void set_backface_culling(const bool &on); ///< Set backface culling on/off
    void set_vertical_sync(const bool &on); ///< Set vertical_sync on/off
    void set_zwrite(const bool &enabled); ///< Enable or disable writing to the Z-Buffer
    void set_ztest(const bool &enabled); ///< Enable or disable testing of the Z-Buffer
    void set_alpha_test(const bool &enabled, const TEST &test, const float &value); ///< Set the alpha test

    // Color and Texturing
    void set_Color(const Color &color); ///< Set the current color
    void set_clear_Color(const Color &color); ///< Set the blank background color
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
    void select_world_matrix(); ///< Select the world (model view) matrix; Call before [translate/rotate/scale] scene
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

    // Vertex Buffer Functions
    inline void pglBindBufferARB(const GLenum target, const GLuint buffer) const; ///< The glBindBufferARB OpenGL function as provided by an extension; Will segfault if has_vertex_buffers() returns false
    inline void pglDeleteBuffersARB(const GLsizei n, GLuint * const buffers) const; ///< The glDeleteBuffersARB OpenGL function as provided by an extension; Will segfault if has_vertex_buffers() returns false
    inline void pglGenBuffersARB(const GLsizei n, GLuint * const buffers) const; ///< The glGenBuffersARB OpenGL function as provided by an extension; Will segfault if has_vertex_buffers() returns false
    inline void pglBufferDataARB(const GLenum target, const int size, const GLvoid * const data, 
      const GLenum usage) const; ///< The glBufferDataARB OpenGL function as provided by an extension; Will segfault if has_vertex_buffers() returns false
    
#ifndef REQUIRE_GL_ES
    PFNGLBINDBUFFERARBPROC get_pglBindBufferARB() const {return m_pglBindBufferARB;}
    PFNGLDELETEBUFFERSARBPROC get_pglDeleteBuffersARB() const {return m_pglDeleteBuffersARB;}
    PFNGLGENBUFFERSARBPROC get_pglGenBuffersARB() const {return m_pglGenBuffersARB;}
    PFNGLBUFFERDATAARBPROC get_pglBufferDataARB() const {return m_pglBufferDataARB;}
#endif

#if SDL_VERSION_ATLEAST(1,3,0)
    virtual void alert_window_destroyed(); ///< Tell Video that its SDL_Window has been destroyed
#endif

  protected:
    void init();
    void uninit();

  private:
#if SDL_VERSION_ATLEAST(1,3,0)
    SDL_GLContext m_context;
#endif

#ifndef REQUIRE_GL_ES
#ifdef _LINUX
    GLXEW::PFNGLXSWAPINTERVALEXTPROC m_pglSwapIntervalEXT;
    GLXEW::PFNGLXSWAPINTERVALSGIPROC m_pglSwapIntervalSGI;
#endif
    PFNGLBINDBUFFERARBPROC m_pglBindBufferARB;
    PFNGLDELETEBUFFERSARBPROC m_pglDeleteBuffersARB;
    PFNGLGENBUFFERSARBPROC m_pglGenBuffersARB;
    PFNGLBUFFERDATAARBPROC m_pglBufferDataARB;
#endif

    int m_maximum_anisotropy;
    bool m_vertex_buffers, m_zwrite;

    Texture_GL * m_render_target;

#ifdef MANUAL_GL_VSYNC_DELAY
    Zeni::Time m_buffer_swap_end_time;
    float m_time_taken;
    float m_weight_new;
#endif
  };

}

#endif

#endif
