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

#include <Zeni/Fonts.h>

#include <Zeni/Database.hxx>
#include <Zeni/Video.hxx>

#include <iostream>
#include <fstream>

using namespace std;

namespace Zeni {

  Fonts::Fonts()
    : Database<Font>("config/fonts.xml", "Fonts")
  {
    TTF_Init();

    init();
  }

  Fonts::~Fonts() {
    uninit();

    TTF_Quit();
  }

  Fonts & get_Fonts() {
    static Fonts e_fonts;
    return e_fonts;
  }

  Font * Fonts::load(XML_Element_c &xml_element) {
    const string filepath = xml_element["filepath"].to_string();
    const float height = xml_element["height"].to_float();
    const bool bold = xml_element["bold"].to_bool();
    const bool italics = xml_element["italics"].to_bool();
    const float virtual_screen_height = xml_element["virtual_screen_height"].to_float();

    return get_Video().create_Font(filepath, bold, italics, height, virtual_screen_height);
  }

}
