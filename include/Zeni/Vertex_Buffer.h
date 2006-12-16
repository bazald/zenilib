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

/**
 * \class Zeni::Vertex_Buffer
 *
 * \ingroup Zenilib
 *
 * \brief A Base Class for all Vertex_Buffers
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Vertex_Buffer_3FC
 *
 * \ingroup Zenilib
 *
 * \brief A Vertex_Buffer that accepts Triangle<Vertex3f_Color>s.
 *
 * \note All Triangles are expected to have either the base Render_Wrapper or a Material_Render_Wrapper.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Vertex_Buffer_3FT
 *
 * \ingroup Zenilib
 *
 * \brief A Vertex_Buffer that accepts Triangle<Vertex3f_Texture>s.
 *
 * \note All Triangles are expected to have either a Texture_Render_Wrapper or a Multiple_Render_Wrapper containing a Texture_Render_Wrapper and a Material_Render_Wrapper, in that order.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_VERTEX_BUFFER_H
#define ZENI_VERTEX_BUFFER_H

#include "Triangle.h"
#include "Vertex3f.h"

#include <vector>

#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif

namespace Zeni {

  class Render_Wrapper;

  class Vertex_Buffer  {
  public:
    struct Vertex_Buffer_Range {
      Vertex_Buffer_Range(Render_Wrapper *rw, const int &s, const int &ne);

      std::auto_ptr<Render_Wrapper> render_wrapper;
      int start;
      int num_elements;
    };

    Vertex_Buffer();
    virtual ~Vertex_Buffer() {}

    virtual void add_triangle(Renderable *triangle) = 0; ///< Add a Triangle to the Vertex_Buffer
    virtual void add_quadrilateral(Renderable *quadrilateral) = 0; ///< Add a Quadrilateral to the Vertex_Buffer

    virtual void debug_render() = 0; ///< Render all Triangles in the Vertex_Buffer individually; Will fail if prerender has been called

    virtual void prerender() = 0; ///< Create the vertex buffer in the GPU/VPU
    virtual void render_begin(); ///< Start using the Vertex_Buffer; optional - use to increase performance when rendering multiple times
    virtual void render() = 0; ///< Render the Vertex_Buffer
    virtual void render_end(); ///< End use of the Vertex_Buffer; NOT optional if render_begin was called

  protected:
    inline int get_begin_end() const;

  private:
    int m_begin_end;
  };

  class Vertex_Buffer_3FC : public Vertex_Buffer {
  public:
    virtual ~Vertex_Buffer_3FC();

    virtual void add_triangle(Renderable *triangle); ///< Add a Triangle to the Vertex_Buffer
    virtual void add_quadrilateral(Renderable *quadrilateral); ///< Add a Quadrilateral to the Vertex_Buffer

    virtual void debug_render(); ///< Render all Triangles in the Vertex_Buffer individually; Will fail if prerender has been called

  protected:
    inline int num_triangles() const;
    inline int num_vertices() const;

    // Sort push_no_mat by color
    // Sort push_mat by color first, material (arbitrary order) second
    void sort_triangles();

    // Generate lists of vertex ranges to be rendered more efficiently
    void set_descriptors();

    std::vector<Triangle<Vertex3f_Color> *> m_triangles_no_mat;
    std::vector<Triangle<Vertex3f_Color> *> m_triangles_mat;

    std::vector<Vertex_Buffer_Range *> m_descriptors_no_mat;
    std::vector<Vertex_Buffer_Range *> m_descriptors_mat;

  private:
    void push_no_mat(Triangle<Vertex3f_Color> *p_tri);
    void push_mat(Triangle<Vertex3f_Color> *p_tri);
  };

  class Vertex_Buffer_3FT : public Vertex_Buffer {
  public:
    virtual ~Vertex_Buffer_3FT();

    virtual void add_triangle(Renderable *triangle); ///< Add a Triangle to the Vertex_Buffer
    virtual void add_quadrilateral(Renderable *quadrilateral); ///< Add a Quadrilateral to the Vertex_Buffer

    virtual void debug_render(); ///< Render all Triangles in the Vertex_Buffer individually; Will fail if prerender has been called

  protected:
    inline int num_triangles() const;
    inline int num_vertices() const;

    // Sort push_no_mat by texture name
    // Sort push_mat by texture name first, material (arbitrary order) second
    void sort_triangles();

    // Generate lists of vertex ranges to be rendered more efficiently
    void set_descriptors();

    std::vector<Triangle<Vertex3f_Texture> *> m_triangles_no_mat;
    std::vector<Triangle<Vertex3f_Texture> *> m_triangles_mat;

    std::vector<Vertex_Buffer_Range *> m_descriptors_no_mat;
    std::vector<Vertex_Buffer_Range *> m_descriptors_mat;

  private:
    void push_no_mat(Triangle<Vertex3f_Texture> *p_tri);
    void push_mat(Triangle<Vertex3f_Texture> *p_tri, const Multiple_Render_Wrapper * const multirw);
  };

#ifndef DISABLE_GL

  class Vertex_Buffer_3FC_GL : public Vertex_Buffer_3FC {
  public:
    Vertex_Buffer_3FC_GL();
    virtual ~Vertex_Buffer_3FC_GL();

    virtual void prerender();
    virtual void render_begin();
    virtual void render();
    virtual void render_end();

  private:
    inline int vertex_size() const;
    inline int buffer_size() const;

    inline int normal_size() const;
    inline int normbuf_size() const;

    inline int color_size() const;
    inline int colorbuf_size() const;

    GLuint m_vbuf[3];
  };

  class Vertex_Buffer_3FT_GL : public Vertex_Buffer_3FT {
  public:
    Vertex_Buffer_3FT_GL();
    virtual ~Vertex_Buffer_3FT_GL();

    virtual void prerender();
    virtual void render_begin();
    virtual void render();
    virtual void render_end();

  private:
    inline int vertex_size() const;
    inline int buffer_size() const;

    inline int normal_size() const;
    inline int normbuf_size() const;

    inline int texel_size() const;
    inline int texbuf_size() const;

    GLuint m_vbuf[3];
  };

#endif
#ifndef DISABLE_DX9

  class Vertex_Buffer_3FC_DX9 : public Vertex_Buffer_3FC {
  public:
    Vertex_Buffer_3FC_DX9();
    virtual ~Vertex_Buffer_3FC_DX9();

    virtual void prerender();
    virtual void render_begin();
    virtual void render();
    virtual void render_end();

  private:
    inline int vertex_size() const;
    inline int buffer_size() const;

    IDirect3DVertexBuffer9 *m_vbuf;
  };

  class Vertex_Buffer_3FT_DX9 : public Vertex_Buffer_3FT {
  public:
    Vertex_Buffer_3FT_DX9();
    virtual ~Vertex_Buffer_3FT_DX9();

    virtual void prerender();
    virtual void render_begin();
    virtual void render();
    virtual void render_end();

  private:
    inline int vertex_size() const;
    inline int buffer_size() const;

    IDirect3DVertexBuffer9 *m_vbuf;
  };

#endif

  struct VBuf_Init_Failure : public Error {
    VBuf_Init_Failure() : Error("Zeni Vertex Buffer Failed to Initialize Correctly") {}
  };

  struct VBuf_Render_Failure : public Error {
    VBuf_Render_Failure() : Error("Zeni Vertex Buffer Failed to Render") {}
  };

}

#ifdef ZENI_INLINES
#include "Vertex_Buffer.hxx"
#endif

#endif
