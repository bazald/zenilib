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

#include <Zeni/Texture.h>

#include <Zeni/Textures.hxx>
#include <Zeni/Video.hxx>
#include <Zeni/Video_DX9.hxx>

#ifndef DISABLE_GL
#include <SDL/SDL_rotozoom.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif

namespace Zeni {

  Sprite::Sprite()
    : m_frame(0)
  {
  }

  void Sprite::append_frame(const std::string &name) {
    m_frames.push_back(make_pair(name, Textures::get_reference().get_texture_id(name)));
  }

  int Sprite::find_frame(const std::string &name, const int &starting_point) {
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

  int Sprite::get_num_frames() {
    return int(m_frames.size());
  }

  int Sprite::get_current_frame() {
    if(m_frames.empty())
      return -1;
    return m_frame;
  }

  void Sprite::set_current_frame(const int &frame_number) {
    if(frame_number < 0 || frame_number > int(m_frames.size()))
      throw Frame_Out_of_Range();

    m_frame = frame_number;
  }

  void Sprite::apply_texture() const {
    static bool no_recurse = true;

    if(no_recurse)
      no_recurse = false;
    else
      throw Sprite_Containing_Sprite();

    if(m_frames.empty() || m_frame < 0 || m_frame >= int(m_frames.size()))
      throw Frame_Out_of_Range();

    try {
      Textures::get_reference().apply_texture(m_frames[m_frame].second);
    }
    catch(Texture_Not_Found &) {
      try {
        m_frames[m_frame].second = Textures::get_reference().get_texture_id(m_frames[m_frame].first);
        Textures::get_reference().apply_texture(m_frames[m_frame].second);
      }
      catch(...) {
        no_recurse = true;
        throw;
      }
    }
    catch(...) {
      no_recurse = true;
      throw;
    }
    no_recurse = true;
  }

#ifndef DISABLE_GL
  Texture_GL::Texture_GL(const std::string &filename, Video_GL &)
    : m_texture_id(0)
  {
    SDL_Surface *surface = IMG_Load(filename.c_str());
    if(!surface)
      throw Texture_Init_Failure();

    build_from_surface(surface);
  }

  Texture_GL::Texture_GL(SDL_Surface *surface)
    : m_texture_id(0)
  {
    build_from_surface(surface);
  }

  Texture_GL::~Texture_GL() {
    glDeleteTextures(1, &m_texture_id);
  }

  void Texture_GL::apply_texture() const {
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }

  void Texture_GL::build_from_surface(SDL_Surface *surface) {
    bool width_pow2 = false, height_pow2 = false;

    for(int i = 1; i < 1024; i <<= 1) {
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

    GLint mode;
    switch(surface->format->BytesPerPixel) {
    case 3:  mode = GL_RGB;  break;
    case 4:  mode = GL_RGBA; break;
    default:
      throw Texture_Init_Failure();
    }

    // Allocate a unique id for the texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
      gluBuild2DMipmaps(GL_TEXTURE_2D, mode, surface->w, surface->h, mode, GL_UNSIGNED_BYTE, surface->pixels);
    else
      glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

    SDL_FreeSurface(surface);
  }
#endif

#ifndef DISABLE_DX9
  Texture_DX9::Texture_DX9(const std::string &filename, Video_DX9 &video) 
    : m_texture(0)
  {
    if(Textures::get_anisotropic_filtering()) {
      if(Textures::get_anisotropic_filtering() < 0 || Textures::get_anisotropic_filtering() > video.get_maximum_anisotropy())
        throw Invalid_Anisotropy_Setting();

      video.get_d3d_device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
      video.get_d3d_device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
      video.get_d3d_device()->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, int(Textures::get_anisotropic_filtering()));
    }
    else if(Textures::get_bilinear_filtering()) {
      video.get_d3d_device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
      video.get_d3d_device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    else {
      video.get_d3d_device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
      video.get_d3d_device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    }

    video.get_d3d_device()->SetSamplerState(0, D3DSAMP_MIPFILTER, (Textures::get_mipmapping() ? D3DTEXF_LINEAR : D3DTEXF_NONE));

    if(FAILED(D3DXCreateTextureFromFile(static_cast<Video_DX9 &>(Video::get_reference()).get_d3d_device(), filename.c_str(), &m_texture)))
      throw Texture_Init_Failure();
  }

  Texture_DX9::~Texture_DX9() {
    m_texture->Release();
  }

  void Texture_DX9::apply_texture() const {
    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(Video::get_reference());

    //Set the colour to come completely from the texture
    if(vdx.get_lighting())
      vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    else
      vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
    vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

    //Set the alpha to come completely from the texture
    if(vdx.get_lighting())
      vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    else
      vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
    vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
    vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

    vdx.get_d3d_device()->SetTexture(0, m_texture);
  }
#endif

}
