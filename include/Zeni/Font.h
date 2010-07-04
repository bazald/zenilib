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

#ifdef _MACOSX
#include <SDL_ttf/SDL_ttf.h>
#else
#include <SDL/SDL_ttf.h>
#endif

#include <string>

#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif

#include <Zeni/Global.h>

namespace Zeni {

  struct Color;
  class Video;
  class Video_GL;
  class Video_DX9;
  class Texture;

  enum JUSTIFY {ZENI_LEFT = 0, ZENI_CENTER = 1, ZENI_RIGHT = 2};

  class Font {
  public:
    Font(); ///< Instantiate a new Font with a call to get_Video().create_Font()
    Font(const bool &bold, const bool &italic,
         const float &glyph_height,
         const float &virtual_screen_height,
         const std::string &font_name = "Untitled Font"); ///< Instantiate a new Font with a call to get_Video().create_Font()
    virtual ~Font() {}

    inline bool is_bold() const; ///< Check if a font has been artifically bolded (a bad thing).  You want to use bold versions of TrueType fonts whenever possible rather than bolding a regular TrueType font.
    inline bool is_italic() const; ///< Check if a font has been italicized.
    inline const std::string & get_font_name() const; ///< Get the name of the font.
    inline float get_text_height() const; ///< Get the height of the font.  The width is usually half the height, by default.
    virtual float get_text_width(const std::string &text) const = 0; ///< Get the width of text rendering using this font.  Approximately text_height * text.length() / 2.0f
    inline float get_virtual_screen_height() const; ///< Get the intended virtual screen height for the rendering of this Font

    /// Render text at screen position (x, y), with justification JUSTIFY.
    virtual void render_text(const std::string &text, const Point2f &position,
      const Color &color, const JUSTIFY &justify = ZENI_DEFAULT_JUSTIFY) const = 0;
    /// Render text with greater control over position and orientation, with justification JUSTIFY.
    virtual void render_text(const std::string &text, const Point3f &position, const Vector3f &right, const Vector3f &down,
      const Color &color, const JUSTIFY &justify = ZENI_DEFAULT_JUSTIFY) const = 0;

  private:
    bool m_bold, m_italic;
    float m_glyph_height;
    float m_virtual_screen_height;
    std::string m_font_name;
  };

  class Font_FT : public Font {
    static const int num_glyphs = 256;

    struct Glyph {
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
    Font_FT(const std::string &filepath,
            const bool &bold, const bool &italic,
            const float &glyph_height,
            const float &virtual_screen_height); ///< Instantiate a new Font with a call to get_Video().create_Font()
    ~Font_FT();

    virtual float get_text_width(const std::string &text) const; ///< Get the width of text rendering using this font.  Approximately text_height * text.length() / 2.0f

    virtual void render_text(const std::string &text, const Point2f &position,
      const Color &color, const JUSTIFY &justify = ZENI_LEFT) const;
    virtual void render_text(const std::string &text, const Point3f &position, const Vector3f &right, const Vector3f &down,
      const Color &color, const JUSTIFY &justify = ZENI_DEFAULT_JUSTIFY) const;

  private:
    void init(const std::string &filepath);

    Glyph *m_glyph[num_glyphs];
    Texture *m_texture;
    float m_font_height;
    float m_vratio;
  };

  struct Font_Type_Unsupported : Error {
    Font_Type_Unsupported(const std::string &filename) : Error("Unsupported Font Type: ") 
    {msg += filename;}
  };

  struct Font_Init_Failure : Error {
    Font_Init_Failure() : Error("Zeni Font Initialization Failure!") {}
  };

}

#include <Zeni/Global_Undef.h>

#endif
