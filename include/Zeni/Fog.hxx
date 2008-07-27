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

#ifndef ZENI_FOG_HXX
#define ZENI_FOG_HXX

#include <Zeni/Fog.h>

namespace Zeni {

  FOG_TYPE Fog::get_type() const {
    return m_type;
  }

  const Color & Fog::get_color() const {
    return m_color;
  }

  const float & Fog::get_density() const {
    return m_density;
  }

  const float & Fog::get_start() const {
    return m_start;
  }

  const float & Fog::get_end() const {
    return m_end;
  }

  void Fog::set_type(const FOG_TYPE &type_) {
    m_type = type_;
  }

  void Fog::set_color(const Color &color_) {
    m_color = color_;
  }

  void Fog::set_density(const float &density_) {
    m_density = density_;
  }
  
  void Fog::set_start(const float &start_) {
    m_start = start_;
  }

  void Fog::set_end(const float &end_) {
    m_end = end_;
  }

}

#endif
