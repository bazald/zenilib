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
 * \class Zeni::Font
 *
 * \ingroup zenilib
 *
 * \brief Font Abstraction
 *
 * The Font class allows one to render text using a TrueType font in both 
 * Direct3D and in OpenGL.  It is done rather differently in both cases, however.
 * In Direct3D, it is done using built in DirectX functions and rendered with 
 * screen coordinates.  In OpenGL, it is done using SDL_ttf and textures.  The 
 * result appears much the same.
 *
 * \note TrueType fonts must be installed to work correctly in DirectX.
 *
 * \note Created with a call to get_Video().create_Font(...)
 *
 * \warning Always instantiate a new Font with a call to get_Video().create_Font().  Do not directly call the class constrcutors.
 *
 * \warning Text will look faded if you render to a clipped portion of the screen in OpenGL (e.g. split-screen multiplayer).  The workaround is to choose a smaller font size and to transform the position of the text manually.  Not to do so is essentially a coding error.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_FONT_H
#define ZENI_FONT_H

#include <Zeni/Color.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Core.h>
#include <Zeni/String.h>

#ifdef _MACOSX
#include <SDL_ttf/SDL_ttf.h>
#else
#include <SDL/SDL_ttf.h>
#endif

#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif

#include <Zeni/Define.h>

namespace Zeni {

  struct Color;
  class ZENI_GRAPHICS_DLL Video;
  class ZENI_GRAPHICS_DLL Video_GL;
  class ZENI_GRAPHICS_DLL Video_DX9;
  class ZENI_GRAPHICS_DLL Texture;

  enum JUSTIFY {ZENI_LEFT = 0, ZENI_CENTER = 1, ZENI_RIGHT = 2};

  class ZENI_GRAPHICS_DLL Font {
  public:
    Font(); ///< Instantiate a new Font with a call to get_Video().create_Font()
    Font(const bool &bold, const bool &italic,
                          const float &glyph_height,
                          const float &virtual_screen_height,
                          const String &font_name = "Untitled Font"); ///< Instantiate a new Font with a call to get_Video().create_Font()
    virtual ~Font() {}

    inline bool is_bold() const; ///< Check if a font has been artifically bolded (a bad thing).  You want to use bold versions of TrueType fonts whenever possible rather than bolding a regular TrueType font.
    inline bool is_italic() const; ///< Check if a font has been italicized.
    inline const String & get_font_name() const; ///< Get the name of the font.
    inline float get_text_height() const; ///< Get the height of the font.  The width is usually half the height, by default.
    virtual float get_text_width(const String &text) const = 0; ///< Get the width of text rendering using this font.  Approximately text_height * text.length() / 2.0f
    inline float get_virtual_screen_height() const; ///< Get the intended virtual screen height for the rendering of this Font

    /// Render text at screen position (x, y), with justification JUSTIFY.
    virtual void render_text(const String &text, const Point2f &position,
      const Color &color, const JUSTIFY &justify = ZENI_DEFAULT_JUSTIFY) const = 0;
    /// Render text with greater control over position and orientation, with justification JUSTIFY.
    virtual void render_text(const String &text, const Point3f &position, const Vector3f &right, const Vector3f &down,
      const Color &color, const JUSTIFY &justify = ZENI_DEFAULT_JUSTIFY) const = 0;

  private:
    bool m_bold, m_italic;
    float m_glyph_height;
    float m_virtual_screen_height;
    String m_font_name;
  };

  class ZENI_GRAPHICS_DLL Font_FT : public Font {
    static const int num_glyphs = 256;

    struct ZENI_GRAPHICS_DLL Glyph {
      Glyph();
      Glyph(TTF_Font *font_, const unsigned char &c,
                             SDL_Surface *source,
                             SDL_Surface *render_target, const SDL_Rect &dstrect,
                             const int &total_width, const int &total_height,
                             const float &vratio);

      inline float get_glyph_width() const;

      inline void render(Video &vr, const Point2f &position, const float &vratio) const;
      inline void render(Video &vr, const Point3f &position, const Vector3f &right, const Vector3f &down) const;

    private:
      float m_glyph_width;
      Point2f m_upper_left_point, m_lower_right_point;
      Point2f m_upper_left_texel, m_lower_right_texel;
    };

  public:
    Font_FT(); ///< Instantiate a new Font with a call to get_Video().create_Font()
    Font_FT(const String &filepath,
                             const bool &bold, const bool &italic,
                             const float &glyph_height,
                             const float &virtual_screen_height); ///< Instantiate a new Font with a call to get_Video().create_Font()
    ~Font_FT();

    virtual float get_text_width(const String &text) const; ///< Get the width of text rendering using this font.  Approximately text_height * text.length() / 2.0f

    virtual void render_text(const String &text, const Point2f &position,
      const Color &color, const JUSTIFY &justify = ZENI_LEFT) const;
    virtual void render_text(const String &text, const Point3f &position, const Vector3f &right, const Vector3f &down,
      const Color &color, const JUSTIFY &justify = ZENI_DEFAULT_JUSTIFY) const;

  private:
    void init(const String &filepath);

    Glyph *m_glyph[num_glyphs];
    Texture *m_texture;
    float m_font_height;
    float m_vratio;
  };

  struct ZENI_GRAPHICS_DLL Font_Type_Unsupported : Error {
    Font_Type_Unsupported(const String &filename) : Error("Unsupported Font Type: ") 
    {msg += filename;}
  };

  struct ZENI_GRAPHICS_DLL Font_Init_Failure : Error {
    Font_Init_Failure() : Error("Zeni Font Initialization Failure!") {}
  };

}

#include <Zeni/Undefine.h>

#endif
