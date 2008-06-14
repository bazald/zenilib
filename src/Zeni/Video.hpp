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

#include <Zeni/Video.hxx>

#include <Zeni/Camera.hxx>
#include <Zeni/Coordinate.hxx>
#include <Zeni/Matrix4f.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Video_GL.h>
#include <Zeni/Video_DX9.h>

#include <SDL/SDL_image.h>
#include <iostream>

using std::string;
using std::cerr;
using std::cout;
using std::endl;

namespace Zeni {

  Video::Video()
    : m_display_surface(0), 
    m_icon_surface(0),
    m_opengl_flag(0), 
    m_title("Zenipex Library Application"), 
    m_taskmsg("Zenipex Library Application"), 
    m_icon("icons/icon.gif"),
    m_color(1.0f, 1.0f, 1.0f, 1.0f),
    m_clear_color(1.0f, 0.0f, 0.0f, 0.0f),
    m_preview(Matrix4f::Scale(Vector3f(0.5f, -0.5f, -1.0f)) * Matrix4f::Translate(Vector3f(1.0f, -1.0f, 0.0f)))
  {
  }

  Video::~Video() {
  }

  Video & Video::get_reference() {
    if(!e_video)
      switch(g_video_mode) {
      case ZENI_VIDEO_ANY:
#ifndef DISABLE_GL
      case ZENI_VIDEO_GL:
        e_video = new Video_GL();
        break;
#endif
#ifndef DISABLE_DX9
      case ZENI_VIDEO_DX9:
        e_video = new Video_DX9();
        break;
#endif
      default:
        throw Video_Init_Failure();
    }

    return *e_video;
  }

  void Video::set_2d() {
    set_2d_view(make_pair(Point2f(0.0f, 0.0f), Point2f(float(get_screen_width()), float(get_screen_height()))),
                make_pair(Point2i(0, 0), Point2i(get_screen_width(), get_screen_height())));
  }

  void Video::set_2d(const std::pair<Point2f, Point2f> &camera2d) {
    set_2d_view(camera2d,
                make_pair(Point2i(0, 0), Point2i(get_screen_width(), get_screen_height())));
  }

  void Video::set_3d(const Camera &camera) {
    set_3d_view(camera,
                make_pair(Point2i(0, 0), Point2i(get_screen_width(), get_screen_height())));
  }

  void Video::set_2d_view(const std::pair<Point2f, Point2f> &camera2d, const std::pair<Point2i, Point2i> &viewport) {
    set_zwrite(false);
    set_ztest(false);

    const Matrix4f view = Matrix4f::Identity();
    set_view_matrix(view);

    const Matrix4f projection = Matrix4f::Orthographic(camera2d.first.x, camera2d.second.x, camera2d.second.y, camera2d.first.y, 0.0f, 1.0f);
    set_projection_matrix(projection);

    set_viewport(viewport);
  }

  void Video::set_3d_view(const Camera &camera, const std::pair<Point2i, Point2i> &viewport) {
    set_zwrite(true);
    set_ztest(true);

    const Matrix4f view = camera.get_view_matrix();
    set_view_matrix(view);

    Matrix4f projection = camera.get_projection_matrix(make_pair(Point2f(float(viewport.first.x), float(viewport.first.y)), Point2f(float(viewport.second.x), float(viewport.second.y))));
    set_projection_matrix(projection);

    set_viewport(viewport);
  }

  void Video::set_backface_culling(const bool &on) {
    g_backface_culling = on;
  }

  void Video::set_vertical_sync(const bool &on) {
    g_vertical_sync = on;
  }
  
  void Video::set_zwrite(const bool &enabled) {
    m_zwrite = enabled;
  }

  void Video::set_ztest(const bool &enabled) {
    m_ztest = enabled;
  }

  void Video::set_color(const Color &color) {
    m_color = color;
  }

  void Video::set_clear_color(const Color &color) {
    m_clear_color = color;
  }

  void Video::set_lighting(const bool &on) {
    g_lighting = on;
  }
  void Video::set_normal_interpolation(const bool &on) {
    g_normal_interp = on;
  }

  void Video::preinit(const VIDEO_MODE &vm, const int &w, const int &h, const bool &full, const int &multisampling, const bool &show_frame_) {
    if(g_initialized)
      throw Video_Initialized();

    g_video_mode = vm;
    g_screen_width = w;
    g_screen_height = h;
    g_screen_full = full;
    g_multisampling = multisampling;
    g_screen_show_frame = show_frame_;
  }

  void Video::set_tt(const string &title, const string &taskmsg) {
    m_title = title;
    m_taskmsg = taskmsg;
    set_tt();
  }

  void Video::set_title(const string &title) {
    m_title = title;
    set_tt();
  }

  void Video::set_taskmsg(const string &taskmsg) {
    m_taskmsg = taskmsg;
    set_tt();
  }

  const bool Video::set_icon(const string &filename) {
    m_icon = filename;
    return set_icon();
  }

  void Video::init() {
    // Ensure Core is initialized
    Core::get_reference();
    g_initialized = true;

    // Initialize SDL + Variablse
    const SDL_VideoInfo *VideoInfo = SDL_GetVideoInfo();

    set_tt();
    set_icon();

    if(g_screen_width == -1)
      g_screen_width = 0;
    if(g_screen_height == -1)
      g_screen_height = 0;

    // Initialize Window
    m_display_surface = SDL_SetVideoMode(g_screen_width, g_screen_height, 32,
      (get_opengl_flag() ? SDL_OPENGL : 0) | 
      (g_screen_full ? SDL_FULLSCREEN : 
      (VideoInfo->wm_available && g_screen_show_frame ? 0 : SDL_NOFRAME)));

    if(!m_display_surface) {
      g_initialized = false;
      throw Video_Init_Failure();
    }

    g_screen_width = m_display_surface->w;
    g_screen_height = m_display_surface->h;
  }

  void Video::uninit() {
    SDL_FreeSurface(m_display_surface);
    m_display_surface = 0;
    g_initialized = false;
  }
  
  void Video::set_view_matrix(const Matrix4f &view) {
    m_view = view;
    regenerate_compound_matrices();
  }

  void Video::set_projection_matrix(const Matrix4f &projection) {
    m_projection = projection;
    regenerate_compound_matrices();
  }

  void Video::set_viewport(const std::pair<Point2i, Point2i> &viewport) {
    m_viewport = viewport;
  }

  void Video::set_tt() {
    const SDL_VideoInfo *VideoInfo = SDL_GetVideoInfo();
    if(VideoInfo->wm_available)
      SDL_WM_SetCaption(m_title.c_str(), m_taskmsg.c_str());
  }

  bool Video::set_icon() {
    const SDL_VideoInfo *VideoInfo = SDL_GetVideoInfo();
    if(!VideoInfo->wm_available)
      return false;

    m_icon_surface = IMG_Load(m_icon.c_str());

    if(!m_icon_surface) {
      cerr << "Could not load display window icon\n";
      return false;
    }

    SDL_WM_SetIcon(m_icon_surface, NULL);
    return true;
  }

  void Video::regenerate_compound_matrices() {
    m_world_to_screen = m_preview * m_view * m_projection;
    m_screen_to_world = m_world_to_screen.inverted();
  }

  Video *Video::e_video = 0;
  VIDEO_MODE Video::g_video_mode = ZENI_VIDEO_ANY;
  int Video::g_screen_width = 0;
  int Video::g_screen_height = 0;
  bool Video::g_screen_full = false;
  bool Video::g_screen_show_frame = true;
  bool Video::g_initialized = false;
  bool Video::g_backface_culling = false;
  bool Video::g_lighting = false;
  bool Video::g_normal_interp = false;
  bool Video::g_vertical_sync = false;
  int Video::g_multisampling = 1;

}
