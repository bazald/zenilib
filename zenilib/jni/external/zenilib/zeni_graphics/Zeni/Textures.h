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
  class Textures;

#ifdef _WINDOWS
  ZENI_GRAPHICS_EXT template class ZENI_GRAPHICS_DLL Singleton<Textures>;
  ZENI_GRAPHICS_EXT template class ZENI_GRAPHICS_DLL Database<Texture>;
#endif

  class ZENI_GRAPHICS_DLL Textures : public Singleton<Textures>, public Database<Texture> {
    friend class Singleton<Textures>;

    static Textures * create();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    static class Lose : public Event::Handler {
      void operator()() {
        get().lose_resources();
      }

      Lose * duplicate() const {
        return new Lose;
      }

    public:
      Lose() {}

      // Undefined
      Lose(const Lose &);
      Lose operator=(const Lose &);
    } g_lose;

    static class Unlose : public Event::Handler {
      void operator()() {
        get().unlose_resources();
      }

      Unlose * duplicate() const {
        return new Unlose;
      }

    public:
      Unlose() {}

      // Undefined
      Unlose(const Unlose &);
      Unlose operator=(const Unlose &);
    } g_unlose;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

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
    void apply_Texture(const String &name); ///< Apply a texture for upcoming polygons (Called by Video::apply_Texture)
    void apply_Texture(const unsigned long &id); ///< Apply a texture for upcoming polygons

    // Sprite-specific
    bool is_Sprite(const unsigned long &id); ///< Does this id point to a Sprite
    int get_num_frames(const unsigned long &id); ///< Get the number of frames; returns 0 if it is not a Sprite
    int get_current_frame(const unsigned long &id); ///< Get the currently selected frame number for a Sprite
    void set_current_frame(const unsigned long &id, const int &frame_number); ///< Set the frame number for a Sprite

  private:
    virtual void on_load();
    virtual void on_clear();
    virtual void on_lose();

    virtual Texture * load(XML_Element_c &xml_element, const String &name, const String &filename);

    static bool m_loaded;
    static bool m_bilinear_filtering;
    static bool m_mipmapping;
    static int m_anisotropic_filtering;
    static bool m_lazy_loading;
  };

  ZENI_GRAPHICS_DLL Textures & get_Textures(); ///< Get access to the singleton.

  struct ZENI_GRAPHICS_DLL Sprite_Function_Misapplied : public Error {
    Sprite_Function_Misapplied() : Error("Sprite Function Misapplied") {}
  };

}

#endif
