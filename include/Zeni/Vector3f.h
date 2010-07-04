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

namespace Zeni {

  struct Point3f;
  struct Vector3f;

  extern const float pi; ///< pi == 3.1415926...
  extern const float pi_over_two; ///< pi/2
  extern const float three_pi_over_two; ///< 3*pi/2
  extern const float over_three; ///< 1/3
  extern const float sqrt_two; ///< sqrt(2.0f)
  extern const float sqrt_three; ///< sqrt(3.0f)

  extern const Vector3f vector_i; ///< i == Vector3f(1, 0, 0)
  extern const Vector3f vector_j; ///< j == Vector3f(0, 1, 0)
  extern const Vector3f vector_k; ///< k == Vector3f(0, 0, 1)

  struct Vector3f {
    /// The best way to create a Vector3f
    inline explicit Vector3f(const bool &degenerate_ = false);
    inline Vector3f(const float &i_, const float &j_, const float &k_, const bool &degenerate_ = false);
    inline Vector3f(const Vector3f &rhs, const bool &degenerate_ = false);
    inline Vector3f(const Point3f &rhs);

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

}

#endif
