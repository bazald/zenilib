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

#ifndef ZENI_VIDEO_HXX
#define ZENI_VIDEO_HXX

// HXXed below
#include <Zeni/Camera.h>
#include <Zeni/Renderable.h>
#include <Zeni/Textures.h>
#include <Zeni/Quaternion.h>

#include <Zeni/Video.h>

#include <Zeni/Define.h>

namespace Zeni {

  const Video::VIDEO_MODE & Video::get_video_mode() {
    return g_video_mode;
  }

  const bool & Video::get_backface_culling() {
    return g_backface_culling;
  }

  const bool & Video::get_lighting() {
    return g_lighting;
  }

  const Color & Video::get_ambient_lighting() {
    return g_ambient_lighting;
  }

  const bool & Video::get_normal_interpolation() {
    return g_normal_interp;
  }

  const bool & Video::get_vertical_sync() {
    return g_vertical_sync;
  }

  const int & Video::get_multisampling() {
    return g_multisampling;
  }

  const bool & Video::is_zwrite_enabled() const {
    return g_zwrite;
  }

  const bool & Video::is_ztest_enabled() const {
    return g_ztest;
  }

  const bool & Video::is_alpha_test_enabled() const {
    return m_alpha_test;
  }

  const Video::TEST & Video::get_alpha_test_function() const {
    return m_alpha_function;
  }

  const float & Video::get_alpha_test_value() const {
    return m_alpha_value;
  }

  const bool & Video::is_3d() const {
    return m_3d;
  }

  void Video::set_2d() {
    const Point2i &size = get_render_target_size();
    set_2d_view(std::make_pair(Point2f(), Point2f(float(size.x), float(size.y))),
                std::make_pair(Point2i(), size));
  }

  void Video::set_2d(const std::pair<Point2f, Point2f> &camera2d, const bool &fix_aspect_ratio) {
    set_2d_view(camera2d,
                std::make_pair(Point2i(), get_render_target_size()),
                fix_aspect_ratio);
  }

  void Video::set_3d(const Camera &camera) {
    set_3d_view(camera,
                std::make_pair(Point2i(), get_render_target_size()));
  }

  const Color & Video::get_Color() const {
    return m_color;
  }

  const Color & Video::get_clear_Color() const {
    return g_clear_color;
  }

  void Video::apply_Texture(const String &name) {
    apply_Texture(get_Textures().get_id(name));
  }

  void Video::rotate_scene(const Quaternion &rotation) {
    const std::pair<Vector3f, float> rayngel = rotation.get_rotation();
    rotate_scene(rayngel.first, rayngel.second);
  }

  const Matrix4f & Video::get_view_matrix() const {
    return m_view;
  }

  const Matrix4f & Video::get_projection_matrix() const {
    return m_projection;
  }

  const std::pair<Point2i, Point2i> & Video::get_viewport() const {
    return m_viewport;
  }

}

#include <Zeni/Undefine.h>

#include <Zeni/Camera.hxx>
#include <Zeni/Renderable.hxx>
#include <Zeni/Textures.hxx>
#include <Zeni/Quaternion.hxx>

#endif
