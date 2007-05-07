/* This file is part of the Zenipex Library.
* Copyleft (C) 2006 Mitchell Keith Bloch a.k.a. bazald
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

#ifndef ZENI_VERTEX2F_HXX
#define ZENI_VERTEX2F_HXX

#include "Vertex2f.h"

namespace Zeni {

  void Vertex2f::set_position(const Point3f &position) {
    m_position = position;
  }

  void Vertex2f::set_position(const Point2f &position) {
    m_position = Point3f(position.x, position.y, position.z);
  }

  void * Vertex2f::get_address() const {
    return const_cast<Point3f *>(&m_position);
  }

  unsigned long Vertex2f::get_offset() const {
    return reinterpret_cast<unsigned long>(&m_position) - reinterpret_cast<unsigned long>(this);
  }

  const unsigned long & Vertex2f_Color::get_color() const {
    return m_argb;
  }

  void Vertex2f_Color::set_color(const long &argb) {
    m_argb = argb;
  }

  Point2f Vertex2f_Texture::get_texture_coordinate() const {
    return m_texture_coordinate;
  }

  void Vertex2f_Texture::set_texture_coordinate(const Point2f &texture_coordinate) {
    m_texture_coordinate = texture_coordinate;
  }

}

#endif
