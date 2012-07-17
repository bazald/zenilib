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
 * \class Zeni::Vector2f
 *
 * \ingroup zenilib
 *
 * \brief A 2-Space Vector Class
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_VECTOR2F_H
#define ZENI_VECTOR2F_H

#include <Zeni/Serialization.h>

namespace Zeni {

  struct Point2f;
  struct Vector2f;
  struct Vector3f;

  struct ZENI_DLL Vector2f {
    /// The best way to create a Vector3f
    inline explicit Vector2f(const bool &degenerate_ = false);
    inline Vector2f(const float &i_, const float &j_, const bool &degenerate_ = false);
    inline Vector2f(const Vector2f &rhs, const bool &degenerate_ = false);
    inline Vector2f(const Point2f &rhs);
    inline explicit Vector2f(const Vector3f &rhs);

    // Vector addition/subtraction
    inline Vector2f operator+(const Vector2f &rhs) const; ///< Get the sum
    inline Vector2f operator-(const Vector2f &rhs) const; ///< Get the difference
    inline Vector2f & operator+=(const Vector2f &rhs); ///< Set equal to the sum
    inline Vector2f & operator-=(const Vector2f &rhs); ///< Set equal to the difference

    // Vector Dot-Product and Cross-Product
    inline float operator*(const Vector2f &rhs) const; ///< Get the dot-product

    // Vector Scalar Multiplication I of II
    inline Vector2f operator*(const float &rhs) const; ///< Get the scalar multiple
    inline Vector2f operator/(const float &rhs) const; ///< Get the scalar... something
    inline Vector2f & operator*=(const float &rhs); ///< Set equal to the scalar multiple
    inline Vector2f & operator/=(const float &rhs); ///< Set equal to the scalar something
    inline Vector2f operator-() const; ///< Get the negation

    // Other Standard Functions
    Vector2f & normalize(); ///< Normalize the vector
    Vector2f normalized() const; ///< Get the normalized vector
    inline float magnitude2() const; ///< Get the 'magnitude squared' of the vector
    inline float magnitude() const; ///< Get the magnitude of the vector

    // Other helpful functions
    inline Vector2f get_i() const; ///< Get just the i-part
    inline Vector2f get_j() const; ///< Get just the j-part
    inline Vector2f multiply_by(const Vector2f &rhs) const; ///< Multiply corresponding members
    inline Vector2f divide_by(const Vector2f &rhs) const; ///< Divide corresponding members
    inline float angle_between(const Vector2f &rhs) const; ///< Find the angle between the Vector3fs

    // Indexing
    inline const float & operator[](const int &index) const; ///< Get 'index'
    inline float & operator[](const int &index); ///< Get 'index'

    // Spherical Accessors and Modifiers
    float theta() const; ///< theta == radians north of vector i
    void set_spherical(const float &theta, const float &magnitude = 1); ///< Set the vector using spherical coordinates

    union {
      float i;
      float x;
    };
    union {
      float j;
      float y;
    };

    bool degenerate;
  };

  // Vector Scalar Multiplication Part II of II
  inline Vector2f operator*(const float &lhs, const Vector2f &rhs); ///< Get the scalar multiple

  ZENI_DLL std::ostream & serialize(std::ostream &os, const Vector2f &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, Vector2f &value);
}

#endif
