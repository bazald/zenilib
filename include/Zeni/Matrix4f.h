/* This file is part of the Zenipex Library.
* Copyleft (C) 2007 Mitchell Keith Bloch a.k.a. bazald
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

#ifndef ZENI_MATRIX_H
#define ZENI_MATRIX_H

#include "Vector3f.h"

namespace Zeni {

  struct Matrix4f {
  public:
    /// The best way to create a Vector3f
    Matrix4f();
    Matrix4f(
      const float &aw, const float &ax, const float &ay, const float &az,
      const float &bw, const float &bx, const float &by, const float &bz,
      const float &cw, const float &cx, const float &cy, const float &cz,
      const float &dw, const float &dx, const float &dy, const float &dz);
    Matrix4f(const Matrix4f &rhs);

    // Vector addition/subtraction
    inline Matrix4f operator+(const Matrix4f &rhs) const; ///< Get the sum
    inline Matrix4f operator-(const Matrix4f &rhs) const; ///< Get the difference
    inline Matrix4f & operator+=(const Matrix4f &rhs); ///< Set equal to the sum
    inline Matrix4f & operator-=(const Matrix4f &rhs); ///< Set equal to the difference

    // Vector Dot-Product and Cross-Product
    inline Matrix4f operator*(const Matrix4f &rhs) const; ///< Get the product
    inline Matrix4f operator*=(const Matrix4f &rhs); ///< Get the product
    inline Matrix4f operator/(const Matrix4f &rhs) const; ///< Get the product with the inverse
    inline Matrix4f operator/=(const Matrix4f &rhs); ///< Set equal to the product with the inverse

    // Vector Scalar Multiplication I of II
    inline Matrix4f operator*(const float &rhs) const; ///< Get the scalar multiple
    inline Matrix4f operator/(const float &rhs) const; ///< Get the scalar... something
    inline Matrix4f & operator*=(const float &rhs); ///< Set equal to the scalar multiple
    inline Matrix4f & operator/=(const float &rhs); ///< Set equal to the scalar something
    inline Matrix4f operator-() const; ///< Get the negation

    // Other Standard Functions
    Matrix4f & invert(); ///< Invert the matrix
    Matrix4f inverted() const; ///< Get the inverse
    Matrix4f & transpose(); ///< Transpose the matrix
    Matrix4f transposed() const; ///< Get the transpose
    float determinant() const; ///< Get the determinant

  private:
	  float m_matrix[4][4];
  };

}

#ifdef ZENI_INLINES
#include "Matrix4f.hxx"
#endif

#endif
