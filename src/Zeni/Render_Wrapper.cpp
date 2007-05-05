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

#include <Zeni/Material.hxx>

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

  Material_Render_Wrapper::Material_Render_Wrapper(const Material &material)
    : m_material(material),
      optimization(0)
  {
  }

  void Material_Render_Wrapper::prerender() const {
    Video::get_reference().set_material(m_material, optimization);
  }

  void Material_Render_Wrapper::postrender() const {
    Video::get_reference().unset_material(m_material, optimization);
  }

  Render_Wrapper * Material_Render_Wrapper::get_duplicate() const {
    return new Material_Render_Wrapper(m_material);
  }

  bool Material_Render_Wrapper::less_than(const Render_Wrapper &rhs) const {
    return !equal_to(rhs) && this < &rhs;///HACK *this < dynamic_cast<const Material_Render_Wrapper &>(rhs);
  }

  bool Material_Render_Wrapper::equal_to(const Render_Wrapper &rhs) const {
    return *this == dynamic_cast<const Material_Render_Wrapper &>(rhs);
  }

  void Material_Render_Wrapper::optimize_to_follow(const Render_Wrapper &rhs) {
    const Material_Render_Wrapper &mrw = dynamic_cast<const Material_Render_Wrapper &>(rhs);
    const Material &material = mrw.get_material();

    if(m_material.get_ambient() == material.get_ambient())
      optimization |= (1 << 0);
    if(m_material.get_diffuse() == material.get_diffuse())
      optimization |= (1 << 1);
    if(m_material.get_specular() == material.get_specular())
      optimization |= (1 << 2);
    if(m_material.get_emissive() == material.get_emissive())
      optimization |= (1 << 3);
    if(m_material.get_power() == material.get_power())
      optimization |= (1 << 4);
    if(m_material.get_texture() == material.get_texture())
      optimization |= (1 << 5);
  }

  void Material_Render_Wrapper::optimize_to_precede(const Render_Wrapper &rhs) {
    const Material_Render_Wrapper &mrw = dynamic_cast<const Material_Render_Wrapper &>(rhs);
    const Material &material = mrw.get_material();

    if(m_material.get_ambient() == material.get_ambient())
      optimization |= (1 << 6);
    if(m_material.get_diffuse() == material.get_diffuse())
      optimization |= (1 << 7);
    if(m_material.get_specular() == material.get_specular())
      optimization |= (1 << 8);
    if(m_material.get_emissive() == material.get_emissive())
      optimization |= (1 << 9);
    if(m_material.get_power() == material.get_power())
      optimization |= (1 << 10);
    if(m_material.get_texture() == material.get_texture())
      optimization |= (1 << 11);
  }

  void Material_Render_Wrapper::clear_optimization() {
    optimization = 0;
  }

}
