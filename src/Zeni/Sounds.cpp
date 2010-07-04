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

#include <Zeni/Sounds.h>

#include <Zeni/Database.hxx>
#include <Zeni/Sound.hxx>
#include <Zeni/Sound_Source_Pool.h>
#include <Zeni/Coordinate.hxx>
#include <Zeni/Mutex.hxx>
#include <Zeni/Vector3f.hxx>

#include <iostream>
#include <fstream>

namespace Zeni {

  Sounds::Sounds()
    : Database<Sound_Buffer>("config/sounds.xml", "Sounds")
  {
    init();
  }

  Sounds::~Sounds() {
    uninit();
  }

  Sounds & get_Sounds() {
    static Sounds e_sounds;
    return e_sounds;
  }

  Sound_Buffer * Sounds::load(XML_Element_c &xml_element, const std::string &/*name*/, const std::string &/*filename*/) {
    const std::string filepath = xml_element["filepath"].to_string();

    return new Sound_Buffer(filepath);
  }

}
