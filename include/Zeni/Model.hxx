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

#ifndef ZENI_MODEL_HXX
#define ZENI_MODEL_HXX

#include <Zeni/Model.h>

namespace Zeni {

  Lib3dsFile * Model::get_file() const {
    GUARANTEED_FINISHED_BEGIN(m_loader);
    return m_file;
    GUARANTEED_FINISHED_END();
  }

  Lib3dsFile * Model::thun_get_file() const {
    return m_file;
  }

  Model_Extents Model::get_extents() const {
    GUARANTEED_FINISHED_BEGIN(m_loader);
    return m_extents;
    GUARANTEED_FINISHED_END();
  }

  float Model::get_keyframes() const {
    GUARANTEED_FINISHED_BEGIN(m_loader);
    return float(m_file->frames);
    GUARANTEED_FINISHED_END();
  }

  void Model::set_scale(const Point3f &multiplier) {
    GUARANTEED_FINISHED_BEGIN(m_loader);
    m_scale = multiplier;
    GUARANTEED_FINISHED_END();
  }

  void Model::set_rotate(const float &angle, const Point3f &ray) {
    GUARANTEED_FINISHED_BEGIN(m_loader);
    m_rotate_angle = angle;
    m_rotate = ray;
    GUARANTEED_FINISHED_END();
  }

  void Model::set_translate(const Point3f &vector) {
    GUARANTEED_FINISHED_BEGIN(m_loader);
    m_translate = vector;
    GUARANTEED_FINISHED_END();
  }

  void Model::set_keyframe(const float &keyframe) {
    GUARANTEED_FINISHED_BEGIN(m_loader);
    m_keyframe = keyframe;
    lib3ds_file_eval(m_file, keyframe);
    GUARANTEED_FINISHED_END();
  }

}

#endif
