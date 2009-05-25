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

#include <Zeni/Database.hxx>
#include <Zeni/Video.hxx>

#include <iostream>
#include <fstream>

using namespace std;

namespace Zeni {

  Textures::Textures()
    : Database<Texture>("config/textures.xml", "Textures")
  {
    init();
  }

  Textures::~Textures() {
    uninit();
  }

  Textures & get_Textures() {
    static Textures e_texturedb;
    return e_texturedb;
  }

  void Textures::apply_texture(const string &name) {
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

      tr.reload();
    }
    else {
      m_anisotropic_filtering = af;
      m_bilinear_filtering = bilinear_filtering_;
      m_mipmapping = mipmapping_;
    }
  }

  void Textures::post_init() {
    m_loaded = true;
  }

  void Textures::post_uninit() {
    m_loaded = false;
  }

  void Textures::post_lose() {
    m_loaded = false;
  }

  Texture * Textures::load(XML_Element &xml_element) {
    bool is_sprite = false;
    try {
      is_sprite = xml_element["is_sprite"].to_bool();
    }
    catch(Bad_XML_Access &)
    {
      /// not a Sprite
    }

    if(!is_sprite) {
      const string filepath = xml_element["filepath"].to_string();
      const bool tile = xml_element["tile"].to_bool();

      return get_Video().load_Texture(filepath, tile, m_lazy_loading);
    }
    else {
      Sprite * s = new Sprite();

      for(XML_Element texture = xml_element.first(); texture.good(); texture = texture.next())
        if(texture.value() != "is_sprite")
          s->append_frame(texture.to_string());

      return s;
    }
  }

  bool Textures::keep(const Texture &type) {
    return dynamic_cast<const Sprite *>(&type) != 0;
  }

  bool Textures::m_loaded = false;
  bool Textures::m_bilinear_filtering = true;
  bool Textures::m_mipmapping = true;
  int Textures::m_anisotropic_filtering = 0;
  bool Textures::m_lazy_loading = false;

}
