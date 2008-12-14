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

#include <Zeni/Video_GL.h>
#include <Zeni/Resource.hxx>
#include <Zeni/Video.hxx>
#include <Zeni/XML.hxx>

#include <iostream>
#include <fstream>

using namespace std;

namespace Zeni {

  Fonts::Fonts()
    : m_filename("config/fonts.xml")
  {
    init();
  }

  Fonts::~Fonts() {
    uninit();
  }

  Fonts & get_Fonts() {
    static Fonts e_fonts;
    return e_fonts;
  }

  unsigned long Fonts::get_font_id(const string &font) const {
    stdext::hash_map<string, unsigned long>::const_iterator it = m_font_lookup.find(font);

    if(it == m_font_lookup.end() || !it->second)
      throw Font_Not_Found(font);

    return it->second;
  }

  Font & Fonts::operator[](const std::string &font) const {
    return (*this)[get_font_id(font)];
  }

  Font & Fonts::operator[](const unsigned long &font) const {
    stdext::hash_map<unsigned long, Font *>::const_iterator it = m_fonts.find(font);

    if(it == m_fonts.end()) {
      char buf[64];
#ifdef _WINDOWS
      sprintf_s
#else
      sprintf
#endif
        (buf, "ID = %u", static_cast<unsigned int>(font));
      throw Font_Not_Found(buf);
    }

    return *it->second;
  }

  unsigned long Fonts::set_font(const std::string &name, Font * const font) {
    {
      stdext::hash_map<std::string, unsigned long>::iterator it = m_font_lookup.find(name);
      if(it != m_font_lookup.end()) {
        stdext::hash_map<unsigned long, Font *>::iterator jt = m_fonts.find(it->second);
        delete jt->second;
        m_fonts.erase(jt);
        m_font_lookup.erase(it);
      }
    }

    unsigned long id = get_Resource().assign();
    m_font_lookup[name] = id;
    m_fonts[id] = font;
    return id;
  }

  void Fonts::clear_font(const std::string &name) {
    stdext::hash_map<string, unsigned long>::iterator it = m_font_lookup.find(name);

    if(it == m_font_lookup.end())
      throw Font_Not_Found(name);

    m_fonts.erase(it->second);
    m_font_lookup.erase(it);
  }

  void Fonts::reload(const string &filename) {
    if(filename.length()) {
      m_filename = filename;
    }
    
    lose_resources();
    init();
  }

  void Fonts::init() {
    TTF_Init();

    Video &vr = get_Video();

    XML_Reader fonts_xml(m_filename.c_str());
    XML_Element fonts = fonts_xml["Fonts"];
    string name;
    bool error = false;

    try {
      for(XML_Element it = fonts.first();; it = it.next()) {
        name = it.value();
        error = true;
        const string filepath = it["filepath"].to_string();
        const int height = it["height"].to_int();
        const bool bold = it["bold"].to_bool();
        const bool italics = it["italics"].to_bool();
        error = false;

        try {
          Font *font = vr.create_Font(filepath, bold, italics, height);

          unsigned long id = get_Resource().assign();
          m_font_lookup[name] = id;
          m_fonts[id] = font;
        }
        catch(Font_Init_Failure &) {
          uninit();
          cerr << "Fonts: Error Loading '" << name << "' from '" << filepath << "'\n";
          throw;
        }
      }
    }
    catch(Bad_XML_Access &)
    {
      if(error) {
        cerr << "Error loading Font '" << name << "'\n";
        throw;
      }
    }
  }

  void Fonts::uninit() {
    for(stdext::hash_map<unsigned long, Font *>::iterator it = m_fonts.begin(); it != m_fonts.end(); ++it)
      delete it->second;
    m_fonts.clear();
    m_font_lookup.clear();
    TTF_Quit();
  }

  void Fonts::lose_resources() {
    uninit();
  }

}
