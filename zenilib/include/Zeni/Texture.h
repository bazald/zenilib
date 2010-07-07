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
 * \class Zeni::Texture
 *
 * \ingroup zenilib
 *
 * \brief An Abstraction of a Texture
 *
 * A Texture can be applied to polygons in any supported rendering engine.
 *
 * \note Created with a call to get_Video().load_Texture(...)
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_TEXTURE_H
#define ZENI_TEXTURE_H

#include <Zeni/Coordinate.h>
#include <Zeni/Core.h>
#include <Zeni/IV.h>

#ifdef _MACOSX
#include <SDL_image/SDL_image.h>
#else
#include <SDL/SDL_image.h>
#endif

#include <string>
#include <vector>

#ifndef DISABLE_GL
#ifdef _MACOSX
#include <GLEW/glew.h>
#else
#include <GL/glew.h>
#endif
#endif

#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif

namespace Zeni {

  class Video;
  class Video_GL;
  class Video_DX9;

  class Texture_Base {
  public:
    enum VTYPE {
#ifndef DISABLE_GL
      VTYPE_GL = 1,
#endif
#ifndef DISABLE_DX9
      VTYPE_DX9 = 2,
#endif
      VTYPE_SPRITE = 3};

    typedef ::IV<Texture_Base, VTYPE> IV;
  };

  class Texture : public Texture_Base::IV {
    Texture(const Texture &);
    Texture & operator=(const Texture &);

  public:
    Texture(const Texture_Base::VTYPE &vtype_, const bool &repeat_) : Texture_Base::IV(vtype_), m_repeat(repeat_) {}
    virtual ~Texture() {}

    inline void apply_texture() const; ///< Apply a Texture to upcoming polygons

  protected:
    static void simplify_surface(SDL_Surface * &surface);
    static int build_from_surface(SDL_Surface * &surface, const Point2i &max_resolution, Point2i &built_size, const std::string * const &name = 0);

    const bool m_repeat;
  };

  class Sprite : public Texture {
  public:
    Sprite();
    Sprite(const Sprite &rhs);
    Sprite & operator=(const Sprite &rhs);

    void append_frame(const std::string &name); ///< Add a frame to the Sprite
    void append_frame(const std::string &name, const unsigned long &id); ///< Add a frame to the Sprite
    int find_frame(const std::string &name, const int &starting_point = 0) const; ///< Search for a frame
    void insert_frame(const std::string &name, const int &at_this_index); ///< Insert a frame
    void insert_frame(const std::string &name, const unsigned long &id, const int &at_this_index); ///< Insert a frame
    void remove_frame(const int &frame_number); ///< Remove a frame

    int get_num_frames() const; ///< Get the number of frames
    int get_current_frame() const; ///< Get the currently selected frame number
    void set_current_frame(const int &frame_number); ///< Set this frame

    inline void apply_texture_impl() const; ///< Apply the current Texture frame to upcoming polygons

  private:
    mutable std::vector<std::pair<std::string, unsigned long> > m_frames;
    int m_frame;
  };

#ifndef DISABLE_GL
  class Texture_GL : public Texture {
    Texture_GL(const Texture_GL &);
    Texture_GL & operator=(const Texture_GL &);

    friend class Video_GL;
    
  public:
    Texture_GL(const std::string &filename, const bool &repeat /* otherwise clamp */,
               const bool &lazy_loading = false);
    Texture_GL(SDL_Surface *surface, const bool &repeat /* otherwise clamp */);
    Texture_GL(const Point2i &size, const bool &repeat /* otherwise clamp */); ///< For render-to-texture
    virtual ~Texture_GL();

    inline void apply_texture_impl() const;

    inline const Point2i & get_size() const;

  private:
    static GLuint build_from_surface(SDL_Surface *surface, const bool &repeat, Point2i &built_size, const std::string * const &name = 0);

    Point2i m_size;
    mutable GLuint m_texture_id;
    GLuint m_render_buffer;
    GLuint m_frame_buffer_object;
    
    void load(const std::string &filename, const bool &repeat) const;
    
    const std::string m_filename;
  };
#endif

#ifndef DISABLE_DX9
  class Texture_DX9 : public Texture {
    Texture_DX9(const Texture_DX9 &);
    Texture_DX9 & operator=(const Texture_DX9 &);

    friend class Video_DX9;

  public:
    Texture_DX9(const std::string &filename, const bool &repeat /* otherwise clamp */);
    Texture_DX9(SDL_Surface *surface, const bool &repeat /* otherwise clamp */);
    Texture_DX9(const Point2i &size, const bool &repeat /* otherwise clamp */); ///< For render-to-texture
    virtual ~Texture_DX9();

    inline void apply_texture_impl() const;

    inline const Point2i & get_size() const;
    inline ID3DXRenderToSurface * render_to_surface() const;

  private:
    static void set_sampler_states();
    static IDirect3DTexture9 * build_from_surface(SDL_Surface *surface, Point2i &built_size, const std::string * const &name = 0);

    Point2i m_size;
    mutable IDirect3DTexture9 *m_texture;

    ID3DXRenderToSurface *m_render_to_surface;

    void load(const std::string &filename) const;
  };
#endif

  struct Texture_Init_Failure : public Error {
    Texture_Init_Failure() : Error("Zeni Texture Failed to Initialize Correctly") {}
  };

  struct Frame_Out_of_Range : public Error {
    Frame_Out_of_Range() : Error("Frame Choice is Out of Range") {}
  };

  struct Sprite_Containing_Sprite : public Error {
    Sprite_Containing_Sprite() : Error("Sprite Found Containing Another Sprite; This is Not Allowed") {}
  };

}

#endif
