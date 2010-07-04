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

#include <Zeni/Matrix4f.hxx>

#include <Zeni/Vector3f.hxx>
#include <Zeni/Core.h> // just for Error; should be removed

#include <cmath>

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
    m_matrix[1][0] = ax;
    m_matrix[2][0] = ay;
    m_matrix[3][0] = az;

    m_matrix[0][1] = bw;
    m_matrix[1][1] = bx;
    m_matrix[2][1] = by;
    m_matrix[3][1] = bz;

    m_matrix[0][2] = cw;
    m_matrix[1][2] = cx;
    m_matrix[2][2] = cy;
    m_matrix[3][2] = cz;

    m_matrix[0][3] = dw;
    m_matrix[1][3] = dx;
    m_matrix[2][3] = dy;
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
      m_matrix[1][0] = first.j;
      m_matrix[2][0] = first.k;
      m_matrix[0][1] = second.i;
      m_matrix[2][1] = second.k;
      m_matrix[0][2] = third.i;
      m_matrix[1][2] = third.j;
    }
    else {
      m_matrix[1][0] = second.i;
      m_matrix[2][0] = third.i;
      m_matrix[0][1] = first.j;
      m_matrix[2][1] = third.j;
      m_matrix[0][2] = first.k;
      m_matrix[1][2] = second.k;
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

#define m(i,j) (m_matrix[j][i])

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
#define m(i,j) m_matrix[j][i]
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
  
  Vector3f Matrix4f::operator*(const Vector3f &vector) const {
    return Vector3f(m_matrix[0][0] * vector.i + m_matrix[1][0] * vector.j + m_matrix[2][0] * vector.k + m_matrix[3][0],
                    m_matrix[0][1] * vector.i + m_matrix[1][1] * vector.j + m_matrix[2][1] * vector.k + m_matrix[3][1],
                    m_matrix[0][2] * vector.i + m_matrix[1][2] * vector.j + m_matrix[2][2] * vector.k + m_matrix[3][2]);
  }
}
