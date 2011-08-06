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

#ifndef IV_H
#define IV_H

/**
 * \brief Inline Virtual base class
 *
 * This template offers the storage for the virtual type specifier.
 *
 * Additionally, it offers a getter for the type.
 *
 * \note This pattern should be used with caution.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

template <class Base_Class,
          typename BASE_VTYPE>
class IV {
  BASE_VTYPE m_vtype;

protected:
  typedef BASE_VTYPE VTYPE;

  IV(const VTYPE &vtype_) : m_vtype(vtype_) {}
  ~IV() {}

public:
  VTYPE vtype() const {return m_vtype;}
};

#endif
