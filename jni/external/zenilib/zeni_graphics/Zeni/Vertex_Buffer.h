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
 * \class Zeni::Vertex_Buffer
 *
 * \ingroup zenilib
 *
 * \brief A Vertex_Buffer that accepts Triangle and Quadrilaterals.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_VERTEX_BUFFER_H
#define ZENI_VERTEX_BUFFER_H

#include <Zeni/Triangle.h>
#include <Zeni/Quadrilateral.h>
#include <Zeni/Vertex2f.h>
#include <Zeni/Vertex3f.h>

#include <vector>
#include <set>
#include <memory>

#ifndef DISABLE_GL
#if defined(REQUIRE_GL_ES)
#include <GLES/gl.h>
#else
#include <GL/glew.h>
#endif
#endif

struct IDirect3DVertexBuffer9;

namespace Zeni {

  class ZENI_GRAPHICS_DLL Render_Wrapper;
  class ZENI_GRAPHICS_DLL Vertex_Buffer;

  class ZENI_GRAPHICS_DLL Vertex_Buffer_Microrenderer {
  public:
    virtual ~Vertex_Buffer_Microrenderer() {}

    virtual void operator()() const = 0;
  };

  class ZENI_GRAPHICS_DLL Vertex_Buffer_Macrorenderer {
  public:
    virtual ~Vertex_Buffer_Macrorenderer() {}

    virtual void operator()(const Vertex_Buffer_Microrenderer &microrenderer) const;
  };

  class ZENI_GRAPHICS_DLL Vertex_Buffer_Renderer {
    Vertex_Buffer_Renderer(const Vertex_Buffer_Renderer &);
    Vertex_Buffer_Renderer & operator=(const Vertex_Buffer_Renderer &);

  public:
    Vertex_Buffer_Renderer(Vertex_Buffer &vertex_buffer);
    virtual ~Vertex_Buffer_Renderer() {}

    virtual void render() = 0;

  protected:
    Vertex_Buffer &m_vbo;
  };

  class ZENI_GRAPHICS_DLL Vertex_Buffer {
    Vertex_Buffer(const Vertex_Buffer &);
    Vertex_Buffer & operator=(const Vertex_Buffer &);

    friend class ZENI_GRAPHICS_DLL Vertex_Buffer_Renderer_GL;
    friend class ZENI_GRAPHICS_DLL Vertex_Buffer_Renderer_DX9;

  public:
    struct ZENI_GRAPHICS_DLL Vertex_Buffer_Range {
      Vertex_Buffer_Range(Material * const &m, const size_t &s, const size_t &ne);

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
      std::auto_ptr<Material> material;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
      size_t start;
      size_t num_elements;
    };

    Vertex_Buffer();
    ~Vertex_Buffer();

    inline void do_normal_alignment(const bool align_normals_ = true); // Set whether Vertex_Buffer should try to fix broken normals in the prerender phase;
    inline bool will_do_normal_alignment() const; // Find out whether Vertex_Buffer is set to try to fix broken normals in the prerender phase;

    void give_Triangle(Triangle<Vertex2f_Color> * const &triangle); ///< Give the Vertex_Buffer a Triangle (which it will delete later)
    void fax_Triangle(const Triangle<Vertex2f_Color> * const &triangle); ///< Give the Vertex_Buffer a copy of a Triangle
    void give_Quadrilateral(Quadrilateral<Vertex2f_Color> * const &quadrilateral); ///< Give the Vertex_Buffer a Quadrilateral (which it will delete later)
    void fax_Quadrilateral(const Quadrilateral<Vertex2f_Color> * const &quadrilateral); ///< Give the Vertex_Buffer a copy of a Quadrilateral

    void give_Triangle(Triangle<Vertex2f_Texture> * const &triangle); ///< Give the Vertex_Buffer a Triangle (which it will delete later)
    void fax_Triangle(const Triangle<Vertex2f_Texture> * const &triangle); ///< Give the Vertex_Buffer a copy of a Triangle
    void give_Quadrilateral(Quadrilateral<Vertex2f_Texture> * const &quadrilateral); ///< Give the Vertex_Buffer a Quadrilateral (which it will delete later)
    void fax_Quadrilateral(const Quadrilateral<Vertex2f_Texture> * const &quadrilateral); ///< Give the Vertex_Buffer a copy of a Quadrilateral

    void give_Triangle(Triangle<Vertex3f_Color> * const &triangle); ///< Give the Vertex_Buffer a Triangle (which it will delete later)
    void fax_Triangle(const Triangle<Vertex3f_Color> * const &triangle); ///< Give the Vertex_Buffer a copy of a Triangle
    void give_Quadrilateral(Quadrilateral<Vertex3f_Color> * const &quadrilateral); ///< Give the Vertex_Buffer a Quadrilateral (which it will delete later)
    void fax_Quadrilateral(const Quadrilateral<Vertex3f_Color> * const &quadrilateral); ///< Give the Vertex_Buffer a copy of a Quadrilateral

    void give_Triangle(Triangle<Vertex3f_Texture> * const &triangle); ///< Give the Vertex_Buffer a Triangle (which it will delete later)
    void fax_Triangle(const Triangle<Vertex3f_Texture> * const &triangle); ///< Give the Vertex_Buffer a copy of a Triangle
    void give_Quadrilateral(Quadrilateral<Vertex3f_Texture> * const &quadrilateral); ///< Give the Vertex_Buffer a Quadrilateral (which it will delete later)
    void fax_Quadrilateral(const Quadrilateral<Vertex3f_Texture> * const &quadrilateral); ///< Give the Vertex_Buffer a copy of a Quadrilateral

    void debug_render(); ///< Render all Triangles in the Vertex_Buffer individually; Will fail if prerender has been called
    void give_Macrorenderer(Vertex_Buffer_Macrorenderer * const &macrorenderer); ///< Wraps the final render call

    void render(); ///< Render the Vertex_Buffer
    void lose(); ///< Lose the Vertex_Buffer

  private:
    void prerender(); ///< Create the vertex buffer in the GPU/VPU

    inline size_t num_vertices_cm() const;
    inline size_t num_vertices_t() const;

    inline void unprerender(); ///< Allow prerender() to be called again

    // Sort buffers by Material
    void sort_triangles();

    // Generate lists of vertex ranges to be rendered more efficiently
    void set_descriptors();

    // Align normals of similar vertices
    void align_similar_normals();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::vector<Triangle<Vertex3f_Color> *> m_triangles_cm;
    std::vector<Triangle<Vertex3f_Texture> *> m_triangles_t;

    std::vector<Vertex_Buffer_Range *> m_descriptors_cm;
    std::vector<Vertex_Buffer_Range *> m_descriptors_t;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

    bool m_align_normals;

    Vertex_Buffer_Renderer * m_renderer;
    bool m_prerendered;

    Vertex_Buffer_Macrorenderer * m_macrorenderer;

  public:
    static void lose_all(); /// Lose all Vertex_Buffer objects, presumably when losing resources in Textures and Fonts

  private:
  private:
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    static class Uninit : public Event::Handler {
      void operator()() {
        Vertex_Buffer::lose_all();
      }

      Uninit * duplicate() const {
        return new Uninit;
      }

    public:
      Uninit() {}

    private:
      // Undefined
      Uninit(const Uninit &);
      Uninit operator=(const Uninit &);
    } g_uninit;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

    static std::set<Vertex_Buffer *> & get_vbos();
  };

#ifndef DISABLE_GL

  class ZENI_GRAPHICS_DLL Vertex_Buffer_Renderer_GL : public Vertex_Buffer_Renderer {
    Vertex_Buffer_Renderer_GL(const Vertex_Buffer_Renderer_GL &);
    Vertex_Buffer_Renderer_GL operator=(const Vertex_Buffer_Renderer_GL &);

  public:
    Vertex_Buffer_Renderer_GL(Vertex_Buffer &vertex_buffer);
    virtual ~Vertex_Buffer_Renderer_GL();

    virtual void render();

  private:
    inline size_t vertex_size() const;
    inline size_t normal_size() const;
    inline size_t color_size() const;
    inline size_t texel_size() const;
    inline bool buffers_supported(Video_GL &vgl) const;

    union ZENI_GRAPHICS_DLL VBO_GL {
      GLuint vbo;
      unsigned char * alt;
    } m_vbuf[6];
  };

#endif
#ifndef DISABLE_DX9

  class ZENI_GRAPHICS_DLL Vertex_Buffer_Renderer_DX9 : public Vertex_Buffer_Renderer {
    Vertex_Buffer_Renderer_DX9(const Vertex_Buffer_Renderer_DX9 &);
    Vertex_Buffer_Renderer_DX9 operator=(const Vertex_Buffer_Renderer_DX9 &);

  public:
    Vertex_Buffer_Renderer_DX9(Vertex_Buffer &vertex_buffer);
    virtual ~Vertex_Buffer_Renderer_DX9();

    virtual void render();

  private:
    inline size_t vertex_c_size() const;
    inline size_t vertex_t_size() const;

  public:
    struct ZENI_GRAPHICS_DLL VBO_DX9 {
      bool is_vbo;

      union ZENI_GRAPHICS_DLL VBO_DX9_impl {
        IDirect3DVertexBuffer9 * vbo;
        char * alt;
      } data;
    } m_buf_c, m_buf_t;
  };

#endif

  struct ZENI_GRAPHICS_DLL VBuf_Init_Failure : public Error {
    VBuf_Init_Failure() : Error("Zeni Vertex Buffer Failed to Initialize Correctly") {}
  };

  struct ZENI_GRAPHICS_DLL VBuf_Render_Failure : public Error {
    VBuf_Render_Failure() : Error("Zeni Vertex Buffer Failed to Render") {}
  };

}

#endif
