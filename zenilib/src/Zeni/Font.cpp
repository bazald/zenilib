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

#include <Zeni/Font.hxx>

#include <Zeni/Texture.h>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Quadrilateral.hxx>
#include <Zeni/Vertex2f.hxx>
#include <Zeni/Video.hxx>

using namespace std;

namespace Zeni {

  Font::Font ()
    : m_bold(0),
    m_italic(0),
    m_glyph_height(0)
  {
  }

  Font::Font(const bool &bold, const bool &italic, const int &glyph_height, const std::string &font_name)
    : m_bold(bold),
    m_italic(italic),
    m_glyph_height(glyph_height),
    m_font_name(font_name)
  {
  }

  Font_FT::Glyph::Glyph()
    : m_glyph_width(0)
  {
  }

  Font_FT::Glyph::Glyph(TTF_Font *font_, const char &c, SDL_Surface *source, SDL_Surface *render_target, const SDL_Rect &dstrect, const int &total_width, const int &total_height)
    : m_glyph_width(0)
  {
    int minx, maxy;
    TTF_GlyphMetrics(font_, c, &minx, 0, 0, &maxy, &m_glyph_width);

    m_upper_left_point.x = float(minx);
    m_upper_left_point.y = float(TTF_FontAscent(font_) - maxy);
    m_lower_right_point.x = m_upper_left_point.x + source->w;
    m_lower_right_point.y = m_upper_left_point.y + source->h;

    SDL_Rect dstrect2 = {dstrect.x, dstrect.y, Uint16(source->w), Uint16(source->h)};

    m_upper_left_texel.x = float(dstrect2.x) / float(total_width);
    m_upper_left_texel.y = float(dstrect2.y) / float(total_width);
    m_lower_right_texel.x = float(dstrect2.x + source->w) / float(total_width);
    m_lower_right_texel.y = float(dstrect2.y + source->h) / float(total_height);

    SDL_BlitSurface(source, 0, render_target, &dstrect2);
    SDL_FreeSurface(source);
  }

  void Font_FT::Glyph::render(const Point2f &position) const {
    static Video &vr = get_Video();
    const float &x = position.x;
    const float &y = position.y;

    Quadrilateral<Vertex2f_Texture> rect
      (Vertex2f_Texture(Point2f(m_upper_left_point.x + x, m_upper_left_point.y + y), m_upper_left_texel),
      Vertex2f_Texture(Point2f(m_upper_left_point.x + x, m_lower_right_point.y + y), Point2f(m_upper_left_texel.x, m_lower_right_texel.y)),
      Vertex2f_Texture(Point2f(m_lower_right_point.x + x, m_lower_right_point.y + y), m_lower_right_texel),
      Vertex2f_Texture(Point2f(m_lower_right_point.x + x, m_upper_left_point.y + y), Point2f(m_lower_right_texel.x, m_upper_left_texel.y)));

    vr.render(rect);
  }

  Font_FT::Font_FT()
    : m_font_height(0)
  {
    memset(m_glyph, 0, num_glyphs * sizeof(Glyph *));
  }

  Font_FT::Font_FT(const std::string &codename, const bool &bold, const bool &italic, const int &glyph_height)
    : Font(bold, italic, glyph_height, codename),
    m_font_height(0)
  {
    static const string directory = "fonts/", extension = ".ttf";
    string filename = directory + codename + extension;

    TTF_Font *font = TTF_OpenFont(filename.c_str(), glyph_height);
    if(!font)
      throw Font_Init_Failure();

    /*** Set Style ***/

    if(bold && italic)
      TTF_SetFontStyle(font, TTF_STYLE_BOLD | TTF_STYLE_ITALIC);
    else if(bold)
      TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    else if(italic)
      TTF_SetFontStyle(font, TTF_STYLE_ITALIC);

    /*** Determine Width & Height ***/

    m_font_height = TTF_FontHeight(font);

    int font_width = 0;
    int font_height = 0;
    SDL_Color color2 = {0xFF, 0xFF, 0xFF, 0xFF};
    SDL_Surface *source[256] = {0};
    for(unsigned char c = 1; c; ++c) {
      //char t[2] = {c, '\0'};
      //source[c] = TTF_RenderText_Blended(font, t, color2);
      source[c] = TTF_RenderGlyph_Blended(font, c, color2);
      font_width = max(font_width, source[c] ? source[c]->w : 0);
      font_height = max(font_height, source[c] ? source[c]->h : 0);
    }

    /*** Initialize Intermediate SDL Surface ***/

    int next_w = int(pow(2.0f, ceil(log(float(16 * font_width ))/log(2.0f)))); 
    int next_h = int(pow(2.0f, ceil(log(float(16 * font_height))/log(2.0f))));

    if(next_w > next_h)
      next_h = next_w;
    else if(next_h > next_w)
      next_w = next_h;

    SDL_Surface *font_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, next_w, next_h, 32, source[42]->format->Rmask, source[42]->format->Gmask, source[42]->format->Bmask, source[42]->format->Amask);
    if(!font_surface)
      throw Font_Init_Failure();

    SDL_FillRect(font_surface, 0, SDL_MapRGBA(font_surface->format, 0, 0, 0, SDL_ALPHA_TRANSPARENT));

    /*** Initialize Glyphs ***/

    SDL_Rect dstrect = {0, 0, Uint16(font_width), Uint16(font_height)};
    m_glyph[0] = 0;
    for(unsigned char c = 1; c; ++c) {
      dstrect.x = Sint16((c % 16) * font_width);
      dstrect.y = Sint16((c / 16) * font_height);
      m_glyph[c] = new Glyph(font, c, source[c], font_surface, dstrect, next_w, next_h);
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

  Font_FT::~Font_FT() {
    for(int i = 1; i < num_glyphs; ++i)
      delete m_glyph[i];
  }
  
  int Font_FT::get_text_width(const std::string &text) const {
    int max_width = 0, width = 0;
    unsigned int pos = 0;

    for(; pos < text.size(); ++pos)
      if(text[pos] != '\r' && text[pos] != '\n' &&
        m_glyph[int(text[pos])])
        width += m_glyph[int(text[pos])]->get_glyph_width();
      else {
        max_width = max(max_width, width);
        width = 0;
      }

    return max(max_width, width);
  }

  void Font_FT::render_text(const std::string &text, const Point2f &position, const Color &color, const JUSTIFY &justify) const {
    Video &vr = get_Video();
    const float &x = position.x;
    const float &y = position.y;

    const Color previous_color = vr.get_color();

    vr.set_color(color);
    vr.apply_texture(*m_texture);

    float cx, x_diff, cy = y;
    int i = 0;

NEXT_LINE:

    cx = x;
    x_diff = 0;

    if(justify != ZENI_LEFT) {
      for(unsigned int j = i; j < text.size(); ++j) {
        if(text[j] == '\r' || text[j] == '\n' ||
          !m_glyph[int(text[j])])
          break;

        x_diff -= m_glyph[int(text[j])]->get_glyph_width();
      }

      if(justify == ZENI_CENTER)
        cx += x_diff / 2;
      else if(justify == ZENI_RIGHT)
        cx += x_diff;
    }

    for(; i < int(text.size()); ++i) {
      if(text[i] == '\r' && i+1 < int(text.size()) && text[i+1] == '\n')
        ++i;

      if(text[i] == '\r' || text[i] == '\n') {
        ++i;
        cy += m_font_height;
        goto NEXT_LINE;
      }
      else if(m_glyph[int(text[i])]) {
        m_glyph[int(text[i])]->render(Point2f(cx, cy));
        cx += m_glyph[int(text[i])]->get_glyph_width();
      }
    }

    vr.unapply_texture();

    vr.set_color(previous_color);
  }

#ifndef DISABLE_DX9
  Font_DX9::Font_DX9()
    : font(0)
  {
  }

  Font_DX9::Font_DX9(const std::string &codename, const bool &bold, const bool &italic, const int &glyph_height)
    : Font(bold, italic, glyph_height, codename), 
    font(0), 
    resized(0), 
    ratio(0)
  {
    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(get_Video());

    if(FAILED(D3DXCreateFont(vdx.get_d3d_device(),
      get_text_height(),
      0,
      is_bold() ? FW_BOLD : FW_REGULAR,
      0,
      is_italic() ? TRUE : FALSE,
      DEFAULT_CHARSET,
      OUT_TT_ONLY_PRECIS, //OUT_DEFAULT_PRECIS,
      ANTIALIASED_QUALITY, //DEFAULT_QUALITY,
      DEFAULT_PITCH | FF_DONTCARE,
      get_font_name().c_str(),
      &font)))
      throw Font_Init_Failure();
  }

  Font_DX9::~Font_DX9() {
    if(font)
      font->Release();
    if(resized)
      resized->Release();
  }
  
  int Font_DX9::get_text_width(const std::string &text) const {
    static Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(get_Video());

    D3DCOLOR color2 = D3DCOLOR_ARGB(
      static_cast<unsigned char>(0xFF),
      static_cast<unsigned char>(0xFF),
      static_cast<unsigned char>(0xFF),
      static_cast<unsigned char>(0xFF));

    RECT rect;    
    font->DrawText(NULL, text.c_str(), -1, &rect, DT_CALCRECT, color2);

    return rect.right - rect.left;
  }

  void Font_DX9::render_text(const std::string &text, const Point2f &position, const Color &color, const JUSTIFY &justify) const {
    static Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(get_Video());
    const float &x = position.x;
    const float &y = position.y;

    D3DCOLOR color2 = D3DCOLOR_ARGB(color.a_ub(),
      color.r_ub(),
      color.g_ub(),
      color.b_ub());

    LPD3DXFONT ptr = font;
    int x_diff = 0, y_diff = 0;
    
    // !!FIXME!!

    D3DVIEWPORT9 vp;
    if(FAILED(vdx.get_d3d_device()->GetViewport(&vp)))
      throw Font_Init_Failure();
    else {
      const float y_scale = float(vp.Height) / get_Video().get_screen_height();

      if(y_scale != 1.0f) {
        if(ratio == y_scale)
          ptr = resized;
        else {
          if(resized)
            resized->Release();

          ratio = y_scale;

//#ifdef UNICODE
//          D3DXFONT_DESCW desc;
//#else
//          D3DXFONT_DESCA desc;
//#endif
//          if(FAILED(font->GetDesc(&desc))) {
//            ratio = 0;
//            resized = 0;
//            throw Font_Init_Failure();
//          }
//          desc.Height = int(desc.Height * ratio);
//
//          if(FAILED(D3DXCreateFontIndirect(vdx.get_d3d_device(), &desc, &resized))) {
            if(FAILED(D3DXCreateFont(vdx.get_d3d_device(),
                  get_text_height(),
                  0,
                  is_bold() ? FW_BOLD : FW_REGULAR,
                  0,
                  is_italic() ? TRUE : FALSE,
                  DEFAULT_CHARSET,
                  OUT_TT_ONLY_PRECIS, //OUT_DEFAULT_PRECIS,
                  ANTIALIASED_QUALITY, //DEFAULT_QUALITY,
                  DEFAULT_PITCH | FF_DONTCARE,
                  get_font_name().c_str(),
                  &resized))) {
            ratio = 0;
            resized = 0;
            throw Font_Init_Failure();
          }
          else
            ptr = resized;
        }
      }

      x_diff = vp.X + int(x * y_scale);
      y_diff = vp.Y + int(y * y_scale);
    }

    RECT rect = {0, 0, 0, 0};
    ptr->DrawText(NULL, text.c_str(), -1, &rect, DT_CALCRECT, color2);

    x_diff -= rect.right * justify / 2;

    rect.left += x_diff;
    rect.right += x_diff;
    rect.top += y_diff;
    rect.bottom += y_diff;

    ptr->DrawText(NULL, text.c_str(), -1, &rect, 0, color2);
  }
#endif

}
