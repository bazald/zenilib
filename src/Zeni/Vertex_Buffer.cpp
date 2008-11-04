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

#include <Zeni/Vertex_Buffer.hxx>

#include <Zeni/Color.hxx>
#include <Zeni/Coordinate.hxx>
#include <Zeni/Material.hxx>
#include <Zeni/Quadrilateral.hxx>
#include <Zeni/Render_Wrapper.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Vertex2f.hxx>
#include <Zeni/Vertex3f.hxx>
#include <Zeni/Video_GL.hxx>

#include <algorithm>

//#define DISABLE_VBO

using namespace std;

namespace Zeni {

  Vertex_Buffer::Vertex_Buffer_Range::Vertex_Buffer_Range(Render_Wrapper *rw, const int &s, const int &ne)
    : render_wrapper(rw), 
    start(s), 
    num_elements(ne)
  {
  }

  Vertex_Buffer::Vertex_Buffer()
    : m_align_normals(false)
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

  void Vertex_Buffer::add_triangle(Triangle<Vertex2f_Color> *triangle, const bool &give_ownership) {
    if(!triangle)
      throw VBuf_Init_Failure();

    std::auto_ptr<Renderable> to_delete(give_ownership ? triangle : 0);

    const Vertex2f_Color &v0 = triangle->a;
    const Vertex2f_Color &v1 = triangle->b;
    const Vertex2f_Color &v2 = triangle->c;

    add_triangle(new Triangle<Vertex3f_Color>(
      Vertex3f_Color(v0.position, v0.get_color()),
      Vertex3f_Color(v1.position, v1.get_color()),
      Vertex3f_Color(v2.position, v2.get_color()),
      triangle->get_render_wrapper()->get_duplicate()));
  }

  void Vertex_Buffer::add_triangle(Triangle<Vertex2f_Texture> *triangle, const bool &give_ownership) {
    if(!triangle)
      throw VBuf_Init_Failure();

    std::auto_ptr<Renderable> to_delete(give_ownership ? triangle : 0);

    const Vertex2f_Texture &v0 = triangle->a;
    const Vertex2f_Texture &v1 = triangle->b;
    const Vertex2f_Texture &v2 = triangle->c;

    add_triangle(new Triangle<Vertex3f_Texture>(
      Vertex3f_Texture(v0.position, v0.texture_coordinate),
      Vertex3f_Texture(v1.position, v1.texture_coordinate),
      Vertex3f_Texture(v2.position, v2.texture_coordinate),
      triangle->get_render_wrapper()->get_duplicate()));
  }

  void Vertex_Buffer::add_quadrilateral(Quadrilateral<Vertex2f_Color> *quad, const bool &give_ownership) {
    if(!quad)
      throw VBuf_Init_Failure();

    std::auto_ptr<Renderable> to_delete(give_ownership ? quad : 0);

    const Vertex2f_Color &v0 = quad->a;
    const Vertex2f_Color &v1 = quad->b;
    const Vertex2f_Color &v2 = quad->c;
    const Vertex2f_Color &v3 = quad->d;

    add_quadrilateral(new Quadrilateral<Vertex3f_Color>(
      Vertex3f_Color(v0.position, v0.get_color()),
      Vertex3f_Color(v1.position, v1.get_color()),
      Vertex3f_Color(v2.position, v2.get_color()),
      Vertex3f_Color(v3.position, v3.get_color()),
      quad->get_render_wrapper()->get_duplicate()));
  }

  void Vertex_Buffer::add_quadrilateral(Quadrilateral<Vertex2f_Texture> *quad, const bool &give_ownership) {
    if(!quad)
      throw VBuf_Init_Failure();

    std::auto_ptr<Renderable> to_delete(give_ownership ? quad : 0);

    const Vertex2f_Texture &v0 = quad->a;
    const Vertex2f_Texture &v1 = quad->b;
    const Vertex2f_Texture &v2 = quad->c;
    const Vertex2f_Texture &v3 = quad->d;

    add_quadrilateral(new Quadrilateral<Vertex3f_Texture>(
      Vertex3f_Texture(v0.position, v0.texture_coordinate),
      Vertex3f_Texture(v1.position, v1.texture_coordinate),
      Vertex3f_Texture(v2.position, v2.texture_coordinate),
      Vertex3f_Texture(v3.position, v3.texture_coordinate),
      quad->get_render_wrapper()->get_duplicate()));
  }

  void Vertex_Buffer::add_triangle(Triangle<Vertex3f_Color> *triangle, const bool &give_ownership) {
    if(!triangle)
      throw VBuf_Init_Failure();

    const Material_Render_Wrapper *mrw = dynamic_cast<const Material_Render_Wrapper * const>(triangle->get_render_wrapper());

    if(mrw)
      if(mrw->get_material().get_texture().empty())
        m_triangles_cm.push_back(give_ownership ? triangle
                                                : triangle->get_duplicate());
      else
        throw VBuf_Init_Failure();
    else
      m_triangles_c.push_back(give_ownership ? triangle
                                             : triangle->get_duplicate());
  }

  void Vertex_Buffer::add_triangle(Triangle<Vertex3f_Texture> *triangle, const bool &give_ownership) {
    if(!triangle)
      throw VBuf_Init_Failure();

    const Material_Render_Wrapper *mrw = dynamic_cast<const Material_Render_Wrapper * const>(triangle->get_render_wrapper());

    if(mrw && !mrw->get_material().get_texture().empty())
      m_triangles_t.push_back(give_ownership ? triangle
                                             : triangle->get_duplicate());
    else
      throw VBuf_Init_Failure();
  }

  void Vertex_Buffer::add_quadrilateral(Quadrilateral<Vertex3f_Color> *quad, const bool &give_ownership) {
    std::auto_ptr<Renderable> to_delete(give_ownership ? quad : 0);

    if(!quad)
      throw VBuf_Init_Failure();

    Triangle<Vertex3f_Color> *tri0 = quad->get_duplicate_t0(),
                             *tri1 = quad->get_duplicate_t1();

    try {
      add_triangle(tri0);
      tri0 = 0;
      add_triangle(tri1);
    }
    catch(...) {
      delete tri0;
      delete tri1;
      throw;
    }
  }

  void Vertex_Buffer::add_quadrilateral(Quadrilateral<Vertex3f_Texture> *quad, const bool &give_ownership) {
    std::auto_ptr<Renderable> to_delete(give_ownership ? quad : 0);

    if(!quad)
      throw VBuf_Init_Failure();

    Triangle<Vertex3f_Texture> *tri0 = quad->get_duplicate_t0(),
                               *tri1 = quad->get_duplicate_t1();

    try {
      add_triangle(tri0);
      tri0 = 0;
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

  template <typename VERTEX, typename RENDER_WRAPPER = Render_Wrapper>
  struct SORTER {
    bool operator()(const Triangle<VERTEX> * const lhs, const Triangle<VERTEX> * const rhs) const {
      return dynamic_cast<const RENDER_WRAPPER &>(*lhs->get_render_wrapper()) < dynamic_cast<const RENDER_WRAPPER &>(*rhs->get_render_wrapper());
    }
  };

  void Vertex_Buffer::prerender() {
    sort_triangles();
    set_descriptors();
    if(m_align_normals)
      align_similar_normals();
  }

  void Vertex_Buffer::sort_triangles() {
    std::sort(m_triangles_c.begin(), m_triangles_c.end(), SORTER<Vertex3f_Color>());
    std::sort(m_triangles_cm.begin(), m_triangles_cm.end(), SORTER<Vertex3f_Color, Material_Render_Wrapper>());
    std::sort(m_triangles_t.begin(), m_triangles_t.end(), SORTER<Vertex3f_Texture, Material_Render_Wrapper>());
  }

  template <typename VERTEX, typename RENDER_WRAPPER = Render_Wrapper>
  struct DESCRIBER {
    bool operator()(vector<Triangle<VERTEX> *> &triangles,
                    vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors,
                    const int &triangles_done) const {
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
      return triangles.size() > 0;
    }
  };

  template<typename VERTEX>
  struct Vertex_Ref {
    Vertex_Ref() : t(0), which(-1) {}
    Vertex_Ref(Triangle<VERTEX> * const &t_, const int &which_) : t(t_), which(which_) {}

    Triangle<VERTEX> *t;
    int which;

    struct X_Sorter {
      bool operator()(const Vertex_Ref<VERTEX> &lhs, const Vertex_Ref<VERTEX> &rhs) const {
        return lhs.t->get_vertex(lhs.which).position.x <
               rhs.t->get_vertex(rhs.which).position.x;
      }
    };

    struct Y_Sorter {
      bool operator()(const Vertex_Ref<VERTEX> &lhs, const Vertex_Ref<VERTEX> &rhs) const {
        return lhs.t->get_vertex(lhs.which).position.y <
               rhs.t->get_vertex(rhs.which).position.y;
      }
    };

    struct Z_Sorter {
      bool operator()(const Vertex_Ref<VERTEX> &lhs, const Vertex_Ref<VERTEX> &rhs) const {
        return (*lhs.t)[lhs.which].position.z <
               (*rhs.t)[rhs.which].position.z;
      }
    };
  };

  template<typename VERTEX>
  inline void align_similar_normals(const VERTEX v0,
                                    Triangle<VERTEX> &t1,
                                    const int &which)
  {
    const float closeness_threshold = 0.00001f;
    const float alikeness_threshold = 0.95f;

    const VERTEX &v1 = t1[which];

    if((v0.position - v1.position).magnitude2() < closeness_threshold &&
       fabs(v0.normal * v1.normal) > alikeness_threshold)
    {
      VERTEX next(v1);
      next.normal = v0.normal;
      t1[which] = next;
    }
  }

  template<typename VERTEX>
  static void align_similar_normals(std::vector<Triangle<VERTEX> *> &triangles,
                                    std::vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors)
  {
    const float closeness_threshold = 0.001f;

    for(std::vector<Vertex_Buffer::Vertex_Buffer_Range *>::iterator it = descriptors.begin();
        it != descriptors.end();
        ++it)
    {
      vector< Vertex_Ref<VERTEX> > verts;

      verts.reserve(3 * (*it)->num_elements);
      for(int i = (*it)->start, iend = (*it)->start + (*it)->num_elements;
          i != iend;
          ++i)
      {
        verts.push_back(Vertex_Ref<VERTEX>(triangles[i], 0));
        verts.push_back(Vertex_Ref<VERTEX>(triangles[i], 1));
        verts.push_back(Vertex_Ref<VERTEX>(triangles[i], 2));
      }

      std::stable_sort(verts.begin(), verts.end(), Vertex_Ref<VERTEX>::Z_Sorter());

      vector< Vertex_Ref<VERTEX> >::iterator kend = verts.begin();

      for(vector< Vertex_Ref<VERTEX> >::iterator jt = verts.begin();
          jt != verts.end();
          ++jt)
      {
        for(; kend != verts.end(); ++kend)
        {
          if((*kend->t)[kend->which].position.z -
             (*jt->t)[jt->which].position.z > closeness_threshold)
          {
            break;
          }
        }

        for(vector< Vertex_Ref<VERTEX> >::iterator kt = jt + 1;
            kt != kend;
            ++kt)
        {
          align_similar_normals((*jt->t)[jt->which],
                                *kt->t,
                                kt->which);
        }
      }
    }
  }

  void Vertex_Buffer::align_similar_normals() {
    Zeni::align_similar_normals(m_triangles_c, m_descriptors_c);
    Zeni::align_similar_normals(m_triangles_cm, m_descriptors_cm);
    Zeni::align_similar_normals(m_triangles_t, m_descriptors_t);
  }

  void Vertex_Buffer::set_descriptors() {
    {
      int done = DESCRIBER<Vertex3f_Color>()(m_triangles_c, m_descriptors_c, 0);
      DESCRIBER<Vertex3f_Color, Material_Render_Wrapper>()(m_triangles_cm, m_descriptors_cm, done);
    }

    {
      DESCRIBER<Vertex3f_Texture, Material_Render_Wrapper>()(m_triangles_t, m_descriptors_t, 0);
    }
  }

#ifndef DISABLE_GL

  Vertex_Buffer_GL::Vertex_Buffer_GL() {
    memset(m_vbuf, 0, sizeof(VBO_GL) * 6);
  }

  Vertex_Buffer_GL::~Vertex_Buffer_GL() {
    if(m_pglDeleteBuffersARB) {
      for(int i = 0; i < 6; ++i)
        if(m_vbuf[i].vbo)
          m_pglDeleteBuffersARB(1, &m_vbuf[i].vbo);
    }
    else {
      for(int i = 0; i < 6; ++i)
        delete [] m_vbuf[i].alt;
    }
  }

  void Vertex_Buffer_GL::prerender() {
    if(m_vbuf[0].vbo || m_vbuf[0].alt ||
       m_vbuf[3].vbo || m_vbuf[3].alt)
      throw VBuf_Render_Failure();

    Vertex_Buffer::prerender();

    Video_GL &vgl = dynamic_cast<Video_GL &>(Video::get_reference());

#ifndef DISABLE_VBO
    if(!m_pglDeleteBuffersARB)
      m_pglDeleteBuffersARB = vgl.get_pglDeleteBuffersARB();
#endif

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
      unsigned char *p_verts = new unsigned char [vbuf_c_size];
      unsigned char *p_normals = new unsigned char [nbuf_c_size];
      unsigned char *p_colors = new unsigned char [cbuf_size];

      unsigned char *buffered_verts = p_verts;
      unsigned char *buffered_normals = p_normals;
      unsigned char *buffered_colors = p_colors;

      for(unsigned int i = 0; i < m_triangles_c.size(); ++i)
        for(int j = 0; j < 3; ++j) {
          memcpy(buffered_verts, (*m_triangles_c[i])[j].get_address(), v_size);
          buffered_verts += v_size;

          memcpy(buffered_normals, reinterpret_cast<float *>((*m_triangles_c[i])[j].get_address())+3, n_size);
          buffered_normals += n_size;

          memcpy(buffered_colors, reinterpret_cast<float *>((*m_triangles_c[i])[j].get_address())+6, c_size);
          swap(buffered_colors[0], buffered_colors[2]); /// HACK: Switch to BGRA order
          buffered_colors += c_size;
        }

      for(unsigned int i = 0; i < m_triangles_cm.size(); ++i)
        for(int j = 0; j < 3; ++j) {
          memcpy(buffered_verts, (*m_triangles_cm[i])[j].get_address(), v_size);
          buffered_verts += v_size;

          memcpy(buffered_normals, reinterpret_cast<float *>((*m_triangles_cm[i])[j].get_address())+3, n_size);
          buffered_normals += n_size;

          memcpy(buffered_colors, reinterpret_cast<float *>((*m_triangles_cm[i])[j].get_address())+6, c_size);
          swap(buffered_colors[0], buffered_colors[2]); /// HACK: Switch to BGRA order
          buffered_colors += c_size;
        }

      if(m_pglDeleteBuffersARB) {
        for(int i = 0; i < 3; ++i)
          vgl.pglGenBuffersARB(1, &m_vbuf[i].vbo);

        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[0].vbo);
        vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, vbuf_c_size, p_verts, GL_STATIC_DRAW_ARB);
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[1].vbo);
        vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, nbuf_c_size, p_normals, GL_STATIC_DRAW_ARB);
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[2].vbo);
        vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, cbuf_size, p_colors, GL_STATIC_DRAW_ARB);

        delete [] p_verts;
        delete [] p_normals;
        delete [] p_colors;
      }
      else {
        m_vbuf[0].alt = p_verts;
        m_vbuf[1].alt = p_normals;
        m_vbuf[2].alt = p_colors;
      }
    }

    if(vbuf_t_size) {
      unsigned char *p_verts = new unsigned char [vbuf_t_size];
      unsigned char *p_normals = new unsigned char [nbuf_t_size];
      unsigned char *p_texels = new unsigned char [tbuf_size];

      unsigned char *buffered_verts = p_verts;
      unsigned char *buffered_normals = p_normals;
      unsigned char *buffered_texels = p_texels;

      for(unsigned int i = 0; i < m_triangles_t.size(); ++i)
        for(int j = 0; j < 3; ++j) {
          memcpy(buffered_verts, (*m_triangles_t[i])[j].get_address(), v_size);
          buffered_verts += v_size;

          memcpy(buffered_normals, reinterpret_cast<float *>((*m_triangles_t[i])[j].get_address())+3, n_size);
          buffered_normals += n_size;

          memcpy(buffered_texels, reinterpret_cast<float *>((*m_triangles_t[i])[j].get_address())+6, t_size);
          buffered_texels += t_size;
        }

      if(m_pglDeleteBuffersARB) {
        for(int i = 3; i < 6; ++i)
          vgl.pglGenBuffersARB(1, &m_vbuf[i].vbo);

        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[3].vbo);
        vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, vbuf_t_size, p_verts, GL_STATIC_DRAW_ARB);
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[4].vbo);
        vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, nbuf_t_size, p_normals, GL_STATIC_DRAW_ARB);
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[5].vbo);
        vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, tbuf_size, p_texels, GL_STATIC_DRAW_ARB);

        delete [] p_verts;
        delete [] p_normals;
        delete [] p_texels;
      }
      else {
        m_vbuf[3].alt = p_verts;
        m_vbuf[4].alt = p_normals;
        m_vbuf[5].alt = p_texels;
      }
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
    if(!m_vbuf[0].vbo && !m_vbuf[0].alt &&
       !m_vbuf[3].vbo && !m_vbuf[3].alt)
      prerender();

    Video_GL &vgl = dynamic_cast<Video_GL &>(Video::get_reference());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    if(!m_descriptors_c.empty() || !m_descriptors_cm.empty()) {
      // Bind Vertex Buffer
      if(m_pglDeleteBuffersARB)
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[0].vbo);
      glVertexPointer(3, GL_FLOAT, 0, m_pglDeleteBuffersARB ? 0 : m_vbuf[0].alt);
      // Bind Normal Buffer
      if(m_pglDeleteBuffersARB)
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[1].vbo);
      glNormalPointer(GL_FLOAT, 0, m_pglDeleteBuffersARB ? 0 : m_vbuf[1].alt);
      // Bind Color Buffer
      glEnableClientState(GL_COLOR_ARRAY);
      if(m_pglDeleteBuffersARB)
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[2].vbo);
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, m_pglDeleteBuffersARB ? 0 : m_vbuf[2].alt);

      Zeni::render(m_descriptors_c);
      Zeni::render(m_descriptors_cm);

      glDisableClientState(GL_COLOR_ARRAY);
    }

    if(!m_descriptors_t.empty()) {
      // Bind Vertex Buffer
      if(m_pglDeleteBuffersARB)
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[3].vbo);
      glVertexPointer(3, GL_FLOAT, 0, m_pglDeleteBuffersARB ? 0 : m_vbuf[3].alt);
      // Bind Normal Buffer
      if(m_pglDeleteBuffersARB)
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[4].vbo);
      glNormalPointer(GL_FLOAT, 0, m_pglDeleteBuffersARB ? 0 : m_vbuf[4].alt);
      // Bind Texel Buffer
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      if(m_pglDeleteBuffersARB)
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[5].vbo);
      glTexCoordPointer(2, GL_FLOAT, 0, m_pglDeleteBuffersARB ? 0 : m_vbuf[5].alt);

      Zeni::render(m_descriptors_t);

      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
  }
  
  PFNGLDELETEBUFFERSARBPROC Vertex_Buffer_GL::m_pglDeleteBuffersARB = 0;

#endif
#ifndef DISABLE_DX9

  Vertex_Buffer_DX9::Vertex_Buffer_DX9() {
    memset(&m_buf_c, 0, sizeof(VBO_DX9));
    memset(&m_buf_t, 0, sizeof(VBO_DX9));
  }

  Vertex_Buffer_DX9::~Vertex_Buffer_DX9() {
    if(m_buf_c.is_vbo) {
      if(m_buf_c.data.vbo)
        m_buf_c.data.vbo->Release();
    }
    else
      delete [] m_buf_c.data.alt;

    if(m_buf_t.is_vbo) {
      if(m_buf_t.data.vbo)
        m_buf_t.data.vbo->Release();
    }
    else
      delete [] m_buf_t.data.alt;
  }

  void Vertex_Buffer_DX9::prerender() {
    if(m_buf_c.data.vbo || m_buf_c.data.alt ||
       m_buf_t.data.vbo || m_buf_t.data.alt)
      throw VBuf_Render_Failure();

    Vertex_Buffer::prerender();

    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(Video::get_reference());
    unsigned int vertex_size;
    char *buffered;

    if(!m_triangles_c.empty() || !m_triangles_cm.empty()) {
      const unsigned int buf_size = vertex_c_size() * (num_vertices_c() + num_vertices_cm());

#ifndef DISABLE_VBO
      if(FAILED
        (vdx.get_d3d_device()->CreateVertexBuffer(
        buf_size,
        D3DUSAGE_WRITEONLY,
        D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE,
        D3DPOOL_MANAGED,
        &m_buf_c.data.vbo, NULL)))
#endif
      {
        m_buf_c.is_vbo = false;
        m_buf_c.data.alt = new char [buf_size];
      }
#ifndef DISABLE_VBO
      else
        m_buf_c.is_vbo = true;
#endif

      vertex_size = vertex_c_size();
      buffered = 0;

      if(m_buf_c.is_vbo) {
        if(FAILED(m_buf_c.data.vbo->Lock(0, 0, reinterpret_cast<void **>(&buffered), 0))) {
          m_buf_c.data.vbo->Release();
          throw VBuf_Render_Failure();
        }
      }
      else
        buffered = m_buf_c.data.alt;

      for(unsigned int i = 0; i < m_triangles_c.size(); ++i)
        for(unsigned int j = 0; j < 3; ++j) {
          memcpy(buffered, (*m_triangles_c[i])[j].get_address(), vertex_size);
          buffered += vertex_size;
        }

      for(unsigned int i = 0; i < m_triangles_cm.size(); ++i)
        for(unsigned int j = 0; j < 3; ++j) {
          memcpy(buffered, (*m_triangles_cm[i])[j].get_address(), vertex_size);
          buffered += vertex_size;
        }

      if(m_buf_c.is_vbo)
        m_buf_c.data.vbo->Unlock();
    }

    if(!m_triangles_t.empty()) {
      const unsigned int buf_size = vertex_t_size() * num_vertices_t();

#ifndef DISABLE_VBO
      if(FAILED
        (vdx.get_d3d_device()->CreateVertexBuffer(
        buf_size,
        D3DUSAGE_WRITEONLY,
        D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
        D3DPOOL_MANAGED,
        &m_buf_t.data.vbo, NULL)))
#endif
      {
        m_buf_t.is_vbo = false;
        m_buf_t.data.alt = new char [buf_size];
      }
#ifndef DISABLE_VBO
      else
        m_buf_t.is_vbo = true;
#endif

      vertex_size = vertex_t_size();
      buffered = 0;

      if(m_buf_t.is_vbo) {
        if(FAILED(m_buf_t.data.vbo->Lock(0, 0, reinterpret_cast<void **>(&buffered), 0))) {
          m_buf_t.data.vbo->Release();
          throw VBuf_Render_Failure();
        }
      }
      else
        buffered = m_buf_t.data.alt;

      for(unsigned int i = 0; i < m_triangles_t.size(); ++i)
        for(unsigned int j = 0; j < 3; ++j) {
          memcpy(buffered, (*m_triangles_t[i])[j].get_address(), vertex_size);
          buffered += vertex_size;
        }

      if(m_buf_t.is_vbo)
        m_buf_t.data.vbo->Unlock();
    }

    clear_triangles(m_triangles_c);
    clear_triangles(m_triangles_cm);
    clear_triangles(m_triangles_t);
  }

  static void render(vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors,
    Vertex_Buffer_DX9::VBO_DX9 &vbo_dx9,
    const unsigned int &stride,
    Video_DX9 &vdx) {
      for(unsigned int i = 0; i < descriptors.size(); ++i) {
        descriptors[i]->render_wrapper->prerender();

        if(vbo_dx9.is_vbo)
          vdx.get_d3d_device()->DrawPrimitive(D3DPT_TRIANGLELIST,
                                              3 * descriptors[i]->start,
                                              descriptors[i]->num_elements);
        else
          vdx.get_d3d_device()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
                                                descriptors[i]->num_elements,
                                                vbo_dx9.data.alt + 3 * descriptors[i]->start,
                                                stride);

        descriptors[i]->render_wrapper->postrender();
      }
  }

  void Vertex_Buffer_DX9::render() {
    if(!m_buf_c.data.vbo && !m_buf_c.data.alt &&
       !m_buf_t.data.vbo && !m_buf_t.data.alt)
      prerender();

    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(Video::get_reference());

    bool flip_3d = !vdx.get_3d();

    if(flip_3d)
      vdx.set_3d(true);

    if(m_buf_c.data.vbo || m_buf_c.data.alt) {
      if(m_buf_c.is_vbo)
        vdx.get_d3d_device()->SetStreamSource(0, m_buf_c.data.vbo, 0, vertex_c_size());
      Zeni::render(m_descriptors_c, m_buf_c, vertex_c_size(), vdx);
      Zeni::render(m_descriptors_cm, m_buf_c, vertex_c_size(), vdx);
    }
    if(m_buf_t.data.vbo || m_buf_t.data.alt) {
      if(m_buf_t.is_vbo)
        vdx.get_d3d_device()->SetStreamSource(0, m_buf_t.data.vbo, 0, vertex_t_size());
      Zeni::render(m_descriptors_t, m_buf_t, vertex_t_size(), vdx);
    }

    if(flip_3d)
      vdx.set_3d(false);
  }

#endif

}
