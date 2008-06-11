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
