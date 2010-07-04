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

#ifndef ZENI_RENDERABLE_HXX
#define ZENI_RENDERABLE_HXX

// HXXed below
#include <Zeni/Material.h>

#include <Zeni/Renderable.h>

namespace Zeni {

  Renderable::Renderable()
    : m_material(0),
    delete_m_material(false)
  {
  }

  Renderable::Renderable(const Renderable &rhs)
    : m_material(0),
    delete_m_material(false)
  {
    if(rhs.delete_m_material)
      fax_Material(rhs.m_material);
    else
      lend_Material(rhs.m_material);
  }

  Renderable & Renderable::operator=(const Renderable &rhs) {
    if(rhs.delete_m_material)
      fax_Material(rhs.m_material);
    else
      lend_Material(rhs.m_material);

    return *this;
  }

  const Material * Renderable::get_Material() const {
    return m_material;
  }

}

#include <Zeni/Material.hxx>

#endif
