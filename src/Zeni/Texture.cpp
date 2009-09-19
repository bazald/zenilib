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

#include <Zeni/Texture.hxx>

#ifndef DISABLE_GL
#ifdef _MACOSX
#include <SDL_gfx/SDL_rotozoom.h>
#else
#include <SDL/SDL_rotozoom.h>
#endif
#endif

#include <iostream>

#include <Zeni/Global.h>

namespace Zeni {

  int Texture::build_from_surface(SDL_Surface * &surface, const Point2i &max_resolution) {
    bool scale = false;

    int next_w = 1;
    for(; next_w; next_w <<= 1)
      if(next_w > surface->w) {
        scale = true;
        break;
      }
      else if(surface->w == next_w)
        break;
    if(max_resolution.x && next_w > max_resolution.x) {
      next_w = max_resolution.x;
      scale = true;
    }

    int next_h = 1;
    for(; next_h; next_h <<= 1)
      if(next_h > surface->h) {
        scale = true;
        break;
      }
      else if(surface->h == next_h)
        break;
    if(max_resolution.y && next_h > max_resolution.y) {
      next_h = max_resolution.y;
      scale = true;
    }

    double scale_w = next_w;
    double scale_h = next_h;

    if(scale) {
      int actual_w, actual_h;
      for(int i = 0; i < END_OF_TIME; ++i) {
        zoomSurfaceSize(surface->w, surface->h,
                        scale_w/surface->w, scale_h/surface->h,
                        &actual_w, &actual_h);

        if(next_w == actual_w &&
           next_h == actual_h)
          break;

        if(actual_w < next_w)
          scale_w += 0.5;
        else if(actual_w > next_w)
          scale_w -= 0.5;

        if(actual_h < next_h)
          scale_h += 0.5;
        else if(actual_h > next_h)
          scale_h -= 0.5;
      };

      SDL_Surface *surf2 = zoomSurface(surface,
        scale_w/surface->w,
        scale_h/surface->h, 1);

      if(surf2) {
        SDL_FreeSurface(surface);
        surface = surf2;
      }
    }

    switch(surface->format->BytesPerPixel) {
    case 3:
      if(surface->format->Rshift == 0 && surface->format->Gshift == 8 && surface->format->Bshift == 16)
        return -'R';
      else if(surface->format->Bshift == 0 && surface->format->Gshift == 8 && surface->format->Rshift == 16)
        return -'B';
      break;

    case 4:
      if(surface->format->Rshift == 0 && surface->format->Gshift == 8 && surface->format->Bshift == 16 && surface->format->Ashift == 24)
        return 'R';
      else if(surface->format->Bshift == 0 && surface->format->Gshift == 8 && surface->format->Rshift == 16 && surface->format->Ashift == 24)
        return 'B';
      break;

    default:
      break;
    }

    throw Texture_Init_Failure();
  }

  Sprite::Sprite()
    : Texture(Texture_Base::VTYPE_SPRITE, false),
    m_frame(0)
  {
  }
  
  Sprite::Sprite(const Sprite &rhs)
    : Texture(Texture_Base::VTYPE_SPRITE, false),
    m_frames(rhs.m_frames),
    m_frame(rhs.m_frame)
  {
  }

  Sprite & Sprite::operator=(const Sprite &rhs) {
    Sprite lhs(rhs);
    std::swap(lhs.m_frames, m_frames);
    std::swap(lhs.m_frame, m_frame);
    return *this;
  }

  void Sprite::append_frame(const std::string &name) {
    append_frame(name, get_Textures().get_id(name));
  }

  void Sprite::append_frame(const std::string &name, const unsigned long &id) {
    m_frames.push_back(make_pair(name, id));
  }

  int Sprite::find_frame(const std::string &name, const int &starting_point) const {
    if(starting_point < 0)
      throw Frame_Out_of_Range();

    for(int i = starting_point, end = int(m_frames.size()); i < end; ++i)
      if(m_frames[size_t(i)].first == name)
        return i;
    return -1;
  }

  void Sprite::insert_frame(const std::string &name, const int &at_this_index) {
    insert_frame(name, get_Textures().get_id(name), at_this_index);
  }

  void Sprite::insert_frame(const std::string &name, const unsigned long &id, const int &at_this_index) {
    std::pair<std::string, unsigned long> new_frame = make_pair(name, id);
    
    if(at_this_index < 0 || at_this_index > int(m_frames.size()))
      throw Frame_Out_of_Range();

    m_frames.push_back(new_frame);

    memmove(
      &m_frames + at_this_index + 1,
      &m_frames + at_this_index,
      (m_frames.size() - at_this_index - 1) * sizeof(std::pair<std::string, unsigned long>));

    m_frames[size_t(at_this_index)] = new_frame;
  }

  void Sprite::remove_frame(const int &frame_number) {
    if(frame_number < 0 || frame_number > int(m_frames.size()))
      throw Frame_Out_of_Range();

    memmove(
      &m_frames + frame_number,
      &m_frames + frame_number + 1,
      (m_frames.size() - frame_number - 1) * sizeof(std::pair<std::string, unsigned long>));

    m_frames.pop_back();
  }

  int Sprite::get_num_frames() const {
    return int(m_frames.size());
  }

  int Sprite::get_current_frame() const {
    if(m_frames.empty())
      return -1;
    return m_frame;
  }

  void Sprite::set_current_frame(const int &frame_number) {
    if(frame_number < 0 || frame_number > int(m_frames.size()))
      throw Frame_Out_of_Range();

    m_frame = frame_number;
  }

#ifndef DISABLE_GL
  Texture_GL::Texture_GL(const std::string &filename, const bool &repeat, const bool &lazy_loading)
    : Texture(Texture_Base::VTYPE_GL, repeat),
    m_texture_id(0),
    m_render_buffer(0),
    m_frame_buffer_object(0),
    m_filename(filename)
  {
    if(!lazy_loading)
      load(m_filename, m_repeat);
  }

  Texture_GL::Texture_GL(SDL_Surface *surface, const bool &repeat)
    : Texture(Texture_Base::VTYPE_GL, repeat),
    m_size(surface->w, surface->h),
    m_texture_id(build_from_surface(surface, repeat)),
    m_render_buffer(0),
    m_frame_buffer_object(0)
  {
  }

  Texture_GL::Texture_GL(const Point2i &size, const bool &repeat)
    : Texture(Texture_Base::VTYPE_GL, repeat),
    m_size(size),
    m_texture_id(build_from_surface(SDL_CreateRGBSurface(SDL_SWSURFACE, size.x, size.y, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000), repeat)),
    m_render_buffer(0),
    m_frame_buffer_object(0)
  {
  }

  Texture_GL::~Texture_GL() {
    if(m_render_buffer)
      glDeleteRenderbuffersEXT(1, &m_render_buffer);
    if(m_frame_buffer_object)
      glDeleteFramebuffersEXT(1, &m_frame_buffer_object);

    if(m_texture_id)
      glDeleteTextures(1, &m_texture_id);
  }

  GLuint Texture_GL::build_from_surface(SDL_Surface *surface, const bool &repeat) {
    GLuint texture_id = 0;

    const int mode = Texture::build_from_surface(surface, Point2i(ZENI_MAX_TEXTURE_WIDTH, ZENI_MAX_TEXTURE_HEIGHT));

    const GLint mode1 = mode > 0 ? GL_RGBA : GL_RGB;
    const GLenum mode2 = GLenum(
      mode == -'R' ? GL_RGB :
      mode == -'B' ? GL_BGR :
      mode == 'R' ? GL_RGBA :
      /*mode == 'A' ?*/ GL_BGRA);

    // Allocate a unique id for the texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);

    if(glGetError() == GL_INVALID_ENUM) {
      static bool printed = false;
      if(!printed) {
        std::cerr << "Quality Warning:  Your graphics card does not support GL_CLAMP_TO_EDGE in OpenGL.\n";
        printed = true;
      }
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Textures::get_bilinear_filtering() ? GL_LINEAR : GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      Textures::get_mipmapping() ?
      (Textures::get_bilinear_filtering() ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST) :
      (Textures::get_bilinear_filtering() ? GL_LINEAR : GL_NEAREST));

    if(Textures::get_anisotropic_filtering()) {
      if(Textures::get_anisotropic_filtering() < 0 || Textures::get_anisotropic_filtering() > get_Video().get_maximum_anisotropy())
        Textures::set_texturing_mode(get_Video().get_maximum_anisotropy(), Textures::get_bilinear_filtering(), Textures::get_mipmapping());

	  if(Textures::get_anisotropic_filtering())
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Textures::get_anisotropic_filtering());
    }

    /*
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_FOG_HINT, GL_NICEST);
    */
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    if(Textures::get_mipmapping())
      glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    if(glGetError() == GL_INVALID_ENUM)
      gluBuild2DMipmaps(GL_TEXTURE_2D, mode1, surface->w, surface->h, mode2, GL_UNSIGNED_BYTE, surface->pixels);
    else
      glTexImage2D(GL_TEXTURE_2D, 0, mode1, surface->w, surface->h, 0, mode2, GL_UNSIGNED_BYTE, surface->pixels);

    SDL_FreeSurface(surface);
    
    return texture_id;
  }
  
  void Texture_GL::load(const std::string &filename, const bool &repeat) const {
    SDL_Surface *surface;
    
    {
      surface = IMG_Load(filename.c_str());
      if(!surface)
        throw Texture_Init_Failure();
    }
    
    try {
      const_cast<int &>(m_size.x) = surface->w;
      const_cast<int &>(m_size.y) = surface->h;
      m_texture_id = build_from_surface(surface, repeat);
    }
    catch(...) {
      SDL_FreeSurface(surface);
      throw;
    }
  }
  
#endif

#ifndef DISABLE_DX9
  Texture_DX9::Texture_DX9(const std::string &filename, const bool &repeat) 
    : Texture(Texture_Base::VTYPE_DX9, repeat),
    m_texture(0)
  {
    load(filename);
  }

  Texture_DX9::Texture_DX9(SDL_Surface *surface, const bool &repeat)
    : Texture(Texture_Base::VTYPE_DX9, repeat),
    m_size(surface->w, surface->h),
    m_texture(build_from_surface(surface))
  {
  }

  Texture_DX9::Texture_DX9(const Point2i &size, const bool &repeat)
    : Texture(Texture_Base::VTYPE_DX9, repeat),
    m_size(size),
    m_texture(0)
  {
    Video_DX9 &vr = static_cast<Video_DX9 &>(get_Video());

    if(FAILED(D3DXCreateTexture(vr.get_d3d_device(),
                                UINT(size.x), UINT(size.y),
                                D3DX_DEFAULT,
                                D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP,
                                D3DFMT_A8R8G8B8,
                                D3DPOOL_DEFAULT,
                                &m_texture)))
    {
      throw Texture_Init_Failure();
    }
  }

  Texture_DX9::~Texture_DX9() {
    if(m_texture)
      m_texture->Release();
  }

  void Texture_DX9::set_sampler_states() {
    Video_DX9 &vr = static_cast<Video_DX9 &>(get_Video());
    
    if(Textures::get_anisotropic_filtering()) {
      if(Textures::get_anisotropic_filtering() < 0 || Textures::get_anisotropic_filtering() > vr.get_maximum_anisotropy())
        Textures::set_texturing_mode(vr.get_maximum_anisotropy(), Textures::get_bilinear_filtering(), Textures::get_mipmapping());

      vr.get_d3d_device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
      vr.get_d3d_device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
      vr.get_d3d_device()->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, DWORD(Textures::get_anisotropic_filtering()));
    }
    else if(Textures::get_bilinear_filtering()) {
      vr.get_d3d_device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
      vr.get_d3d_device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    else {
      vr.get_d3d_device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
      vr.get_d3d_device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    }

    vr.get_d3d_device()->SetSamplerState(0, D3DSAMP_MIPFILTER, (Textures::get_mipmapping() ? D3DTEXF_LINEAR : D3DTEXF_NONE));
  }

  IDirect3DTexture9 * Texture_DX9::build_from_surface(SDL_Surface *surface) {
    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(get_Video());

    IDirect3DTexture9 * ppTexture;

    const int mode = Texture::build_from_surface(surface,
      Point2i(std::min(ZENI_MAX_TEXTURE_WIDTH,  int(vdx.get_d3d_capabilities().MaxTextureWidth)),
              std::min(ZENI_MAX_TEXTURE_HEIGHT, int(vdx.get_d3d_capabilities().MaxTextureHeight))));
    const int stride = surface->format->BytesPerPixel;

    set_sampler_states();

    if(FAILED(D3DXCreateTexture(vdx.get_d3d_device(),
                                UINT(surface->w), UINT(surface->h),
                                D3DX_DEFAULT,
                                0,
                                mode > 0 ? D3DFMT_A8R8G8B8 : D3DFMT_X8R8G8B8,
                                D3DPOOL_MANAGED,
                                &ppTexture)))
      throw Texture_Init_Failure();

    D3DLOCKED_RECT rect;
    if(FAILED(ppTexture->LockRect(0, &rect, 0, D3DLOCK_DISCARD))) {
      ppTexture->Release();
      throw Texture_Init_Failure();
    }

    for(int i = 0; i < surface->h; ++i)
      for(unsigned char * dest = reinterpret_cast<unsigned char *>(rect.pBits) + i * rect.Pitch,
                        * src = reinterpret_cast<unsigned char *>(surface->pixels) + i * surface->pitch,
                        * src_end = src + surface->w * stride;
          src != src_end;
          dest += 4, src += stride)
      {
        Uint32 rgba = 0;
        rgba |= Uint32(src[0]);
        rgba |= Uint32(src[1]) << 8;
        rgba |= Uint32(src[2]) << 16;
        if(stride == 4)
          rgba |= Uint32(src[3]) << 24;

        dest[0] = static_cast<unsigned char>((rgba >> surface->format->Bshift) << surface->format->Bloss);
        dest[1] = static_cast<unsigned char>((rgba >> surface->format->Gshift) << surface->format->Gloss);
        dest[2] = static_cast<unsigned char>((rgba >> surface->format->Rshift) << surface->format->Rloss);
        dest[3] = static_cast<unsigned char>((rgba >> surface->format->Ashift) << surface->format->Aloss);
      }

    if(FAILED(ppTexture->UnlockRect(0))) {
      ppTexture->Release();
      throw Texture_Init_Failure();
    }

    if(FAILED(D3DXFilterTexture(ppTexture, 0, D3DX_DEFAULT, D3DX_DEFAULT))) {
      ppTexture->Release();
      throw Texture_Init_Failure();
    }

    SDL_FreeSurface(surface);

    return ppTexture;
  }
  
  void Texture_DX9::load(const std::string &filename) const {
    //Video_DX9 &vr = reinterpret_cast<Video_DX9 &>(get_Video());

    //set_sampler_states();

    //if(FAILED(D3DXCreateTextureFromFile(vr.get_d3d_device(), filename.c_str(), &m_texture)))
    //  throw Texture_Init_Failure();

    SDL_Surface *surface;
    
    {
      surface = IMG_Load(filename.c_str());
      if(!surface)
        throw Texture_Init_Failure();
    }
    
    try {
      const_cast<int &>(m_size.x) = surface->w;
      const_cast<int &>(m_size.y) = surface->h;
      m_texture = build_from_surface(surface);
    }
    catch(...) {
      SDL_FreeSurface(surface);
      throw;
    }
  }
#endif

}

#include <Zeni/Global_Undef.h>
