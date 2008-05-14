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

#include <Zeni/Colors.h>
#include <Zeni/Resource.hxx>

#include <iomanip>
#include <iostream>
#include <fstream>

using namespace std;

namespace Zeni {

  Colors::Colors()
    : m_colordb("config/colors.txt")
  {
    init();
  }

  Colors & Colors::get_reference() {
    static Colors e_color;
    return e_color;
  }

  void Colors::reload(const std::string &color) {
    if(color.length())
      m_colordb = color;
    init();
  }

  Uint32 Colors::get_color_id(const string &color) const {
    stdext::hash_map<string, unsigned long>::const_iterator it = m_color_lookup.find(color);

    if(it == m_color_lookup.end() || !it->second)
      throw Color_Not_Found(color);

    return it->second;
  }

  Color Colors::get_color(const string &color) const {
    return get_color(get_color_id(color));
  }

  Color Colors::get_color(const Uint32 &color) const {
    stdext::hash_map<unsigned long, Color>::const_iterator it = m_color.find(color);

    if(it == m_color.end()) {
      char buf[64];
#ifdef _WINDOWS
      sprintf_s
#else
      sprintf
#endif
        (buf, "ID = %u", static_cast<unsigned int>(color));
      throw Color_Not_Found(buf);
    }

    return it->second;
  }

  Uint32 Colors::set_color(const std::string &name, const Color &color) {
    Uint32 id = Resource::get_reference().assign();
    m_color_lookup[name] = id;
    m_color[id] = color;
    return id;
  }

  void Colors::clear_color(const std::string &name) {
    stdext::hash_map<string, unsigned long>::iterator it = m_color_lookup.find(name);

    if(it == m_color_lookup.end())
      throw Color_Not_Found(name);

    m_color.erase(it->second);
    m_color_lookup.erase(it);
  }

  void Colors::init() {
    m_color.clear();
    ifstream colorin(m_colordb.c_str());

    if(!colorin)
      throw Colors_Init_Failure();

    Color tmp;
    short a, r, g, b;
    string name;
    while(colorin >> name >> hex >> a >> r >> g >> b) {
      unsigned long id = Resource::get_reference().assign();
      m_color_lookup[name] = id;
      m_color[id] = Color(a/256.0f, r/256.0f, g/256.0f, b/256.0f);
    }
  }

}
