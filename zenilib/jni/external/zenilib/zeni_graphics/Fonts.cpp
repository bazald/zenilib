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

#include <zeni_graphics.h>

#include <iostream>
#include <fstream>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Database.hxx>
#include <Zeni/Singleton.hxx>

namespace Zeni {
  
  template class Singleton<Fonts>;
  template class Database<Font>;

  Fonts * Fonts::create() {
    return new Fonts;
  }

  Fonts::Lose Fonts::g_lose;
  Fonts::Unlose Fonts::g_unlose;

  Fonts::Fonts()
    : Database<Font>("config/fonts.xml", "Fonts")
  {
    Video &vr = get_Video();

    Database<Font>::init();

    vr.lend_pre_uninit(&g_lose);
    vr.lend_post_reinit(&g_unlose);
  }

  Fonts::~Fonts() {
    Video::remove_pre_uninit(&g_lose);

    Database<Font>::uninit();
  }

  Fonts & get_Fonts() {
    return Singleton<Fonts>::get();
  }

  Font * Fonts::load(XML_Element_c &xml_element, const String &/*name*/, const String &/*filename*/) {
    const String filepath = xml_element["filepath"].to_string();
    const float height = xml_element["height"].to_float();

    const float screen_height = float(get_Window().get_height());
    float virtual_screen_height = screen_height;
    XML_Element_c virtual_screen = xml_element["virtual_screen"];
    if(virtual_screen.good()) {
      XML_Element_c vsw = virtual_screen["width"];
      XML_Element_c vsh = virtual_screen["height"];

      if(vsw.good()) {
        const float vsw_ratio = vsw.to_float() / get_Window().get_width();

        if(vsh.good()) {
          virtual_screen_height = vsh.to_float();

          if(vsw_ratio > virtual_screen_height / screen_height)
            virtual_screen_height = vsw_ratio * screen_height;
        }
        else
          virtual_screen_height = vsw_ratio * screen_height;
      }
      else if(vsh.good())
        virtual_screen_height = vsh.to_float();
    }

    return get_Video().create_Font(filepath, height, virtual_screen_height);
  }

}
