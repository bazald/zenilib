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
#include <Zeni/XML.hxx>

#ifdef _MACOSX
#include <SDL_image/SDL_image.h>
#else
#include <SDL/SDL_image.h>
#endif

#include <algorithm>
#include <iostream>

#include <Zeni/Global.h>

using namespace std;

namespace Zeni {

  static bool video_mode_lt(const Point2i &lhs, const Point2i &rhs) {
    return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
  }

  Video::Video(const Video_Base::VIDEO_MODE &vtype_)
    : Video_Base::IV(vtype_),
    m_display_surface(0), 
    m_icon_surface(0),
    m_opengl_flag(0), 
    m_title("Zenilib Application"), 
    m_taskmsg("Zenilib Application"), 
    m_icon("icons/icon.gif"),
    m_color(1.0f, 1.0f, 1.0f, 1.0f),
    m_clear_color(1.0f, 0.0f, 0.0f, 0.0f),
    m_preview(Matrix4f::Translate(Vector3f(-0.5f, -0.5f, 0.0f)) *
      Matrix4f::Scale(Vector3f(0.5f, -0.5f, -1.0f)) *
      Matrix4f::Translate(Vector3f(1.0f, -1.0f, 0.0f))),
    m_alpha_test(false),
    m_alpha_function(Video::ZENI_ALWAYS),
    m_alpha_value(0.0f)
  {
    if(!g_enabled)
      throw Video_Init_Failure();

    if(SDL_InitSubSystem(SDL_INIT_VIDEO) == -1)
      throw Video_Init_Failure();

    // Initialize Video Mode Listing
    SDL_PixelFormat fmt;
    memset(&fmt, 0, sizeof(SDL_PixelFormat));
    fmt.BitsPerPixel = 32;
    for(SDL_Rect ** mode = SDL_ListModes(&fmt, SDL_FULLSCREEN | SDL_OPENGL); *mode; ++mode)
      m_modes.push_back(Point2i((*mode)->w, (*mode)->h));

    if(m_modes.empty())
      throw Video_Init_Failure();

    std::sort(m_modes.begin(), m_modes.end(), &video_mode_lt);
  }

  Video::~Video() {
  }

  Video & get_Video() {
    if(!Video::e_video) {
      Core &cr = get_Core();

      const std::string appdata_path = cr.get_appdata_path();

      const string user_normal = appdata_path + "config/zenilib.xml";
      const string user_backup = user_normal + ".bak";
      const string local_normal = "config/zenilib.xml";
      const string local_backup = local_normal + ".bak";

      static bool last_resort_taken = false;

      try {
        switch(Video::g_video_mode) {
        case Video_Base::ZENI_VIDEO_ANY:
#ifndef DISABLE_GL
        case Video_Base::ZENI_VIDEO_GL:
          Video::e_video = new Video_GL();
          break;
#endif
#ifndef DISABLE_DX9
        case Video_Base::ZENI_VIDEO_DX9:
          Video::e_video = new Video_DX9();
          break;
#endif
        default:
          throw Video_Init_Failure();
        }
      }
      catch(Video_Init_Failure &) {
        if(cr.copy_file(user_backup, user_normal) && cr.delete_file(user_backup)) {
          cerr << '\'' << user_normal << "' backup restored due to initialization failure.\n";
          Video::preinit_from_file(user_normal);
          get_Video();
        }
        else if(cr.copy_file(local_backup, local_normal) && cr.delete_file(local_backup)) {
          cerr << '\'' << local_normal << "' backup restored due to initialization failure.\n";
          Video::preinit_from_file(local_normal);
          get_Video();
        }
        else if(!last_resort_taken) {
          Video::set_failsafe_defaults();

          last_resort_taken = true;

          get_Video();
        }
        else
          throw;
      }

      //// Make backups of "good" configurations

      //if(cr.create_directory(appdata_path) &&
      //   cr.create_directory(appdata_path + "config/"))
      //{
      //  if(cr.copy_file(user_normal, user_backup))
      //  {
      //    cr.copy_file(user_normal, local_normal);
      //    cr.copy_file(user_normal, local_backup);
      //  }
      //  else
      //  {
      //    cr.copy_file(local_normal, user_normal);
      //    cr.copy_file(local_normal, user_backup);
      //  }
      //}
      //else
      //  cr.copy_file(local_normal, local_backup);

      last_resort_taken = false;
    }

    return *Video::e_video;
  }

  void Video::preinit_video_mode(const Video_Base::VIDEO_MODE &vm) {
    g_video_mode = vm;
  }

  void Video::preinit_screen_resolution(const Point2i &resolution) {
    g_screen_size = resolution;
  }

  void Video::preinit_full_screen(const bool &full_screen) {
    g_screen_full = full_screen;
  }

  void Video::preinit_multisampling(const int &multisampling) {
    g_multisampling = multisampling;
  }

  void Video::preinit_vertical_sync(const bool &vertical_sync) {
    g_vertical_sync = vertical_sync;
  }

  void Video::preinit_show_frame(const bool &show_frame_) {
    g_screen_show_frame = show_frame_;
  }

  void Video::preinit_from_file(const std::string &filename) {
    XML_Document file(filename);
    preinit_from_file(file);
  }

  void Video::preinit_from_file(const XML_Document &file) {
    XML_Element_c zenilib = file["Zenilib"];
    XML_Element_c textures = zenilib["Textures"];
    XML_Element_c video = zenilib["Video"];
    const std::string api = video["API"].to_string();

    preinit_video_mode(
#ifndef DISABLE_DX9
                              api == "DX9" ? Video_Base::ZENI_VIDEO_DX9 :
#endif
#ifndef DISABLE_GL
                              api == "OpenGL" ? Video_Base::ZENI_VIDEO_GL :
#endif
                              Video_Base::ZENI_VIDEO_ANY);
    preinit_screen_resolution(Point2i(video["Resolution"]["Width"].to_int(),
                                             video["Resolution"]["Height"].to_int()));
    preinit_full_screen(video["Full_Screen"].to_bool());
    preinit_multisampling(video["Multisampling"].to_int());
    preinit_vertical_sync(video["Vertical_Sync"].to_bool());
  }

  void Video::destroy() {
    Vertex_Buffer::lose_all();
    get_Fonts().lose_resources();
    get_Textures().lose_resources();

    delete e_video;
    e_video = 0;
    g_initialized = false;
  }

  void Video::reinit() {
    destroy();

    try {
      preinit_from_file(get_Core().get_appdata_path() + "config/zenilib.xml");
    }
    catch(XML_Load_Failure &) {
      preinit_from_file("config/zenilib.xml");
    }

    get_Video();
    get_Textures().unlose_resources();
    get_Fonts().unlose_resources();
  }

  void Video::save() {
    Core &cr = get_Core();

    // Initialize paths

    const std::string appdata_path = cr.get_appdata_path();

    const string user_normal = appdata_path + "config/zenilib.xml";
    const string user_backup = user_normal + ".bak";
    const string local_normal = "config/zenilib.xml";
    const string local_backup = local_normal + ".bak";

    // Create file

    XML_Document file(local_normal);
    XML_Element zenilib = file["Zenilib"];
    XML_Element textures = zenilib["Textures"];
    XML_Element video = zenilib["Video"];

    textures["Anisotropy"].set_int(Textures::get_anisotropic_filtering());
    textures["Bilinear_Filtering"].set_bool(Textures::get_bilinear_filtering());
    textures["Mipmapping"].set_bool(Textures::get_mipmapping());

    video["API"].set_string(
#if !defined(DISABLE_DX9) && !defined(DISABLE_GL)
                            g_video_mode == Video_Base::ZENI_VIDEO_DX9 ? "DX9" : "OpenGL");
#elif !defined(DISABLE_DX9)
                            "DX9");
#elif !defined(DISABLE_GL)
                            "OpenGL");
#else
                            "Disabled");
#endif                            

    video["Full_Screen"].set_bool(g_screen_full);
    video["Multisampling"].set_int(g_multisampling);
    video["Resolution"]["Width"].set_int(g_screen_size.x);
    video["Resolution"]["Height"].set_int(g_screen_size.y);
    video["Vertical_Sync"].set_bool(g_vertical_sync);

    // Local backup and save

    cr.copy_file(local_normal, local_backup);
    file.try_save();

    // User-specific backup and save

    if(cr.create_directory(appdata_path) &&
       cr.create_directory(appdata_path + "config/"))
    {
      cr.copy_file(user_normal, user_backup);
      file.try_save(user_normal);
    }
  }

  void Video::set_failsafe_defaults() {
    Textures::set_texturing_mode(0, false, false);

#if !defined(DISABLE_GL)
    g_video_mode = Video_Base::ZENI_VIDEO_GL;
#elif !defined(DISABLE_DX9)
    g_video_mode = Video_Base::ZENI_VIDEO_DX9;
#else
    g_video_mode = Video_Base::ZENI_VIDEO_ANY;
#endif

    g_screen_full = false;
    g_screen_size.x = MINIMUM_SCREEN_WIDTH;
    g_screen_size.y = MINIMUM_SCREEN_HEIGHT;
    g_vertical_sync = false;
  }

  void Video::set_enabled(const bool &enabled) {
    g_enabled = enabled;
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
    get_Core();
    g_initialized = true;

    // Initialize SDL + Variables
    const SDL_VideoInfo *VideoInfo = SDL_GetVideoInfo();

    set_tt();
    set_icon();

    const Point2i &max_res = *m_modes.rbegin();

    if(g_screen_size.x < 0)
      g_screen_size.x = max_res.x;
    else if(g_screen_size.x == 0)
      g_screen_size.x = VideoInfo->current_w;
    else if(g_screen_size.x < MINIMUM_SCREEN_WIDTH)
      g_screen_size.x = MINIMUM_SCREEN_WIDTH;
    else if(g_screen_size.x > max_res.x)
      g_screen_size.x = max_res.x;

    if(g_screen_size.y < 0)
      g_screen_size.y = max_res.y;
    else if(g_screen_size.y == 0)
      g_screen_size.y = VideoInfo->current_h;
    else if(g_screen_size.y < MINIMUM_SCREEN_HEIGHT)
      g_screen_size.y = MINIMUM_SCREEN_HEIGHT;
    else if(g_screen_size.y > max_res.y)
      g_screen_size.y = max_res.y;

    // Initialize Window
    m_display_surface = SDL_SetVideoMode(g_screen_size.x, g_screen_size.y, 32,
      (get_opengl_flag() ? SDL_OPENGL : 0) | 
      (g_screen_full ? SDL_FULLSCREEN : 
      (VideoInfo->wm_available && g_screen_show_frame ? 0 : SDL_NOFRAME)));

    if(!m_display_surface) {
      g_initialized = false;
      throw Video_Init_Failure();
    }

    g_screen_size.x = m_display_surface->w;
    g_screen_size.y = m_display_surface->h;
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

  Video *Video::e_video = 0;
  Video_Base::VIDEO_MODE Video::g_video_mode = Video_Base::ZENI_VIDEO_ANY;
  bool Video::g_enabled = true;
  Point2i Video::g_screen_size;
  bool Video::g_screen_full = false;
  bool Video::g_screen_show_frame = true;
  bool Video::g_initialized = false;
  bool Video::g_backface_culling = false;
  bool Video::g_lighting = false;
  bool Video::g_normal_interp = false;
  bool Video::g_vertical_sync = false;
  int Video::g_multisampling = 0;

}
