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

#include <Zeni/Video_GL.h>

#ifndef DISABLE_GL

#include <Zeni/Font.h>

#include <Zeni/Camera.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Game.hxx>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Matrix4f.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Video.hxx>

#ifdef _MACOSX
#include <GLEW/glew.h>
#else
#include <GL/glew.h>
#endif

#include <iostream>

namespace SDL {
  #include <SDL/SDL.h>
  #include <SDL/SDL_opengl.h>
}

namespace Zeni {

  Video_GL::Video_GL()
    : Video(Video_Base::ZENI_VIDEO_GL),
#if SDL_VERSION_ATLEAST(1,3,0)
      m_context(0),
#endif
#ifdef _LINUX
      m_pglSwapIntervalEXT(0),
#endif
      m_pglBindBufferARB(0),
      m_pglDeleteBuffersARB(0),
      m_pglGenBuffersARB(0),
      m_pglBufferDataARB(0),
      m_maximum_anisotropy(-1),
      m_vertex_buffers(false),
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

  Video_GL::~Video_GL() {
    uninit();
  }

  void Video_GL::render_all() {
    assert(!m_render_target);

#ifdef _WINDOWS
    glFlush();
#else
    glFinish();
#endif

    get_Textures().unlose_resources();
    get_Fonts().unlose_resources();

    get_Game().prerender();

    glViewport(0, 0, get_screen_width(), get_screen_height());

    if(!is_zwrite_enabled())
      glDepthMask(GL_TRUE);
    set_clear_color_impl(get_clear_color());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!is_zwrite_enabled())
      glDepthMask(GL_FALSE);

    get_Game().render();

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
#if SDL_VERSION_ATLEAST(1,3,0)
   SDL_GL_SwapWindow(get_window());
#else
   SDL_GL_SwapBuffers();
#endif
    
#ifdef MANUAL_GL_VSYNC_DELAY
   m_buffer_swap_end_time = tr.get_time();
#endif
  }

#if SDL_VERSION_ATLEAST(1,3,0)
  void Video_GL::alert_window_destroyed() {
    m_context = 0;
  }
#endif

  void Video_GL::init() {
    std::cout << "Initializing OpenGL" << std::endl;

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

#if SDL_VERSION_ATLEAST(1,3,0)
    m_context = SDL_GL_CreateContext(get_window());
#endif

    {
      const GLenum err = glewInit();
      if(GLEW_OK != err) {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
        throw Video_Init_Failure();
      }
    }

    // Set Fill/Shade Mode
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_NORMALIZE); //GL_RESCALE_NORMALIZE);

    // Enable Alpha Blitting
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendEquation(GL_FUNC_ADD); // default // would require ARB ext

    // Set lighting variables
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    if(glGetError() == GL_INVALID_ENUM)
      std::cerr << "Quality Warning:  Your graphics card does not support separate specular lighting in OpenGL.\n";

    // Initialize Assorted Variables
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glPointSize(static_cast<GLfloat>(sqrt(pow(double(get_screen_width()), 2.) * pow(double(get_screen_height()), 2.)) / 1000000));
    //glLineWidth(static_cast<GLfloat>(sqrt(pow(double(get_screen_width()), 2.) * pow(double(get_screen_height()), 2.)) / 1000000));

    // Finish with a few function calls
    set_2d();
    set_color(get_color());
    set_clear_color(get_clear_color());
    set_backface_culling(get_backface_culling());
    set_lighting(get_lighting());
    set_ambient_lighting(get_ambient_lighting());
    set_alpha_test(is_alpha_test_enabled(), get_alpha_test_function(), get_alpha_test_value());
    set_zwrite(is_zwrite_enabled());
    set_ztest(is_ztest_enabled());

    union {
      void * v;
#ifdef _LINUX
      PFNGLXSWAPINTERVALEXTPROC pglSwapIntervalEXT;
      PFNGLXSWAPINTERVALSGIPROC pglSwapIntervalSGI;
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

    // Has to be done after finding the function pointer
    set_vertical_sync(get_vertical_sync());

    m_vertex_buffers = strstr(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)), "ARB_vertex_buffer_object") != 0;
    if(m_vertex_buffers) {
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

    if(strstr((char*)glGetString(GL_EXTENSIONS), "GL_EXT_texture_filter_anisotropic"))
      glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, reinterpret_cast<GLint *>(&m_maximum_anisotropy));
    else
      m_maximum_anisotropy = 0;
  }

}

#else

namespace Zeni {
  void * this_pointer_is_also_dead_beef = (void *)0xDEADBEEF;
}

#endif
