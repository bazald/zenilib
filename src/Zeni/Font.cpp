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

  Font::Font(const bool &bold, const bool &italic, const int &glyph_height)
    : m_bold(bold),
    m_italic(italic),
    m_glyph_height(glyph_height)
  {
  }

#ifndef DISABLE_GL
  Font_GL::Glyph::Glyph()
    : m_glyph_width(0),
    m_glyph_height(0),
    tex_w(0),
    tex_h(0),
    texture(0)
  {
  }

  Font_GL::Glyph::Glyph(TTF_Font *font_, const char &c)
    : m_glyph_width(0),
    m_glyph_height(TTF_FontHeight(font_)),
    tex_w(0),
    tex_h(0),
    texture(0)
  {
    TTF_GlyphMetrics(font_, c, 0, 0, 0, 0, &m_glyph_width);
    const int 
      next_w = int(pow(2, ceil(log(float(m_glyph_width))/log(2.0f)))), 
      next_h = int(pow(2, ceil(log(float(m_glyph_height))/log(2.0f))));

    SDL_Color color2 = {0xFF, 0xFF, 0xFF, 0xFF};

    char t[2] = {c, '\0'};
    SDL_Surface *surface = TTF_RenderText_Blended(font_, t, color2);
    {
      SDL_Surface *surf2 = SDL_CreateRGBSurface(SDL_SWSURFACE, next_w, next_h, 32
        , 0xff000000
        , 0x00ff0000
        , 0x0000ff00
        , 0x000000ff
        );
      if(surf2) {
        SDL_FillRect(surf2, 0, SDL_MapRGBA(surf2->format, SDL_ALPHA_TRANSPARENT, 255, 255, 255));

        SDL_BlitSurface(surface, 0, surf2, 0);

        SDL_FreeSurface(surface);
        surface = surf2;
      }
    }

    texture = new Texture_GL(surface);

    tex_w = float(m_glyph_width) / next_w;
    tex_h = float(m_glyph_height) / next_h;
  }

  Font_GL::Glyph::~Glyph() {
    delete texture;
  }

  void Font_GL::Glyph::render(const int &x, const int &y) const {
    glEnable(GL_TEXTURE_2D);
    texture->apply_texture();

    Quadrilateral<Vertex2f_Texture> rect
      (Vertex2f_Texture(Point2f(float(x), float(y)), Point2f(0, 0)),
      Vertex2f_Texture(Point2f(float(x), float(y) + m_glyph_height), Point2f(0, tex_h)),
      Vertex2f_Texture(Point2f(float(x) + m_glyph_width, float(y) + m_glyph_height), Point2f(tex_w, tex_h)),
      Vertex2f_Texture(Point2f(float(x) + m_glyph_width, float(y)), Point2f(tex_w, 0)));

    Video::get_reference().render(rect);

    glDisable(GL_TEXTURE_2D);
  }

  Font_GL::Font_GL()
    : m_font_height(0)
  {
    memset(m_glyph, 0, num_glyphs * sizeof(Glyph *));
  }

  Font_GL::Font_GL(const std::string &codename, const bool &bold, const bool &italic, const int &glyph_height)
    : Font(bold, italic, glyph_height),
    m_font_height(0)
  {
    static const string directory = "fonts/", extension = ".ttf";
    string filename = directory + codename + extension;

    TTF_Font *font = TTF_OpenFont(filename.c_str(), glyph_height);
    if(!font)
      throw Font_Init_Failure();

    if(bold && italic)
      TTF_SetFontStyle(font, TTF_STYLE_BOLD | TTF_STYLE_ITALIC);
    else if(bold)
      TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    else if(italic)
      TTF_SetFontStyle(font, TTF_STYLE_ITALIC);

    for(int i = 0; i < num_glyphs; ++i)
      m_glyph[i] = new Glyph(font, char(i));

    m_font_height = TTF_FontHeight(font);

    TTF_CloseFont(font);
  }

  Font_GL::~Font_GL() {
    for(int i = 0; i < num_glyphs; ++i)
      delete m_glyph[i];
  }

  void Font_GL::render_text(const std::string &text, const int &x, const int &y, const Color &color, const JUSTIFY &justify) const {
    Video::get_reference().set_color_to(color);

    int cx = x, x_diff = 0;

    if(justify != ZENI_LEFT) {
      for(unsigned int i = 0; i < text.size(); ++i) {
        if(text[i] == '\r' || text[i] == '\n')
          break;

        x_diff -= m_glyph[int(text[i])]->get_glyph_width();
      }

      if(justify == ZENI_CENTER)
        cx += x_diff / 2;
      else if(justify == ZENI_RIGHT)
        cx += x_diff;
    }

    for(unsigned int i = 0; i < text.size(); ++i) {
      m_glyph[int(text[i])]->render(cx, y);

      cx += m_glyph[int(text[i])]->get_glyph_width();

      if(text[i] == '\r' && i+1 < text.size() && text[i+1] == '\n')
        ++i;
      if(text[i] == '\r' || text[i] == '\n') {
        render_text(text.substr(i + 1, text.length() - i - 1), x, y + m_font_height, color, justify);
        break;
      }
    }
  }
#endif

#ifndef DISABLE_DX9
  Font_DX9::Font_DX9()
    : font(0)
  {
  }

  Font_DX9::Font_DX9(const std::string &codename, const bool &bold, const bool &italic, const int &glyph_height)
    : Font(bold, italic, glyph_height), 
    font(0), 
    resized(0), 
    ratio(0)
  {
    if(FAILED(D3DXCreateFont(dynamic_cast<Video_DX9 &>(Video::get_reference()).get_d3d_device(),
      glyph_height,
      0,
      bold ? FW_BOLD : FW_REGULAR,
      0,
      italic ? TRUE : FALSE,
      DEFAULT_CHARSET,
      OUT_DEFAULT_PRECIS,
      DEFAULT_QUALITY,
      DEFAULT_PITCH | FF_DONTCARE,
      codename.c_str(),
      &font)))
      throw Font_Init_Failure();
  }

  Font_DX9::~Font_DX9() {
    if(font)
      font->Release();
    if(resized)
      resized->Release();
  }

  void Font_DX9::render_text(const std::string &text, const int &x, const int &y, const Color &color, const JUSTIFY &justify) const {
    D3DCOLOR color2 = D3DCOLOR_ARGB(color.a_ub(),
      color.r_ub(),
      color.g_ub(),
      color.b_ub());

    LPD3DXFONT ptr = font;
    int x_diff = 0, y_diff = 0;
    float x_scale = 1.0f, y_scale = 1.0f;

    D3DVIEWPORT9 vp;
    if(dynamic_cast<Video_DX9 &>(Video::get_reference()).get_d3d_device()->GetViewport(&vp) == S_OK) {
      x_scale = float(vp.Height) / Video::get_reference().get_screen_height();
      y_scale = float(vp.Height) / Video::get_reference().get_screen_height();

      if(x_scale != 1.0f || y_scale != 1.0f) {
        if(ratio == y_scale)
          ptr = resized;
        else {
          if(resized)
            resized->Release();

          ratio = y_scale;

          D3DXFONT_DESCA desc;
          font->GetDesc(&desc);
          desc.Height = int(desc.Height * ratio);

          if(FAILED(D3DXCreateFontIndirect(dynamic_cast<Video_DX9 &>(Video::get_reference()).get_d3d_device(), &desc, &resized))) {
            ratio = 0;
            resized = 0;
            throw Font_Init_Failure();
          }
          else
            ptr = resized;
        }

        x_diff += vp.X;
        y_diff += vp.Y;
      }
    }

    RECT rect;    
    ptr->DrawText(NULL, text.c_str(), -1, &rect, DT_CALCRECT, color2);

    int x_scaled = int(x * x_scale), y_scaled = int(y * y_scale);
    switch(justify) {
    case ZENI_CENTER:
      x_diff += x_scaled - (rect.left + rect.right) / 2;
      y_diff += y_scaled - rect.top;
      break;
    case ZENI_RIGHT:
      x_diff += x_scaled - rect.right;
      y_diff += y_scaled - rect.top;
      break;
    case ZENI_LEFT:
    default:
      x_diff += x_scaled - rect.left;
      y_diff += y_scaled - rect.top;
    }

    rect.left += x_diff;
    rect.right += x_diff;
    rect.top += y_diff;
    rect.bottom += y_diff;

    ptr->DrawText(NULL, text.c_str(), -1, &rect, 0, color2);
  }
#endif

}
