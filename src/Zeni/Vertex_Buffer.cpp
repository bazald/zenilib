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

#include <Zeni/Vertex_Buffer.hxx>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Material.hxx>
#include <Zeni/Quadrilateral.hxx>
#include <Zeni/Render_Wrapper.hxx>
#include <Zeni/Vertex3f.hxx>
#include <Zeni/Video_GL.hxx>

#include <algorithm>

using namespace std;

namespace Zeni {

  Vertex_Buffer::Vertex_Buffer_Range::Vertex_Buffer_Range(Render_Wrapper *rw, const int &s, const int &ne)
    : render_wrapper(rw), 
    start(s), 
    num_elements(ne)
  {
  }

  Vertex_Buffer::Vertex_Buffer()
  {
  }

  template <typename VERTEX>
  static void clear_triangles(vector<Triangle<VERTEX> *> &triangles) {
    for(unsigned int i = 0; i < triangles.size(); ++i)
      delete triangles[i];
    triangles.clear();
  }

  Vertex_Buffer::~Vertex_Buffer() {
    clear_triangles(m_triangles_c);
    clear_triangles(m_triangles_cm);
    clear_triangles(m_triangles_t);
  }

  void Vertex_Buffer::add_triangle(Triangle<Vertex3f_Color> *triangle) {
    if(!triangle)
      throw VBuf_Init_Failure();

    const Material_Render_Wrapper *mrw = dynamic_cast<const Material_Render_Wrapper * const>(triangle->get_render_wrapper());

    if(mrw)
      if(mrw->get_material().get_texture().empty())
        m_triangles_cm.push_back(triangle);
      else
        throw VBuf_Init_Failure();
    else
      m_triangles_c.push_back(triangle);
  }

  void Vertex_Buffer::add_triangle(Triangle<Vertex3f_Texture> *triangle) {
    if(!triangle)
      throw VBuf_Init_Failure();

    const Material_Render_Wrapper *mrw = dynamic_cast<const Material_Render_Wrapper * const>(triangle->get_render_wrapper());

    if(mrw && !mrw->get_material().get_texture().empty())
      m_triangles_t.push_back(triangle);
    else
      throw VBuf_Init_Failure();
  }

  void Vertex_Buffer::add_quadrilateral(Quadrilateral<Vertex3f_Color> *quad) {
    std::auto_ptr<Renderable> to_delete(quad);

    if(!quad)
      throw VBuf_Init_Failure();

    Triangle<Vertex3f_Color> *tri0 = quad->get_duplicate_t0(),
                             *tri1 = quad->get_duplicate_t1();

    try {
      add_triangle(tri0);
      add_triangle(tri1);
    }
    catch(...) {
      delete tri0;
      delete tri1;
      throw;
    }
  }

  void Vertex_Buffer::add_quadrilateral(Quadrilateral<Vertex3f_Texture> *quad) {
    std::auto_ptr<Renderable> to_delete(quad);

    if(!quad)
      throw VBuf_Init_Failure();

    Triangle<Vertex3f_Texture> *tri0 = quad->get_duplicate_t0(),
                               *tri1 = quad->get_duplicate_t1();

    try {
      add_triangle(tri0);
      add_triangle(tri1);
    }
    catch(...) {
      delete tri0;
      delete tri1;
      throw;
    }
  }

  void Vertex_Buffer::debug_render() {
    for(unsigned int i = 0; i < m_triangles_c.size(); ++i)
      Video::get_reference().render(*m_triangles_c[i]);
    for(unsigned int i = 0; i < m_triangles_cm.size(); ++i)
      Video::get_reference().render(*m_triangles_cm[i]);
    for(unsigned int i = 0; i < m_triangles_t.size(); ++i)
      Video::get_reference().render(*m_triangles_t[i]);
  }

  template <typename VERTEX>
  struct SORTER {
    bool operator()(const Triangle<VERTEX> * const lhs, const Triangle<VERTEX> * const rhs) const {
      return *lhs->get_render_wrapper() < *rhs->get_render_wrapper();
    }
  };

  void Vertex_Buffer::sort_triangles() {
    std::sort(m_triangles_c.begin(), m_triangles_c.end(), SORTER<Vertex3f_Color>());
    std::sort(m_triangles_cm.begin(), m_triangles_cm.end(), SORTER<Vertex3f_Color>());
    std::sort(m_triangles_t.begin(), m_triangles_t.end(), SORTER<Vertex3f_Texture>());
  }

  template <typename VERTEX, typename RENDER_WRAPPER>
  static int set_descriptors(vector<Triangle<VERTEX> *> &triangles,
                             vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors,
                             const RENDER_WRAPPER &,
                             const int &triangles_done) {
    int last = 0;
    if(!triangles.empty()) {
      descriptors.push_back(new Vertex_Buffer::Vertex_Buffer_Range(triangles[0]->get_render_wrapper()->get_duplicate(), triangles_done, 1));
      RENDER_WRAPPER *rw = dynamic_cast<RENDER_WRAPPER *>(descriptors[last]->render_wrapper.get());
      rw->clear_optimization();
      for(unsigned int i = 1; i < triangles.size(); ++i) {
        RENDER_WRAPPER *rw2 = const_cast<RENDER_WRAPPER *>(dynamic_cast<const RENDER_WRAPPER * const>(triangles[i]->get_render_wrapper()));

        if(*rw == *rw2)
          ++descriptors[last]->num_elements;
        else {
          descriptors.push_back(new Vertex_Buffer::Vertex_Buffer_Range(triangles[i]->get_render_wrapper()->get_duplicate(), triangles_done+i, 1));
          ++last;
          rw2 = dynamic_cast<RENDER_WRAPPER *>(descriptors[last]->render_wrapper.get());
          rw2->clear_optimization();
          rw2->optimize_to_follow(*rw);
          rw->optimize_to_precede(*rw2);
          rw = rw2;
        }
      }
    }
    return int(triangles.size());
  }

  void Vertex_Buffer::set_descriptors() {
    int done = 0;
    done += Zeni::set_descriptors(m_triangles_c, m_descriptors_c, Render_Wrapper(), done);
    done += Zeni::set_descriptors(m_triangles_cm, m_descriptors_cm, Material_Render_Wrapper(Material()), done);
    done += Zeni::set_descriptors(m_triangles_t, m_descriptors_t, Material_Render_Wrapper(Material()), done);
  }

#ifndef DISABLE_GL

  Vertex_Buffer_GL::Vertex_Buffer_GL() {
    for(int i = 0; i < 6; ++i)
      m_vbuf[i] = 0;
  }

  Vertex_Buffer_GL::~Vertex_Buffer_GL() {
    Video_GL &vgl = dynamic_cast<Video_GL &>(Video::get_reference());

    if(m_vbuf[0])
      vgl.pglDeleteBuffersARB(3, m_vbuf);
    if(m_vbuf[3])
      vgl.pglDeleteBuffersARB(3, m_vbuf+3);
  }

  void Vertex_Buffer_GL::prerender() {
    if(m_vbuf[0])
      throw VBuf_Render_Failure();

    sort_triangles();
    set_descriptors();

    Video_GL &vgl = dynamic_cast<Video_GL &>(Video::get_reference());

    const unsigned int v_size = vertex_size();
    const unsigned int n_size = normal_size();
    const unsigned int c_size = color_size();
    const unsigned int t_size = texel_size();

    const unsigned int vbuf_c_size = v_size * (num_vertices_c() + num_vertices_cm());
    const unsigned int nbuf_c_size = n_size * (num_vertices_c() + num_vertices_cm());
    const unsigned int cbuf_size = c_size * (num_vertices_c() + num_vertices_cm());
    const unsigned int vbuf_t_size = v_size * (num_vertices_t());
    const unsigned int nbuf_t_size = n_size * (num_vertices_t());
    const unsigned int tbuf_size = t_size * (num_vertices_t());
    
    if(vbuf_c_size) {
      char *p_verts = reinterpret_cast<char *>(malloc(vbuf_c_size));
      char *p_normals = reinterpret_cast<char *>(malloc(nbuf_c_size));
      char *p_colors = reinterpret_cast<char *>(malloc(cbuf_size));

      char *buffered_verts = p_verts;
      char *buffered_normals = p_normals;
      char *buffered_colors = p_colors;

      for(unsigned int i = 0; i < m_triangles_c.size(); ++i)
        for(int j = 0; j < 3; ++j) {
          memcpy(buffered_verts, m_triangles_c[i]->get_vertex(j).get_address(), v_size);
          buffered_verts += v_size;

          memcpy(buffered_normals, reinterpret_cast<float *>(m_triangles_c[i]->get_vertex(j).get_address())+3, n_size);
          buffered_normals += n_size;

          memcpy(buffered_colors, reinterpret_cast<float *>(m_triangles_c[i]->get_vertex(j).get_address())+6, c_size);
          buffered_colors += c_size;
        }

      for(unsigned int i = 0; i < m_triangles_cm.size(); ++i)
        for(int j = 0; j < 3; ++j) {
          memcpy(buffered_verts, m_triangles_cm[i]->get_vertex(j).get_address(), v_size);
          buffered_verts += v_size;

          memcpy(buffered_normals, reinterpret_cast<float *>(m_triangles_cm[i]->get_vertex(j).get_address())+3, n_size);
          buffered_normals += n_size;

          memcpy(buffered_colors, reinterpret_cast<float *>(m_triangles_cm[i]->get_vertex(j).get_address())+6, c_size);
          buffered_colors += c_size;
        }

      vgl.pglGenBuffersARB(3, m_vbuf);

      vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[0]);
      vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, vbuf_c_size, p_verts, GL_STATIC_DRAW_ARB);
      vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[1]);
      vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, nbuf_c_size, p_normals, GL_STATIC_DRAW_ARB);
      vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[2]);
      vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, cbuf_size, p_colors, GL_STATIC_DRAW_ARB);

      free(p_verts);
      free(p_normals);
      free(p_colors);
    }

    if(vbuf_t_size) {
      char *p_verts = reinterpret_cast<char *>(malloc(vbuf_t_size));
      char *p_normals = reinterpret_cast<char *>(malloc(nbuf_t_size));
      char *p_texels = reinterpret_cast<char *>(malloc(tbuf_size));

      char *buffered_verts = p_verts;
      char *buffered_normals = p_normals;
      char *buffered_texels = p_texels;

      for(unsigned int i = 0; i < m_triangles_t.size(); ++i)
        for(int j = 0; j < 3; ++j) {
          memcpy(buffered_verts, m_triangles_t[i]->get_vertex(j).get_address(), v_size);
          buffered_verts += v_size;

          memcpy(buffered_normals, reinterpret_cast<float *>(m_triangles_t[i]->get_vertex(j).get_address())+3, n_size);
          buffered_normals += n_size;

          memcpy(buffered_texels, reinterpret_cast<float *>(m_triangles_t[i]->get_vertex(j).get_address())+6, t_size);
          buffered_texels += t_size;
        }

      vgl.pglGenBuffersARB(3, m_vbuf+3);

      vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[3]);
      vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, vbuf_t_size, p_verts, GL_STATIC_DRAW_ARB);
      vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[4]);
      vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, nbuf_t_size, p_normals, GL_STATIC_DRAW_ARB);
      vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[5]);
      vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, tbuf_size, p_texels, GL_STATIC_DRAW_ARB);

      free(p_verts);
      free(p_normals);
      free(p_texels);
    }

    clear_triangles(m_triangles_c);
    clear_triangles(m_triangles_cm);
    clear_triangles(m_triangles_t);
  }

  static void render(vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors) {
    for(unsigned int i = 0; i < descriptors.size(); ++i) {
      descriptors[i]->render_wrapper->prerender();
      glDrawArrays(GL_TRIANGLES, 3*descriptors[i]->start, 3*descriptors[i]->num_elements);
      descriptors[i]->render_wrapper->postrender();
    }
  }

  void Vertex_Buffer_GL::render() {
    if(!m_vbuf[0] && !m_vbuf[3])
      prerender();

    Video_GL &vgl = dynamic_cast<Video_GL &>(Video::get_reference());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    if(!m_descriptors_c.empty() || !m_descriptors_cm.empty()) {
      // Bind Vertex Buffer
      vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[0]);
      glVertexPointer(3, GL_FLOAT, 0, 0);
      // Bind Normal Buffer
      vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[1]);
      glNormalPointer(GL_FLOAT, 0, 0);
      // Bind Color Buffer
      glEnableClientState(GL_COLOR_ARRAY);
      vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[2]);
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, 0);

      Zeni::render(m_descriptors_c);
      Zeni::render(m_descriptors_cm);

      glDisableClientState(GL_COLOR_ARRAY);
    }

    if(!m_descriptors_t.empty()) {
      // Bind Vertex Buffer
      vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[3]);
      glVertexPointer(3, GL_FLOAT, 0, 0);
      // Bind Normal Buffer
      vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[4]);
      glNormalPointer(GL_FLOAT, 0, 0);
      // Bind Texel Buffer
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[5]);
      glTexCoordPointer(2, GL_FLOAT, 0, 0);
      Zeni::render(m_descriptors_t);

      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
  }

#endif
#ifndef DISABLE_DX9

  Vertex_Buffer_DX9::Vertex_Buffer_DX9()
    : m_buf_c(0),
      m_buf_t(0)
  {
  }

  Vertex_Buffer_DX9::~Vertex_Buffer_DX9() {
    if(m_buf_c)
      m_buf_c->Release();
    if(m_buf_t)
      m_buf_t->Release();
  }

  void Vertex_Buffer_DX9::prerender() {
    sort_triangles();
    set_descriptors();

    if(m_buf_c || m_buf_t)
      throw VBuf_Render_Failure();

    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(Video::get_reference());
    unsigned int vertex_size;
    char *buffered;

    if(!m_triangles_c.empty() || !m_triangles_cm.empty()) {
      if(FAILED
        (vdx.get_d3d_device()->CreateVertexBuffer
        ((vertex_c_size() * (num_vertices_c() + num_vertices_cm())),
        D3DUSAGE_WRITEONLY,
        D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE,
        D3DPOOL_MANAGED,
        &m_buf_c, NULL))) {
          throw VBuf_Render_Failure();
      }

      vertex_size = vertex_c_size();
      buffered = 0;
      if(FAILED(m_buf_c->Lock(0, 0, reinterpret_cast<void **>(&buffered), 0)))
        throw VBuf_Render_Failure();

      for(unsigned int i = 0; i < m_triangles_c.size(); ++i)
        for(unsigned int j = 0; j < 3; ++j) {
          memcpy(buffered, m_triangles_c[i]->get_vertex(j).get_address(), vertex_size);
          buffered += vertex_size;
        }

      for(unsigned int i = 0; i < m_triangles_cm.size(); ++i)
        for(unsigned int j = 0; j < 3; ++j) {
          memcpy(buffered, m_triangles_cm[i]->get_vertex(j).get_address(), vertex_size);
          buffered += vertex_size;
        }

      m_buf_c->Unlock();
    }

    if(!m_triangles_t.empty()) {
      if(FAILED
        (vdx.get_d3d_device()->CreateVertexBuffer
        ((vertex_t_size() * num_vertices_t()),
        D3DUSAGE_WRITEONLY,
        D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
        D3DPOOL_MANAGED,
        &m_buf_t, NULL))) {
          throw VBuf_Render_Failure();
      }

      vertex_size = vertex_t_size();
      buffered = 0;
      if(FAILED(m_buf_t->Lock(0, 0, reinterpret_cast<void **>(&buffered), 0)))
        throw VBuf_Render_Failure();

      for(unsigned int i = 0; i < m_triangles_t.size(); ++i)
        for(unsigned int j = 0; j < 3; ++j) {
          memcpy(buffered, m_triangles_t[i]->get_vertex(j).get_address(), vertex_size);
          buffered += vertex_size;
        }

      m_buf_t->Unlock();
    }

    clear_triangles(m_triangles_c);
    clear_triangles(m_triangles_cm);
    clear_triangles(m_triangles_t);
  }

  static void render(vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors,
    Video_DX9 &vdx) {
      for(unsigned int i = 0; i < descriptors.size(); ++i) {
        descriptors[i]->render_wrapper->prerender();
        vdx.get_d3d_device()->DrawPrimitive(D3DPT_TRIANGLELIST, 3*descriptors[i]->start, descriptors[i]->num_elements);
        descriptors[i]->render_wrapper->postrender();
      }
  }

  void Vertex_Buffer_DX9::render() {
    if(!m_buf_c && !m_buf_t)
      prerender();

    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(Video::get_reference());

    if(m_buf_c) {
      vdx.get_d3d_device()->SetStreamSource(0, m_buf_c, 0, vertex_c_size());
      Zeni::render(m_descriptors_c, vdx);
      Zeni::render(m_descriptors_cm, vdx);
    }
    if(m_buf_t) {
      vdx.get_d3d_device()->SetStreamSource(0, m_buf_t, 0, vertex_t_size());
      Zeni::render(m_descriptors_t, vdx);
    }
  }

#endif

}
