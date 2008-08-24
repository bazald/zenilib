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

#include <Zeni/Matrix4f.hxx>

#include <Zeni/Vector3f.hxx>
#include <Zeni/Core.h> // just for Error; should be removed

#include <cmath>

using namespace std;

namespace Zeni {

  Matrix4f::Matrix4f()
  {
    memset(&m_matrix, 0, sizeof(m_matrix));
  }

  Matrix4f::Matrix4f(
    const float &aw, const float &ax, const float &ay, const float &az,
    const float &bw, const float &bx, const float &by, const float &bz,
    const float &cw, const float &cx, const float &cy, const float &cz,
    const float &dw, const float &dx, const float &dy, const float &dz)
  {
    m_matrix[0][0] = aw;
    m_matrix[0][1] = ax;
    m_matrix[0][2] = ay;
    m_matrix[0][3] = az;

    m_matrix[1][0] = bw;
    m_matrix[1][1] = bx;
    m_matrix[1][2] = by;
    m_matrix[1][3] = bz;

    m_matrix[2][0] = cw;
    m_matrix[2][1] = cx;
    m_matrix[2][2] = cy;
    m_matrix[2][3] = cz;

    m_matrix[3][0] = dw;
    m_matrix[3][1] = dx;
    m_matrix[3][2] = dy;
    m_matrix[3][3] = dz;
  }
  
  Matrix4f::Matrix4f(const Vector3f &first, const Vector3f &second, const Vector3f &third, const bool &rows)
  {
    m_matrix[0][0] = first.i;
    m_matrix[1][1] = second.j;
    m_matrix[2][2] = third.k;
    m_matrix[0][3] = 0.0f;
    m_matrix[1][3] = 0.0f;
    m_matrix[2][3] = 0.0f;
    m_matrix[3][0] = 0.0f;
    m_matrix[3][1] = 0.0f;
    m_matrix[3][2] = 0.0f;
    m_matrix[3][3] = 1.0f;

    if(rows) {
      m_matrix[0][1] = first.j;
      m_matrix[0][2] = first.k;
      m_matrix[1][0] = second.i;
      m_matrix[1][2] = second.k;
      m_matrix[2][0] = third.i;
      m_matrix[2][1] = third.j;
    }
    else {
      m_matrix[0][1] = second.i;
      m_matrix[0][2] = third.i;
      m_matrix[1][0] = first.j;
      m_matrix[1][2] = third.j;
      m_matrix[2][0] = first.k;
      m_matrix[2][1] = second.k;
    }
  }

  Matrix4f::Matrix4f(const Matrix4f &rhs)
  {
    *this = rhs;
  }

  Matrix4f & Matrix4f::invert()
  {
    return *this = inverted();
  }

  Matrix4f Matrix4f::inverted() const
  {
    const float det_M = determinant();

#define m(i,j) (m_matrix[i][j])

    const float m00 = (m(1,2)*m(2,3)*m(3,1) - m(1,3)*m(2,2)*m(3,1) + m(1,3)*m(2,1)*m(3,2) - m(1,1)*m(2,3)*m(3,2) - m(1,2)*m(2,1)*m(3,3) + m(1,1)*m(2,2)*m(3,3)) / det_M;
    const float m01 = (m(0,3)*m(2,2)*m(3,1) - m(0,2)*m(2,3)*m(3,1) - m(0,3)*m(2,1)*m(3,2) + m(0,1)*m(2,3)*m(3,2) + m(0,2)*m(2,1)*m(3,3) - m(0,1)*m(2,2)*m(3,3)) / det_M;
    const float m02 = (m(0,2)*m(1,3)*m(3,1) - m(0,3)*m(1,2)*m(3,1) + m(0,3)*m(1,1)*m(3,2) - m(0,1)*m(1,3)*m(3,2) - m(0,2)*m(1,1)*m(3,3) + m(0,1)*m(1,2)*m(3,3)) / det_M;
    const float m03 = (m(0,3)*m(1,2)*m(2,1) - m(0,2)*m(1,3)*m(2,1) - m(0,3)*m(1,1)*m(2,2) + m(0,1)*m(1,3)*m(2,2) + m(0,2)*m(1,1)*m(2,3) - m(0,1)*m(1,2)*m(2,3)) / det_M;
    const float m10 = (m(1,3)*m(2,2)*m(3,0) - m(1,2)*m(2,3)*m(3,0) - m(1,3)*m(2,0)*m(3,2) + m(1,0)*m(2,3)*m(3,2) + m(1,2)*m(2,0)*m(3,3) - m(1,0)*m(2,2)*m(3,3)) / det_M;
    const float m11 = (m(0,2)*m(2,3)*m(3,0) - m(0,3)*m(2,2)*m(3,0) + m(0,3)*m(2,0)*m(3,2) - m(0,0)*m(2,3)*m(3,2) - m(0,2)*m(2,0)*m(3,3) + m(0,0)*m(2,2)*m(3,3)) / det_M;
    const float m12 = (m(0,3)*m(1,2)*m(3,0) - m(0,2)*m(1,3)*m(3,0) - m(0,3)*m(1,0)*m(3,2) + m(0,0)*m(1,3)*m(3,2) + m(0,2)*m(1,0)*m(3,3) - m(0,0)*m(1,2)*m(3,3)) / det_M;
    const float m13 = (m(0,2)*m(1,3)*m(2,0) - m(0,3)*m(1,2)*m(2,0) + m(0,3)*m(1,0)*m(2,2) - m(0,0)*m(1,3)*m(2,2) - m(0,2)*m(1,0)*m(2,3) + m(0,0)*m(1,2)*m(2,3)) / det_M;
    const float m20 = (m(1,1)*m(2,3)*m(3,0) - m(1,3)*m(2,1)*m(3,0) + m(1,3)*m(2,0)*m(3,1) - m(1,0)*m(2,3)*m(3,1) - m(1,1)*m(2,0)*m(3,3) + m(1,0)*m(2,1)*m(3,3)) / det_M;
    const float m21 = (m(0,3)*m(2,1)*m(3,0) - m(0,1)*m(2,3)*m(3,0) - m(0,3)*m(2,0)*m(3,1) + m(0,0)*m(2,3)*m(3,1) + m(0,1)*m(2,0)*m(3,3) - m(0,0)*m(2,1)*m(3,3)) / det_M;
    const float m22 = (m(0,1)*m(1,3)*m(3,0) - m(0,3)*m(1,1)*m(3,0) + m(0,3)*m(1,0)*m(3,1) - m(0,0)*m(1,3)*m(3,1) - m(0,1)*m(1,0)*m(3,3) + m(0,0)*m(1,1)*m(3,3)) / det_M;
    const float m23 = (m(0,3)*m(1,1)*m(2,0) - m(0,1)*m(1,3)*m(2,0) - m(0,3)*m(1,0)*m(2,1) + m(0,0)*m(1,3)*m(2,1) + m(0,1)*m(1,0)*m(2,3) - m(0,0)*m(1,1)*m(2,3)) / det_M;
    const float m30 = (m(1,2)*m(2,1)*m(3,0) - m(1,1)*m(2,2)*m(3,0) - m(1,2)*m(2,0)*m(3,1) + m(1,0)*m(2,2)*m(3,1) + m(1,1)*m(2,0)*m(3,2) - m(1,0)*m(2,1)*m(3,2)) / det_M;
    const float m31 = (m(0,1)*m(2,2)*m(3,0) - m(0,2)*m(2,1)*m(3,0) + m(0,2)*m(2,0)*m(3,1) - m(0,0)*m(2,2)*m(3,1) - m(0,1)*m(2,0)*m(3,2) + m(0,0)*m(2,1)*m(3,2)) / det_M;
    const float m32 = (m(0,2)*m(1,1)*m(3,0) - m(0,1)*m(1,2)*m(3,0) - m(0,2)*m(1,0)*m(3,1) + m(0,0)*m(1,2)*m(3,1) + m(0,1)*m(1,0)*m(3,2) - m(0,0)*m(1,1)*m(3,2)) / det_M;
    const float m33 = (m(0,1)*m(1,2)*m(2,0) - m(0,2)*m(1,1)*m(2,0) + m(0,2)*m(1,0)*m(2,1) - m(0,0)*m(1,2)*m(2,1) - m(0,1)*m(1,0)*m(2,2) + m(0,0)*m(1,1)*m(2,2)) / det_M;

#undef m

    return Matrix4f(
      m00, m01, m02, m03,
      m10, m11, m12, m13,
      m20, m21, m22, m23,
      m30, m31, m32, m33);
  }

  Matrix4f & Matrix4f::transpose()
  {
    for(int i = 1; i < 4; ++i)
      for(int j = 0; j < i; ++j) {
        float temp = m_matrix[i][j];
        m_matrix[i][j] = m_matrix[j][i];
        m_matrix[j][i] = temp;
      }

    return *this;
  }

  Matrix4f Matrix4f::transposed() const
  {
    Matrix4f matrix;

    for(int i = 0; i < 4; ++i)
      for(int j = 0; j < 4; ++j)
        matrix.m_matrix[i][j] = m_matrix[j][i];

    return matrix;
  }

  float Matrix4f::determinant() const
  {
#define m(i,j) m_matrix[i][j]
#define term(a,b,c,d) m(0,a)*m(1,b)*m(2,c)*m(3,d)

    return
      + term(0,1,2,3) - term(0,1,3,2)
      - term(0,2,1,3) + term(0,2,3,1)
      + term(0,3,1,2) + term(0,3,2,1)

      - term(1,0,2,3) + term(1,0,3,2)
      + term(1,2,0,3) - term(1,2,3,0)
      - term(1,3,0,2) + term(1,3,2,0)

      + term(2,0,1,3) - term(2,0,3,1)
      - term(2,1,0,3) + term(2,1,3,0)
      + term(2,3,0,1) - term(2,3,1,0)

      - term(3,0,1,2) + term(3,0,2,1)
      + term(3,1,0,2) - term(3,1,2,0)
      - term(3,2,0,1) + term(3,2,1,0);

#undef term
#undef m
  }

  //pair<Matrix4f, Matrix4f> Matrix4f::qr3() const {
  //  const Vector3f a1 = get_column(0);
  //  const Vector3f a2 = get_column(1);
  //  const Vector3f a3 = get_column(2);

  //  const Vector3f u1 = get_column(0);
  //  const Vector3f e1 = u1.normalized();
  //  Vector3f subsum = e1 * a2;

  //  const Vector3f u2 = a2 - subsum;
  //  const Vector3f e2 = u2.normalized();
  //  subsum += e2 * a3;

  //  const Vector3f u3 = a3 - subsum;
  //  const Vector3f e3 = u3.normalized();

  //  const Matrix4f q(e1, e2, e3);
  //  const Matrix4f r = q.transposed() * *this;

  //  return make_pair(q, r);
  //}

  //const Matrix4f & Matrix4f::rref() {
  //  float *row[4] = {m_matrix[0], m_matrix[1], m_matrix[2], m_matrix[3]};
  //  int ones_place[4] = {-1, -1, -1, -1};

  //  /*** Obtain zeroes in the lower-left triangle insomuch as it is possible ***/

  //  int subrow = 0;
  //  for(int k = 0; k < 4; ++k) {
  //    const int start_row = k - subrow, end_row = 4;
  //    const int start_col = k, end_col = 4;

  //    int choice = start_row;
  //    float mag = fabs(m_matrix[start_row][start_col]);
  //    for(int i = start_row + 1; i < end_row; ++i) {
  //      float mag2 = fabs(m_matrix[i][start_col]);
  //      if(mag2 > mag) {
  //        choice = i;
  //        mag = mag2;
  //      }
  //    }

  //    if(mag != 0.0f) {
  //      swap(row[start_row], row[choice]);
  //      ones_place[start_col] = choice;

  //      const float div = 1 / row[start_row][start_col];
  //      row[start_row][start_col] = 1.0f;
  //      for(int i = start_col + 1; i < end_col; ++i)
  //        row[start_row][i] *= div;

  //      for(int i = start_row + 1; i < end_row; ++i) {
  //        const float amt = row[i][start_col];
  //        row[i][start_col] = 0.0f;
  //        for(int j = start_col + 1; j < end_col; ++j)
  //          row[i][j] -= amt * row[start_row][j];
  //      }
  //    }
  //    else
  //      ++subrow;
  //  }

  //  /*** Zero out upper-left triangle insomuch as it is possible ***/

  //  for(int k = 3; k > 0; --k) {
  //    const int bottom_row = ones_place[k];
  //    if(ones_place[k] == -1)
  //      continue;

  //    for(int i = 0; i < bottom_row; ++i) {
  //      const float amt = row[i][k];
  //      row[i][k] = 0.0f;

  //      for(int j = k + 1; j < 4; ++j)
  //        row[i][j] -= amt * row[bottom_row][j];
  //    }
  //  }

  //  return *this;
  //}

  //Matrix4f Matrix4f::rrefed() const {
  //  Matrix4f matrix = *this;
  //  matrix.rref();
  //  return matrix;
  //}
  
  Vector3f Matrix4f::operator*(const Vector3f &vector) const {
    Vector3f rv;

    rv.i = m_matrix[0][0] * vector.i + m_matrix[0][1] * vector.j + m_matrix[0][2] * vector.k + m_matrix[0][3];
    rv.j = m_matrix[1][0] * vector.i + m_matrix[1][1] * vector.j + m_matrix[1][2] * vector.k + m_matrix[1][3];
    rv.k = m_matrix[2][0] * vector.i + m_matrix[2][1] * vector.j + m_matrix[2][2] * vector.k + m_matrix[2][3];

    const float scaling_factor = m_matrix[3][3];

    return rv / scaling_factor;
  }
}
