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

#include <zeni_graphics.h>

#ifndef DISABLE_GL

#if defined(REQUIRE_GL_ES)
#include <GLES/gl.h>
#else
#include <GL/glew.h>
#endif

#include <SDL/SDL.h>
#ifndef TEMP_DISABLE
#ifdef REQUIRE_GL_ES
#include <SDL/SDL_opengles.h>
#else
namespace SDLOPENGL {
#include <SDL/SDL_opengl.h>
}
#endif
#endif

#ifdef _LINUX
namespace GLXEW {
#include <GL/glxew.h>
}
#endif

#include <GLSLANG/ShaderLang.h>

#include <iostream>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#ifndef TEMP_DISABLE
#ifdef REQUIRE_GL_ES
extern "C" GL_API int GL_APIENTRY _dgles_load_library(void *, void *(*)(void *, const char *));

static void * proc_loader(void *, const char * name) {
	return SDL_GL_GetProcAddress(name);
}
#endif
#endif

namespace Zeni {

  Video_GL_Fixed::Video_GL_Fixed()
    :
#if SDL_VERSION_ATLEAST(1,3,0)
      m_context(0),
#endif
#ifdef _LINUX
      m_pglSwapIntervalEXT(0),
#endif
#ifndef REQUIRE_GL_ES
      m_pglBindBufferARB(0),
      m_pglDeleteBuffersARB(0),
      m_pglGenBuffersARB(0),
      m_pglBufferDataARB(0),
#endif
      m_maximum_anisotropy(-1),
      m_zwrite(false),
      m_render_target(0)
#ifdef MANUAL_GL_VSYNC_DELAY
      ,
      m_buffer_swap_end_time(0u),
      m_time_taken(0.0f),
      m_weight_new(0.2f)
#endif
  {
    init();
  }

  Video_GL_Fixed::~Video_GL_Fixed() {
    uninit();
  }

  bool Video_GL_Fixed::begin_prerender() {
    assert(!m_render_target);

#ifdef _WINDOWS
    glFlush();
#else
    glFinish();
#endif

    get_Textures().unlose_resources();
    get_Fonts().unlose_resources();

    return true;
  }

  bool Video_GL_Fixed::begin_render() {
    assert(!m_render_target);

    glViewport(0, 0, get_Window().get_width(), get_Window().get_height());

    if(!is_zwrite_enabled())
      glDepthMask(GL_TRUE);
    set_clear_Color(get_clear_Color());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!is_zwrite_enabled())
      glDepthMask(GL_FALSE);

    return true;
  }

  void Video_GL_Fixed::end_render() {
    /*** Begin CPU saver ***/
#ifdef MANUAL_GL_VSYNC_DELAY
   Timer &tr = get_Timer();
   
   if(get_vertical_sync()) {
     Time buffer_swap_start_time = tr.get_time();
     
     const unsigned int time_allowed = 1000u/60u - 2u;
     const unsigned int new_time_taken = buffer_swap_start_time.get_ticks_since(m_buffer_swap_end_time);
     m_time_taken = (1.0f - m_weight_new) * m_time_taken + m_weight_new * new_time_taken;
     
     if(m_time_taken < time_allowed)
       SDL_Delay(static_cast<unsigned int>(time_allowed - m_time_taken));
   }
#endif
    
    /// Swap the buffers <-- NOT part of the CPU saver, but the reason it is "needed"
#ifndef TEMP_DISABLE
#if SDL_VERSION_ATLEAST(1,3,0)
   SDL_GL_SwapWindow(get_Window().get_window());
#else
   SDL_GL_SwapBuffers();
#endif
#endif
    
#ifdef MANUAL_GL_VSYNC_DELAY
   m_buffer_swap_end_time = tr.get_time();
#endif
  }

#if SDL_VERSION_ATLEAST(1,3,0)
  void Video_GL_Fixed::alert_window_destroyed() {
    m_context = 0;
  }
#endif

  void Video_GL_Fixed::render(const Renderable &renderable) {
    class PrePostRenderActor {
      PrePostRenderActor & operator=(const PrePostRenderActor &) {return *this;}

    public:
      PrePostRenderActor(const Renderable &renderable_)
        : renderable(renderable_)
      {
        renderable.pre_render();
      }

      ~PrePostRenderActor() {
        renderable.post_render();
      }
    private:
      const Renderable &renderable;
    } ppra(renderable);

    renderable.render_to(*this);
  }

  void Video_GL_Fixed::clear_depth_buffer() {
    if(!is_zwrite_enabled())
      glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);
    if(!is_zwrite_enabled())
      glDepthMask(GL_FALSE);
  }

  int Video_GL_Fixed::get_maximum_anisotropy() const {
    return m_maximum_anisotropy;
  }

  bool Video_GL_Fixed::has_vertex_buffers() const {
    return GLEW_ARB_vertex_buffer_object != 0;
  }

  void Video_GL_Fixed::set_2d_view(const std::pair<Point2f, Point2f> &camera2d, const std::pair<Point2i, Point2i> &viewport, const bool &fix_aspect_ratio) {
    Video::set_2d_view(camera2d, viewport, fix_aspect_ratio);

    if(m_render_target) {
      select_world_matrix();
      translate_scene(Vector3f(0.0f, camera2d.second.y, 0.0f));
      scale_scene(Vector3f(1.0f, -1.0f, 1.0f));
      translate_scene(Vector3f(0.0f, -camera2d.first.y, 0.0f));

      if(get_backface_culling())
        glCullFace(GL_FRONT);
    }
    else if(get_backface_culling())
      glCullFace(GL_BACK);
  }

  void Video_GL_Fixed::set_3d_view(const Camera &camera, const std::pair<Point2i, Point2i> &viewport) {
    Video::set_3d_view(camera, viewport);

    if(m_render_target) {
      if(get_backface_culling())
        glCullFace(GL_FRONT);
    }
    else if(get_backface_culling())
      glCullFace(GL_BACK);
  }

  void Video_GL_Fixed::set_backface_culling(const bool &on) {
    Video::set_backface_culling(on);

    if(on) {
      // Enable Backface Culling
      glEnable(GL_CULL_FACE);

      if(m_render_target)
        glCullFace(GL_FRONT);
      else
        glCullFace(GL_BACK);
    }
    else
      glDisable(GL_CULL_FACE);
  }

  void Video_GL_Fixed::set_vertical_sync(const bool &on_) {
    Core::assert_no_error();
    Video::set_vertical_sync(on_);

#ifdef MANUAL_GL_VSYNC_DELAY
    const bool on = false;
#else
    const bool on = on_;
#endif

#ifndef TEMP_DISABLE
#if SDL_VERSION_ATLEAST(1,3,0)
    SDL_GL_SetSwapInterval(on);
#elif !defined(DISABLE_WGL)
    typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
    PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

    const char *extensions = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));

    if(strstr(extensions, "WGL_EXT_swap_control")) {
      wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

      if(wglSwapIntervalEXT)
        wglSwapIntervalEXT(on);
    }
#elif defined(_LINUX)
    if(m_pglSwapIntervalSGI)
      m_pglSwapIntervalSGI(on);
    else if(m_pglSwapIntervalEXT)
      m_pglSwapIntervalEXT(0, 0, on);
    else
      SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, on);
#else
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, on);
#endif
#endif

    SDL_ClearError();
  }

  void Video_GL_Fixed::set_zwrite(const bool &enabled) {
    Video::set_zwrite(enabled);

    glDepthMask(GLboolean(enabled));
  }

  void Video_GL_Fixed::set_ztest(const bool &enabled) {
    Video::set_ztest(enabled);

    if(enabled) {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
    }
    else
      glDisable(GL_DEPTH_TEST);
  }

  void Video_GL_Fixed::set_alpha_test(const bool &enabled,
                                const TEST &test,
                                const float &value)
  {
    Video::set_alpha_test(enabled, test, value);

    GLenum func;

    switch(test) {
      case ZENI_NEVER:            func = GL_NEVER;    break;
      case ZENI_LESS:             func = GL_LESS;     break;
      case ZENI_EQUAL:            func = GL_EQUAL;    break;
      case ZENI_GREATER:          func = GL_GREATER;  break;
      case ZENI_NOT_EQUAL:        func = GL_NOTEQUAL; break;
      case ZENI_LESS_OR_EQUAL:    func = GL_LEQUAL;   break;
      case ZENI_GREATER_OR_EQUAL: func = GL_GEQUAL;   break;
      case ZENI_ALWAYS:           func = GL_ALWAYS;   break;
      default:
        assert(false);
        return;
    }

    if(enabled)
      glEnable(GL_ALPHA_TEST);
    else
      glDisable(GL_ALPHA_TEST);

    glAlphaFunc(func, value); 
  }

  void Video_GL_Fixed::set_Color(const Color &color) {
    Video::set_Color(color);

    glColor4f(color.r, color.g, color.b, color.a);
  }

  void Video_GL_Fixed::set_clear_Color(const Color &color) {
    Video::set_clear_Color(color);

    glClearColor(color.r, color.g, color.b, color.a);
  }

  void Video_GL_Fixed::apply_Texture(const unsigned long &id) {
    get_Textures().apply_Texture(id);
  }

  void Video_GL_Fixed::apply_Texture(const Texture &texture) {
    texture.apply_Texture();
  }

  void Video_GL_Fixed::unapply_Texture() {
    glDisable(GL_TEXTURE_2D);
  }

  void Video_GL_Fixed::set_lighting(const bool &on) {
    Video::set_lighting(on);

    if(on)
      glEnable(GL_LIGHTING);
    else
      glDisable(GL_LIGHTING);
  }

  void Video_GL_Fixed::set_ambient_lighting(const Color &color) {
    Video::set_ambient_lighting(color);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, reinterpret_cast<const GLfloat *>(&color));
  }

  void Video_GL_Fixed::set_Light(const int &number, const Light &light) {
    GLenum ln;
    switch(number) {
    case 0: ln = GL_LIGHT0; break;
    case 1: ln = GL_LIGHT1; break;
    case 2: ln = GL_LIGHT2; break;
    case 3: ln = GL_LIGHT3; break;
    case 4: ln = GL_LIGHT4; break;
    case 5: ln = GL_LIGHT5; break;
    case 6: ln = GL_LIGHT6; break;
    case 7: ln = GL_LIGHT7; break;
    default:
      throw Light_Out_of_Range();
    }

    Video::set_Light(number, light);

    light.set(ln, *this);
  }

  void Video_GL_Fixed::unset_Light(const int &number) {
    GLenum ln;
    switch(number) {
    case 0: ln = GL_LIGHT0; break;
    case 1: ln = GL_LIGHT1; break;
    case 2: ln = GL_LIGHT2; break;
    case 3: ln = GL_LIGHT3; break;
    case 4: ln = GL_LIGHT4; break;
    case 5: ln = GL_LIGHT5; break;
    case 6: ln = GL_LIGHT6; break;
    case 7: ln = GL_LIGHT7; break;
    default:
      throw Light_Out_of_Range();
    }

    Video::unset_Light(number);

    glDisable(ln);
  }

  void Video_GL_Fixed::set_Material(const Material &material) {
    material.set(*this);
  }

  void Video_GL_Fixed::unset_Material(const Material &material) {
    material.unset(*this);
  }

  void Video_GL_Fixed::set_Fog(const Fog &fog) {
    Video::set_Fog(fog);

    glEnable(GL_FOG);
    fog.set(*this);
  }

  void Video_GL_Fixed::unset_Fog() {
    Video::unset_Fog();

    glDisable(GL_FOG);
  }
  
  void Video_GL_Fixed::set_program(Program &program) {
    program.link();
    glUseProgramObjectARB(dynamic_cast<Program_GL_Fixed &>(program).get());
  }

  void Video_GL_Fixed::unset_program() {
    glUseProgramObjectARB(0);
  }

  void Video_GL_Fixed::set_render_target(Texture &
#if !defined(REQUIRE_GL_ES) || defined(GL_OES_VERSION_2_0)
    texture
#endif
    )
  {
#if defined(REQUIRE_GL_ES) && !defined(GL_OES_VERSION_2_0)
    throw Video_Render_To_Texture_Error();
#else
    if(m_render_target)
      throw Video_Render_To_Texture_Error();

    Texture_GL &tgl = dynamic_cast<Texture_GL &>(texture);

    if(!tgl.m_frame_buffer_object) {
      const Point2i &tex_size = tgl.get_size();

      // Generate Depth Buffer
      glGenRenderbuffersEXT(1, &tgl.m_render_buffer);
      glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, tgl.m_render_buffer);
      glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT16, tex_size.x, tex_size.y);
      glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

      // Generate Framebuffer Object
      glGenFramebuffersEXT(1, &tgl.m_frame_buffer_object);
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, tgl.m_frame_buffer_object);

      // Bind Both to the Texture
      glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, tgl.m_render_buffer);
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tgl.m_texture_id, 0);
    }
    else
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, tgl.m_frame_buffer_object);

    m_render_target = &tgl;
#endif
  }

  void Video_GL_Fixed::unset_render_target() {
#if defined(REQUIRE_GL_ES) && !defined(GL_OES_VERSION_2_0)
    throw Video_Render_To_Texture_Error();
#else
    if(!m_render_target)
      throw Video_Render_To_Texture_Error();

    // Unbind all
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    // Prepare to Generate Mipmap
    glBindTexture(GL_TEXTURE_2D, m_render_target->m_texture_id);
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      Textures::get_bilinear_filtering() ? GL_LINEAR : GL_NEAREST);

    glGenerateMipmapEXT(GL_TEXTURE_2D);

    // Cleanup after Mipmap Generation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      Textures::get_mipmapping() ?
      (Textures::get_bilinear_filtering() ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST) :
      (Textures::get_bilinear_filtering() ? GL_LINEAR : GL_NEAREST));
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_render_target = 0;
#endif
  }

  void Video_GL_Fixed::clear_render_target(const Color &color) {
    set_clear_Color(color);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  inline const Point2i & Video_GL_Fixed::get_render_target_size() const {
    if(m_render_target)
      return m_render_target->get_size();
    else
      return get_Window().get_size();
  }

  void Video_GL_Fixed::select_world_matrix() {
    glMatrixMode(GL_MODELVIEW);
  }

  void Video_GL_Fixed::push_world_stack() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
  }

  void Video_GL_Fixed::pop_world_stack() {
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }

  void Video_GL_Fixed::translate_scene(const Vector3f &direction) {
    glTranslatef(direction.i, direction.j, direction.k);
  }

  void Video_GL_Fixed::rotate_scene(const Vector3f &about, const float &radians) {
    glRotatef(radians * 180.0f / Global::pi, about.i, about.j, about.k);
  }

  void Video_GL_Fixed::scale_scene(const Vector3f &factor) {
    glScalef(factor.i, factor.j, factor.k);
  }

  void Video_GL_Fixed::transform_scene(const Matrix4f &transformation) {
    glMultMatrixf(reinterpret_cast<const GLfloat * const>(&transformation));
  }

  Point2f Video_GL_Fixed::get_pixel_offset() const {
    return Point2f(0.0f, 0.0f);
  }

  void Video_GL_Fixed::set_view_matrix(const Matrix4f &view) {
    Video::set_view_matrix(view);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(reinterpret_cast<GLfloat *>(const_cast<Matrix4f *>(&view)));
  }

  void Video_GL_Fixed::set_projection_matrix(const Matrix4f &projection) {
    Video::set_projection_matrix(projection);

    glMatrixMode(GL_PROJECTION);
    if(m_render_target && is_3d()) {
      const Matrix4f flipped = Matrix4f::Scale(Vector3f(1.0f, -1.0f, 1.0f)) * projection;
      glLoadMatrixf(reinterpret_cast<GLfloat *>(const_cast<Matrix4f *>(&flipped)));
    }
    else
      glLoadMatrixf(reinterpret_cast<GLfloat *>(const_cast<Matrix4f *>(&projection)));
  }

  void Video_GL_Fixed::set_viewport(const std::pair<Point2i, Point2i> &viewport) {
    Video::set_viewport(viewport);

    if(m_render_target)
      glViewport(viewport.first.x, viewport.first.y, viewport.second.x - viewport.first.x, viewport.second.y - viewport.first.y);
    else
      glViewport(viewport.first.x, get_Window().get_height() - viewport.second.y, viewport.second.x - viewport.first.x, viewport.second.y - viewport.first.y);
  }

  Texture * Video_GL_Fixed::load_Texture(const String &filename, const bool &repeat, const bool &lazy_loading) {
    return new Texture_GL(filename, repeat, lazy_loading);
  }

  Texture * Video_GL_Fixed::create_Texture(const Image &image) {
    return new Texture_GL(image);
  }

  Texture * Video_GL_Fixed::create_Texture(const Point2i &size, const bool &repeat) {
    return new Texture_GL(size, repeat);
  }

  Font * Video_GL_Fixed::create_Font(const String &filename, const float &glyph_height, const float &virtual_screen_height) {
    return new Font_FT(filename, glyph_height, virtual_screen_height);
  }

  Vertex_Buffer_Renderer * Video_GL_Fixed::create_Vertex_Buffer_Renderer(Vertex_Buffer &vertex_buffer) {
    return new Vertex_Buffer_Renderer_GL_Fixed(vertex_buffer);
  }
  
  Shader * Video_GL_Fixed::create_Vertex_Shader(const String &filename) {
    return new Shader_GL_Fixed(compile_glsles_shader(filename, m_vertex_compiler), Shader::VERTEX);
  }

  Shader * Video_GL_Fixed::create_Fragment_Shader(const String &filename) {
    return new Shader_GL_Fixed(compile_glsles_shader(filename, m_fragment_compiler), Shader::FRAGMENT);
  }

  Program * Video_GL_Fixed::create_Program() {
    return new Program_GL_Fixed();
  }

  void Video_GL_Fixed::init() {
    Core::assert_no_error();

    std::cout << "Initializing OpenGL" << std::endl;

#ifndef TEMP_DISABLE
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

#ifdef REQUIRE_GL_ES
    {
      int err = _dgles_load_library(0, proc_loader);
      if(err)
        throw Video_Init_Failure();
    }
#endif

    Core::assert_no_error();

#if SDL_VERSION_ATLEAST(2,0,0)
    std::list<std::pair<int, int> > contexts;
    contexts.push_back(std::make_pair(1, 4));
    contexts.push_back(std::make_pair(1, 3));
    contexts.push_back(std::make_pair(1, 2));
    contexts.push_back(std::make_pair(1, 1));

    while(!contexts.empty()) {
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, contexts.front().first);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, contexts.front().second);

      m_context = SDL_GL_CreateContext(get_Window().get_window());
      if(m_context)
        break;
      
      std::cerr << "OpenGL context " << contexts.front().first << '.' << contexts.front().second << " failed." << std::endl;
      SDL_ClearError();
      contexts.pop_front();
    }

    if(!m_context)
      throw Video_Init_Failure();

    /* This had to be before SDL_GL_CreateContext to work correctly on OS X some time ago.
     * Now it causes an error to be before. Test.
     */
    Core::assert_no_error();
    SDL_GL_SetSwapInterval(get_vertical_sync());
    SDL_ClearError();
#endif

    Core::assert_no_error();

#ifndef REQUIRE_GL_ES
    {
      const GLenum err = glewInit();
      if(GLEW_OK != err) {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
        throw Video_Init_Failure();
      }
    }
#endif
#endif

    Core::assert_no_error();

    // Set Fill/Shade Mode
    glShadeModel(GL_SMOOTH);
#ifndef REQUIRE_GL_ES
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
    glEnable(GL_NORMALIZE); //GL_RESCALE_NORMALIZE);

    // Enable Alpha Blitting
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendEquation(GL_FUNC_ADD); // default // would require ARB ext

    // Set lighting variables
#ifndef REQUIRE_GL_ES
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    if(glGetError() == GL_INVALID_ENUM)
      std::cerr << "Quality Warning:  Your graphics card does not support separate specular lighting in OpenGL.\n";
#endif

    // Initialize Assorted Variables
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glPointSize(static_cast<GLfloat>(sqrt(pow(double(get_screen_width()), 2.) * pow(double(get_screen_height()), 2.)) / 1000000));
    //glLineWidth(static_cast<GLfloat>(sqrt(pow(double(get_screen_width()), 2.) * pow(double(get_screen_height()), 2.)) / 1000000));

    Core::assert_no_error();

    // Finish with a few function calls
    set_2d();
    set_Color(get_Color());
    set_clear_Color(get_clear_Color());
    set_backface_culling(get_backface_culling());
    set_lighting(get_lighting());
    set_ambient_lighting(get_ambient_lighting());
    for(int i = 0; i != 8; ++i)
      if(const Light * const lp = get_Light(i))
        set_Light(i, *lp);
    if(const Fog * const fp = get_Fog())
      set_Fog(*fp);
    set_alpha_test(is_alpha_test_enabled(), get_alpha_test_function(), get_alpha_test_value());
    set_zwrite(is_zwrite_enabled());
    set_ztest(is_ztest_enabled());

    Core::assert_no_error();

#ifndef REQUIRE_GL_ES
    union {
      void * v;
#ifdef _LINUX
      GLXEW::PFNGLXSWAPINTERVALEXTPROC pglSwapIntervalEXT;
      GLXEW::PFNGLXSWAPINTERVALSGIPROC pglSwapIntervalSGI;
#endif
      PFNGLBINDBUFFERARBPROC pglBindBufferARB;
      PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB;
      PFNGLGENBUFFERSARBPROC pglGenBuffersARB;
      PFNGLBUFFERDATAARBPROC pglBufferDataARB;
    } ptr;

#ifdef _LINUX
    ptr.v = SDL_GL_GetProcAddress("glXSwapIntervalEXT");
    if(!ptr.v)
      ptr.v = SDL_GL_GetProcAddress("wglSwapIntervalEXT");
    m_pglSwapIntervalEXT = ptr.pglSwapIntervalEXT;

    ptr.v = SDL_GL_GetProcAddress("glXSwapIntervalSGI");
    if(!ptr.v)
      ptr.v = SDL_GL_GetProcAddress("wglSwapIntervalSGI");
    m_pglSwapIntervalSGI = ptr.pglSwapIntervalSGI;
#endif

    if(GLEW_ARB_vertex_buffer_object) {
      ptr.v = SDL_GL_GetProcAddress("glBindBufferARB");
      m_pglBindBufferARB = ptr.pglBindBufferARB;

      ptr.v = SDL_GL_GetProcAddress("glDeleteBuffersARB");
      m_pglDeleteBuffersARB = ptr.pglDeleteBuffersARB;

      ptr.v = SDL_GL_GetProcAddress("glGenBuffersARB");
      m_pglGenBuffersARB = ptr.pglGenBuffersARB;

      ptr.v = SDL_GL_GetProcAddress("glBufferDataARB");
      m_pglBufferDataARB = ptr.pglBufferDataARB;
    }
    else
      std::cerr << "Performance Warning:  Your graphics card does not offer Vertex Buffer Objects (VBO) in OpenGL.\n";

    if(GLEW_EXT_texture_filter_anisotropic)
      glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, reinterpret_cast<GLint *>(&m_maximum_anisotropy));
    else
      m_maximum_anisotropy = 0;
#else
    m_maximum_anisotropy = 0;
#endif

    // Has to be done after finding the function pointer
    set_vertical_sync(get_vertical_sync());
    
    /// Generate vertex and fragment shader compilers

    ShBuiltInResources resources;
    ShInitBuiltInResources(&resources);

    resources.MaxVertexAttribs = 8;
    resources.MaxVertexUniformVectors = 128;
    resources.MaxVaryingVectors = 8;
    resources.MaxVertexTextureImageUnits = 0;
    resources.MaxCombinedTextureImageUnits = 8;
    resources.MaxTextureImageUnits = 8;
    resources.MaxFragmentUniformVectors = 16;
    resources.MaxDrawBuffers = 1;

    resources.OES_standard_derivatives = 0;
    resources.OES_EGL_image_external = 0;

    m_vertex_compiler = ShConstructCompiler(SH_VERTEX_SHADER, SH_GLES2_SPEC, SH_GLSL_OUTPUT, &resources);
    m_fragment_compiler = ShConstructCompiler(SH_FRAGMENT_SHADER, SH_GLES2_SPEC, SH_GLSL_OUTPUT, &resources);

    Core::assert_no_error();
  }

  void Video_GL_Fixed::uninit() {
    ShDestruct(m_vertex_compiler);
    ShDestruct(m_fragment_compiler);

#if SDL_VERSION_ATLEAST(1,3,0)
    if(m_context)
      SDL_GL_DeleteContext(m_context);
#endif
  }

}

#else

namespace Zeni {
  void * this_pointer_is_also_dead_beef = (void *)0xDEADBEEF;
}

#endif
