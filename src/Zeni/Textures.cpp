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

#include <Zeni/Textures.h>
#include <Zeni/Resource.hxx>
#include <Zeni/Video.h>

#include <iostream>
#include <fstream>

using namespace std;

namespace Zeni {

  Textures::Textures()
    : m_texturedb("config/textures.txt")
  {
    init();
  }

  Textures::~Textures() {
    uninit();
  }

  Textures & Textures::get_reference() {
    static Textures e_texturedb;
    return e_texturedb;
  }
  
  unsigned long Textures::set_texture(const std::string &name, Texture * const texture) {
    {
      stdext::hash_map<std::string, unsigned long>::iterator it = m_texture_lookup.find(name);
      if(it != m_texture_lookup.end()) {
        stdext::hash_map<unsigned long, Texture *>::iterator jt = m_textures.find(it->second);
        delete jt->second;
        m_textures.erase(jt);
        m_texture_lookup.erase(it);
      }
    }

    if(!texture)
      throw Null_Texture_Set();

    unsigned long id = Resource::get_reference().assign();
    m_texture_lookup[name] = id;
    m_textures[id] = texture;
    return id;
  }

  void Textures::clear_texture(const std::string &name) {
    stdext::hash_map<string, unsigned long>::iterator it = m_texture_lookup.find(name);

    if(it == m_texture_lookup.end())
      throw Texture_Not_Found(name);

    m_textures.erase(it->second);
    m_texture_lookup.erase(it);
  }

  unsigned long Textures::get_texture_id(const string &texture) const {
    stdext::hash_map<string, unsigned long>::const_iterator it = m_texture_lookup.find(texture);

    if(it == m_texture_lookup.end() || !it->second)
      throw Texture_Not_Found(texture);

    return it->second;
  }

  Texture * Textures::get_texture(const unsigned long &id) const {
    stdext::hash_map<unsigned long, Texture *>::const_iterator it = m_textures.find(id);

    if(it == m_textures.end() || !it->second) {
      char buf[64];
#ifdef _WINDOWS
      sprintf_s
#else
      sprintf
#endif
        (buf, "ID = %u", static_cast<unsigned int>(id));
      throw Texture_Not_Found(buf);
    }

    return it->second;
  }

  void Textures::apply_texture(const string &name) {
    stdext::hash_map<string, unsigned long>::const_iterator it = m_texture_lookup.find(name);

    try {
      apply_texture(it->second);
    }
    catch(Texture_Not_Found &) {
      std::cerr << "Missing Texture: " << name << std::endl;
      throw;
    }
  }

  void Textures::apply_texture(const unsigned long &id) {
    stdext::hash_map<unsigned long, Texture *>::const_iterator it = m_textures.find(id);

    if(it == m_textures.end()) {
      char buf[64];
#ifdef _WINDOWS
      sprintf_s
#else
      sprintf
#endif
        (buf, "ID = %u", static_cast<unsigned int>(id));
      throw Texture_Not_Found(buf);
    }

    it->second->apply_texture();
  }

  bool Textures::is_sprite(const unsigned long &id) {
    stdext::hash_map<unsigned long, Texture *>::const_iterator it = m_textures.find(id);
    
    if(it == m_textures.end()) {
      char buf[64];
#ifdef _WINDOWS
      sprintf_s
#else
      sprintf
#endif
        (buf, "ID = %u", static_cast<unsigned int>(id));
      throw Texture_Not_Found(buf);
    }

    return dynamic_cast<Sprite *>(it->second) != 0;
  }
  
  int Textures::get_num_frames(const unsigned long &id) {
    stdext::hash_map<unsigned long, Texture *>::const_iterator it = m_textures.find(id);
    
    if(it == m_textures.end()) {
      char buf[64];
#ifdef _WINDOWS
      sprintf_s
#else
      sprintf
#endif
        (buf, "ID = %u", static_cast<unsigned int>(id));
      throw Texture_Not_Found(buf);
    }

    Sprite *sprite = dynamic_cast<Sprite *>(it->second);
    if(!sprite)
      return 0;

    return sprite->get_num_frames();
  }

  int Textures::get_current_frame(const unsigned long &id) {
    stdext::hash_map<unsigned long, Texture *>::const_iterator it = m_textures.find(id);
    
    if(it == m_textures.end()) {
      char buf[64];
#ifdef _WINDOWS
      sprintf_s
#else
      sprintf
#endif
        (buf, "ID = %u", static_cast<unsigned int>(id));
      throw Texture_Not_Found(buf);
    }

    Sprite *sprite = dynamic_cast<Sprite *>(it->second);
    if(!sprite)
      throw Sprite_Function_Misapplied();

    return sprite->get_current_frame();
  }

  void Textures::set_current_frame(const unsigned long &id, const int &frame_number) {
    stdext::hash_map<unsigned long, Texture *>::const_iterator it = m_textures.find(id);
    
    if(it == m_textures.end()) {
      char buf[64];
#ifdef _WINDOWS
      sprintf_s
#else
      sprintf
#endif
        (buf, "ID = %u", static_cast<unsigned int>(id));
      throw Texture_Not_Found(buf);
    }

    Sprite *sprite = dynamic_cast<Sprite *>(it->second);
    if(!sprite)
      throw Sprite_Function_Misapplied();

    return sprite->set_current_frame(frame_number);
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
    m_texture_lookup.clear();
    m_textures.clear();

    ifstream tdbin(m_texturedb.c_str());

    if(!tdbin)
      throw Texture_Init_Failure();

    string fileName, name;
    bool repeat;
    while(tdbin >> name >> fileName >> repeat) {
      try {
        set_texture(name, Video::get_reference().load_Texture(fileName, repeat));
      }
      catch(Texture_Init_Failure &) {
        cerr << "Textures: Error Loading '" << name << "' from '" << fileName << "'\n";
        throw;
      }
    }

    m_loaded = true;
  }

  void Textures::uninit() {
    for(stdext::hash_map<unsigned long, Texture *>::iterator it = m_textures.begin(); it != m_textures.end(); ++it)
      delete it->second;
    m_textures.clear();
    m_texture_lookup.clear();
    m_loaded = false;
  }

  void Textures::lose_resources() {
    uninit();
  }

  bool Textures::m_loaded = false;
  bool Textures::m_bilinear_filtering = true;
  bool Textures::m_mipmapping = true;
  int Textures::m_anisotropic_filtering = 0;

}
