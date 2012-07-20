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

#include <cassert>
#include <cmath>
#include <png.h>

namespace Zeni {

//   static void png_read_from_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
//     png_bytep * const p = reinterpret_cast<png_bytep *>(png_get_io_ptr(png_ptr));
//     memcpy(data, *p, length);
//     *p += length;
//   }

  Image::Image()
   : m_color_space(Image::RGBA),
   m_bytes_per_pixel(4),
   m_row_size(0),
   m_tileable(false)
  {
//     ZENI_LOGD(("Image: " + itoa(m_size.x) + "x" + itoa(m_size.y) + "x" + itoa(m_bytes_per_pixel) + " = " + itoa(m_row_size) + ", " + itoa(m_data.size())).c_str());
  }

  Image::Image(const String &filename, const bool &tileable_)
    : m_tileable(tileable_)
  {
//     String memory;
//     ZENI_LOGD("Begin Image::Image(...)::File_Ops::get_asset_FILE(...).");
//     File_Ops::load_asset(memory, filename);
    FILE * const file = File_Ops::get_asset_FILE(filename);
    class file_Destroyer {
    public:
      file_Destroyer(FILE * const &file_) : m_file(file_) {}
      ~file_Destroyer() {fclose(m_file);}

    private:
      FILE * m_file;
    } file_destroyer(file);
//     ZENI_LOGD("End Image::Image(...)::File_Ops::load_asset(...).");

//   if(memory.length() < 8u || png_sig_cmp(reinterpret_cast<png_byte *>(const_cast<char *>(memory.c_str())), 0, 8)) {
    png_byte header[8];
    if(!fread(header, 8u, 1u, file) || png_sig_cmp(header, 0, 8)) {
      ZENI_LOGE("PNG detection failure.");
      throw Texture_Init_Failure();
    }
//     else
//       ZENI_LOGD("PNG detection success.");

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr) {
      ZENI_LOGE("png_create_read_struct(...) failed.");
      throw Texture_Init_Failure();
    }
//     else
//       ZENI_LOGD("png_create_read_struct(...) success.");

    class png_structp_Destroyer {
    public:
      png_structp_Destroyer(const png_structp &ptr) : m_ptr(ptr) {}
      ~png_structp_Destroyer() {png_destroy_read_struct(&m_ptr, NULL, NULL);}

    private:
      png_structp m_ptr;
    } png_structp_destroyer(png_ptr);

    //create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
      ZENI_LOGE("png_create_info_struct(...) failed.");
      throw Texture_Init_Failure();
    }
//     else
//       ZENI_LOGD("png_create_info_struct(...) success.");
    
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4611 )
#endif
    //png error stuff, not sure libpng man suggests this.
    if(setjmp(png_jmpbuf(png_ptr))) {
#ifdef _WINDOWS
#pragma warning( pop )
#endif
      ZENI_LOGE("setjmp(png_jmpbuf(...)) failed.");
      throw Texture_Init_Failure();
    }
//     else
//       ZENI_LOGD("setjmp(png_jmpbuf(...)) success.");

    //init png reading
//     png_bytep mem_ptr = reinterpret_cast<png_bytep>(const_cast<char *>(memory.c_str() + 8u));
//     png_set_read_fn(png_ptr, &mem_ptr, png_read_from_memory);
    png_init_io(png_ptr, file);

    //let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    //variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 twidth, theight;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);
    if(bit_depth != 8) {
      ZENI_LOGE("Image using bit depth other than 8.");
      throw Texture_Init_Failure();
    }
    switch(color_type) {
      case PNG_COLOR_TYPE_GRAY:
        m_color_space = Luminance;
        m_bytes_per_pixel = 1;
        break;

      case PNG_COLOR_TYPE_GRAY_ALPHA:
        m_color_space = Luminance_Alpha;
        m_bytes_per_pixel = 2;
        break;

      case PNG_COLOR_TYPE_RGB:
        m_color_space = RGB;
        m_bytes_per_pixel = 3;
        break;

      case PNG_COLOR_TYPE_RGB_ALPHA:
        m_color_space = RGBA;
        m_bytes_per_pixel = 4;
        break;

      default:
        ZENI_LOGE("Image using must be Grayscale, RGB, or RGBA.");
        throw Texture_Init_Failure();
    }

    //update width and height based on png info
    m_size.x = int(twidth);
    m_size.y = int(theight);

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    m_row_size = png_get_rowbytes(png_ptr, info_ptr);

    // Allocate the image_data as a big block, to be given to opengl
    m_data.resize(m_row_size * theight);
    //row_pointers is for pointing to image_data for reading the png with libpng
    std::vector<Uint8 *> row_pointers(theight);
    // set the individual row_pointers to point at the correct offsets of image_data
    for(int j = 0; j < m_size.y; ++j)
      row_pointers[j] = &m_data[0] + j * m_row_size;

    //read the png into image_data through row_pointers
    png_read_image(png_ptr, reinterpret_cast<png_bytep *>(&row_pointers[0]));

//     ZENI_LOGD(("Image: " + itoa(m_size.x) + "x" + itoa(m_size.y) + "x" + itoa(m_bytes_per_pixel) + " = " + itoa(m_row_size) + ", " + itoa(m_data.size())).c_str());
  }

  Image::Image(const Point2i &size_, const Color_Space &color_space_, const bool &tileable_)
   : m_size(size_),
   m_color_space(color_space_),
   m_bytes_per_pixel(color_space_ == Luminance ? 1 : color_space_ == Luminance_Alpha ? 2 : color_space_ == RGB ? 3 : 4),
   m_row_size(size_.x * m_bytes_per_pixel),
   m_data(size_.y * m_row_size, '\0'),
   m_tileable(tileable_)
  {
//     ZENI_LOGD(("Image: " + itoa(m_size.x) + "x" + itoa(m_size.y) + "x" + itoa(m_bytes_per_pixel) + " = " + itoa(m_row_size) + ", " + itoa(m_data.size())).c_str());
  }

  void Image::set_Color(const Point2i &pixel, const Color &color) {
    set_RGBA(pixel, color.get_rgba());
  }

  void Image::set_RGBA(const Point2i &pixel, const Uint32 &rgba) {
    const Uint8 * const rgba_value = reinterpret_cast<const Uint8 *>(&rgba);
    Uint8 * const pixel_value = get_pixel(pixel);

    switch(m_color_space) {
      case Luminance_Alpha:
        pixel_value[1] = rgba_value[3];
      case Luminance:
        pixel_value[0] = Uint8((Uint16(rgba_value[0]) + Uint16(rgba_value[1]) + Uint16(rgba_value[2])) / 3);
        break;

      case RGBA:
        pixel_value[3] = rgba_value[3];
      case RGB:
        pixel_value[2] = rgba_value[2];
        pixel_value[1] = rgba_value[1];
        pixel_value[0] = rgba_value[0];
        break;

      default:
        assert(false);
    }
  }

  Color Image::extract_Color(const Point2i &pixel) const {
    const Uint8 * const pixel_value = get_pixel(pixel);

    switch(m_color_space) {
      case Luminance:
      {
        const float rgb = pixel_value[0] / 255.0f;
        return Color(1.0f, rgb, rgb, rgb);
      }

      case Luminance_Alpha:
      {
        const float rgb = pixel_value[0] / 255.0f;
        const float alpha = pixel_value[1] / 255.0f;
        return Color(alpha, rgb, rgb, rgb);
      }

      case RGB:
        return Color(1.0f, pixel_value[0] / 255.0f, pixel_value[1] / 255.0f, pixel_value[2] / 255.0f);

      case RGBA:
        return Color(pixel_value[3] / 255.0f, pixel_value[0] / 255.0f, pixel_value[1] / 255.0f, pixel_value[2] / 255.0f);

      default:
        assert(false);
        return Color();
    }
  }

  Uint32 Image::extract_RGBA(const Point2i &pixel) const {
    const Uint8 * const pixel_value = get_pixel(pixel);

    switch(m_color_space) {
      case Luminance:
      {
        const Uint32 rgb = pixel_value[0];
        return (rgb << 24) | (rgb << 16) | (rgb << 8) | 0xFF;
      }

      case Luminance_Alpha:
      {
        const Uint32 rgb = pixel_value[0];
        return (rgb << 24) | (rgb << 16) | (rgb << 8) | pixel_value[1];
      }

      case RGB:
        return (Uint32(pixel_value[0]) << 24) | (Uint32(pixel_value[1]) << 16) | (Uint32(pixel_value[2]) << 8) | 0xFF;

      case RGBA:
        return (Uint32(pixel_value[0]) << 24) | (Uint32(pixel_value[1]) << 16) | (Uint32(pixel_value[2]) << 8) | pixel_value[3];

      default:
        assert(false);
        return 0xFFFFFFFF;
    }
  }

  Color Image::extract_Color(const Point2f &coordinate) const {
    const Point2f scaled(coordinate.x * m_size.x,
                         coordinate.y * m_size.y);

    Point2i ul(int(scaled.x),
               int(scaled.y));

    if(m_tileable) {
      if(ul.x < 0)
        ul.x = m_size.x - ((-ul.x) % m_size.x);
      if(ul.x >= m_size.x)
        ul.x = ul.x % m_size.x;
      
      if(ul.y < 0)
        ul.y = m_size.y - ((-ul.y) % m_size.y);
      if(ul.y >= m_size.y)
        ul.y = ul.y % m_size.y;
    }
    else {
      if(ul.x < 0)
        ul.x = 0;
      else if(ul.x >= m_size.x)
        ul.x = m_size.x - 1;

      if(ul.y < 0)
        ul.y = 0;
      else if(ul.y >= m_size.y)
        ul.y = m_size.y - 1;
    }

    Point2i lr(ul.x + 1,
               ul.y + 1);

    if(m_tileable) {
      lr.x = lr.x % m_size.x;
      lr.y = lr.y % m_size.y;
    }
    else {
      if(lr.x == m_size.x)
        --lr.x;
      if(lr.y == m_size.y)
        --lr.y;
    }

    const Color ulc = extract_Color(ul);
    const Color llc = extract_Color(Point2i(ul.x, lr.y));
    const Color lrc = extract_Color(lr);
    const Color urc = extract_Color(Point2i(lr.x, ul.y));

    const float x_rhs_part = scaled.x - float(floor(scaled.x));
    const float y_rhs_part = scaled.y - float(floor(scaled.y));

    const Color uc = ulc.interpolate_to(x_rhs_part, urc);
    const Color lc = llc.interpolate_to(x_rhs_part, lrc);

    return uc.interpolate_to(y_rhs_part, lc);
  }

  void Image::resize(const int &width, const int &height) {
    Image resized(Point2i(width, height), m_color_space, m_tileable);

    const float w = float(width);
    const float h = float(height);

    for(int i = 0; i != width; ++i)
      for(int j = 0; j != height; ++j)
        resized.set_Color(Point2i(i, j), extract_Color(Point2f(i / w, j / h)));

    std::swap(m_size, resized.m_size);
    std::swap(m_color_space, resized.m_color_space);
    std::swap(m_bytes_per_pixel, resized.m_bytes_per_pixel);
    std::swap(m_row_size, resized.m_row_size);
    std::swap(m_data, resized.m_data);
    std::swap(m_tileable, resized.m_tileable);
  }

  bool Image::blit(const Point2i &upper_left, const Image &source) {
    if(this == &source || m_color_space != source.m_color_space)
      return false;

//     for(int j = 0; j != source.height(); ++j) {
//       const int y = upper_left.y + j;
//       for(int i = 0; i != source.width(); ++i) {
//         const int x = upper_left.x + i;
//         if(0 <= x && 0 <= y && x < m_size.x && y < m_size.y) {
//           Uint8 * dst = get_pixel(Point2i(x, y));
//           const Uint8 * src = source.get_pixel(Point2i(i, j));
//           switch(m_color_space) {
//             case RGBA:
//               dst[3] = src[3];
//             case RGB:
//               dst[2] = src[2];
//             case Luminance_Alpha:
//               dst[1] = src[1];
//             case Luminance:
//               dst[0] = src[0];
//             default:
//               break;
//           }
//         }
//       }
//     }

    const Point2i lower_right(upper_left.x + source.m_size.x,
                              upper_left.y + source.m_size.y);

    const int dx0 = std::max(0, upper_left.x);
    const int dy0 = std::max(0, upper_left.y);
    const int dx1 = std::min(m_size.x, lower_right.x);
    const int dy1 = std::min(m_size.y, lower_right.y);
    const int dx = dx1 - dx0;
    const int dy = dy1 - dy0;
    const int sx0 = dx0 - upper_left.x;
    const int sy0 = dy0 - upper_left.y;

//     ZENI_LOGD(String("Image::blit((" + itoa(m_size.x) + ", " + itoa(m_size.y) + "), (" + itoa(upper_left.x) + ", " + itoa(upper_left.y) + "), (" + itoa(source.m_size.x) + ", " + itoa(source.m_size.y) + ")) : {" + itoa(dx0) + "," + itoa(dy0) + "," + itoa(dx1) + "," + itoa(dy1) + "," + itoa(dx) + "," + itoa(dy) + "," + itoa(sx0) + "," + itoa(sy0) + "}").c_str());

    if(!dx || !dy)
      return true;

    Uint8 * dst = get_pixel(Point2i(dx0, dy0));
    const Uint8 * src = source.get_pixel(Point2i(sx0, sy0));
    const Uint8 * const dend = dst + dy * m_row_size;
    int sz = dx * m_bytes_per_pixel;
    while(dst != dend) {
      assert(&m_data[0] <= dst && dst + sz <= &m_data[0] + m_data.size());
      assert(&source.m_data[0] <= src && src + sz <= &source.m_data[0] + source.m_data.size());
      memcpy(dst, src, sz);
      dst += m_row_size;
      src += source.m_row_size;
    }

    return true;
  }

  const Uint8 * Image::get_pixel(const Point2i &pixel) const {
    assert(0 <= pixel.x && pixel.x < m_size.x);
    assert(0 <= pixel.y && pixel.y < m_size.y);

    return &m_data[0] + pixel.y * m_row_size + pixel.x * m_bytes_per_pixel;
  }

  Uint8 * Image::get_pixel(const Point2i &pixel) {
    assert(0 <= pixel.x && pixel.x < m_size.x);
    assert(0 <= pixel.y && pixel.y < m_size.y);

    return &m_data[0] + pixel.y * m_row_size + pixel.x * m_bytes_per_pixel;
  }

}
