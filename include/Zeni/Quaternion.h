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

/**
 * \class Zeni::Quaternion
 *
 * \ingroup zenilib
 *
 * \brief A Featureful Quaternion Class
 *
 * Quaternion provides functionality for representing "pure" rotations.
 * You can think of a Quaternion as the combination of an axis and an angle.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_QUATERNION_H
#define ZENI_QUATERNION_H

#include <string>

#include <Zeni/Vector3f.h>
#include <Zeni/Matrix4f.h>

namespace Zeni {

  struct Quaternion {
    inline explicit Quaternion(const bool &degenerate_ = false);
    Quaternion(const float &time, const Vector3f &space, const bool &degenerate_ = false); ///< For advanced use only
    Quaternion(const float &yaw, const float &pitch, const float &roll, const bool &degenerate_ = false);
    Quaternion(const Quaternion &rhs, const bool &degenerate_ = false);

    /// Create a Quaternion from an Axis/Angle pair
    static Quaternion Axis_Angle(const Vector3f &v, const float &theta);
    /// Create a Quaternion from a Forward/Up Vector3f pair
    static Quaternion Forward_Up(const Vector3f &destination_forward,
                                 const Vector3f &destination_up,
                                 const Vector3f &default_forward,
                                 const Vector3f &default_up);

    // Vector addition/subtraction
    inline Quaternion operator+(const Quaternion &rhs) const; ///< Get the sum
    inline Quaternion operator-(const Quaternion &rhs) const; ///< Get the difference
    inline Quaternion & operator+=(const Quaternion &rhs); ///< Set equal to the sum
    inline Quaternion & operator-=(const Quaternion &rhs); ///< Set equal to the difference

    // Vector Dot-Product and Cross-Product
    inline Quaternion operator*(const Quaternion &rhs) const; ///< Get the grassman_product
    inline Quaternion operator*=(const Quaternion &rhs); ///< Get the grassman_odd_product
    inline Quaternion grassman_product(const Quaternion &rhs) const; ///< Get the Grassman-product
    inline Quaternion grassman_even_product(const Quaternion &rhs) const; ///< Get the Grassman even/inner-product
    inline Quaternion grassman_odd_product(const Quaternion &rhs) const; ///< Get the Grassman odd/outer-product
    inline Quaternion euclidean_product(const Quaternion &rhs) const; ///< Get the Euclidean product
    inline Quaternion euclidean_even_product(const Quaternion &rhs) const; ///< Get the Euclidean even/inner-product
    inline Quaternion euclidean_odd_product(const Quaternion &rhs) const; ///< Get the Euclidean odd/outer-product

    // Vector Scalar Multiplication I of II
    inline Quaternion operator*(const float &rhs) const; ///< Get the scalar multiple
    inline Quaternion operator/(const float &rhs) const; ///< Get the scalar... something
    inline Quaternion & operator*=(const float &rhs); ///< Set equal to the scalar multiple
    inline Quaternion & operator/=(const float &rhs); ///< Set equal to the scalar something
    inline Quaternion operator-() const; ///< Get the negation

    // Other Standard Functions
    Quaternion & normalize(); ///< Normalize the vector
    Quaternion normalized() const; ///< Get the normalized vector
    inline float magnitude2() const; ///< Get the 'magnitude squared' of the vector
    inline float magnitude() const; ///< Get the magnitude of the vector
    inline Quaternion conjugate() const; ///< Get the spacial conjugation
    inline Quaternion reciprocal() const;
    inline Quaternion absolute_value() const;
    inline Quaternion norm() const;
    inline Quaternion determinant() const;
    inline Quaternion adjoint() const;

    // Useful interops
    inline Vector3f operator*(const Vector3f &rhs) const; ///< Rotate a vector, maintaining constant magnitude
    inline std::pair<Vector3f, float> get_rotation() const; ///< Get the rotation in radians left about an axis
    inline Matrix4f get_matrix() const; ///< Get the matrix form of the rotation in row-major order

    // Indexing
    inline const float & operator[](const int &index) const; ///< Get 'index'
    inline float & operator[](const int &index); ///< Get 'index'

    float time;
	  Vector3f space;

    bool degenerate;
  };

  inline Quaternion operator*(const float &lhs, const Quaternion &rhs) { ///< Get the scalar multiple
    return rhs * lhs;
  }

  inline Quaternion operator/(const float &lhs, const Quaternion &rhs) { ///< Get the scalar... something
    return rhs * lhs;
  }

}

#endif
