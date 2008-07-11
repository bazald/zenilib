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
 * \class Zeni::Vertex_Buffer
 *
 * \ingroup Zenilib
 *
 * \brief A Vertex_Buffer that accepts both Triangle<Vertex3f_Color>s and Triangle<Vertex3f_Texture>s.
 *
 * \note All Triangles are expected to have either the base Render_Wrapper or a Material_Render_Wrapper.
 *
 * \note Created with a call to Video::get_reference().create_Vertex_Buffer()
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

#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif
#ifndef DISABLE_GL
#include <GL/glext.h>
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
    virtual ~Vertex_Buffer();

    void add_triangle(Triangle<Vertex2f_Color> *triangle); ///< Add a Triangle to the Vertex_Buffer
    void add_triangle(Triangle<Vertex2f_Texture> *triangle); ///< Add a Triangle to the Vertex_Buffer
    void add_quadrilateral(Quadrilateral<Vertex2f_Color> *quadrilateral); ///< Add a Quadrilateral to the Vertex_Buffer
    void add_quadrilateral(Quadrilateral<Vertex2f_Texture> *quadrilateral); ///< Add a Quadrilateral to the Vertex_Buffer

    void add_triangle(Triangle<Vertex3f_Color> *triangle); ///< Add a Triangle to the Vertex_Buffer
    void add_triangle(Triangle<Vertex3f_Texture> *triangle); ///< Add a Triangle to the Vertex_Buffer
    void add_quadrilateral(Quadrilateral<Vertex3f_Color> *quadrilateral); ///< Add a Quadrilateral to the Vertex_Buffer
    void add_quadrilateral(Quadrilateral<Vertex3f_Texture> *quadrilateral); ///< Add a Quadrilateral to the Vertex_Buffer

    void debug_render(); ///< Render all Triangles in the Vertex_Buffer individually; Will fail if prerender has been called

    virtual void render() = 0; ///< Render the Vertex_Buffer

  protected:
    virtual void prerender() = 0; ///< Create the vertex buffer in the GPU/VPU

    inline int num_vertices_c() const;
    inline int num_vertices_cm() const;
    inline int num_vertices_t() const;

    // Sort buffers by Material
    void sort_triangles();

    // Generate lists of vertex ranges to be rendered more efficiently
    void set_descriptors();

    std::vector<Triangle<Vertex3f_Color> *> m_triangles_c;
    std::vector<Triangle<Vertex3f_Color> *> m_triangles_cm;
    std::vector<Triangle<Vertex3f_Texture> *> m_triangles_t;

    std::vector<Vertex_Buffer_Range *> m_descriptors_c;
    std::vector<Vertex_Buffer_Range *> m_descriptors_cm;
    std::vector<Vertex_Buffer_Range *> m_descriptors_t;
  };

#ifndef DISABLE_GL

  class Vertex_Buffer_GL : public Vertex_Buffer {
  public:
    Vertex_Buffer_GL();
    virtual ~Vertex_Buffer_GL();

    virtual void render();

  private:
    virtual void prerender();
    
    inline int vertex_size() const;
    inline int normal_size() const;
    inline int color_size() const;
    inline int texel_size() const;

    GLuint m_vbuf[6];
    
    static PFNGLDELETEBUFFERSARBPROC m_pglDeleteBuffersARB;
  };

#endif
#ifndef DISABLE_DX9

  class Vertex_Buffer_DX9 : public Vertex_Buffer {
  public:
    Vertex_Buffer_DX9();
    virtual ~Vertex_Buffer_DX9();

    virtual void render();

  private:
    virtual void prerender();
    
    inline int vertex_c_size() const;
    inline int vertex_t_size() const;

    IDirect3DVertexBuffer9 *m_buf_c, *m_buf_t;
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
#include <Zeni/Vertex_Buffer.hxx>
#endif

#endif
