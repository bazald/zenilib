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

#ifndef ZENI_VIDEO_GL_HXX
#define ZENI_VIDEO_GL_HXX

// HXXed below
#include <Zeni/Color.h>
#include <Zeni/Game.h>
#include <Zeni/Light.h>
#include <Zeni/Render_Wrapper.h>
#include <Zeni/Video.h>

#include <Zeni/Video_GL.h>

// Not HXXed
#include <Zeni/Fog.h>
#include <Zeni/Vertex_Buffer.h>

#ifndef DISABLE_GL

namespace Zeni {

  void Video_GL::render_impl(const Renderable &renderable) {
    renderable.render_to(*this);
  }

  int Video_GL::get_maximum_anisotropy_impl() const {
    return m_maximum_anisotropy;
  }

  bool Video_GL::has_vertex_buffers_impl() const {
    return m_vertex_buffers;
  }

  void Video_GL::set_backface_culling_impl(const bool &on) {
    if(on) {
      // Enable Backface Culling
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
    }
    else
      glDisable(GL_CULL_FACE);
  }

#ifdef DISABLEWGL
  void Video_GL::set_vertical_sync_impl(const bool &on) {
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, on);
#else
  void Video_GL::set_vertical_sync_impl(const bool &on) {
    typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
    PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

    const char *extensions = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));

    if(strstr(extensions, "WGL_EXT_swap_control")) {
      wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

      if(wglSwapIntervalEXT)
        wglSwapIntervalEXT(on);
    }
#endif
  }

  void Video_GL::set_zwrite_impl(const bool &enabled) {
    glDepthMask(enabled);
  }

  void Video_GL::set_ztest_impl(const bool &enabled) {
    if(enabled) {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);
    }
    else
      glDisable(GL_DEPTH_TEST);
  }

  void Video_GL::set_color_impl(const Color &color) {
    glColor4f(color.r, color.g, color.b, color.a);
  }

  void Video_GL::set_clear_color_impl(const Color &color) {
    glClearColor(color.r, color.g, color.b, 0.0f);
  }

  void Video_GL::apply_texture_impl(const unsigned long &id) {
    get_Textures().apply_texture(id);
  }

  void Video_GL::apply_texture_impl(const Texture &texture) {
    texture.apply_texture();
  }

  void Video_GL::unapply_texture_impl() {
    glDisable(GL_TEXTURE_2D);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
  }

  void Video_GL::set_lighting_impl(const bool &on) {
    if(on)
      glEnable(GL_LIGHTING);
    else
      glDisable(GL_LIGHTING);
  }

  void Video_GL::set_normal_interpolation_impl(const bool &on) {
    if(on)
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    else
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
  }

  void Video_GL::set_ambient_lighting_impl(const Color &color) {
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, reinterpret_cast<const GLfloat *>(&color));
  }

  void Video_GL::set_light_impl(const int &number, const Light *light) {
    GLint ln;
    switch(number) {
    case 1: ln = GL_LIGHT1; break;
    case 2: ln = GL_LIGHT2; break;
    case 3: ln = GL_LIGHT3; break;
    case 4: ln = GL_LIGHT4; break;
    case 5: ln = GL_LIGHT5; break;
    case 6: ln = GL_LIGHT6; break;
    case 7: ln = GL_LIGHT7; break;
    case 0: 
    default:
      ln = GL_LIGHT0; break;
    }

    if(light)
      light->set(ln, *this);
    else
      glDisable(ln);
  }

  void Video_GL::set_material_impl(const Material &material, const int &optimization) {
    material.set(*this, optimization);
  }

  void Video_GL::unset_material_impl(const Material &material, const int &optimization) {
    material.unset(*this, optimization);
  }

  void Video_GL::set_fog_impl(const Fog * const fog) {
    if(fog) {
      glEnable(GL_FOG);
      fog->set(*this);
    }
    else
      glDisable(GL_FOG);
  }

  void Video_GL::select_world_matrix_impl() {
    glMatrixMode(GL_MODELVIEW);
  }

  void Video_GL::push_world_stack_impl() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
  }

  void Video_GL::pop_world_stack_impl() {
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }

  void Video_GL::translate_scene_impl(const Vector3f &direction) {
    glTranslatef(direction.i, direction.j, direction.k);
  }

  void Video_GL::rotate_scene_impl(const Vector3f &about, const float &radians) {
    glRotatef(radians * 180.0f / pi, about.i, about.j, about.k);
  }

  void Video_GL::scale_scene_impl(const Vector3f &factor) {
    glScalef(factor.i, factor.j, factor.k);
  }

  void Video_GL::transform_scene_impl(const Matrix4f &transformation) {
    glMultMatrixf(reinterpret_cast<const GLfloat * const>(&transformation));
  }

  Point2f Video_GL::get_pixel_offset_impl() const {
    return Point2f(0.0f, 0.0f);
  }

  void Video_GL::set_view_matrix_impl(const Matrix4f &view) {
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(reinterpret_cast<GLfloat *>(const_cast<Matrix4f *>(&view)));
  }

  void Video_GL::set_projection_matrix_impl(const Matrix4f &projection) {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(reinterpret_cast<GLfloat *>(const_cast<Matrix4f *>(&projection)));
  }

  void Video_GL::set_viewport_impl(const std::pair<Point2i, Point2i> &viewport) {
    glViewport(viewport.first.x, get_screen_height() - viewport.second.y, viewport.second.x - viewport.first.x, viewport.second.y - viewport.first.y);
  }

  Texture * Video_GL::load_Texture_impl(const std::string &filename, const bool &repeat, const bool &lazy_loading) {
    return new Texture_GL(filename, repeat, lazy_loading);
  }

  Texture * Video_GL::create_Texture_impl(SDL_Surface * const &surface, const bool &repeat) {
    return new Texture_GL(surface, repeat);
  }

  Font * Video_GL::create_Font_impl(const std::string &filename, const bool &bold, const bool &italic, const int &glyph_height) {
    return new Font_FT(filename, bold, italic, glyph_height);
  }

  Vertex_Buffer * Video_GL::create_Vertex_Buffer_impl() {
    return new Vertex_Buffer_GL();
  }

  void Video_GL::pglBindBufferARB(const GLenum target, const GLuint buffer) const {
    m_pglBindBufferARB(target, buffer);
  }

  void Video_GL::pglDeleteBuffersARB(const GLsizei n, GLuint * const buffers) const {
    m_pglDeleteBuffersARB(n, buffers);
  }

  void Video_GL::pglGenBuffersARB(const GLsizei n, GLuint * const buffers) const {
    m_pglGenBuffersARB(n, buffers);
  }

  void Video_GL::pglBufferDataARB(const GLenum target, const int size, const GLvoid * const data, const GLenum usage) const {
    m_pglBufferDataARB(target, size, data, usage);
  }

}

#endif

#include <Zeni/Color.hxx>
#include <Zeni/Game.hxx>
#include <Zeni/Light.hxx>
#include <Zeni/Render_Wrapper.hxx>
#include <Zeni/Video.hxx>

#endif
