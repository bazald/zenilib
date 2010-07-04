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

#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif
#ifndef DISABLE_GL
#ifdef _MACOSX
#include <GLEW/glew.h>
#else
#include <GL/glew.h>
#endif
#endif

namespace Zeni {

  template <typename TYPE>
  class safe_ptr : public
#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 3)) && defined(__GXX_EXPERIMENTAL_CXX0X__)
    std::unique_ptr<TYPE>
#else
    std::auto_ptr<TYPE>
#endif
  {
  public:
    safe_ptr() {}
    safe_ptr(TYPE * const &ptr) :
#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 3)) && defined(__GXX_EXPERIMENTAL_CXX0X__)
      std::unique_ptr<TYPE>
#else
      std::auto_ptr<TYPE>
#endif
        (ptr)
    {
    }
  };

  class Render_Wrapper;
  class Vertex_Buffer;

  class Vertex_Buffer_Renderer {
    Vertex_Buffer_Renderer(const Vertex_Buffer_Renderer &);
    Vertex_Buffer_Renderer & operator=(const Vertex_Buffer_Renderer &);

  public:
    Vertex_Buffer_Renderer(Vertex_Buffer &vertex_buffer);
    virtual ~Vertex_Buffer_Renderer() {}

    virtual void render() = 0;

  protected:
    Vertex_Buffer &m_vbo;
  };

  class Vertex_Buffer {
    Vertex_Buffer(const Vertex_Buffer &);
    Vertex_Buffer & operator=(const Vertex_Buffer &);

    friend class Vertex_Buffer_Renderer_GL;
    friend class Vertex_Buffer_Renderer_DX9;

  public:
    struct Vertex_Buffer_Range {
      Vertex_Buffer_Range(Material * const &m, const size_t &s, const size_t &ne);

      safe_ptr<Material> material;
      size_t start;
      size_t num_elements;
    };

    Vertex_Buffer();
    ~Vertex_Buffer();

    inline void do_normal_alignment(const bool align_normals_ = true); // Set whether Vertex_Buffer should try to fix broken normals in the prerender phase;
    inline bool will_do_normal_alignment() const; // Find out whether Vertex_Buffer is set to try to fix broken normals in the prerender phase;

    void give_triangle(Triangle<Vertex2f_Color> * const &triangle); ///< Give the Vertex_Buffer a Triangle (which it will delete later)
    void fax_triangle(const Triangle<Vertex2f_Color> * const &triangle); ///< Give the Vertex_Buffer a copy of a Triangle
    void give_quadrilateral(Quadrilateral<Vertex2f_Color> * const &quadrilateral); ///< Give the Vertex_Buffer a Quadrilateral (which it will delete later)
    void fax_quadrilateral(const Quadrilateral<Vertex2f_Color> * const &quadrilateral); ///< Give the Vertex_Buffer a copy of a Quadrilateral

    void give_triangle(Triangle<Vertex2f_Texture> * const &triangle); ///< Give the Vertex_Buffer a Triangle (which it will delete later)
    void fax_triangle(const Triangle<Vertex2f_Texture> * const &triangle); ///< Give the Vertex_Buffer a copy of a Triangle
    void give_quadrilateral(Quadrilateral<Vertex2f_Texture> * const &quadrilateral); ///< Give the Vertex_Buffer a Quadrilateral (which it will delete later)
    void fax_quadrilateral(const Quadrilateral<Vertex2f_Texture> * const &quadrilateral); ///< Give the Vertex_Buffer a copy of a Quadrilateral

    void give_triangle(Triangle<Vertex3f_Color> * const &triangle); ///< Give the Vertex_Buffer a Triangle (which it will delete later)
    void fax_triangle(const Triangle<Vertex3f_Color> * const &triangle); ///< Give the Vertex_Buffer a copy of a Triangle
    void give_quadrilateral(Quadrilateral<Vertex3f_Color> * const &quadrilateral); ///< Give the Vertex_Buffer a Quadrilateral (which it will delete later)
    void fax_quadrilateral(const Quadrilateral<Vertex3f_Color> * const &quadrilateral); ///< Give the Vertex_Buffer a copy of a Quadrilateral

    void give_triangle(Triangle<Vertex3f_Texture> * const &triangle); ///< Give the Vertex_Buffer a Triangle (which it will delete later)
    void fax_triangle(const Triangle<Vertex3f_Texture> * const &triangle); ///< Give the Vertex_Buffer a copy of a Triangle
    void give_quadrilateral(Quadrilateral<Vertex3f_Texture> * const &quadrilateral); ///< Give the Vertex_Buffer a Quadrilateral (which it will delete later)
    void fax_quadrilateral(const Quadrilateral<Vertex3f_Texture> * const &quadrilateral); ///< Give the Vertex_Buffer a copy of a Quadrilateral

    void debug_render(); ///< Render all Triangles in the Vertex_Buffer individually; Will fail if prerender has been called

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

    std::vector<Triangle<Vertex3f_Color> *> m_triangles_cm;
    std::vector<Triangle<Vertex3f_Texture> *> m_triangles_t;

    std::vector<Vertex_Buffer_Range *> m_descriptors_cm;
    std::vector<Vertex_Buffer_Range *> m_descriptors_t;

    bool m_align_normals;

    Vertex_Buffer_Renderer * m_renderer;
    bool m_prerendered;

  public:
    static void lose_all(); /// Lose all Vertex_Buffer objects, presumably when losing resources in Textures and Fonts

  private:
    static std::set<Vertex_Buffer *> & get_vbos();
  };

#ifndef DISABLE_GL

  class Vertex_Buffer_Renderer_GL : public Vertex_Buffer_Renderer {
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

    union VBO_GL {
      GLuint vbo;
      unsigned char * alt;
    } m_vbuf[6];
    
    static PFNGLDELETEBUFFERSARBPROC m_pglDeleteBuffersARB;
  };

#endif
#ifndef DISABLE_DX9

  class Vertex_Buffer_Renderer_DX9 : public Vertex_Buffer_Renderer {
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
    struct VBO_DX9 {
      bool is_vbo;

      union VBO_DX9_impl {
        IDirect3DVertexBuffer9 * vbo;
        char * alt;
      } data;
    } m_buf_c, m_buf_t;
  };

#endif

  struct VBuf_Init_Failure : public Error {
    VBuf_Init_Failure() : Error("Zeni Vertex Buffer Failed to Initialize Correctly") {}
  };

  struct VBuf_Render_Failure : public Error {
    VBuf_Render_Failure() : Error("Zeni Vertex Buffer Failed to Render") {}
  };

}

#endif
