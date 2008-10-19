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
#include <SDL/SDL_rotozoom.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif

namespace Zeni {

  int Texture::build_from_surface(SDL_Surface * &surface) {
    bool width_pow2 = false, height_pow2 = false;

    for(int i = 1; i; i <<= 1) {
      if(surface->w == i)
        width_pow2 = true;
      if(surface->h == i)
        height_pow2 = true;
    }

    if(!width_pow2 || !height_pow2) {
      float next_w = pow(2.0f, ceil(log(float(surface->w))/log(2.0f)));
      float next_h = pow(2.0f, ceil(log(float(surface->h))/log(2.0f)));

      SDL_Surface *surf2 = rotozoomSurfaceXY(surface, 0,
        next_w/surface->w,
        next_h/surface->h, 1);

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
    m_frames.push_back(make_pair(name, Textures::get_reference().get_texture_id(name)));
  }

  int Sprite::find_frame(const std::string &name, const int &starting_point) const {
    if(starting_point < 0)
      throw Frame_Out_of_Range();

    for(int i = starting_point, end = int(m_frames.size()); i < end; ++i)
      if(m_frames[i].first == name)
        return i;
    return -1;
  }

  void Sprite::insert_frame(const std::string &name, const int &at_this_index) {
    std::pair<std::string, unsigned long> new_frame = make_pair(name, Textures::get_reference().get_texture_id(name));
    
    if(at_this_index < 0 || at_this_index > int(m_frames.size()))
      throw Frame_Out_of_Range();

    m_frames.push_back(new_frame);

    memmove(
      &m_frames + at_this_index + 1,
      &m_frames + at_this_index,
      (m_frames.size() - at_this_index - 1) * sizeof(std::pair<std::string, unsigned long>));

    m_frames[at_this_index] = new_frame;
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
      m_filename(filename)
  {
    if(!lazy_loading)
      load(m_filename, m_repeat);
  }

  Texture_GL::Texture_GL(SDL_Surface *surface, const bool &repeat)
    : Texture(Texture_Base::VTYPE_GL, repeat),
      m_texture_id(build_from_surface(surface, repeat))
  {
  }

  Texture_GL::~Texture_GL() {
    if(m_texture_id)
      glDeleteTextures(1, &m_texture_id);
  }

  GLuint Texture_GL::build_from_surface(SDL_Surface *surface, const bool &repeat) {
    GLuint texture_id = 0;

    const int mode = Texture::build_from_surface(surface);

    const GLint mode1 = mode > 0 ? GL_RGBA : GL_RGB;
    const GLint mode2 =
      mode == -'R' ? GL_RGB :
      mode == -'B' ? GL_BGR :
      mode == 'R' ? GL_RGBA :
      /*mode == 'A' ?*/ GL_BGRA;

    // Allocate a unique id for the texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Textures::get_bilinear_filtering() ? GL_LINEAR : GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      Textures::get_mipmapping() ?
      (Textures::get_bilinear_filtering() ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST) :
      (Textures::get_bilinear_filtering() ? GL_LINEAR : GL_NEAREST));

    if(Textures::get_anisotropic_filtering()) {
      if(Textures::get_anisotropic_filtering() < 0 || Textures::get_anisotropic_filtering() > Video::get_reference().get_maximum_anisotropy())
        throw Invalid_Anisotropy_Setting();

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
      m_texture(build_from_surface(surface))
  {
  }

  Texture_DX9::~Texture_DX9() {
    if(m_texture)
      m_texture->Release();
  }

  void Texture_DX9::set_sampler_states() {
    Video_DX9 &vr = reinterpret_cast<Video_DX9 &>(Video::get_reference());
    
    if(Textures::get_anisotropic_filtering()) {
      if(Textures::get_anisotropic_filtering() < 0 || Textures::get_anisotropic_filtering() > vr.get_maximum_anisotropy())
        throw Invalid_Anisotropy_Setting();

      vr.get_d3d_device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
      vr.get_d3d_device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
      vr.get_d3d_device()->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, int(Textures::get_anisotropic_filtering()));
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
    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(Video::get_reference());

    IDirect3DTexture9 * ppTexture;

    const int mode = Texture::build_from_surface(surface);
    const int stride = surface->format->BytesPerPixel;

    set_sampler_states();

    if(FAILED(D3DXCreateTexture(vdx.get_d3d_device(),
                                surface->w, surface->h,
                                D3DX_DEFAULT,
                                0,
                                mode > 0 ? D3DFMT_A8R8G8B8 : D3DFMT_X8R8G8B8,
                                D3DPOOL_MANAGED,
                                &ppTexture)))
      throw Texture_Init_Failure();

    D3DLOCKED_RECT rect;
    if(FAILED(ppTexture->LockRect(0, &rect, 0, 0))) {
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
    //Video_DX9 &vr = reinterpret_cast<Video_DX9 &>(Video::get_reference());

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
      m_texture = build_from_surface(surface);
    }
    catch(...) {
      SDL_FreeSurface(surface);
      throw;
    }
  }
#endif

}
