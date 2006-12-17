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

#include <Zeni/Render_Wrapper.hxx>

#include <Zeni/Video.h>
#include <Zeni/Coordinate.h>

namespace Zeni {

  float Renderable::get_distance(const Renderable &renderable) const {
    return get_distance(renderable.get_position());
  }

  float Renderable::get_distance(const Point3f &position) const {
    Point3f point[2] = {get_position(), position};

    return sqrt(pow(point[0].x - point[1].x, 2) + pow(point[0].y - point[1].y, 2) + pow(point[0].z - point[1].z, 2));
  }

  Render_Wrapper * Render_Wrapper::get_duplicate() const {
    return new Render_Wrapper();
  }

  bool Render_Wrapper::less_than(const Render_Wrapper &rhs) const {
    return reinterpret_cast<long>(this) < reinterpret_cast<long>(&rhs);
  }

  bool Render_Wrapper::equal_to(const Render_Wrapper &rhs) const {
    return this == &rhs;
  }

  Texture_Render_Wrapper::Texture_Render_Wrapper(const std::string &texture)
    : m_texture(texture)
  {
  }

  void Texture_Render_Wrapper::prerender() const {
    Video::get_reference().apply_texture(m_texture);
  }

  void Texture_Render_Wrapper::postrender() const {
    Video::get_reference().unapply_texture();
  }

  Render_Wrapper * Texture_Render_Wrapper::get_duplicate() const {
    return new Texture_Render_Wrapper(m_texture);
  }

  bool Texture_Render_Wrapper::less_than(const Render_Wrapper &rhs) const {
    return *this < dynamic_cast<const Texture_Render_Wrapper &>(rhs);
  }

  bool Texture_Render_Wrapper::equal_to(const Render_Wrapper &rhs) const {
    return *this == dynamic_cast<const Texture_Render_Wrapper &>(rhs);
  }

  Material_Render_Wrapper::Material_Render_Wrapper(const Material &material)
    : m_material(material)
  {
  }

  void Material_Render_Wrapper::prerender() const {
    Video::get_reference().set_material(m_material);
  }

  void Material_Render_Wrapper::postrender() const {}

  Render_Wrapper * Material_Render_Wrapper::get_duplicate() const {
    return new Material_Render_Wrapper(m_material);
  }

  bool Material_Render_Wrapper::less_than(const Render_Wrapper &rhs) const {
    return *this < dynamic_cast<const Material_Render_Wrapper &>(rhs);
  }

  bool Material_Render_Wrapper::equal_to(const Render_Wrapper &rhs) const {
    return *this == dynamic_cast<const Material_Render_Wrapper &>(rhs);
  }

  Multiple_Render_Wrapper::Multiple_Render_Wrapper(Render_Wrapper *first, Render_Wrapper *second)
    : m_first(first),
    m_second(second)
  {
  }

  Multiple_Render_Wrapper::~Multiple_Render_Wrapper() {
    delete m_first;
    delete m_second;
  }

  void Multiple_Render_Wrapper::prerender() const {
    m_first->prerender();
    m_second->prerender();
  }

  void Multiple_Render_Wrapper::postrender() const {
    m_second->postrender();
    m_first->postrender();
  }

  Render_Wrapper * Multiple_Render_Wrapper::get_duplicate() const {
    return new Multiple_Render_Wrapper(m_first->get_duplicate(), m_second->get_duplicate());
  }

  bool Multiple_Render_Wrapper::less_than(const Render_Wrapper &rhs) const {
    return *this < dynamic_cast<const Multiple_Render_Wrapper &>(rhs);
  }

  bool Multiple_Render_Wrapper::equal_to(const Render_Wrapper &rhs) const {
    return *this == dynamic_cast<const Multiple_Render_Wrapper &>(rhs);
  }

}
