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
 * \class Zeni::Vector3f
 *
 * \ingroup zenilib
 *
 * \brief A Featureful 3-Space Vector Class
 *
 * Vector3f provides everything you might expect from a specialized vector 
 * class:  addition and subtraction, scalar multiplication, dot products, 
 * cross products, normalization functions, and a handful of useful constants.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_VECTOR3F_H
#define ZENI_VECTOR3F_H

#include <Zeni/Serialization.h>

namespace Zeni {

  struct Point3f;
  struct Vector2f;
  struct Vector3f;

  namespace Global {
    ZENI_DLL extern const float pi; ///< pi == 3.1415926...
    ZENI_DLL extern const float pi_over_two; ///< pi/23
    ZENI_DLL extern const float three_pi_over_two; ///< 3*pi/2
    ZENI_DLL extern const float over_three; ///< 1/3
    ZENI_DLL extern const float sqrt_two; ///< sqrt(2.0f)
    ZENI_DLL extern const float sqrt_three; ///< sqrt(3.0f)

    ZENI_DLL extern const Vector3f vector_i; ///< i == Vector3f(1, 0, 0)
    ZENI_DLL extern const Vector3f vector_j; ///< j == Vector3f(0, 1, 0)
    ZENI_DLL extern const Vector3f vector_k; ///< k == Vector3f(0, 0, 1)
  }

  struct ZENI_DLL Vector3f {
    /// The best way to create a Vector3f
    inline explicit Vector3f(const bool &degenerate_ = false);
    inline Vector3f(const float &i_, const float &j_, const float &k_, const bool &degenerate_ = false);
    inline Vector3f(const Vector3f &rhs, const bool &degenerate_ = false);
    inline Vector3f(const Point3f &rhs);
    inline explicit Vector3f(const Vector2f &rhs);

    // Vector addition/subtraction
    inline Vector3f operator+(const Vector3f &rhs) const; ///< Get the sum
    inline Vector3f operator-(const Vector3f &rhs) const; ///< Get the difference
    inline Vector3f & operator+=(const Vector3f &rhs); ///< Set equal to the sum
    inline Vector3f & operator-=(const Vector3f &rhs); ///< Set equal to the difference

    // Vector Dot-Product and Cross-Product
    inline float operator*(const Vector3f &rhs) const; ///< Get the dot-product
    inline Vector3f operator%(const Vector3f &rhs) const; ///< Get the cross-product
    inline Vector3f & operator%=(const Vector3f &rhs); ///< Set equal to the cross-product

    // Vector Scalar Multiplication I of II
    inline Vector3f operator*(const float &rhs) const; ///< Get the scalar multiple
    inline Vector3f operator/(const float &rhs) const; ///< Get the scalar... something
    inline Vector3f & operator*=(const float &rhs); ///< Set equal to the scalar multiple
    inline Vector3f & operator/=(const float &rhs); ///< Set equal to the scalar something
    inline Vector3f operator-() const; ///< Get the negation

    // Other Standard Functions
    Vector3f & normalize(); ///< Normalize the vector
    Vector3f normalized() const; ///< Get the normalized vector
    inline float magnitude2() const; ///< Get the 'magnitude squared' of the vector
    inline float magnitude() const; ///< Get the magnitude of the vector

    // Other helpful functions
    inline Vector3f get_i() const; ///< Get just the i-part
    inline Vector3f get_j() const; ///< Get just the j-part
    inline Vector3f get_k() const; ///< Get just the k-part
    inline Vector3f get_ij() const; ///< Get just the i and j parts
    inline Vector3f get_ik() const; ///< Get just the i and k parts
    inline Vector3f get_jk() const; ///< Get just the j and k parts
    inline Vector3f multiply_by(const Vector3f &rhs) const; ///< Multiply corresponding members
    inline Vector3f divide_by(const Vector3f &rhs) const; ///< Divide corresponding members
    inline float angle_between(const Vector3f &rhs) const; ///< Find the angle between the Vector3fs

    // Indexing
    inline const float & operator[](const int &index) const; ///< Get 'index'
    inline float & operator[](const int &index); ///< Get 'index'

    // Spherical Accessors and Modifiers
    float theta() const; ///< theta == radians north of vector i
    float phi() const; ///< phi   == radians down from k
    void set_spherical(const float &theta, const float &phi, const float &magnitude = 1); ///< Set the vector using spherical coordinates

    union {
      float i;
      float x;
    };
    union {
      float j;
      float y;
    };
    union {
      float k;
      float z;
    };

    bool degenerate;
  };

  // Vector Scalar Multiplication Part II of II
  inline Vector3f operator*(const float &lhs, const Vector3f &rhs); ///< Get the scalar multiple

  ZENI_DLL std::ostream & serialize(std::ostream &os, const Vector3f &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, Vector3f &value);
}

#endif
