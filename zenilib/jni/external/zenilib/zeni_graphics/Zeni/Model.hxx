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

#ifndef ZENI_MODEL_HXX
#define ZENI_MODEL_HXX

#include <Zeni/Model.h>
#include <Zeni/Quaternion.h>
#include <Zeni/Vector3f.h>

namespace Zeni {

#ifndef TEMP_DISABLE
  Lib3dsFile * const & Model::get_file() const {
//     GUARANTEED_FINISHED_BEGIN(m_loader);
    return m_file;
//     GUARANTEED_FINISHED_END();
  }

  Lib3dsFile * const & Model::thun_get_file() const {
    return m_file;
  }

  const Model_Extents & Model::get_extents() const {
//     GUARANTEED_FINISHED_BEGIN(m_loader);
    return m_extents;
//     GUARANTEED_FINISHED_END();
  }

  const Vector3f & Model::get_scale() {
//     GUARANTEED_FINISHED_BEGIN(m_loader);
    return m_scale;
//     GUARANTEED_FINISHED_END();
  }

  std::pair<Vector3f, float> Model::get_rotate() {
//     GUARANTEED_FINISHED_BEGIN(m_loader);
    return std::make_pair(m_rotate, m_rotate_angle);
//     GUARANTEED_FINISHED_END();
  }

  const Point3f & Model::get_translate() {
//     GUARANTEED_FINISHED_BEGIN(m_loader);
    return m_translate;
//     GUARANTEED_FINISHED_END();
  }

  const float & Model::get_keyframe() {
//     GUARANTEED_FINISHED_BEGIN(m_loader);
    return m_keyframe;
//     GUARANTEED_FINISHED_END();
  }

  bool Model::will_do_normal_alignment() const {
    return m_align_normals;
  }

  void Model::set_scale(const Vector3f &multiplier) {
//     GUARANTEED_FINISHED_BEGIN(m_loader);
    m_scale = multiplier;
//     GUARANTEED_FINISHED_END();
  }

  void Model::set_rotate(const float &angle, const Vector3f &ray) {
//     GUARANTEED_FINISHED_BEGIN(m_loader);
    m_rotate_angle = angle;
    m_rotate = ray;
//     GUARANTEED_FINISHED_END();
  }

  void Model::set_rotate(const Quaternion &rotation) {
    const std::pair<Vector3f, float> rayngel = rotation.get_rotation();
    set_rotate(rayngel.second, rayngel.first);
  }

  void Model::set_translate(const Point3f &vector) {
//     GUARANTEED_FINISHED_BEGIN(m_loader);
    m_translate = vector;
//     GUARANTEED_FINISHED_END();
  }

  void Model::do_normal_alignment(const bool align_normals_) {
    m_align_normals = align_normals_;
  }
#endif

}

#endif
