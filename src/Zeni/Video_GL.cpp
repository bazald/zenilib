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

#include <Zeni/Video_GL.h>

#ifndef DISABLE_GL

#include <Zeni/Font.h>
#include <Zeni/Light.h>
#include <Zeni/Render_Wrapper.h>
#include <Zeni/Textures.h>
#include <Zeni/Vertex_Buffer.h>

#include <Zeni/Camera.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Game.hxx>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Matrix4f.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Video.hxx>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>

using namespace std;

namespace Zeni {

  Video_GL::Video_GL()
    : m_pglBindBufferARB(0),
      m_pglDeleteBuffersARB(0),
      m_pglGenBuffersARB(0),
      m_pglBufferDataARB(0),
      m_maximum_anisotropy(-1),
      m_vertex_buffers(false),
      m_zwrite(false),
      m_buffer_swap_end_time(0u),
      m_time_taken(0.0f),
      m_weight_new(0.2f)
  {
    init();
  }

  Video_GL::~Video_GL() {
    uninit();
  }

  void Video_GL::render_all() {
#ifdef _WINDOWS
    glFlush();
#else
    glFinish();
#endif

    glDepthMask(GL_TRUE);
    glViewport(0, 0, get_screen_width(), get_screen_height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Game::get_reference().render();
    
    /*** Begin CPU saver ***/
//#ifdef _WINDOWS
//    Timer &tr = Timer::get_reference();
//    
//    if(get_vertical_sync()) {
//      Time buffer_swap_start_time = tr.get_time();
//      
//      const unsigned int time_allowed = 1000u/60u - 2u;
//      const unsigned int new_time_taken = buffer_swap_start_time.get_ticks_since(m_buffer_swap_end_time);
//      m_time_taken = (1.0f - m_weight_new) * m_time_taken + m_weight_new * new_time_taken;
//      
//      if(m_time_taken < time_allowed)
//        SDL_Delay(unsigned int(time_allowed - m_time_taken));
//    }
//#endif
    
    /// Swap the buffers <-- NOT part of the CPU saver, but the reason it is "needed"
    SDL_GL_SwapBuffers();
    
//#ifdef _WINDOWS
//    m_buffer_swap_end_time = tr.get_time();
//#endif
  }

  void Video_GL::render(const Renderable &renderable) {
    renderable.render_to(*this);
  }

  int Video_GL::get_maximum_anisotropy() const {
    return m_maximum_anisotropy;
  }

  bool Video_GL::has_vertex_buffers() const {
    return m_vertex_buffers;
  }

  bool Video_GL::zwrite_enabled() const {
    return m_zwrite;
  }

  void Video_GL::set_color_to(const Color &color) {
    glColor4f(color.r(), color.g(), color.b(), color.a());
  }

  void Video_GL::set_clear_color_to(const Color &color) {
    glClearColor(color.r(), color.g(), color.b(), 0.0f);
  }

  void Video_GL::apply_texture(const std::string &name) {
    Video_GL::apply_texture(Textures::get_reference().get_texture_id(name));
  }

  void Video_GL::apply_texture(const unsigned long &id) {
    Textures::get_reference().apply_texture(id);

    glEnable(GL_TEXTURE_2D);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
  }

  void Video_GL::apply_texture(const Texture &texture) {
    texture.apply_texture();

    glEnable(GL_TEXTURE_2D);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
  }

  void Video_GL::unapply_texture() {
    glDisable(GL_TEXTURE_2D);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
  }

  void Video_GL::set_3d_view(const Camera &camera, const bool &on, const std::pair<Point2i, Point2i> &viewport) {
    if(on) {
      // Set Camera
      const Point3f &position = camera.get_position();
      const Vector3f &forward = camera.get_forward().normalized(),
        up = camera.get_up().normalized();
      const float
        &x = position.x, &y = position.y, &z = position.z,
        &i = forward.i, &j = forward.j, &k = forward.k;

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(x, y, z,
        x + i, y + j, z + k,
        up.i, up.j, up.k);

      // Enable Depth Buffer
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);
      glDepthMask(GL_TRUE);

      // Initialize Projection Matrix
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(camera.get_fov_deg(), float(viewport.second.x - viewport.first.x) / (viewport.second.y - viewport.first.y), camera.get_near_clip(), camera.get_far_clip());
    }
    else {
      glDisable(GL_DEPTH_TEST);
      glDepthMask(GL_FALSE);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(0, get_screen_width(), get_screen_height(), 0);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
    }

    glViewport(viewport.first.x, get_screen_height() - viewport.second.y, viewport.second.x - viewport.first.x, viewport.second.y - viewport.first.y);
  }

  void Video_GL::set_lighting(const bool &on) {
    Video::set_lighting(on);

    if(on)
      glEnable(GL_LIGHTING);
    else
      glDisable(GL_LIGHTING);
  }

  void Video_GL::set_normal_interpolation(const bool &on) {
    Video::set_normal_interpolation(on);

    if(on)
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    else
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
  }

  void Video_GL::set_ambient_lighting(const Color &color) {
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, reinterpret_cast<const GLfloat *>(&color));
  }

  void Video_GL::set_light(const int &number, const Light *light) {
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

  void Video_GL::set_material(const Material &material, const int &optimization) {
    material.set(*this, optimization);
  }

  void Video_GL::unset_material(const Material &material, const int &optimization) {
    material.unset(*this, optimization);
  }

  void Video_GL::select_world_matrix() {
    glMatrixMode(GL_MODELVIEW);
  }

  void Video_GL::push_world_stack() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
  }

  void Video_GL::pop_world_stack() {
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }

  void Video_GL::translate_scene(const Vector3f &direction) {
    glTranslatef(direction.i, direction.j, direction.k);
  }

  void Video_GL::rotate_scene(const Vector3f &about, const float &radians) {
    glRotatef(radians * 180.0f / pi, about.i, about.j, about.k);
  }

  void Video_GL::scale_scene(const Vector3f &factor) {
    glScalef(factor.i, factor.j, factor.k);
  }

  void Video_GL::transform_scene(const Matrix4f &transformation) {
    Matrix4f transpose = transformation.transposed(); // to get column-major matrix
    glMultMatrixf(reinterpret_cast<const GLfloat * const>(&transpose));
  }

  void Video_GL::set_backface_culling(const bool &on) {
    Video::set_backface_culling(on);

    if(on) {
      // Enable Backface Culling
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);

      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    }
    else {
      glDisable(GL_CULL_FACE);

      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    }
  }

#ifdef DISABLEWGL
  void Video_GL::set_vertical_sync(const bool &on) {
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, on);
#else
  void Video_GL::set_vertical_sync(const bool &on) {
    typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
    PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

    const char *extensions = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));

    if(strstr(extensions, "WGL_EXT_swap_control")) {
      wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

      if(wglSwapIntervalEXT)
        wglSwapIntervalEXT(on);

      Video::set_vertical_sync(on);
    }
#endif
  }

  void Video_GL::set_zwrite(const bool &enabled) {
    m_zwrite = enabled;

    if(m_zwrite)
      glDepthMask(GL_TRUE);
    else
      glDepthMask(GL_FALSE);
  }

  Texture * Video_GL::load_Texture(const std::string &filename, const bool &repeat) {
    return new Texture_GL(filename, repeat, *this);
  }

  Font * Video_GL::create_Font(const std::string &filename, const bool &bold, const bool &italic, const int &glyph_height) {
    return new Font_GL(filename, bold, italic, glyph_height);
  }

  Vertex_Buffer * Video_GL::create_Vertex_Buffer() {
    return new Vertex_Buffer_GL();
  }

  void Video_GL::init() {
    std::cout << "Initializing OpenGL" << endl;

    //double buffer, no stencil, no accumulation buffer
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 0);

    if(get_multisampling() > 1) {
      SDL_GL_SetAttribute (SDL_GL_MULTISAMPLESAMPLES, get_multisampling());
      SDL_GL_SetAttribute (SDL_GL_MULTISAMPLEBUFFERS, 1);
    }

    set_opengl_flag(true);
    Video::init();

    glViewport(0, 0, get_screen_width(), get_screen_height());
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Set Fill/Shade Mode
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_NORMALIZE); //GL_RESCALE_NORMALIZE);

    // Enable Alpha Blitting
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendEquation(GL_FUNC_ADD); // default // would require ARB ext

    // Initialize Assorted Variables
    glPointSize(static_cast<GLfloat>(sqrt(pow(double(get_screen_width()), 2.) * pow(double(get_screen_height()), 2.)) / 1000000));
    glLineWidth(static_cast<GLfloat>(sqrt(pow(double(get_screen_width()), 2.) * pow(double(get_screen_height()), 2.)) / 1000000));

    // Finish with a few function calls
    set_2d();
    set_backface_culling(false);
    set_vertical_sync(get_vertical_sync());
    set_lighting(get_lighting());
    set_ambient_lighting(Color());
    set_normal_interpolation(get_normal_interpolation());

    // Manage extensions
    union {
      PFNGLBINDBUFFERARBPROC proc;
      void *ptr;
    } uni;

    m_vertex_buffers = strstr(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)), "ARB_vertex_buffer_object") != 0;
    if(m_vertex_buffers) {
      uni.ptr = SDL_GL_GetProcAddress("glBindBufferARB");;
      m_pglBindBufferARB = (PFNGLBINDBUFFERARBPROC)uni.proc;

      uni.ptr = SDL_GL_GetProcAddress("glDeleteBuffersARB");
      m_pglDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)uni.proc;

      uni.ptr = SDL_GL_GetProcAddress("glGenBuffersARB");
      m_pglGenBuffersARB = (PFNGLGENBUFFERSARBPROC)uni.proc;

      uni.ptr = SDL_GL_GetProcAddress("glBufferDataARB");
      m_pglBufferDataARB = (PFNGLBUFFERDATAARBPROC)uni.proc;
    }

    if(strstr((char*)glGetString(GL_EXTENSIONS), "GL_EXT_texture_filter_anisotropic"))
      glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_maximum_anisotropy);
    else
      m_maximum_anisotropy = 0;
  }

}

#else

namespace Zeni {
  void * this_pointer_is_also_dead_beef = (void *)0xDEADBEEF;
}

#endif
