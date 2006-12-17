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

#include <Zeni/Textures.h>
#include <Zeni/Video.h>
#include <fstream>

using std::string;
using std::set;
using std::ifstream;

namespace Zeni {

  Textures::Textures()
    : m_texturedb("config/textures.txt")
  {
    init();
  }

  Textures::~Textures() {
    for(std::set<Texture *, Texture_Cmp>::iterator it = m_textures.begin(); it != m_textures.end(); ++it)
      delete *it;

    m_loaded = false;
  }

  Textures & Textures::get_reference() {
    static Textures e_texturedb;
    return e_texturedb;
  }

  void Textures::apply_texture(const string &name) {
    if(!m_loaded)
      reload();

    Texture temp(name);
    std::set<Texture *, Texture_Cmp>::const_iterator it = m_textures.find(&temp);

    if(it == m_textures.end())
      throw Texture_Not_Found();

    (*it)->apply_texture();
  }

  void Textures::unapply_texture(const string &name) {
    Texture temp(name);
    std::set<Texture *, Texture_Cmp>::const_iterator it = m_textures.find(&temp);

    if(it == m_textures.end())
      throw Texture_Not_Found();

    (*it)->unapply_texture();
  }

  void Textures::set_texturing_mode(const int &anisotropic_filtering_, const bool &bilinear_filtering_, const bool &mipmapping_) {
    int af = anisotropic_filtering_;
    if(af == -1)
      af = Video::get_reference().get_maximum_anisotropy();

    bool reload_ = ((af != m_anisotropic_filtering) | (bilinear_filtering_ ^ m_bilinear_filtering) | (mipmapping_ ^ m_mipmapping)) != 0;

    m_anisotropic_filtering = af;
    m_bilinear_filtering = bilinear_filtering_;
    m_mipmapping = mipmapping_;

    if(m_loaded && reload_)
      get_reference().reload();
  }

  void Textures::reload(const string &texturedb) {
    if(texturedb.length())
      m_texturedb = texturedb;

    init();
  }

  void Textures::reload() {
    init();
  }

  void Textures::init() {
    m_textures.clear();

    ifstream tdbin(m_texturedb.c_str());

    if(!tdbin)
      throw Texture_Init_Failure();

    string fileName, name;
    while(tdbin >> name >> fileName)
      m_textures.insert(Video::get_reference().load_Texture(name, fileName));

    m_loaded = true;
  }

  bool Textures::m_loaded = false;
  bool Textures::m_bilinear_filtering = true;
  bool Textures::m_mipmapping = true;
  int Textures::m_anisotropic_filtering = 0;

}
