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

#ifndef ZENI_MATRIX4F_HXX
#define ZENI_MATRIX4F_HXX

#include <Zeni/Matrix4f.h>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Quaternion.hxx>

#include <cassert>
#include <cmath>

namespace Zeni {

  Matrix4f::Matrix4f_Row::Matrix4f_Row(float * const &row_)
    : row(row_)
  {
  }

  const float & Matrix4f::Matrix4f_Row::operator[](const int &index) const {
    assert(-1 < index && index < 4);
    return row[index];
  }

  float & Matrix4f::Matrix4f_Row::operator[](const int &index) {
    assert(-1 < index && index < 4);
    return row[index];
  }

  Matrix4f Matrix4f::Zero() {
    return Matrix4f();
  }

  Matrix4f Matrix4f::Identity() {
    return Matrix4f(1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);
  }

  Matrix4f Matrix4f::Scale(const Vector3f &scaling_factor) {
    return Matrix4f(scaling_factor.i, 0.0f, 0.0f, 0.0f,
                    0.0f, scaling_factor.j, 0.0f, 0.0f,
                    0.0f, 0.0f, scaling_factor.k, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);
  }

  Matrix4f Matrix4f::Rotate(const Quaternion &rotation) {
    return rotation.get_matrix();
  }
  
  Matrix4f Matrix4f::Translate(const Vector3f &translation_factor) {
    return Matrix4f(1.0f, 0.0f, 0.0f, translation_factor.i,
                    0.0f, 1.0f, 0.0f, translation_factor.j,
                    0.0f, 0.0f, 1.0f, translation_factor.k,
                    0.0f, 0.0f, 0.0f, 1.0f);
  }
  
  Matrix4f Matrix4f::View(const Point3f &position, const Vector3f &forward, const Vector3f &up) {
    const Vector3f l = forward.normalized();
    const Vector3f s = (l % up.normalized()).normalized();
    const Vector3f u = s % l;

    return (Matrix4f(s.i, s.j, s.k, 0.0f,
                     u.i, u.j, u.k, 0.0f,
                     -l.i, -l.j, -l.k, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f) *
            Translate(-Vector3f(position))).transposed();
  }

  Matrix4f Matrix4f::Orthographic(const float &left, const float &right, const float &bottom, const float &top, const float &near_, const float &far_) {
    const float denom_x = left - right;
    const float denom_y = bottom - top;
    const float denom_z = near_ - far_;

    const float t_x = (left + right) / denom_x;
    const float t_y = (bottom + top) / denom_y;
    const float t_z = (near_ + far_) / denom_z;

    return Matrix4f(-2.0f / denom_x, 0.0f, 0.0f, 0.0f,
                    0.0f, -2.0f / denom_y, 0.0f, 0.0f,
                    0.0f, 0.0f, 2.0f / denom_z, 0.0f,
                    t_x, t_y, t_z, 1.0f);
  }

  Matrix4f Matrix4f::Perspective(const float &fov_rad_y, const float &aspect, const float &near_, const float &far_) {
    const float f = atan(fov_rad_y);
    const float denom = near_ - far_;

    return Matrix4f(f / aspect, 0.0f, 0.0f, 0.0f,
                    0.0f, f, 0.0f, 0.0f,
                    0.0f, 0.0f, (far_ + near_) / denom, -1.0f,
                    0.0f, 0.0f, 2.0f * far_ * near_ / denom, 0.0f);
  }

  const Matrix4f::Matrix4f_Row Matrix4f::operator[](const int &index) const {
    assert(-1 < index && index < 4);
    return Matrix4f_Row(const_cast<float *>(m_matrix[index]));
  }

  Matrix4f::Matrix4f_Row Matrix4f::operator[](const int &index) {
    assert(-1 < index && index < 4);
    return Matrix4f_Row(m_matrix[index]);
  }

  Matrix4f Matrix4f::operator+(const Matrix4f &rhs) const {
    Matrix4f matrix;

    const float
      *mp = reinterpret_cast<const float *>(m_matrix),
      *rp = reinterpret_cast<const float *>(rhs.m_matrix);

    for(float *lp = reinterpret_cast<float *>(matrix.m_matrix), *lp_end = lp + 16; lp != lp_end; ++lp, ++mp, ++rp)
      *lp = *mp + *rp;

    return matrix;
	}

	Matrix4f Matrix4f::operator-(const Matrix4f &rhs) const {
    Matrix4f matrix;

    const float
      *mp = reinterpret_cast<const float *>(m_matrix),
      *rp = reinterpret_cast<const float *>(rhs.m_matrix);

    for(float *lp = reinterpret_cast<float *>(matrix.m_matrix), *lp_end = lp + 16; lp != lp_end; ++lp, ++mp, ++rp)
      *lp = *mp - *rp;

    return matrix;
	}

	Matrix4f & Matrix4f::operator+=(const Matrix4f &rhs) {
    Matrix4f matrix;

    const float *rp = reinterpret_cast<const float *>(rhs.m_matrix);

    for(float *lp = reinterpret_cast<float *>(m_matrix), *lp_end = lp + 16; lp != lp_end; ++lp, ++rp)
      *lp += *rp;

    return *this;
	}

	Matrix4f & Matrix4f::operator-=(const Matrix4f &rhs) {
    Matrix4f matrix;

    const float *rp = reinterpret_cast<const float *>(rhs.m_matrix);

    for(float *lp = reinterpret_cast<float *>(m_matrix), *lp_end = lp + 16; lp != lp_end; ++lp, ++rp)
      *lp -= *rp;

    return *this;
	}

  Matrix4f Matrix4f::operator*(const Matrix4f &rhs) const {
    Matrix4f matrix;

    for(int i = 0; i < 4; ++i)
      for(int j = 0; j < 4; ++j)
        matrix.m_matrix[i][j] =
        m_matrix[i][0] * rhs.m_matrix[0][j] +
        m_matrix[i][1] * rhs.m_matrix[1][j] +
        m_matrix[i][2] * rhs.m_matrix[2][j] +
        m_matrix[i][3] * rhs.m_matrix[3][j];

    return matrix;
  }

  Matrix4f Matrix4f::operator*=(const Matrix4f &rhs) {
    return *this = *this * rhs;
  }

  Matrix4f Matrix4f::operator/(const Matrix4f &rhs) const {
    return *this * rhs.inverted();
  }

  Matrix4f Matrix4f::operator/=(const Matrix4f &rhs) {
    return *this *= rhs.inverted();
  }

  Matrix4f Matrix4f::operator*(const float &rhs) const {
    Matrix4f matrix;

    const float *mp = reinterpret_cast<const float *>(m_matrix);

    for(float *lp = reinterpret_cast<float *>(matrix.m_matrix), *lp_end = lp + 16; lp != lp_end; ++lp, ++mp)
      *lp = *mp * rhs;

    return matrix;
  }

  Matrix4f Matrix4f::operator/(const float &rhs) const {
    Matrix4f matrix;

    const float *mp = reinterpret_cast<const float *>(m_matrix);

    for(float *lp = reinterpret_cast<float *>(matrix.m_matrix), *lp_end = lp + 16; lp != lp_end; ++lp, ++mp)
      *lp = *mp / rhs;

    return matrix;
  }

  Matrix4f & Matrix4f::operator*=(const float &rhs) {
    for(float *lp = reinterpret_cast<float *>(m_matrix), *lp_end = lp + 16; lp != lp_end; ++lp)
      *lp *= rhs;

    return *this;
  }

  Matrix4f & Matrix4f::operator/=(const float &rhs) {
    for(float *lp = reinterpret_cast<float *>(m_matrix), *lp_end = lp + 16; lp != lp_end; ++lp)
      *lp /= rhs;

    return *this;
  }

  Matrix4f Matrix4f::operator-() const {
    Matrix4f matrix;

    const float *mp = reinterpret_cast<const float *>(m_matrix);

    for(float *lp = reinterpret_cast<float *>(matrix.m_matrix), *lp_end = lp + 16; lp != lp_end; ++lp, ++mp)
      *lp = *mp * -1;

    return matrix;
  }
  
  Vector3f Matrix4f::get_column(const int &column) const {
    assert(-1 < column && column < 3);
    return Vector3f(m_matrix[0][column], m_matrix[1][column], m_matrix[2][column]);
  }

  Vector3f Matrix4f::get_row(const int &row) const {
    assert(-1 < row && row < 3);
    return Vector3f(m_matrix[row][0], m_matrix[row][1], m_matrix[row][2]);
  }

}

#endif
