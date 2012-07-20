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

#include <zeni_graphics.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <Zeni/Image.h>

#include <Zeni/Define.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace Zeni {

  Font::Font ()
    : m_glyph_height(0),
    m_virtual_screen_height(0.0f)
  {
  }

  Font::Font(const float &glyph_height,
             const float &virtual_screen_height,
             const String &font_name)
    : m_glyph_height(glyph_height),
    m_virtual_screen_height(virtual_screen_height),
    m_font_name(font_name)
  {
  }

  Font_FT::Glyph::Glyph()
    : m_glyph_width(0.0f)
  {
  }

  Font_FT::Glyph::Glyph(FT_Face &face,
                        const char &c,
                        const int &ascent,
                        const float &vratio,
                        const Point2i &position,
                        const Point2i &total_size)
    : m_glyph_width(0.0f)
  {
    // Reload the Glyph for our character.
    if(FT_Load_Char(face, c, FT_LOAD_RENDER))
      throw Error("FT_Load_Char(...) failed.");

    // Move the face's glyph into a Glyph object.
    FT_Glyph glyph;
    if(FT_Get_Glyph(face->glyph, &glyph))
      throw Error("FT_Get_Glyph(...) failed.");

    FT_BitmapGlyph bglyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

    m_glyph_width = int(glyph->advance.x) / (65536.0f * vratio);
    m_upper_left_point.x = bglyph->left / vratio;
    m_upper_left_point.y = (ascent - bglyph->top) / vratio;
    m_lower_right_point.x = (bglyph->left + bglyph->bitmap.width) / vratio;
    m_lower_right_point.y = (ascent - bglyph->top + bglyph->bitmap.rows) / vratio;

    m_upper_left_texel.x = float(position.x) / total_size.x;
    m_upper_left_texel.y = float(position.y) / total_size.y;
    m_lower_right_texel.x = float(position.x + bglyph->bitmap.width) / total_size.x;
    m_lower_right_texel.y = float(position.y + bglyph->bitmap.rows) / total_size.y;

//     ZENI_LOGD(("Generating Glyph " + itoa(glyph->advance.x) + "|" + ftoa(m_glyph_width) + " (" + itoa(bglyph->left) + "," + itoa(bglyph->top) + ") [" + itoa(bglyph->bitmap.width) + "," + itoa(bglyph->bitmap.rows) + "]").c_str());

    // Destroy the glyph object
    FT_Done_Glyph(glyph);
  }

  void Font_FT::Glyph::render(Video &vr, const Point2f &position, const float &vratio) const {
    const float x = int(position.x * vratio + 0.5f) / vratio;
    const float y = int(position.y * vratio + 0.5f) / vratio;

    Quadrilateral<Vertex2f_Texture> rect(
      (Vertex2f_Texture(Point2f(m_upper_left_point.x + x, m_upper_left_point.y + y), m_upper_left_texel)) ,
      (Vertex2f_Texture(Point2f(m_upper_left_point.x + x, m_lower_right_point.y + y), Point2f(m_upper_left_texel.x, m_lower_right_texel.y))) ,
      (Vertex2f_Texture(Point2f(m_lower_right_point.x + x, m_lower_right_point.y + y), m_lower_right_texel)) ,
      (Vertex2f_Texture(Point2f(m_lower_right_point.x + x, m_upper_left_point.y + y), Point2f(m_lower_right_texel.x, m_upper_left_texel.y))) );

//     ZENI_LOGD(("Rendering Glyph " + ftoa(m_glyph_width) + " (" + ftoa(m_upper_left_point.x + x) + "," + ftoa(m_upper_left_point.y + y) + "), " + ftoa(m_lower_right_point.x + x) + "x" + ftoa(m_lower_right_point.y + y) + ", texture (" + ftoa(m_upper_left_texel.x) + "," + ftoa(m_upper_left_texel.y) + "), " + ftoa(m_lower_right_texel.x) + "x" + ftoa(m_lower_right_texel.y)).c_str());

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
    : m_ascent(0),
    m_font_height(0.0f),
    m_vratio(0.0f)
  {
  }

  Font_FT::Font_FT(const String &filepath,
                   const float &glyph_height,
                   const float &virtual_screen_height)
    : Font(glyph_height,
           (virtual_screen_height < MINIMUM_VIRTUAL_SCREEN_HEIGHT ||
           virtual_screen_height > MAXIMUM_VIRTUAL_SCREEN_HEIGHT) ?
           float(get_Window().get_height()) : virtual_screen_height,
           filepath),
    m_ascent(0),
    m_font_height(glyph_height),
#ifdef TEMP_DISABLE
    m_vratio(1.0f)
#else
    m_vratio(get_Window().get_height() / get_virtual_screen_height())
#endif
  {
    ZENI_LOGD(("Generating font '" + filepath + "', size " + ftoa(m_font_height) + ", ratio " + ftoa(m_vratio)).c_str());
    init(filepath);
  }
  
  float Font_FT::get_text_width(const String &text) const {
    float max_width = 0.0f, width = 0.0f;
    unsigned int pos = 0;
    const unsigned int size = static_cast<unsigned int>(text.size());

    for(; pos < size; ++pos)
      if(text[pos] != '\r' && text[pos] != '\n')
        width += m_glyph[int(text[pos])].get_glyph_width();
      else {
        max_width = std::max(max_width, width);
        width = 0.0f;
      }

    return std::max(max_width, width);
  }

  void Font_FT::render_text(const String &text, const Point2f &position, const Color &color, const JUSTIFY &justify) const {
    Video &vr = get_Video();
    const float &x = position.x;
    const float &y = position.y;

    const Color previous_color = vr.get_Color();

    vr.set_Color(color);
    vr.apply_Texture(*m_texture);

    float cx, x_diff, cy = y;
    unsigned int i = 0u;

NEXT_LINE:

    cx = x;
    x_diff = 0.0f;

    if(justify != ZENI_LEFT) {
      for(unsigned int j = i; j < text.size(); ++j) {
        if(text[j] == '\r' || text[j] == '\n')
          break;

        x_diff -= m_glyph[int(text[j])].get_glyph_width();
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
      else {
        m_glyph[int(text[i])].render(vr, Point2f(cx, cy), m_vratio);
        cx += m_glyph[int(text[i])].get_glyph_width();
      }
    }

    vr.unapply_Texture();

    vr.set_Color(previous_color);
  }

  void Font_FT::render_text(const String &text, const Point3f &position, const Vector3f &right, const Vector3f &down, const Color &color, const JUSTIFY &justify) const {
    Video &vr = get_Video();

    const Color previous_color = vr.get_Color();

    vr.set_Color(color);
    vr.apply_Texture(*m_texture);

    Point3f pos, vertical_pos = position;
    float x_diff;
    unsigned int i = 0u;

NEXT_LINE_2:

    pos = vertical_pos;
    x_diff = 0.0f;

    if(justify != ZENI_LEFT) {
      for(unsigned int j = i; j < text.size(); ++j) {
        if(text[j] == '\r' || text[j] == '\n')
          break;

        x_diff -= m_glyph[int(text[j])].get_glyph_width();
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
      else {
        m_glyph[int(text[i])].render(vr, pos, right, down);
        pos += m_glyph[int(text[i])].get_glyph_width() * right;
      }
    }

    vr.unapply_Texture();

    vr.set_Color(previous_color);
  }

  void Font_FT::init(const String &filepath) {
    String filename(filepath.c_str());
    String file;
    File_Ops::load_asset(file, filename);

    FT_Open_Args foargs;
    memset(&foargs, 0, sizeof(FT_Open_Args));
    foargs.flags = FT_OPEN_MEMORY;
    foargs.memory_base = reinterpret_cast<const FT_Byte *>(file.c_str());
    foargs.memory_size = file.size();

    const int height = int(get_text_height() * m_vratio + 0.5f);

    //Create and initilize a freetype font library.
    FT_Library library;
    if(FT_Init_FreeType(&library)) {
      ZENI_LOGE("FT_Init_FreeType(...) failed.");
      throw Error("FT_Init_FreeType(...) failed.");
    }
    ZENI_LOGD("FT_Init_FreeType(...) success.");

    //The object in which Freetype holds information on a given
    //font is called a "face".
    FT_Face face;

    //This is where we load in the font information from the file.
    //Of all the places where the code might die, this is the most likely,
    //as FT_New_Face will die if the font file does not exist or is somehow broken.
    if(FT_Open_Face(library, &foargs, 0, &face)) {
      ZENI_LOGE("FT_Open_Face(...) failed.");
      throw Error("FT_Open_Face(...) failed.");
    }
    ZENI_LOGD("FT_Open_Face(...) success.");

    if(FT_Set_Pixel_Sizes(face, 0, height)) {
      ZENI_LOGE("FT_Set_Pixel_Sizes(...) failed.");
      throw Error("FT_Set_Pixel_Sizes(...) failed.");
    }
    ZENI_LOGD("FT_Set_Pixel_Sizes(...) success.");

    //This is where we actually create each of the fonts display lists.
    std::vector<Image> glyph(96);
    Point2i maxsz;
    for(int i = 32; i != 128; ++i) {
      Image &img = glyph[i - 32];
      img = gen_glyph(face, char(i));
      if(img.width() >= maxsz.x)
        maxsz.x = img.width() + 1;
      if(img.height() >= maxsz.y)
        maxsz.y = img.height() + 1;
    }

    Image glyphs(Point2i(next_power_of_two(10 * maxsz.x), next_power_of_two(10 * maxsz.y)), Image::Luminance_Alpha, false);
    m_glyph.reserve(256);
		m_glyph.resize(32);
    for(int i = 0; i != 96; ++i) {
      const Point2i pos((i % 10) * maxsz.x, (i / 10) * maxsz.y);
      glyphs.blit(pos, glyph[i]);
      m_glyph.push_back(Glyph(face, char(i + 32), m_ascent, m_vratio, pos, glyphs.size()));
    }
    glyph.clear();
		m_glyph.resize(256);

    ZENI_LOGD("Generating Texture from glyphs.");
    m_texture = std::auto_ptr<Texture>(get_Video().create_Texture(glyphs));

    //We don't need the face information now that the display
    //lists have been created, so we free the assosiated resources.
    ZENI_LOGD("Calling FT_Done_Face(...).");
    FT_Done_Face(face);

    //Ditto for the library.
    ZENI_LOGD("Calling FT_Done_FreeType(...).");
    FT_Done_FreeType(library);
  }

  int Font_FT::next_power_of_two(const int &a) {
    int rval=1;
    while(rval<a) rval<<=1;
    return rval;
  }

  Image Font_FT::gen_glyph(FT_Face &face, const char &ch) {
    //Load the Glyph for our character.
    if(FT_Load_Char(face, ch, FT_LOAD_RENDER)) {
      ZENI_LOGE("FT_Load_Char(...) failed.");
      throw Error("FT_Load_Char(...) failed.");
    }
//     ZENI_LOGD("FT_Load_Char(...) success.");

    //Move the face's glyph into a Glyph object.
    FT_Glyph glyph;
    if(FT_Get_Glyph(face->glyph, &glyph)) {
      ZENI_LOGE("FT_Get_Glyph(...) failed.");
      throw Error("FT_Get_Glyph(...) failed.");
    }
//     ZENI_LOGD("FT_Get_Glyph(...) success.");

    FT_BitmapGlyph bglyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

    //Update the maximum Glyph ascent
    if(bglyph->top > m_ascent)
      m_ascent = bglyph->top;

    //Here we fill in the data for the expanded bitmap.
    //Notice that we are using two channel bitmap (one for
    //luminocity and one for alpha), but we assign
    //both luminocity and alpha to the value that we
    //find in the FreeType bitmap. 
    //We use the ?: operator so that value which we use
    //will be 0 if we are in the padding zone, and whatever
    //is the the Freetype bitmap otherwise.
    Image image(Point2i(bglyph->bitmap.width, bglyph->bitmap.rows), Image::Luminance_Alpha, false);
    Uint8 * image_data = image.get_data();
//     ZENI_LOGD("Copying glyph into Image.");
    for(int pixel = 0, pend = bglyph->bitmap.width * bglyph->bitmap.rows; pixel != pend; ++pixel) {
      image_data[2 * pixel] = 0xFF;
      image_data[2 * pixel + 1] = bglyph->bitmap.buffer[pixel];
    }
//     ZENI_LOGD("Glyph copied into Image.");

    //Destroy the glyph object
    FT_Done_Glyph(glyph);

    return image;
  }

}

#include <Zeni/Undefine.h>
