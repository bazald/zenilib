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

#include <zeni.h>

#include <iomanip>
#include <iostream>
#include <fstream>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Database.hxx>
#include <Zeni/Singleton.hxx>

namespace Zeni {
  
  template class Singleton<Colors>;
  template class Database<Color>;

  Colors * Colors::create() {
    return new Colors;
  }

  Colors::Colors()
    : Database<Color>("config/colors.xml", "Colors")
  {
    init();
  }

  Colors::~Colors() {
    Database<Color>::uninit();
  }

  Colors & get_Colors() {
    return Colors::get();
  }

  Color * Colors::load(XML_Element_c &xml_element, const String &/*name*/, const String &/*filename*/) {
    const float alpha = xml_element["alpha"].to_float();
    const float red = xml_element["red"].to_float();
    const float green = xml_element["green"].to_float();
    const float blue = xml_element["blue"].to_float();

    return new Color(alpha, red, green, blue);
  }

}
