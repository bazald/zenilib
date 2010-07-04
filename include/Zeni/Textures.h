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

/**
 * \class Zeni::Textures
 *
 * \ingroup zenilib
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

#include <Zeni/Core.h>
#include <Zeni/Database.h>

#ifdef _WINDOWS
#include <Windows.h>
#endif

namespace Zeni {

  class Texture;

  class Textures : public Database<Texture> {
    // Get reference to only instance;
    friend Textures & get_Textures(); ///< Get access to the singleton.

    Textures();
    ~Textures();

    // Undefined
    Textures(const Textures &);
    Textures & operator=(const Textures &);

  public:
    // Accessors
    inline static bool get_bilinear_filtering(); ///< Check if bilinear filtering is in use
    inline static bool get_mipmapping(); ///< Check if mipmapping is in use
    inline static bool get_trilinear_filtering(); ///< Check if trilinear filtering (the combination of bilinear filtering and mipmapping) is in use
    inline static int get_anisotropic_filtering(); ///< Check the current level of anisotropy
    inline static bool get_lazy_loading(); /// Check to see if Textures is set to use lazy loading if possible

    // Loading Options
    static void set_texturing_mode(const int &anisotropic_filtering_,
                                   const bool &bilinear_filtering_,
                                   const bool &mipmapping_); ///< Set the texturing mode
    inline static void set_lazy_loading(const bool &lazy_loading = true); ///< Set whether Textures should use lazy loading if possible, or if it should always load Textures immediately.

    // Appliers
    void apply_texture(const std::string &name); ///< Apply a texture for upcoming polygons (Called by Video::apply_texture)
    void apply_texture(const unsigned long &id); ///< Apply a texture for upcoming polygons

    // Sprite-specific
    bool is_sprite(const unsigned long &id); ///< Does this id point to a Sprite
    int get_num_frames(const unsigned long &id); ///< Get the number of frames; returns 0 if it is not a Sprite
    int get_current_frame(const unsigned long &id); ///< Get the currently selected frame number for a Sprite
    void set_current_frame(const unsigned long &id, const int &frame_number); ///< Set the frame number for a Sprite

  private:
    virtual void on_load();
    virtual void on_clear();
    virtual void on_lose();

    virtual Texture * load(XML_Element_c &xml_element, const std::string &name, const std::string &filename);

    static bool m_loaded, m_bilinear_filtering, m_mipmapping;
    static int m_anisotropic_filtering;
    static bool m_lazy_loading;

#if defined(_WINDOWS)
    HMODULE g_libpng_dll;
#elif defined(_LINUX)
    void * g_libpng_so;
#endif
  };

  Textures & get_Textures(); ///< Get access to the singleton.

  struct Sprite_Function_Misapplied : public Error {
    Sprite_Function_Misapplied() : Error("Sprite Function Misapplied") {}
  };

}

#endif
