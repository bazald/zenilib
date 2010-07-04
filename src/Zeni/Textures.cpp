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

#include <Zeni/Textures.h>

#include <Zeni/Database.hxx>
#include <Zeni/Video.hxx>

#include <iostream>
#include <fstream>

#if defined(_LINUX)
#include <dlfcn.h>
#endif

namespace Zeni {

  Textures::Textures()
    : Database<Texture>("config/textures.xml", "Textures")
#if defined(_WINDOWS)
    , g_libpng_dll(LoadLibrary("libpng13.dll"))
#elif defined(_LINUX)
    , g_libpng_so(dlopen("libpng.so", RTLD_LAZY | RTLD_GLOBAL))
#endif
  {
    init();
  }

  Textures::~Textures() {
    uninit();

#if defined(_WINDOWS)
    FreeLibrary(g_libpng_dll);
#elif defined(_LINUX)
    dlclose(g_libpng_so);
#endif
  }

  Textures & get_Textures() {
    static Textures e_texturedb;
    return e_texturedb;
  }

  void Textures::apply_texture(const std::string &name) {
    (*this)[name].apply_texture();
  }

  void Textures::apply_texture(const unsigned long &id) {
    (*this)[id].apply_texture();
  }

  bool Textures::is_sprite(const unsigned long &id) {
    return dynamic_cast<const Sprite *>(&(*this)[id]) != 0;
  }
  
  int Textures::get_num_frames(const unsigned long &id) {
    const Sprite *sprite = dynamic_cast<Sprite *>(&(*this)[id]);
    if(!sprite)
      throw Sprite_Function_Misapplied();

    return sprite->get_num_frames();
  }

  int Textures::get_current_frame(const unsigned long &id) {
    const Sprite *sprite = dynamic_cast<Sprite *>(&(*this)[id]);
    if(!sprite)
      throw Sprite_Function_Misapplied();

    return sprite->get_current_frame();
  }

  void Textures::set_current_frame(const unsigned long &id, const int &frame_number) {
    Sprite *sprite = dynamic_cast<Sprite *>(&(*this)[id]);
    if(!sprite)
      throw Sprite_Function_Misapplied();

    return sprite->set_current_frame(frame_number);
  }

  void Textures::set_texturing_mode(const int &anisotropic_filtering_, const bool &bilinear_filtering_, const bool &mipmapping_) {
    const int af = anisotropic_filtering_ == -1 ? get_Video().get_maximum_anisotropy() : anisotropic_filtering_;
    
    if(m_loaded && ((af != m_anisotropic_filtering) | (bilinear_filtering_ != m_bilinear_filtering) | (mipmapping_ != m_mipmapping))) {
      Textures &tr = get_Textures();

      m_anisotropic_filtering = af;
      m_bilinear_filtering = bilinear_filtering_;
      m_mipmapping = mipmapping_;

      if(!tr.lost_resources())
        tr.reload();
    }
    else {
      m_anisotropic_filtering = af;
      m_bilinear_filtering = bilinear_filtering_;
      m_mipmapping = mipmapping_;
    }
  }

  void Textures::on_load() {
    m_loaded = true;
  }

  void Textures::on_clear() {
    m_loaded = false;
  }

  void Textures::on_lose() {
    m_loaded = false;
  }

  Texture * Textures::load(XML_Element_c &xml_element, const std::string &name, const std::string &filename) {
    const XML_Element_c is_sprite_e = xml_element["is_sprite"];
    const bool is_sprite = is_sprite_e.good() && is_sprite_e.to_bool();

    if(!is_sprite) {
      const std::string filepath = xml_element["filepath"].to_string();
      const bool tile = xml_element["tile"].to_bool();

      return get_Video().load_Texture(filepath, tile, m_lazy_loading);
    }
    else {
      Sprite * s = new Sprite();

      size_t frame_number = 0;
      for(XML_Element_c texture = xml_element.first(); texture.good(); texture = texture.next(), ++frame_number)
        try {
          if(texture.value() == "token") {
            const std::string identifier = texture.to_string();
            s->append_frame(identifier, get_id(identifier));
          }
          else if(texture.value() == "file") {
            const std::string filepath = texture["filepath"].to_string();
            const bool tile = texture["tile"].to_bool();
            const std::string frame_name = name + '/' + ulltoa(frame_number);

            Texture * const texture = get_Video().load_Texture(filepath, tile, m_lazy_loading);

            const unsigned int id = give(frame_name, texture, false, filename);

            s->append_frame(frame_name, id);
          }
          else if(texture.value() == "is_sprite")
            --frame_number;
          else
            throw Database_Load_Entry_Failed(name);
        }
        catch(...) {
          delete s;
          throw;
        }

      return s;
    }
  }

  bool Textures::m_loaded = false;
  bool Textures::m_bilinear_filtering = true;
  bool Textures::m_mipmapping = true;
  int Textures::m_anisotropic_filtering = 0;
  bool Textures::m_lazy_loading = false;

}
