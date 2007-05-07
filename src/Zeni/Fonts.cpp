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

#include <Zeni/Fonts.h>
#include <Zeni/Video.h>

#include <Zeni/Colors.hxx>

#include <iostream>
#include <fstream>

using namespace std;

namespace Zeni {

  Fonts::Fonts() {
    init();
  }

  Fonts::~Fonts() {
    uninit();
  }

  Fonts & Fonts::get_reference() {
    static Fonts e_fonts;
    return e_fonts;
  }

  const Font & Fonts::get_font(const string &font) const {
    map<string, Font *>::const_iterator it = m_fonts.find(font);

    if(it == m_fonts.end()) {
      std::cerr << "Missing Font: " << font << std::endl;
      throw Font_Not_Found();
    }

    return *it->second;
  }

  void Fonts::reload(const string &filename) {
    if(filename.length() != 0)
      m_filename = filename;

    uninit();
    init();
  }

  void Fonts::init() {
    ifstream fin(m_filename.c_str());

    if(!fin)
      throw Fonts_Init_Failure();

    TTF_Init();

    string name, codename, color;
    bool bold, italic;
    int height;
    while(fin >> name >> bold >> italic >> height >> ws) {
      getline(fin, codename);
      while(!codename.empty() && isspace(codename[codename.size() - 1]))
        codename.resize(codename.size() - 1);

      try {
        m_fonts[name] = Video::get_reference().create_Font(codename, bold, italic, height);
      }
      catch(...) {
        uninit();
        throw;
      }
    }
  }

  void Fonts::uninit() {
    for(map<string, Font *>::iterator it = m_fonts.begin(); it != m_fonts.end(); ++it)
      delete it->second;
    m_fonts.clear();
    TTF_Quit();
  }

  string Fonts::m_filename = "config/fonts.txt";

}
