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

/**
 * \class Zeni::Textures
 *
 * \ingroup Zenilib
 *
 * \brief A Texture Database Singleton
 *
 * \note Textures will be reloaded automatically if settings are changed with a call to set_texturing_mode.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_TEXTURES_H
#define ZENI_TEXTURES_H

#include <Zeni/Hash_Map.h>
#include <Zeni/Resource.h>
#include <Zeni/Texture.h>

namespace Zeni {

  class Texture;

  class Textures {
    Textures();
    ~Textures();

    // Undefined
    Textures(const Textures &);
    Textures & operator=(const Textures &);

  public:
    // Get reference to only instance;
    static Textures & get_reference(); ///< Get access to the singleton.

    // Accessors
    inline static bool get_bilinear_filtering(); ///< Check if bilinear filtering is in use
    inline static bool get_mipmapping(); ///< Check if mipmapping is in use
    inline static bool get_trilinear_filtering(); ///< Check if trilinear filtering (the combination of bilinear filtering and mipmapping) is in use
    inline static int get_anisotropic_filtering(); ///< Check the current level of anisotropy

    // Modifiers
    static void set_texturing_mode(const int &anisotropic_filtering_,
      const bool &bilinear_filtering_, const bool &mipmapping_); ///< Set the texturing mode
    unsigned long set_texture(const std::string &name, Texture * const); ///< Load a texture
    void clear_texture(const std::string &name); ///< Clear a texture by name.
    unsigned long get_texture_id(const std::string &name) const; ///< Get a texture id by name.
    Texture * get_texture(const unsigned long &id) const; ///< Get a texture by id.
    Texture * get_texture(const std::string &name) const; ///< Get a texture by name.
    void apply_texture(const std::string &name); ///< Apply a texture for upcoming polygons
    void apply_texture(const unsigned long &id); ///< Apply a texture for upcoming polygons

    // Sprite-specific
    bool is_sprite(const unsigned long &id); ///< Does this id point to a Sprite
    int get_num_frames(const unsigned long &id); ///< Get the number of frames; returns 0 if it is not a Sprite
    int get_current_frame(const unsigned long &id); ///< Get the currently selected frame number for a Sprite
    void set_current_frame(const unsigned long &id, const int &frame_number); ///< Set the frame number for a Sprite

    // Initialization Functions
    void reload(const std::string &tdb); ///< (Re)Load a texture database
    void reload(); ///< Reload a texture database

    void lose_resources(); ///< Wipe all resources and prepare to reload them when they are next needed

  private:
    void init();
    void uninit();

    std::string m_texturedb;
    
    stdext::hash_map<std::string, unsigned long> m_texture_lookup;
    stdext::hash_map<unsigned long, Texture *> m_textures;

    static bool m_loaded, m_bilinear_filtering, m_mipmapping;
    static int m_anisotropic_filtering;
  };

  struct Texture_Not_Found : public Error {
    Texture_Not_Found(const std::string &identifier) : Error("Zeni Texture '" + identifier + "' Not Found") {}
  };

  struct Null_Texture_Set : public Error {
    Null_Texture_Set() : Error("Null Texture Added to Textures Database") {}
  };

  struct Sprite_Function_Misapplied : public Error {
    Sprite_Function_Misapplied() : Error("Sprite Function Misapplied") {}
  };

}

#endif

#ifdef ZENI_INLINES
#include <Zeni/Textures.hxx>
#endif
