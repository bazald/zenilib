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

#include <Zeni/Serialization.h>
#include <Zeni/String.h>
#include <Zeni/Matrix4f.h>

#include <Zeni/Define.h>

namespace Zeni {

  struct ZENI_DLL Quaternion {
    inline explicit Quaternion(const bool &degenerate_ = false);
    Quaternion(const float &time, const Vector3f &space, const bool &degenerate_ = false); ///< For advanced use only
    Quaternion(const float &yaw, const float &pitch, const float &roll, const bool &degenerate_ = false);
    Quaternion(const Quaternion &rhs, const bool &degenerate_ = false);

    /// Create a Quaternion from an Axis/Angle pair
    static Quaternion Axis_Angle(const Vector3f &v, const float &theta);
    /// Create a Quaternion rotating to one Vector3f from another
    static Quaternion Vector3f_to_Vector3f(const Vector3f &destination, const Vector3f &source);
    /// Create a Quaternion from a Forward/Up Vector3f pair
    static Quaternion Forward_Up(const Vector3f &destination_forward,
                                 const Vector3f &destination_up,
                                 const Vector3f &default_forward = ZENI_DEFAULT_FORWARD_VECTOR,
                                 const Vector3f &default_up = ZENI_DEFAULT_UP_VECTOR);

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

  ZENI_DLL std::ostream & serialize(std::ostream &os, const Quaternion &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, Quaternion &value);

}

#include <Zeni/Undefine.h>

#endif
