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

#include <Zeni/Colors.h>

#include <iomanip>
#include <fstream>

using std::string;
using std::map;
using std::ifstream;
using std::hex;

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

  void Colors::init() {
    m_color.clear();
    ifstream colorin(m_colordb.c_str());

    if(!colorin)
      throw Colors_Init_Failure();

    Color tmp;
    short r, g, b, a;
    string name;
    while(colorin >> name >> hex >> r >> g >> b >> a)
      m_color[name] = Color(r/256.0f, g/256.0f, b/256.0f, a/256.0f);
  }

  const Color & Colors::access_color(const string &color) const {
    map<string, Color>::const_iterator it = m_color.find(color);

    if(it == m_color.end())
      throw Color_Not_Found();

    return it->second;
  }

}
