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

#include <Zeni/Font.hxx>


#include <Zeni/Coordinate.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Quadrilateral.hxx>
#include <Zeni/Texture.h>
#include <Zeni/Vertex2f.hxx>
#include <Zeni/Video.hxx>

#include <Zeni/Global.h>

namespace Zeni {

  Font::Font ()
    : m_bold(0),
    m_italic(0),
    m_glyph_height(0),
    m_virtual_screen_height(0.0f)
  {
  }

  Font::Font(const bool &bold, const bool &italic,
             const float &glyph_height,
             const float &virtual_screen_height,
             const std::string &font_name)
    : m_bold(bold),
    m_italic(italic),
    m_glyph_height(glyph_height),
    m_virtual_screen_height(virtual_screen_height),
    m_font_name(font_name)
  {
  }

  Font_FT::Glyph::Glyph()
    : m_glyph_width(0.0f)
  {
  }

  Font_FT::Glyph::Glyph(TTF_Font *font_, const unsigned char &c,
                        SDL_Surface *source,
                        SDL_Surface *render_target, const SDL_Rect &dstrect,
                        const int &total_width, const int &total_height,
                        const float &vratio)
    : m_glyph_width(0)
  {
    int minx, maxy, glyph_width;
    TTF_GlyphMetrics(font_, c, &minx, 0, 0, &maxy, &glyph_width);

    m_glyph_width = glyph_width / vratio;
    m_upper_left_point.x = float(minx) / vratio;
    m_upper_left_point.y = float(TTF_FontAscent(font_) - maxy) / vratio;
    m_lower_right_point.x = m_upper_left_point.x + source->w / vratio;
    m_lower_right_point.y = m_upper_left_point.y + source->h / vratio;

    SDL_Rect dstrect2 = {dstrect.x, dstrect.y, Uint16(source->w), Uint16(source->h)};

    m_upper_left_texel.x = float(dstrect2.x) / float(total_width);
    m_upper_left_texel.y = float(dstrect2.y) / float(total_height);
    m_lower_right_texel.x = float(dstrect2.x + source->w) / float(total_width);
    m_lower_right_texel.y = float(dstrect2.y + source->h) / float(total_height);

    SDL_BlitSurface(source, 0, render_target, &dstrect2);
    SDL_FreeSurface(source);
  }

  void Font_FT::Glyph::render(Video &vr, const Point2f &position, const float &vratio) const {
    const float x = int(position.x * vratio + 0.5f) / vratio;
    const float y = int(position.y * vratio + 0.5f) / vratio;

    Quadrilateral<Vertex2f_Texture> rect(
      (Vertex2f_Texture(Point2f(m_upper_left_point.x + x, m_upper_left_point.y + y), m_upper_left_texel)) ,
      (Vertex2f_Texture(Point2f(m_upper_left_point.x + x, m_lower_right_point.y + y), Point2f(m_upper_left_texel.x, m_lower_right_texel.y))) ,
      (Vertex2f_Texture(Point2f(m_lower_right_point.x + x, m_lower_right_point.y + y), m_lower_right_texel)) ,
      (Vertex2f_Texture(Point2f(m_lower_right_point.x + x, m_upper_left_point.y + y), Point2f(m_lower_right_texel.x, m_upper_left_texel.y))) );

    vr.render(rect);
  }

  void Font_FT::Glyph::render(Video &vr, const Point3f &position, const Vector3f &right, const Vector3f &down) const {
    const Vector3f scaled_right = (m_lower_right_point.x - m_upper_left_point.x) * right;
    const Vector3f scaled_down = (m_lower_right_point.y - m_upper_left_point.y) * down;

    const Point3f &ul = position + m_upper_left_point.x * right + m_upper_left_point.y * down;
    const Point3f ll = ul + scaled_down;
    const Point3f lr = ll + scaled_right;
    const Point3f ur = ul + scaled_right;

    Quadrilateral<Vertex3f_Texture> rect(
      (Vertex3f_Texture(ul, m_upper_left_texel)) ,
      (Vertex3f_Texture(ll, Point2f(m_upper_left_texel.x, m_lower_right_texel.y))) ,
      (Vertex3f_Texture(lr, m_lower_right_texel)) ,
      (Vertex3f_Texture(ur, Point2f(m_lower_right_texel.x, m_upper_left_texel.y))) );

    vr.render(rect);
  }

  Font_FT::Font_FT()
    : m_font_height(0.0f),
    m_vratio(0.0f)
  {
    memset(m_glyph, 0, num_glyphs * sizeof(Glyph *));
  }

  Font_FT::Font_FT(const std::string &filepath,
                   const bool &bold, const bool &italic,
                   const float &glyph_height,
                   const float &virtual_screen_height)
    : Font(bold, italic, glyph_height,
           (virtual_screen_height < MINIMUM_VIRTUAL_SCREEN_HEIGHT ||
           virtual_screen_height > MAXIMUM_VIRTUAL_SCREEN_HEIGHT) ?
           float(get_Video().get_screen_height()) : virtual_screen_height,
           filepath),
    m_font_height(glyph_height),
    m_vratio(get_Video().get_screen_height() / get_virtual_screen_height())
  {
    init(filepath);
  }

  Font_FT::~Font_FT() {
    for(int i = 1; i < num_glyphs; ++i)
      delete m_glyph[i];
  }
  
  float Font_FT::get_text_width(const std::string &text) const {
    float max_width = 0.0f, width = 0.0f;
    unsigned int pos = 0;

    for(; pos < text.size(); ++pos)
      if(text[pos] != '\r' && text[pos] != '\n' &&
        m_glyph[int(text[pos])])
        width += m_glyph[int(text[pos])]->get_glyph_width();
      else {
        max_width = std::max(max_width, width);
        width = 0.0f;
      }

    return std::max(max_width, width);
  }

  void Font_FT::render_text(const std::string &text, const Point2f &position, const Color &color, const JUSTIFY &justify) const {
    Video &vr = get_Video();
    const float &x = position.x;
    const float &y = position.y;

    const Color previous_color = vr.get_color();

    vr.set_color(color);
    vr.apply_texture(*m_texture);

    float cx, x_diff, cy = y;
    size_t i = 0u;

NEXT_LINE:

    cx = x;
    x_diff = 0.0f;

    if(justify != ZENI_LEFT) {
      for(size_t j = i; j < text.size(); ++j) {
        if(text[j] == '\r' || text[j] == '\n' ||
          !m_glyph[int(text[j])])
          break;

        x_diff -= m_glyph[int(text[j])]->get_glyph_width();
      }

      if(justify == ZENI_CENTER)
        cx += x_diff / 2u;
      else if(justify == ZENI_RIGHT)
        cx += x_diff;
    }

    for(; i < text.size(); ++i) {
      if(text[i] == '\r' && i+1 < text.size() && text[i+1] == '\n')
        ++i;

      if(text[i] == '\r' || text[i] == '\n') {
        ++i;
        cy += m_font_height;
        goto NEXT_LINE;
      }
      else if(m_glyph[int(text[i])]) {
        m_glyph[int(text[i])]->render(vr, Point2f(cx, cy), m_vratio);
        cx += m_glyph[int(text[i])]->get_glyph_width();
      }
    }

    vr.unapply_texture();

    vr.set_color(previous_color);
  }

  void Font_FT::render_text(const std::string &text, const Point3f &position, const Vector3f &right, const Vector3f &down, const Color &color, const JUSTIFY &justify) const {
    Video &vr = get_Video();

    const Color previous_color = vr.get_color();

    vr.set_color(color);
    vr.apply_texture(*m_texture);

    Point3f pos, vertical_pos = position;
    float x_diff;
    size_t i = 0u;

NEXT_LINE_2:

    pos = vertical_pos;
    x_diff = 0.0f;

    if(justify != ZENI_LEFT) {
      for(size_t j = i; j < text.size(); ++j) {
        if(text[j] == '\r' || text[j] == '\n' ||
          !m_glyph[int(text[j])])
          break;

        x_diff -= m_glyph[int(text[j])]->get_glyph_width();
      }

      if(justify == ZENI_CENTER)
        pos += x_diff / 2.0f * right;
      else if(justify == ZENI_RIGHT)
        pos += x_diff * right;
    }

    for(; i < text.size(); ++i) {
      if(text[i] == '\r' && i+1 < text.size() && text[i+1] == '\n')
        ++i;

      if(text[i] == '\r' || text[i] == '\n') {
        ++i;
        vertical_pos += m_font_height * down;
        goto NEXT_LINE_2;
      }
      else if(m_glyph[int(text[i])]) {
        m_glyph[int(text[i])]->render(vr, pos, right, down);
        pos += m_glyph[int(text[i])]->get_glyph_width() * right;
      }
    }

    vr.unapply_texture();

    vr.set_color(previous_color);
  }

  void Font_FT::init(const std::string &filepath) {
    TTF_Font *font = TTF_OpenFont(filepath.c_str(), int(get_text_height() * m_vratio + 0.5f));
    if(!font)
      throw Font_Init_Failure();

    /*** Set Style ***/

    if(is_bold() && is_italic())
      TTF_SetFontStyle(font, TTF_STYLE_BOLD | TTF_STYLE_ITALIC);
    else if(is_bold())
      TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    else if(is_italic())
      TTF_SetFontStyle(font, TTF_STYLE_ITALIC);

    /*** Determine Width & Height ***/

    float font_width = 0;
    float font_height = 0;
    SDL_Color color2 = {0xFF, 0xFF, 0xFF, 0xFF};
    SDL_Surface *source[256] = {0};
    for(unsigned char c = 1; c; ++c) {
      Core::assert_no_error();
      //char t[2] = {c, '\0'};
      //source[c] = TTF_RenderText_Blended(font, t, color2);
      source[c] = TTF_RenderGlyph_Blended(font, c, color2);
      Core::print_error();
      font_width = std::max(font_width, float(source[c] ? source[c]->w : 0));
      font_height = std::max(font_height, float(source[c] ? source[c]->h : 0));
    }

    /*** Initialize Intermediate SDL Surface ***/

    const int 
      next_w = int(pow(2.0f, ceil(log(float(16 * font_width))/log(2.0f)))), 
      next_h = int(pow(2.0f, ceil(log(float(16 * font_height))/log(2.0f))));
  
    SDL_Surface *font_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, next_w, next_h, 32, source[END_OF_TIME]->format->Rmask, source[END_OF_TIME]->format->Gmask, source[END_OF_TIME]->format->Bmask, source[END_OF_TIME]->format->Amask);
    if(!font_surface) {
      for(unsigned char c = 1; c; ++c)
        SDL_FreeSurface(source[c]);
      TTF_CloseFont(font);

      if(next_h > 1024) {
        m_vratio /= 2.0f;
        init(filepath);
        return;
      }
      else
        throw Font_Init_Failure();
    }

    SDL_LockSurface(font_surface);
    SDL_FillRect(font_surface, 0, SDL_MapRGBA(font_surface->format, 0, 0, 0, SDL_ALPHA_TRANSPARENT));
    SDL_UnlockSurface(font_surface);

    /*** Initialize Glyphs ***/

    SDL_Rect dstrect = {0, 0, Uint16(font_width), Uint16(font_height)};
    m_glyph[0] = 0;
    for(unsigned char c = 1; c; ++c) {
      dstrect.x = Sint16((c % 16) * font_width);
      dstrect.y = Sint16((c / 16) * font_height);
      m_glyph[c] = new Glyph(font, c, source[c], font_surface, dstrect, next_w, next_h, m_vratio);
    }

    /*** Correct Transparency ***/

    const Uint32 transparent_white = font_surface->format->Rmask | font_surface->format->Gmask | font_surface->format->Bmask;
    for(int i = 0; i < font_surface->h; ++i)
      for(Uint32 * src = reinterpret_cast<Uint32 *>(font_surface->pixels) + i * font_surface->pitch / 4,
                 * src_end = src + font_surface->w;
          src != src_end;
          ++src)
        if(*src & font_surface->format->Rmask)
          *src = transparent_white | ((*src & font_surface->format->Rmask) >> font_surface->format->Rshift << font_surface->format->Ashift);

    /*** Initialize Final Texture ***/
    
    m_texture = get_Video().create_Texture(font_surface, false);

    TTF_CloseFont(font);
  }

}

#include <Zeni/Global_Undef.h>
