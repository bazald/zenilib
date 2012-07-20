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
#include <Zeni/String.h>

#include <vector>

#ifndef DISABLE_GL
#if defined(REQUIRE_GL_ES)
#include <GLES/gl.h>
#else
#include <GL/glew.h>
#endif
#endif

struct IDirect3DTexture9;
struct ID3DXRenderToSurface;

namespace Zeni {

  class Video;
  class Video_GL;
  class Video_DX9;

  class ZENI_GRAPHICS_DLL Texture {
    Texture(const Texture &);
    Texture & operator=(const Texture &);

  public:
    Texture(const bool &repeat_) : m_repeat(repeat_) {}
    virtual ~Texture() {}

    virtual void apply_Texture() const = 0; ///< Apply a Texture to upcoming polygons

    virtual const Point2i & get_size() const = 0; ///< Get the resolution of the Texture on the GPU

  protected:
    const bool m_repeat;
  };

  class ZENI_GRAPHICS_DLL Sprite : public Texture {
  public:
    Sprite();
    Sprite(const Sprite &rhs);
    Sprite & operator=(const Sprite &rhs);

    void append_frame(const String &name); ///< Add a frame to the Sprite
    void append_frame(const String &name, const unsigned long &id); ///< Add a frame to the Sprite
    int find_frame(const String &name, const int &starting_point = 0) const; ///< Search for a frame
    void insert_frame(const String &name, const int &at_this_index); ///< Insert a frame
    void insert_frame(const String &name, const unsigned long &id, const int &at_this_index); ///< Insert a frame
    void remove_frame(const int &frame_number); ///< Remove a frame

    int get_num_frames() const; ///< Get the number of frames
    int get_current_frame() const; ///< Get the currently selected frame number
    void set_current_frame(const int &frame_number); ///< Set this frame

    virtual void apply_Texture() const; ///< Apply the current Texture frame to upcoming polygons

    inline const Point2i & get_size() const; ///< Get the resolution of the current Texture on the GPU

  private:
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    mutable std::vector<std::pair<String, unsigned long> > m_frames;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
    int m_frame;
  };

#ifndef DISABLE_GL
  class ZENI_GRAPHICS_DLL Texture_GL : public Texture {
    Texture_GL(const Texture_GL &);
    Texture_GL & operator=(const Texture_GL &);

    friend class Video_GL;

  public:
    Texture_GL(const String &filename, const bool &repeat /* otherwise clamp */,
               const bool &lazy_loading = false);
    Texture_GL(const Image &image);
    Texture_GL(const Point2i &size, const bool &repeat /* otherwise clamp */); ///< For render-to-texture
    virtual ~Texture_GL();

    virtual void apply_Texture() const;

    inline const Point2i & get_size() const;

  private:
    static GLuint build_from_Image(const Image &image);

    mutable Point2i m_size;
    mutable GLuint m_texture_id;
    GLuint m_render_buffer;
    GLuint m_frame_buffer_object;
    
    void load(const String &filename, const bool &repeat) const;
    
    const String m_filename;
  };
#endif

#ifndef TEMP_DISABLE
#ifndef DISABLE_DX9
  class ZENI_GRAPHICS_DLL Texture_DX9 : public Texture {
    Texture_DX9(const Texture_DX9 &);
    Texture_DX9 & operator=(const Texture_DX9 &);

    friend class Video_DX9;

  public:
    Texture_DX9(const String &filename, const bool &repeat /* otherwise clamp */);
    Texture_DX9(const Image &image);
    Texture_DX9(const Point2i &size, const bool &repeat /* otherwise clamp */); ///< For render-to-texture
    virtual ~Texture_DX9();

    virtual void apply_Texture() const;

    inline const Point2i & get_size() const;
    inline ID3DXRenderToSurface * render_to_surface() const;

  private:
    static void set_sampler_states(const bool &disable_mipmapping = false);
    static IDirect3DTexture9 * build_from_Image(const Image &image);

    mutable Point2i m_size;
    mutable IDirect3DTexture9 *m_texture;

    ID3DXRenderToSurface *m_render_to_surface;

    void load(const String &filename) const;
  };
#endif
#endif

  struct ZENI_GRAPHICS_DLL Texture_Init_Failure : public Error {
    Texture_Init_Failure() : Error("Zeni Texture Failed to Initialize Correctly") {}
  };

  struct ZENI_GRAPHICS_DLL Frame_Out_of_Range : public Error {
    Frame_Out_of_Range() : Error("Frame Choice is Out of Range") {}
  };

  struct ZENI_GRAPHICS_DLL Sprite_Containing_Sprite : public Error {
    Sprite_Containing_Sprite() : Error("Sprite Found Containing Another Sprite; This is Not Allowed") {}
  };

}

#endif
