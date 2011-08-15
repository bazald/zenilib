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
 * \class Zeni::Point2i
 *
 * \ingroup zenilib
 *
 * \brief A 2D Point represented with integers
 *
 * \note Point2f is often a better choice.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Point2f
 *
 * \ingroup zenilib
 *
 * \brief A 2D Point represented with floats
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Point3i
 *
 * \ingroup zenilib
 *
 * \brief A 3D Point represented with integers
 *
 * \note Point3f is often a better choice.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Point3f
 *
 * \ingroup zenilib
 *
 * \brief A 3D Point represented with floats
 *
 * \note As of the 0.3.0.0 release, this inherits from Vector3f.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_COORDINATE_H
#define ZENI_COORDINATE_H

#include <Zeni/Vector2f.h>
#include <Zeni/Vector3f.h>

namespace Zeni {

  struct ZENI_DLL Point2i;
  struct ZENI_DLL Point2f;
  struct ZENI_DLL Point3i;
  struct ZENI_DLL Point3f;

  struct ZENI_DLL Point2i {
    inline Point2i();
    inline Point2i(const int &x_, const int &y_);
    inline explicit Point2i(const Point2f &rhs);
    inline explicit Point2i(const Point3i &rhs); ///< z in rhs is ignored.
    inline explicit Point2i(const Point3f &rhs); ///< z in rhs is ignored.

    Point2i interpolate_to(const float &rhs_part, const Point2i &rhs) const; ///< Get a point inbetween this point and another point of the same type.

    Sint32 x, y;
    static const Sint32 z;
  };

  struct ZENI_DLL Point2f {
    inline Point2f();
    inline Point2f(const float &x_, const float &y_);
    inline explicit Point2f(const Point2i &rhs);
    inline explicit Point2f(const Point3i &rhs); ///< z in rhs is ignored.
    inline explicit Point2f(const Point3f &rhs); ///< z in rhs is ignored.
    inline Point2f(const Vector2f &rhs);

    Point2f interpolate_to(const float &rhs_part, const Point2f &rhs) const; ///< Get a point inbetween this point and another point of the same type.

    // Subtraction
    inline Vector2f operator-(const Point2f &rhs) const;

    // Vector3f addition
    inline Point2f operator+(const Vector2f &rhs) const;
    inline Point2f operator-(const Vector2f &rhs) const;
    inline Point2f & operator+=(const Vector2f &rhs);
    inline Point2f & operator-=(const Vector2f &rhs);

    float x, y;
    static const float z;
  };

  struct ZENI_DLL Point3i {
    inline Point3i();
    inline Point3i(const int &x_, const int &y_, const int &z_);
    inline explicit Point3i(const Point2i &rhs); ///< z is set to 0
    inline explicit Point3i(const Point2f &rhs); ///< z is set to 0
    inline explicit Point3i(const Point3f &rhs);

    Point3i interpolate_to(const float &rhs_part, const Point3i &rhs) const; ///< Get a point inbetween this point and another point of the same type.

    Sint32 x, y, z;
  };

  struct ZENI_DLL Point3f {
    inline Point3f();
    inline Point3f(const float &x_, const float &y_, const float &z_);
    inline explicit Point3f(const Point2i &rhs); ///< z is set to 0.0f
    inline explicit Point3f(const Point2f &rhs); ///< z is set to 0.0f
    inline explicit Point3f(const Point3i &rhs);
    inline Point3f(const Vector3f &rhs);

    Point3f interpolate_to(const float &rhs_part, const Point3f &rhs) const;

    // Subtraction
    inline Vector3f operator-(const Point3f &rhs) const;

    // Vector3f addition
    inline Point3f operator+(const Vector3f &rhs) const;
    inline Point3f operator-(const Vector3f &rhs) const;
    inline Point3f & operator+=(const Vector3f &rhs);
    inline Point3f & operator-=(const Vector3f &rhs);

    float x, y, z;
  };

  ZENI_DLL std::ostream & serialize(std::ostream &os, const Point2i &value);
  ZENI_DLL std::ostream & serialize(std::ostream &os, const Point2f &value);
  ZENI_DLL std::ostream & serialize(std::ostream &os, const Point3i &value);
  ZENI_DLL std::ostream & serialize(std::ostream &os, const Point3f &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, Point2i &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, Point2f &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, Point3i &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, Point3f &value);

}

#endif
