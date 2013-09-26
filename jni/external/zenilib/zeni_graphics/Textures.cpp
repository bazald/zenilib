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

#if defined(_LINUX)
#include <dlfcn.h>
#endif

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Database.hxx>
#include <Zeni/Singleton.hxx>

namespace Zeni {

  template class Singleton<Textures>;
  template class Database<Texture>;

  Textures * Textures::create() {
    return new Textures;
  }

  Textures::Lose Textures::g_lose;
  Textures::Unlose Textures::g_unlose;

  Textures::Textures()
    : Database<Texture>("config/textures.xml", "Textures")
  {
    Video &vr = get_Video();

    init();

    vr.lend_pre_uninit(&g_lose);
    vr.lend_post_reinit(&g_unlose);
  }

  Textures::~Textures() {
    Video::remove_pre_uninit(&g_lose);

    Database<Texture>::uninit();
  }

  Textures & get_Textures() {
    return Singleton<Textures>::get();
  }

  void Textures::apply_Texture(const String &name) {
    (*this)[name].apply_Texture();
  }

  void Textures::apply_Texture(const unsigned long &id) {
    (*this)[id].apply_Texture();
  }

  bool Textures::is_Sprite(const unsigned long &id) {
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

  Texture * Textures::load(XML_Element_c &xml_element, const String &name, const String &filename) {
    const XML_Element_c is_sprite_e = xml_element["is_sprite"];
    const bool is_sprite = is_sprite_e.good() && is_sprite_e.to_bool();

    if(!is_sprite) {
      const String filepath = xml_element["filepath"].to_string();
      const bool tile = xml_element["tile"].to_bool();

      return get_Video().load_Texture(filepath, tile, m_lazy_loading);
    }
    else {
      Sprite * s = new Sprite();

      size_t frame_number = 0;
      for(XML_Element_c texture = xml_element.first(); texture.good(); texture = texture.next(), ++frame_number)
        try {
          if(texture.value() == "token") {
            const String identifier = texture.to_string();
            s->append_frame(identifier, get_id(identifier));
          }
          else if(texture.value() == "file") {
            const String filepath = texture["filepath"].to_string();
            const bool tile = texture["tile"].to_bool();
            const String frame_name = name + '/' + ulltoa(frame_number);

            Texture * const texture = get_Video().load_Texture(filepath, tile, m_lazy_loading);

            const unsigned long id = give(frame_name, texture, false, filename);

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
