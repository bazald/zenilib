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

/**
 * \class Zeni::Texture
 *
 * \ingroup Zenilib
 *
 * \brief An Abstraction of a Texture
 *
 * A Texture can be applied to polygons in any supported rendering engine.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_TEXTURE_H
#define ZENI_TEXTURE_H

#include <Zeni/Core.h>

#include <SDL/SDL_image.h>
#include <string>
#include <vector>

#ifndef DISABLE_GL
#include <GL/gl.h>
#endif
#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif

namespace Zeni {

  class Video;
  class Video_GL;
  class Video_DX9;

  class Texture {
  public:
    virtual ~Texture() {}

    virtual void apply_texture() const = 0; ///< Apply a Texture to upcoming polygons
  };

  class Sprite : public Texture {
  public:
    Sprite();
    Sprite(const Sprite &rhs);
    Sprite & operator=(const Sprite &rhs);

    void append_frame(const std::string &name); ///< Add a frame to the Sprite
    int find_frame(const std::string &name, const int &starting_point = 0) const; ///< Search for a frame
    void insert_frame(const std::string &name, const int &at_this_index); ///< Insert a frame
    void remove_frame(const int &frame_number); ///< Remove a frame

    int get_num_frames() const; ///< Get the number of frames
    int get_current_frame() const; ///< Get the currently selected frame number
    void set_current_frame(const int &frame_number); ///< Set this frame

    virtual void apply_texture() const; ///< Apply the current Texture frame to upcoming polygons

  private:
    mutable std::vector<std::pair<std::string, unsigned long> > m_frames;
    int m_frame;
  };

#ifndef DISABLE_GL
  class Texture_GL : public Texture {
  public:
    Texture_GL(const std::string &filename, const bool &repeat /* otherwise clamp */, Video_GL &video);
    Texture_GL(SDL_Surface *surface, const bool &repeat /* otherwise clamp */);
    virtual ~Texture_GL();

    virtual void apply_texture() const;

  private:
    void build_from_surface(SDL_Surface *surface, const bool &repeat);

    GLuint m_texture_id;
  };
#endif

#ifndef DISABLE_DX9
  class Texture_DX9 : public Texture {
  public:
    Texture_DX9(const std::string &filename, const bool &repeat /* otherwise clamp */, Video_DX9 &video);
    virtual ~Texture_DX9();

    virtual void apply_texture() const;

  private:
    IDirect3DTexture9 *m_texture;
  };
#endif

  struct Invalid_Anisotropy_Setting : public Error {
    Invalid_Anisotropy_Setting() : Error("Invalid Anisotropy Setting") {}
  };

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
