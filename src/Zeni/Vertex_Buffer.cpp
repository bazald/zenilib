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
    : m_begin_end(0)
  {
  }

  void Vertex_Buffer::render_begin() {
    ++m_begin_end;
  }

  void Vertex_Buffer::render_end() {
    --m_begin_end;
  }

  template <typename VERTEX>
  static void clear_buffers(vector<Triangle<VERTEX> *> &triangles_no_mat,
    vector<Triangle<VERTEX> *> &triangles_mat,
    vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors_no_mat,
    vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors_mat) {
      for(unsigned int i = 0; i < triangles_no_mat.size(); ++i)
        delete triangles_no_mat[i];
      for(unsigned int i = 0; i < triangles_mat.size(); ++i)
        delete triangles_mat[i];
      for(unsigned int i = 0; i < descriptors_no_mat.size(); ++i)
        delete descriptors_no_mat[i];
      for(unsigned int i = 0; i < descriptors_mat.size(); ++i)
        delete descriptors_mat[i];
  }

  Vertex_Buffer_3FC::~Vertex_Buffer_3FC() {
    clear_buffers(m_triangles_no_mat, m_triangles_mat, m_descriptors_no_mat, m_descriptors_mat);
  }

  Vertex_Buffer_3FT::~Vertex_Buffer_3FT() {
    clear_buffers(m_triangles_no_mat, m_triangles_mat, m_descriptors_no_mat, m_descriptors_mat);
  }

  void Vertex_Buffer_3FC::add_triangle(Renderable *triangle) {
    Triangle<Vertex3f_Color> *p_tri = dynamic_cast<Triangle<Vertex3f_Color> *>(triangle);

    if(!p_tri)
      throw VBuf_Init_Failure();

    if(dynamic_cast<const Material_Render_Wrapper * const>(p_tri->get_render_wrapper()))
      push_mat(p_tri);
    else
      push_no_mat(p_tri);
  }

  void Vertex_Buffer_3FT::add_triangle(Renderable *triangle) {
    Triangle<Vertex3f_Texture> *p_tri = dynamic_cast<Triangle<Vertex3f_Texture> *>(triangle);

    if(!p_tri)
      throw VBuf_Init_Failure();

    if(const Multiple_Render_Wrapper * const multirw = dynamic_cast<const Multiple_Render_Wrapper * const>(p_tri->get_render_wrapper()))
      push_mat(p_tri, multirw);
    else if(dynamic_cast<const Texture_Render_Wrapper * const>(p_tri->get_render_wrapper()))
      push_no_mat(p_tri);
    else {
      delete triangle;
      throw VBuf_Init_Failure();
    }
  }

  void Vertex_Buffer_3FC::add_Quadrilateral(Renderable *quad) {
    std::auto_ptr<Renderable> to_delete(quad);
    const Quadrilateral<Vertex3f_Color> * const p_rect = dynamic_cast<Quadrilateral<Vertex3f_Color> *>(quad);

    if(!p_rect)
      throw VBuf_Init_Failure();

    add_triangle(p_rect->get_duplicate_t0());
    add_triangle(p_rect->get_duplicate_t1());
  }

  void Vertex_Buffer_3FT::add_Quadrilateral(Renderable *quad) {
    std::auto_ptr<Renderable> to_delete(quad);
    const Quadrilateral<Vertex3f_Texture> * const p_rect = dynamic_cast<Quadrilateral<Vertex3f_Texture> *>(quad);

    if(!p_rect)
      throw VBuf_Init_Failure();

    add_triangle(p_rect->get_duplicate_t0());
    add_triangle(p_rect->get_duplicate_t1());
  }

  void Vertex_Buffer_3FC::debug_render() {
    for(unsigned int i = 0; i < m_triangles_no_mat.size(); ++i)
      Video::get_reference().render(*m_triangles_no_mat[i]);
    for(unsigned int i = 0; i < m_triangles_mat.size(); ++i)
      Video::get_reference().render(*m_triangles_mat[i]);
  }

  void Vertex_Buffer_3FT::debug_render() {
    for(unsigned int i = 0; i < m_triangles_no_mat.size(); ++i)
      Video::get_reference().render(*m_triangles_no_mat[i]);
    for(unsigned int i = 0; i < m_triangles_mat.size(); ++i)
      Video::get_reference().render(*m_triangles_mat[i]);
  }

  template <typename VERTEX>
  struct SORTER {
    bool operator()(const Triangle<VERTEX> * const lhs, const Triangle<VERTEX> * const rhs) const {
      return *lhs->get_render_wrapper() < *rhs->get_render_wrapper();
    }
  };

  void Vertex_Buffer_3FC::sort_triangles() {
    std::sort(m_triangles_no_mat.begin(), m_triangles_no_mat.end(), SORTER<Vertex3f_Color>());
    std::sort(m_triangles_mat.begin(), m_triangles_mat.end(), SORTER<Vertex3f_Color>());
  }

  void Vertex_Buffer_3FT::sort_triangles() {
    std::sort(m_triangles_no_mat.begin(), m_triangles_no_mat.end(), SORTER<Vertex3f_Texture>());
    std::sort(m_triangles_mat.begin(), m_triangles_mat.end(), SORTER<Vertex3f_Texture>());
  }

  template <typename VERTEX>
  static void set_descriptors(vector<Triangle<VERTEX> *> &triangles_no_mat,
    vector<Triangle<VERTEX> *> &triangles_mat,
    vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors_no_mat,
    vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors_mat) {
      int last = 0;
      if(!triangles_no_mat.empty()) {
        descriptors_no_mat.push_back(new Vertex_Buffer::Vertex_Buffer_Range(triangles_no_mat[0]->get_render_wrapper()->get_duplicate(), 0, 1));
        for(unsigned int i = 1; i < triangles_no_mat.size(); ++i)
          if(*descriptors_no_mat[last]->render_wrapper == *triangles_no_mat[i]->get_render_wrapper())
            ++descriptors_no_mat[last]->num_elements;
          else {
            descriptors_no_mat.push_back(new Vertex_Buffer::Vertex_Buffer_Range(triangles_no_mat[i]->get_render_wrapper()->get_duplicate(), i, 1));
            ++last;
          }
      }

      last = 0;
      if(!triangles_mat.empty()) {
        descriptors_mat.push_back(new Vertex_Buffer::Vertex_Buffer_Range(triangles_mat[0]->get_render_wrapper()->get_duplicate(), int(triangles_no_mat.size()), 1));
        for(unsigned int i = 1; i < triangles_mat.size(); ++i)
          if(*descriptors_mat[last]->render_wrapper == *triangles_mat[i]->get_render_wrapper())
            ++descriptors_mat[last]->num_elements;
          else {
            descriptors_mat.push_back(new Vertex_Buffer::Vertex_Buffer_Range(triangles_mat[i]->get_render_wrapper()->get_duplicate(), int(triangles_no_mat.size())+i, 1));
            ++last;
          }
      }
  }

  void Vertex_Buffer_3FC::set_descriptors() {
    Zeni::set_descriptors(m_triangles_no_mat, m_triangles_mat, m_descriptors_no_mat, m_descriptors_mat);
  }

  void Vertex_Buffer_3FT::set_descriptors() {
    Zeni::set_descriptors(m_triangles_no_mat, m_triangles_mat, m_descriptors_no_mat, m_descriptors_mat);
  }

  template <typename VERTEX>
  static void clear_triangles(vector<Triangle<VERTEX> *> &triangles_no_mat,
    vector<Triangle<VERTEX> *> &triangles_mat) {
      for(unsigned int i = 0; i < triangles_no_mat.size(); ++i)
        delete triangles_no_mat[i];
      triangles_no_mat.clear();
      for(unsigned int i = 0; i < triangles_mat.size(); ++i)
        delete triangles_mat[i];
      triangles_mat.clear();
  }

  void Vertex_Buffer_3FC::push_no_mat(Triangle<Vertex3f_Color> *p_tri) {
    m_triangles_no_mat.push_back(p_tri);
  }

  void Vertex_Buffer_3FT::push_no_mat(Triangle<Vertex3f_Texture> *p_tri) {
    m_triangles_no_mat.push_back(p_tri);
  }

  void Vertex_Buffer_3FC::push_mat(Triangle<Vertex3f_Color> *p_tri) {
    m_triangles_mat.push_back(p_tri);
  }

  void Vertex_Buffer_3FT::push_mat(Triangle<Vertex3f_Texture> *p_tri, const Multiple_Render_Wrapper *const multirw) {
    const Texture_Render_Wrapper * const texrw = dynamic_cast<const Texture_Render_Wrapper * const>(&multirw->get_first());
    const Material_Render_Wrapper * const matrw = dynamic_cast<const Material_Render_Wrapper * const>(&multirw->get_second());

    if(!texrw || !matrw)
      throw VBuf_Init_Failure();

    m_triangles_mat.push_back(p_tri);
  }

#ifndef DISABLE_GL

  Vertex_Buffer_3FC_GL::Vertex_Buffer_3FC_GL() {
    for(int i = 0; i < 3; ++i)
      m_vbuf[i] = 0;
  }

  Vertex_Buffer_3FT_GL::Vertex_Buffer_3FT_GL() {
    for(int i = 0; i < 3; ++i)
      m_vbuf[i] = 0;
  }

  Vertex_Buffer_3FC_GL::~Vertex_Buffer_3FC_GL() {
    Video_GL &vgl = dynamic_cast<Video_GL &>(Video::get_reference());

    if(m_vbuf[0])
      vgl.pglDeleteBuffersARB(3, m_vbuf);
  }

  Vertex_Buffer_3FT_GL::~Vertex_Buffer_3FT_GL() {
    Video_GL &vgl = dynamic_cast<Video_GL &>(Video::get_reference());

    if(m_vbuf[0])
      vgl.pglDeleteBuffersARB(3, m_vbuf);
  }

  template <typename VERTEX>
  static void prerender(GLuint vbuf[],
    vector<Triangle<VERTEX> *> &triangles_no_mat,
    vector<Triangle<VERTEX> *> &triangles_mat,
    const int &vertex_size,
    const int &buffer_size,
    const int &normal_size,
    const int &normbuf_size,
    const int &special_size,
    const int &specialbuf_size) {
      if(vbuf[0])
        throw VBuf_Render_Failure();

      Video_GL &vgl = dynamic_cast<Video_GL &>(Video::get_reference());

      char *p_verts = reinterpret_cast<char *>(malloc(buffer_size));
      char *p_normals = reinterpret_cast<char *>(malloc(normbuf_size));
      char *p_specials = reinterpret_cast<char *>(malloc(specialbuf_size));

      char *buffered_verts = p_verts;
      char *buffered_normals = p_normals;
      char *buffered_specials = p_specials;

      for(unsigned int i = 0; i < triangles_no_mat.size(); ++i)
        for(int j = 0; j < 3; ++j) {
          memcpy(buffered_verts, triangles_no_mat[i]->get_vertex(j).get_address(), vertex_size);
          buffered_verts += vertex_size;

          memcpy(buffered_normals, reinterpret_cast<float *>(triangles_no_mat[i]->get_vertex(j).get_address())+3, normal_size);
          buffered_normals += normal_size;

          memcpy(buffered_specials, reinterpret_cast<float *>(triangles_no_mat[i]->get_vertex(j).get_address())+6, special_size);
          buffered_specials += special_size;
        }

        for(unsigned int i = 0; i < triangles_mat.size(); ++i)
          for(int j = 0; j < 3; ++j) {
            memcpy(buffered_verts, triangles_mat[i]->get_vertex(j).get_address(), vertex_size);
            buffered_verts += vertex_size;

            memcpy(buffered_normals, reinterpret_cast<float *>(triangles_mat[i]->get_vertex(j).get_address())+3, normal_size);
            buffered_normals += normal_size;

            memcpy(buffered_specials, reinterpret_cast<float *>(triangles_mat[i]->get_vertex(j).get_address())+6, special_size);
            buffered_specials += special_size;
          }

          vgl.pglGenBuffersARB(3, vbuf);

          vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf[0]);
          vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, buffer_size, p_verts, GL_STATIC_DRAW_ARB);
          vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf[1]);
          vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, normbuf_size, p_normals, GL_STATIC_DRAW_ARB);
          vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf[2]);
          vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, specialbuf_size, p_specials, GL_STATIC_DRAW_ARB);

          free(p_verts);
          free(p_normals);
          free(p_specials);

          clear_triangles(triangles_no_mat, triangles_mat);
  }

  void Vertex_Buffer_3FC_GL::prerender() {
    sort_triangles();
    set_descriptors();

    Zeni::prerender(m_vbuf, m_triangles_no_mat, m_triangles_mat, vertex_size(), buffer_size(), normal_size(), normbuf_size(), color_size(), colorbuf_size());
  }

  void Vertex_Buffer_3FT_GL::prerender() {
    sort_triangles();
    set_descriptors();

    Zeni::prerender(m_vbuf, m_triangles_no_mat, m_triangles_mat, vertex_size(), buffer_size(), normal_size(), normbuf_size(), texel_size(), texbuf_size());
  }

  static inline void render_begin(GLuint vbuf[]) {
    Video_GL &vgl = dynamic_cast<Video_GL &>(Video::get_reference());

    glEnableClientState(GL_VERTEX_ARRAY);
    vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf[0]);
      glVertexPointer(3, GL_FLOAT, 0, 0);

    glEnableClientState(GL_NORMAL_ARRAY);
    vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf[1]);
      glNormalPointer(GL_FLOAT, 0, 0);
  }

  void Vertex_Buffer_3FC_GL::render_begin() {
    if(!m_vbuf[0])
      prerender();

    Vertex_Buffer::render_begin();
    if(get_begin_end() != 1)
      return;

    Zeni::render_begin(m_vbuf);

    Video_GL &vgl = dynamic_cast<Video_GL &>(Video::get_reference());

    glEnableClientState(GL_COLOR_ARRAY);
    vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[2]);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, 0);
  }

  void Vertex_Buffer_3FT_GL::render_begin() {
    if(!m_vbuf[0])
      prerender();

    Vertex_Buffer::render_begin();
    if(get_begin_end() != 1)
      return;

    Zeni::render_begin(m_vbuf);

    Video_GL &vgl = dynamic_cast<Video_GL &>(Video::get_reference());

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[2]);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);
  }

  static void render(vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors_no_mat,
    vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors_mat) {
      for(unsigned int i = 0; i < descriptors_no_mat.size(); ++i) {
        descriptors_no_mat[i]->render_wrapper->prerender();
        glDrawArrays(GL_TRIANGLES, 3*descriptors_no_mat[i]->start, 3*descriptors_no_mat[i]->num_elements);
        descriptors_no_mat[i]->render_wrapper->postrender();
      }

      for(unsigned int i = 0; i < descriptors_mat.size(); ++i) {
        descriptors_mat[i]->render_wrapper->prerender();
        glDrawArrays(GL_TRIANGLES, 3*descriptors_mat[i]->start, 3*descriptors_mat[i]->num_elements);
        descriptors_mat[i]->render_wrapper->postrender();
      }
  }

  void Vertex_Buffer_3FC_GL::render() {
    render_begin();

    Zeni::render(m_descriptors_no_mat, m_descriptors_mat);

    render_end();
  }

  void Vertex_Buffer_3FT_GL::render() {
    render_begin();

    Zeni::render(m_descriptors_no_mat, m_descriptors_mat);

    render_end();
  }

  void Vertex_Buffer_3FC_GL::render_end() {
    Vertex_Buffer::render_end();
    if(get_begin_end())
      return;

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
  }

  void Vertex_Buffer_3FT_GL::render_end() {
    Vertex_Buffer::render_end();
    if(get_begin_end())
      return;

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }

#endif
#ifndef DISABLE_DX9

  Vertex_Buffer_3FC_DX9::Vertex_Buffer_3FC_DX9()
    : m_vbuf(0)
  {
  }

  Vertex_Buffer_3FT_DX9::Vertex_Buffer_3FT_DX9()
    : m_vbuf(0)
  {
  }

  Vertex_Buffer_3FC_DX9::~Vertex_Buffer_3FC_DX9() {
    if(m_vbuf)
      m_vbuf->Release();
  }

  Vertex_Buffer_3FT_DX9::~Vertex_Buffer_3FT_DX9() {
    if(m_vbuf)
      m_vbuf->Release();
  }

  template <typename VERTEX>
  static void prerender(IDirect3DVertexBuffer9 *&vbuf,
    vector<Triangle<VERTEX> *> &triangles_no_mat,
    vector<Triangle<VERTEX> *> &triangles_mat,
    const int &buffer_size,
    const int &vertex_size,
    const DWORD &special) {
      if(vbuf)
        throw VBuf_Render_Failure();

      Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(Video::get_reference());

      if(FAILED
        (vdx.get_d3d_device()->CreateVertexBuffer
        (buffer_size,
        D3DUSAGE_WRITEONLY,
        D3DFVF_XYZ | D3DFVF_NORMAL | special,
        D3DPOOL_MANAGED,
        &vbuf, NULL))) {
          throw VBuf_Render_Failure();
      }

      char *buffered = 0;
      if(FAILED(vbuf->Lock(0, 0, reinterpret_cast<void **>(&buffered), 0)))
        throw VBuf_Render_Failure();

      for(unsigned int i = 0; i < triangles_no_mat.size(); ++i)
        for(unsigned int j = 0; j < 3; ++j) {
          memcpy(buffered, triangles_no_mat[i]->get_vertex(j).get_address(), vertex_size);
          buffered += vertex_size;
        }

      for(unsigned int i = 0; i < triangles_mat.size(); ++i)
        for(unsigned int j = 0; j < 3; ++j) {
          memcpy(buffered, triangles_mat[i]->get_vertex(j).get_address(), vertex_size);
          buffered += vertex_size;
        }

      vbuf->Unlock();

      clear_triangles(triangles_no_mat, triangles_mat);
  }

  void Vertex_Buffer_3FC_DX9::prerender() {
    sort_triangles();
    set_descriptors();

    Zeni::prerender(m_vbuf, m_triangles_no_mat, m_triangles_mat, buffer_size(), vertex_size(), D3DFVF_DIFFUSE);
  }

  void Vertex_Buffer_3FT_DX9::prerender() {
    sort_triangles();
    set_descriptors();

    Zeni::prerender(m_vbuf, m_triangles_no_mat, m_triangles_mat, buffer_size(), vertex_size(), D3DFVF_TEX1);
  }

  void Vertex_Buffer_3FC_DX9::render_begin() {
    if(!m_vbuf)
      prerender();

    Vertex_Buffer::render_begin();
    if(get_begin_end() != 1)
      return;

    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(Video::get_reference());

    vdx.get_d3d_device()->SetStreamSource(0, m_vbuf, 0, vertex_size());
  }

  void Vertex_Buffer_3FT_DX9::render_begin() {
    if(!m_vbuf)
      prerender();

    Vertex_Buffer::render_begin();
    if(get_begin_end() != 1)
      return;

    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(Video::get_reference());

    vdx.get_d3d_device()->SetStreamSource(0, m_vbuf, 0, vertex_size());
  }

  static void render(vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors_no_mat,
    vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors_mat, 
    Video_DX9 &vdx) {
      for(unsigned int i = 0; i < descriptors_no_mat.size(); ++i) {
        descriptors_no_mat[i]->render_wrapper->prerender();
        vdx.get_d3d_device()->DrawPrimitive(D3DPT_TRIANGLELIST, 3*descriptors_no_mat[i]->start, descriptors_no_mat[i]->num_elements);
        descriptors_no_mat[i]->render_wrapper->postrender();
      }

      for(unsigned int i = 0; i < descriptors_mat.size(); ++i) {
        descriptors_mat[i]->render_wrapper->prerender();
        vdx.get_d3d_device()->DrawPrimitive(D3DPT_TRIANGLELIST, 3*descriptors_mat[i]->start, descriptors_mat[i]->num_elements);
        descriptors_mat[i]->render_wrapper->postrender();
      }
  }

  void Vertex_Buffer_3FC_DX9::render() {
    render_begin();

    Zeni::render(m_descriptors_no_mat, m_descriptors_mat, dynamic_cast<Video_DX9 &>(Video::get_reference()));

    render_end();
  }

  void Vertex_Buffer_3FT_DX9::render() {
    render_begin();

    Zeni::render(m_descriptors_no_mat, m_descriptors_mat, dynamic_cast<Video_DX9 &>(Video::get_reference()));

    render_end();
  }

  void Vertex_Buffer_3FC_DX9::render_end() {
    Vertex_Buffer::render_end();
  }

  void Vertex_Buffer_3FT_DX9::render_end() {
    Vertex_Buffer::render_end();
  }

#endif

}
