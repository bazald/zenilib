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
 * \class Zeni::Video_GL
 *
 * \ingroup Zenilib
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

#include "Video.h"

#ifndef DISABLE_GL

#include <GL/gl.h>
#include <GL/glext.h>

namespace Zeni {

  class Video_GL : public Video {
    friend class Video;

    Video_GL();
    ~Video_GL();

    // Undefined
    Video_GL(const Video_GL &);
    Video_GL & operator=(const Video_GL &);

  public:
    // Rendering functions
    virtual void render_all(); ///< Render the scene
    virtual void render(const Renderable &renderable); ///< Render a Renderable

    // Accessors
    virtual int get_maximum_anisotropy() const; ///< Get the current level of anisotrophy
    virtual bool has_vertex_buffers() const; ///< Determine whether Vertex_Buffers are supported

    // Modifiers
    virtual void set_3d_view(const Camera &camera, const bool &on, 
      const std::pair<Point2i, Point2i> &viewport); ///< Set a 3D view for a viewport
    virtual void set_backface_culling(const bool &on = true); ///< Set backface culling on/off
    virtual void set_vertical_sync(const bool &on = true); ///< Set vertical_sync on/off

    // Color and Texturing
    virtual void set_color_to(const Color &color); ///< Set the current color
    virtual void set_clear_color_to(const Color &color); ///< Set the blank background color
    virtual void apply_texture(const std::string &name); ///< Apply a texture by name
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
    virtual Texture * load_Texture(const std::string &name, const std::string &filename); ///< Function for loading a Texture; used internally by Textures
    virtual Font * create_Font(const std::string &filename, const bool &bold, const bool &italic, 
      const int &glyph_height); ///< Function for creating a Font; used internally by Fonts
    virtual Vertex_Buffer * create_Vertex_Buffer(); ///< Function for creating a Vertex_Buffer

    // Vertex Buffer Functions
    inline void pglBindBufferARB(const GLenum target, const GLuint buffer) const; ///< The glBindBufferARB OpenGL function as provided by an extension; Will segfault if has_vertex_buffers() returns false
    inline void pglDeleteBuffersARB(const GLsizei n, GLuint * const buffers) const; ///< The glDeleteBuffersARB OpenGL function as provided by an extension; Will segfault if has_vertex_buffers() returns false
    inline void pglGenBuffersARB(const GLsizei n, GLuint * const buffers) const; ///< The glGenBuffersARB OpenGL function as provided by an extension; Will segfault if has_vertex_buffers() returns false
    inline void pglBufferDataARB(const GLenum target, const int size, const GLvoid * const data, 
      const GLenum usage) const; ///< The glBufferDataARB OpenGL function as provided by an extension; Will segfault if has_vertex_buffers() returns false

  protected:
    virtual void init();

  private:
    PFNGLBINDBUFFERARBPROC m_pglBindBufferARB;
    PFNGLDELETEBUFFERSARBPROC m_pglDeleteBuffersARB;
    PFNGLGENBUFFERSARBPROC m_pglGenBuffersARB;
    PFNGLBUFFERDATAARBPROC m_pglBufferDataARB;

    int m_maximum_anisotropy;
    bool m_vertex_buffers;
  };

}

#endif

#ifdef ZENI_INLINES
#include "Video_GL.hxx"
#endif

#endif
