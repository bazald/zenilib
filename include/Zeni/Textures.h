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

#include "Hash_Map.h"
#include "Texture.h"

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
    void apply_texture(const std::string &name); ///< Apply a texture for upcoming polygons

    // Initialization Functions
    void reload(const std::string &tdb); ///< (Re)Load a texture database
    void reload(); ///< Reload a texture database

  private:
    void init();

    std::string m_texturedb;
    
    stdext::hash_map<std::string, Texture *> m_textures;

    static bool m_loaded, m_bilinear_filtering, m_mipmapping;
    static int m_anisotropic_filtering;
  };

  struct Texture_Not_Found : public Error {
    Texture_Not_Found() : Error("Zeni Texture Not Found") {}
  };

}

#ifdef ZENI_INLINES
#include "Textures.hxx"
#endif

#endif
