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

/**
 * \class Zeni::Vector3f
 *
 * \ingroup Zenilib
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

#include <cmath>
#include <iostream>

namespace Zeni {

  struct Vector2f;
  struct Vector3f;

  // Here are some awesome constants that you might want to use
  // These are defined in the header file so that you can use them to initialize other constants,
  // without having to worry that they might be initialized in the wrong order.
  const float pi = 3.1415926535897932384626433832795f;
  const float one_over_pi = 0.31830988618379067153776752674503f;
  const float two_pi = 6.283185307179586476925286766559f;
  const float pi_over_two = 1.5707963267948966192313216916398f;
  const float three_pi_over_two = 4.7123889803846898576939650749193f;
  const float over_three = 0.33333333333333333333333333333333f;
  const float sqrt_two = 1.4142135623730950488016887242097f;
  const float sqrt_three = 1.7320508075688772935274463415059f;
  const float cos_45 = 0.70710678118654752440084436210485f;
  const float sqrt_two_over_two = cos_45; // Just in case you forget, or like this name better
  // There are some other Vector constants at the bottom of this file, that require the Vector definition to initialize.

  struct Vector3f {
    /// The best way to create a Vector3f
    inline Vector3f();
    inline Vector3f(const float &i_, const float &j_, const float &k_);
    inline Vector3f(const Vector3f &rhs);

	// Interop    
	inline explicit Vector3f(const Vector2f &rhs); ///< z is set to 0.0f

	inline bool operator==(const Vector3f &rhs) const; ///< A simple equality test.
    inline bool operator!=(const Vector3f &rhs) const; ///< inverse of ==

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
    inline Vector3f operator*(const float &rhs) const; ///< Get the scalar product
    inline Vector3f operator/(const float &rhs) const; ///< Get the scalar quotient
    inline Vector3f & operator*=(const float &rhs); ///< Set equal to the scalar product
    inline Vector3f & operator/=(const float &rhs); ///< Set equal to the scalar quotient
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
    inline Vector3f interpolate_to(const float &rhs_part, const Vector3f &rhs) const;
    inline float angle_between(const Vector3f &rhs) const; ///< Find the angle between the Vector3fs

    // Indexing
    inline const float & operator[](const int &index) const; ///< Get 'index'
    inline float & operator[](const int &index); ///< Get 'index'

	// Streaming
	friend std::ostream& operator <<(std::ostream &os,const Vector3f &v);
  	friend std::istream& operator >>(std::istream &is, Vector3f &v);

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
  };

  // Vector Scalar Multiplication Part II of II
  inline Vector3f operator*(const float &lhs, const Vector3f &rhs); ///< Get the scalar multiple

  // Here are some more awesome constants, in Vector form!
  const Vector3f vector_i(1.0f, 0.0f, 0.0f);
  const Vector3f vector_j(0.0f, 1.0f, 0.0f);
  const Vector3f vector_k(0.0f, 0.0f, 1.0f);

  static const Vector3f kZero3 = Vector3f( 0.0f, 0.0f, 0.0f );
  static const Vector3f kOne3 = Vector3f( 1.0f, 1.0f, 1.0f );
}

#endif
