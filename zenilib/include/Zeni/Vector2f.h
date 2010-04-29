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
 * \class Zeni::Vector2f
 *
 * \ingroup Zenilib
 *
 * \brief A 2-Space Vector Class
 *
 * Vector2f provides everything you might expect from a specialized vector 
 * class:  addition and subtraction, scalar multiplication.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_VECTOR2F_H
#define ZENI_VECTOR2F_H

#include <cmath>
#include <iostream>

//#include <Zeni/Vector3f.h>

namespace Zeni {

  struct Vector3f;

  struct Vector2f {
    /// The best way to create a Vector2f
    inline Vector2f();
    inline Vector2f(const float &i_, const float &j_);
    inline Vector2f(const Vector2f &rhs);

	// Interop    
	inline explicit Vector2f(const Vector3f &rhs); ///< rhs.z is ignored

	inline bool operator==(const Vector2f &rhs) const; ///< A simple equality test.
    inline bool operator!=(const Vector2f &rhs) const; ///< inverse of ==

    // Vector addition/subtraction
    inline Vector2f operator+(const Vector2f &rhs) const; ///< Get the sum
    inline Vector2f operator-(const Vector2f &rhs) const; ///< Get the difference
    inline Vector2f & operator+=(const Vector2f &rhs); ///< Set equal to the sum
    inline Vector2f & operator-=(const Vector2f &rhs); ///< Set equal to the difference

    // Vector Dot-Product
    inline float operator*(const Vector2f &rhs) const; ///< Get the dot-product

	// Vector Scalar Multiplication I of II
    inline Vector2f operator*(const float &rhs) const; ///< Get the scalar product
    inline Vector2f operator/(const float &rhs) const; ///< Get the scalar quotient
    inline Vector2f & operator*=(const float &rhs); ///< Set equal to the scalar product
    inline Vector2f & operator/=(const float &rhs); ///< Set equal to the scalar quotient
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
    inline Vector2f interpolate_to(const float &rhs_part, const Vector2f &rhs) const;
    inline float angle_between(const Vector2f &rhs) const; ///< Find the angle between the Vector2fs

    // Indexing
    inline const float & operator[](const int &index) const; ///< Get 'index'
    inline float & operator[](const int &index); ///< Get 'index'

	// Streaming
	friend std::ostream& operator <<(std::ostream &os,const Vector2f &v);
  	friend std::istream& operator >>(std::istream &is, Vector2f &v);

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
  };

  // Vector Scalar Multiplication Part II of II
  inline Vector2f operator*(const float &lhs, const Vector2f &rhs); ///< Get the scalar multiple
}

#endif
