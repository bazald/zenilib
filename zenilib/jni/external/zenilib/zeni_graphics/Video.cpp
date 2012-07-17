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

#include <Zeni/GLU.h>

#ifdef _WINDOWS
#include <WinUser.h>
#endif

#include <algorithm>
#include <iostream>

#include <Zeni/Define.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Singleton.hxx>

namespace Zeni {

  template class Singleton<Video>;

  Video * Video::create() {
    Video * video = 0;

    File_Ops &fo = get_File_Ops();

    const String appdata_path = fo.get_appdata_path();

    const String user_normal = appdata_path + "config/zenilib.xml";
    const String user_backup = user_normal + ".bak";
    const String local_normal = "config/zenilib.xml";
    const String local_backup = local_normal + ".bak";

    static bool last_resort_taken = false;

#ifndef ANDROID
    try
#endif
    {
      switch(Video::g_video_mode) {
      case Video::ZENI_VIDEO_ANY:
#ifndef DISABLE_DX9
      case Video::ZENI_VIDEO_DX9:
        video = new Video_DX9();
        break;
#endif
#ifndef DISABLE_GL
      case Video::ZENI_VIDEO_GL:
        video = new Video_GL();
        break;
#endif
      default:
        throw Video_Init_Failure();
      }
    }
#ifndef ANDROID
    catch(Video_Init_Failure &) {
      if(fo.copy_file(user_backup, user_normal) && fo.delete_file(user_backup)) {
        std::cerr << '\'' << user_normal << "' backup restored due to initialization failure.\n";
        Video::preinit_from_file(user_normal);
        get_Video();
      }
      else if(fo.copy_file(local_backup, local_normal) && fo.delete_file(local_backup)) {
        std::cerr << '\'' << local_normal << "' backup restored due to initialization failure.\n";
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
#endif

    last_resort_taken = false;

    return video;
  }

  Singleton<Video>::Uninit Video::g_uninit;
  Singleton<Video>::Reinit Video::g_reinit;

  Video::Video()
    :
    m_color(1.0f, 1.0f, 1.0f, 1.0f),
    m_preview(Matrix4f::Translate(Vector3f(-0.5f, -0.5f, 0.0f)) *
      Matrix4f::Scale(Vector3f(0.5f, -0.5f, -1.0f)) *
      Matrix4f::Translate(Vector3f(1.0f, -1.0f, 0.0f))),
    m_alpha_test(false),
    m_alpha_function(Video::ZENI_ALWAYS),
    m_alpha_value(0.0f),
    m_3d(false)
  {
    Window::remove_post_reinit(&g_reinit);

    Window &wr = get_Window();

    wr.lend_pre_uninit(&g_uninit);
    wr.lend_post_reinit(&g_reinit);
  }

  Video::~Video() {
    Window::remove_pre_uninit(&g_uninit);
  }

  Video & get_Video() {
    return Singleton<Video>::get();
  }

  void Video::set_2d_view(const std::pair<Point2f, Point2f> &camera2d, const std::pair<Point2i, Point2i> &viewport, const bool &fix_aspect_ratio) {
    m_3d = false;

    set_viewport(calculate_viewport(camera2d, viewport, fix_aspect_ratio));

    const Matrix4f view = Matrix4f::Identity();
    set_view_matrix(view);

    const std::pair<Point2i, Point2i> &vp = get_viewport();
    Point2f offset = get_pixel_offset();
    offset.x *= (camera2d.second.x - camera2d.first.x) / (vp.second.x - vp.first.x);
    offset.y *= (camera2d.second.y - camera2d.first.y) / (vp.second.y - vp.first.y);

    const Matrix4f projection = Matrix4f::Orthographic(camera2d.first.x + offset.x,
                                                       camera2d.second.x + offset.x,
                                                       camera2d.second.y + offset.y,
                                                       camera2d.first.y + offset.y,
                                                       ZENI_2D_NEAR, ZENI_2D_FAR);
    set_projection_matrix(projection);
  }

  void Video::set_3d_view(const Camera &camera, const std::pair<Point2i, Point2i> &viewport) {
    m_3d = true;

    const Matrix4f view = camera.get_view_matrix();
    set_view_matrix(view);

    const Matrix4f projection = camera.get_projection_matrix(viewport);
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
    g_zwrite = enabled;
  }

  void Video::set_ztest(const bool &enabled) {
    g_ztest = enabled;
  }

  void Video::set_alpha_test(const bool &enabled,
                             const TEST &test,
                             const float &value) {
    m_alpha_test = enabled;
    m_alpha_function = test;
    m_alpha_value = value;
  }

  void Video::set_Color(const Color &color) {
    m_color = color;
  }

  void Video::set_clear_Color(const Color &color) {
    g_clear_color = color;
  }

  void Video::set_lighting(const bool &on) {
    g_lighting = on;
  }

  void Video::set_ambient_lighting(const Color &color) {
    g_ambient_lighting = color;
  }

  void Video::set_view_matrix(const Matrix4f &view) {
    m_view = view;
  }

  void Video::set_projection_matrix(const Matrix4f &projection) {
    m_projection = projection;
  }

  void Video::set_viewport(const std::pair<Point2i, Point2i> &viewport) {
    m_viewport = viewport;
  }

  void Video::set_viewport(const float &aspect_ratio, const std::pair<Point2i, Point2i> &viewport_) {
    set_viewport(calculate_viewport(aspect_ratio, viewport_));
  }

  std::pair<Point2i, Point2i> Video::calculate_viewport(const std::pair<Point2f, Point2f> &camera2d, const std::pair<Point2i, Point2i> &viewport, const bool &fix_aspect_ratio) {
    if(fix_aspect_ratio)
      return calculate_viewport((camera2d.second.x - camera2d.first.x) / (camera2d.second.y - camera2d.first.y), viewport);
    else
      return viewport;
  }

  std::pair<Point2i, Point2i> Video::calculate_viewport(const float &aspect_ratio, const std::pair<Point2i, Point2i> &viewport_) const {
    std::pair<Point2i, Point2i> viewport = viewport_;

    int width = viewport.second.x - viewport.first.x;
    int height = viewport.second.y - viewport.first.y;
    const float given_ratio = float(width) / height;

    if(given_ratio > aspect_ratio) {
      const int new_width = int(width * aspect_ratio / given_ratio);
      const int cut_side = (width - new_width) / 2;

      viewport.first.x += cut_side;
      viewport.second.x -= cut_side;
    }
    else if(aspect_ratio > given_ratio) {
      const int new_height = int(height * given_ratio / aspect_ratio);
      const int cut_side = (height - new_height) / 2;

      viewport.first.y += cut_side;
      viewport.second.y -= cut_side;
    }

    return viewport;
  }

  const Light * const Video::get_Light(const int &number) const {
    const Unordered_Map<int, Light>::const_iterator it = g_lights.find(number);

    if(it != g_lights.end())
      return &it->second;
    else
      return 0;
  }

  void Video::set_Light(const int &number, const Light &light) {
    g_lights[number] = light;
  }

  void Video::unset_Light(const int &number) {
    g_lights.erase(number);
  }

  const Fog * const Video::get_Fog() const {
    return g_fog_enabled ? &g_fog : 0;
  }

  void Video::set_Fog(const Fog &fog) {
    g_fog = fog;
    g_fog_enabled = false;
  }

  void Video::unset_Fog() {
    g_fog_enabled = false;
  }

  void Video::preinit_video_mode(const Video::VIDEO_MODE &vm) {
    g_video_mode = vm;
  }

  void Video::preinit_multisampling(const int &multisampling) {
    g_multisampling = multisampling;
  }

  void Video::preinit_vertical_sync(const bool &vertical_sync) {
    g_vertical_sync = vertical_sync;
  }

  void Video::preinit_from_file(const String &filename) {
    XML_Document file(filename);
    preinit_from_file(file);
  }

  void Video::preinit_from_file(const XML_Document &file) {
    XML_Element_c zenilib = file["Zenilib"];
    XML_Element_c video = zenilib["Video"];

#ifndef ANDROID
    Window::preinit_from_xml(video);
#endif

    const String api = video["API"].to_string();

    preinit_video_mode(
#ifndef DISABLE_DX9
                              api == "DX9" ? Video::ZENI_VIDEO_DX9 :
#endif
#ifndef DISABLE_GL
                              api == "OpenGL" ? Video::ZENI_VIDEO_GL :
#endif
                              Video::ZENI_VIDEO_ANY);
    preinit_multisampling(video["Multisampling"].to_int());
    preinit_vertical_sync(video["Vertical_Sync"].to_bool());
  }

  void Video::change_resolution(const Point2i &resolution) {
    Window &wr = get_Window();

    if(wr.get_width() == resolution.x && wr.get_height() == resolution.y)
      return;

    destroy();

    try {
      preinit_from_file(get_File_Ops().get_appdata_path() + "config/zenilib.xml");
    }
    catch(XML_Load_Failure &) {
      preinit_from_file("config/zenilib.xml");
    }

#ifndef ANDROID
    wr.alert_window_resized(resolution);
#endif

    get();
  }

#ifndef ANDROID
  void Video::save(const bool &backup) {
    File_Ops &fo = get_File_Ops();

    // Initialize paths

    const String appdata_path = fo.get_appdata_path();

    const String user_normal = appdata_path + "config/zenilib.xml";
    const String user_backup = user_normal + ".bak";
    const String local_normal = "config/zenilib.xml";
    const String local_backup = local_normal + ".bak";

    // Create file

    XML_Document file;
    if(!file.try_load(user_normal))
      file.load(local_normal);

    XML_Element zenilib = file["Zenilib"];
    XML_Element textures = zenilib["Textures"];
    XML_Element video = zenilib["Video"];

    textures["Anisotropy"].set_int(Textures::get_anisotropic_filtering());
    textures["Bilinear_Filtering"].set_bool(Textures::get_bilinear_filtering());
    textures["Mipmapping"].set_bool(Textures::get_mipmapping());

    video["API"].set_string(
#if !defined(DISABLE_DX9) && !defined(DISABLE_GL)
                            g_video_mode == Video::ZENI_VIDEO_DX9 ? "DX9" : "OpenGL");
#elif !defined(DISABLE_DX9)
                            "DX9");
#elif !defined(DISABLE_GL)
                            "OpenGL");
#else
                            "Disabled");
#endif                            

    video["Full_Screen"].set_bool(Window::is_full_screen());
    video["Multisampling"].set_int(g_multisampling);
    video["Resolution"]["Width"].set_int(Window::get_width());
    video["Resolution"]["Height"].set_int(Window::get_height());
    video["Vertical_Sync"].set_bool(g_vertical_sync);

    // User-specific backup and save

    bool user_save = false;
    if(fo.create_directory(appdata_path) &&
       fo.create_directory(appdata_path + "config/"))
    {
      if(backup)
        fo.copy_file(user_normal, user_backup);

      user_save = file.try_save(user_normal);
    }

    // Local backup and save

    if(backup) {
      if(user_save)
        fo.copy_file(user_normal, local_backup);
      else
        fo.copy_file(local_normal, local_backup);
    }

    file.try_save(local_normal);
  }

  bool Video::revert() {
    File_Ops &fo = get_File_Ops();

    const String appdata_path = fo.get_appdata_path();

    const String user_normal = appdata_path + "config/zenilib.xml";
    const String user_backup = user_normal + ".bak";
    const String local_normal = "config/zenilib.xml";
    const String local_backup = local_normal + ".bak";

    bool reverted = false;

    try {
      preinit_from_file(user_normal);
      get_Video();
      reverted = true;
    }
    catch(Video_Init_Failure &) {
    }

    if(!reverted && fo.copy_file(user_backup, user_normal) && fo.delete_file(user_backup)) {
      std::cerr << '\'' << user_normal << "' backup restored due to initialization failure.\n";
      try {
        Video::preinit_from_file(user_normal);
        get_Video();
        reverted = true;
      }
      catch(Video_Init_Failure &) {
      }
    }

    if(!reverted && fo.copy_file(local_backup, local_normal) && fo.delete_file(local_backup)) {
      std::cerr << '\'' << local_normal << "' backup restored due to initialization failure.\n";
      try {
        Video::preinit_from_file(local_normal);
        get_Video();
        reverted = true;
      }
      catch(Video_Init_Failure &) {
      }
    }

    if(!reverted) {
      std::cerr << '\'' << local_normal << "' backup restored due to initialization failure.\n";
      Video::set_failsafe_defaults();
      get_Video();
      reverted = true;
    }

    save(false);

    return true;
  }

  void Video::set_failsafe_defaults() {
    Window::set_failsafe_defaults();

    Textures::set_texturing_mode(0, false, false);

#if !defined(DISABLE_GL)
    g_video_mode = Video::ZENI_VIDEO_GL;
#elif !defined(DISABLE_DX9)
    g_video_mode = Video::ZENI_VIDEO_DX9;
#else
    g_video_mode = Video::ZENI_VIDEO_ANY;
#endif

    g_vertical_sync = false;
  }

  void Video::print_errors() {
#if !defined(DISABLE_GL) && !defined(REQUIRE_GL_ES)
    std::cerr << "OpenGL    : " << Zeni::gluErrorString(glGetError()) << std::endl;
#endif

#ifndef DISABLE_CG
    std::cerr << "Cg        : " << cgGetErrorString(cgGetError()) << std::endl;
#endif
  }
#endif

  Video::VIDEO_MODE Video::g_video_mode = Video::ZENI_VIDEO_ANY;
  bool Video::g_backface_culling = false;
  bool Video::g_lighting = false;
  Color Video::g_ambient_lighting = Color(1.0f, 1.0f, 1.0f, 1.0f);
  Unordered_Map<int, Light> Video::g_lights;
  Fog Video::g_fog;
  bool Video::g_fog_enabled = false;
  Color Video::g_clear_color = Color(1.0f, 0.0f, 0.0f, 0.0f);
  bool Video::g_normal_interp = false;
  bool Video::g_vertical_sync = false;
  int Video::g_multisampling = 0;
  bool Video::g_zwrite = true;
  bool Video::g_ztest = true;

}
