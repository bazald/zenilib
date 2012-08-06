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

#include <zeni_graphics.h>

#include <algorithm>

#ifndef DISABLE_DX9
#include <d3dx9.h>
#endif

//#define DISABLE_VBO

#include <Zeni/Define.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace Zeni {

  void Vertex_Buffer_Macrorenderer::operator()(const Vertex_Buffer_Microrenderer &microrenderer) const {
    microrenderer();
  }

  Vertex_Buffer_Renderer::Vertex_Buffer_Renderer(Vertex_Buffer &vertex_buffer)
    : m_vbo(vertex_buffer)
  {
  }

  Vertex_Buffer::Vertex_Buffer_Range::Vertex_Buffer_Range(Material * const &m, const size_t &s, const size_t &ne)
    : material(m), 
    start(s), 
    num_elements(ne)
  {
  }

  Vertex_Buffer::Vertex_Buffer()
    : m_align_normals(false),
    m_renderer(0),
    m_prerendered(false),
    m_macrorenderer(new Vertex_Buffer_Macrorenderer)
  {
    get_vbos().insert(this);
  }

  template <typename VERTEX>
  static void clear_triangles(std::vector<Triangle<VERTEX> *> &triangles, std::vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors) {
    for(typename std::vector<Triangle<VERTEX> *>::iterator it = triangles.begin(), iend = triangles.end(); it != iend; ++it)
      delete *it;
    triangles.clear();

    for(std::vector<Vertex_Buffer::Vertex_Buffer_Range *>::iterator it = descriptors.begin(), iend = descriptors.end(); it != iend; ++it)
      delete *it;
    descriptors.clear();
  }

  Vertex_Buffer::~Vertex_Buffer() {
    clear_triangles(m_triangles_cm, m_descriptors_cm);
    clear_triangles(m_triangles_t, m_descriptors_t);

    delete m_renderer;
    delete m_macrorenderer;

    get_vbos().erase(this);
  }

  void Vertex_Buffer::give_Triangle(Triangle<Vertex2f_Color> * const &triangle) {
    std::auto_ptr<Renderable> to_delete(triangle);
    fax_Triangle(triangle);
  }

  void Vertex_Buffer::fax_Triangle(const Triangle<Vertex2f_Color> * const &triangle) {
    if(!triangle)
      throw VBuf_Init_Failure();

    const Vertex2f_Color &v0 = triangle->a;
    const Vertex2f_Color &v1 = triangle->b;
    const Vertex2f_Color &v2 = triangle->c;

    Triangle<Vertex3f_Color> facsimile(Vertex3f_Color(v0.position, v0.get_Color()),
                                       Vertex3f_Color(v1.position, v1.get_Color()),
                                       Vertex3f_Color(v2.position, v2.get_Color()));
    facsimile.fax_Material(triangle->get_Material());

    fax_Triangle(&facsimile);
  }

  void Vertex_Buffer::give_Triangle(Triangle<Vertex2f_Texture> * const &triangle) {
    std::auto_ptr<Renderable> to_delete(triangle);
    fax_Triangle(triangle);
  }

  void Vertex_Buffer::fax_Triangle(const Triangle<Vertex2f_Texture> * const &triangle) {
    if(!triangle)
      throw VBuf_Init_Failure();

    const Vertex2f_Texture &v0 = triangle->a;
    const Vertex2f_Texture &v1 = triangle->b;
    const Vertex2f_Texture &v2 = triangle->c;

    Triangle<Vertex3f_Texture> facsimile(Vertex3f_Texture(v0.position, v0.texture_coordinate),
                                         Vertex3f_Texture(v1.position, v1.texture_coordinate),
                                         Vertex3f_Texture(v2.position, v2.texture_coordinate));
    facsimile.fax_Material(triangle->get_Material());

    fax_Triangle(&facsimile);
  }

  void Vertex_Buffer::give_Quadrilateral(Quadrilateral<Vertex2f_Color> * const &quad) {
    std::auto_ptr<Renderable> to_delete(quad);
    fax_Quadrilateral(quad);
  }
  
  void Vertex_Buffer::fax_Quadrilateral(const Quadrilateral<Vertex2f_Color> * const &quad) {
    if(!quad)
      throw VBuf_Init_Failure();

    give_Triangle(quad->get_duplicate_t0());
    give_Triangle(quad->get_duplicate_t1());
  }

  void Vertex_Buffer::give_Quadrilateral(Quadrilateral<Vertex2f_Texture> * const &quad) {
    std::auto_ptr<Renderable> to_delete(quad);
    fax_Quadrilateral(quad);
  }
  
  void Vertex_Buffer::fax_Quadrilateral(const Quadrilateral<Vertex2f_Texture> * const &quad) {
    if(!quad)
      throw VBuf_Init_Failure();

    give_Triangle(quad->get_duplicate_t0());
    give_Triangle(quad->get_duplicate_t1());
  }

  void Vertex_Buffer::give_Triangle(Triangle<Vertex3f_Color> * const &triangle) {
    if(!triangle)
      throw VBuf_Init_Failure();

    if(!triangle->get_Material() ||
       triangle->get_Material()->get_Texture().empty())
    {
      m_triangles_cm.push_back(triangle);
      unprerender();
    }
    else {
      delete triangle;
      throw VBuf_Init_Failure();
    }
  }

  void Vertex_Buffer::fax_Triangle(const Triangle<Vertex3f_Color> * const &triangle) {
    if(!triangle)
      throw VBuf_Init_Failure();

    give_Triangle(triangle->get_duplicate());
  }

  void Vertex_Buffer::give_Triangle(Triangle<Vertex3f_Texture> * const &triangle) {
    if(!triangle)
      throw VBuf_Init_Failure();

    if(triangle->get_Material() &&
       !triangle->get_Material()->get_Texture().empty())
    {
      m_triangles_t.push_back(triangle);
      unprerender();
    }
    else {
      delete triangle;
      throw VBuf_Init_Failure();
    }
  }

  void Vertex_Buffer::fax_Triangle(const Triangle<Vertex3f_Texture> * const &triangle) {
    if(!triangle)
      throw VBuf_Init_Failure();

    give_Triangle(triangle->get_duplicate());
  }

  void Vertex_Buffer::give_Quadrilateral(Quadrilateral<Vertex3f_Color> * const &quad) {
    std::auto_ptr<Renderable> to_delete(quad);
    fax_Quadrilateral(quad);
  }
  
  void Vertex_Buffer::fax_Quadrilateral(const Quadrilateral<Vertex3f_Color> * const &quad) {
    if(!quad)
      throw VBuf_Init_Failure();

    give_Triangle(quad->get_duplicate_t0());
    give_Triangle(quad->get_duplicate_t1());
  }

  void Vertex_Buffer::give_Quadrilateral(Quadrilateral<Vertex3f_Texture> * const &quad) {
    std::auto_ptr<Renderable> to_delete(quad);
    fax_Quadrilateral(quad);
  }
  
  void Vertex_Buffer::fax_Quadrilateral(const Quadrilateral<Vertex3f_Texture> * const &quad) {
    if(!quad)
      throw VBuf_Init_Failure();

    give_Triangle(quad->get_duplicate_t0());
    give_Triangle(quad->get_duplicate_t1());
  }

  void Vertex_Buffer::debug_render() {
    for(unsigned int i = 0; i < m_triangles_cm.size(); ++i)
      get_Video().render(*m_triangles_cm[i]);
    for(unsigned int i = 0; i < m_triangles_t.size(); ++i)
      get_Video().render(*m_triangles_t[i]);
  }

  void Vertex_Buffer::give_Macrorenderer(Vertex_Buffer_Macrorenderer * const &macrorenderer) {
    delete m_macrorenderer;
    m_macrorenderer = macrorenderer;
  }

  template <typename VERTEX>
  struct SORTER {
    bool operator()(const Triangle<VERTEX> * const lhs, const Triangle<VERTEX> * const rhs) const {
      return rhs->get_Material() && (!lhs->get_Material() ||
                                     *lhs->get_Material() < *rhs->get_Material());
    }
  };

  void Vertex_Buffer::render() {
    if(!m_renderer) {
      Video &vr = get_Video();

      prerender();

      vr.lend_pre_uninit(&g_uninit);
      m_renderer = vr.create_Vertex_Buffer_Renderer(*this);
    }

    m_renderer->render();
  }

  void Vertex_Buffer::lose() {
    delete m_renderer;
    m_renderer = 0;
  }

  void Vertex_Buffer::prerender() {
    if(!m_prerendered) {
      sort_triangles();
      set_descriptors();
      if(m_align_normals)
        align_similar_normals();

      m_prerendered = true;
    }
  }

  void Vertex_Buffer::sort_triangles() {
    std::stable_sort(m_triangles_cm.begin(), m_triangles_cm.end(), SORTER<Vertex3f_Color>());
    std::stable_sort(m_triangles_t.begin(), m_triangles_t.end(), SORTER<Vertex3f_Texture>());
  }

  template <typename VERTEX>
  struct DESCRIBER {
    bool operator()(std::vector<Triangle<VERTEX> *> &triangles,
                    std::vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors,
                    const size_t &triangles_done) const {
      const Material mat;
      size_t last = 0;
      if(!triangles.empty()) {
        Material * material_ptr = triangles[0]->get_Material() ?
                                  new Material(*triangles[0]->get_Material()) :
                                  0;
        descriptors.push_back(new Vertex_Buffer::Vertex_Buffer_Range(material_ptr, triangles_done, 1u));
        if(material_ptr)
          material_ptr->clear_optimization();
        for(size_t i = 1; i < triangles.size(); ++i) {
          const Material * material_ptr1 = triangles[i]->get_Material();
          if(!material_ptr1)
            material_ptr1 = &mat;

          if(material_ptr && *material_ptr == *material_ptr1)
            ++descriptors[last]->num_elements;
          else {
            Material * const material_ptr2 = new Material(*material_ptr1);
            descriptors.push_back(new Vertex_Buffer::Vertex_Buffer_Range(material_ptr2, triangles_done+i, 1u));
            ++last;
            material_ptr2->clear_optimization();
            if(material_ptr) {
              material_ptr2->optimize_to_follow(*material_ptr);
              material_ptr->optimize_to_precede(*material_ptr2);
            }
            material_ptr = material_ptr2;
          }
        }
      }
      return triangles.size() != 0u;
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
    const float closeness_threshold_squared = CLOSENESS_THRESHOLD_SQUARED;
    const float alikeness_threshold = ALIKENESS_THRESHOLD;

    const VERTEX &v1 = t1[which];

    if((v0.position - v1.position).magnitude2() < closeness_threshold_squared &&
       fabs(Vector3f(v0.normal) * Vector3f(v1.normal)) > alikeness_threshold)
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
    const float closeness_threshold = CLOSENESS_THRESHOLD;

    for(std::vector<Vertex_Buffer::Vertex_Buffer_Range *>::iterator it = descriptors.begin();
        it != descriptors.end();
        ++it)
    {
      std::vector< Vertex_Ref<VERTEX> > verts;

      verts.reserve(3u * (*it)->num_elements);
      for(size_t i = (*it)->start, iend = (*it)->start + (*it)->num_elements;
          i != iend;
          ++i)
      {
        verts.push_back(Vertex_Ref<VERTEX>(triangles[i], 0u));
        verts.push_back(Vertex_Ref<VERTEX>(triangles[i], 1u));
        verts.push_back(Vertex_Ref<VERTEX>(triangles[i], 2u));
      }

      std::stable_sort(verts.begin(), verts.end(), typename Vertex_Ref<VERTEX>::Z_Sorter());

      typename std::vector< Vertex_Ref<VERTEX> >::iterator kend = verts.begin();

      for(typename std::vector< Vertex_Ref<VERTEX> >::iterator jt = verts.begin();
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

        for(typename std::vector< Vertex_Ref<VERTEX> >::iterator kt = jt + 1;
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
    Zeni::align_similar_normals(m_triangles_cm, m_descriptors_cm);
    Zeni::align_similar_normals(m_triangles_t, m_descriptors_t);
  }

  void Vertex_Buffer::set_descriptors() {
    DESCRIBER<Vertex3f_Color>()(m_triangles_cm, m_descriptors_cm, 0u);
    DESCRIBER<Vertex3f_Texture>()(m_triangles_t, m_descriptors_t, 0u);
  }

  void Vertex_Buffer::lose_all() {
    std::set<Vertex_Buffer *> &vbos = get_vbos();

    for(std::set<Vertex_Buffer *>::iterator it = vbos.begin();
        it != vbos.end();
        ++it)
    {
      (*it)->lose();
    }
  }

  std::set<Vertex_Buffer *> & Vertex_Buffer::get_vbos() {
    static std::set<Vertex_Buffer *> vbos;
    return vbos;
  }

#ifndef DISABLE_GL

  Vertex_Buffer_Renderer_GL::Vertex_Buffer_Renderer_GL(Vertex_Buffer &vbo)
    : Vertex_Buffer_Renderer(vbo)
  {
    memset(m_vbuf, 0, sizeof(VBO_GL) * 6);

    Video_GL &vgl = dynamic_cast<Video_GL &>(get_Video());

    const size_t v_size = vertex_size();
    const size_t n_size = normal_size();
    const size_t c_size = color_size();
    const size_t t_size = texel_size();

    const size_t vbuf_c_size = v_size * (vbo.num_vertices_cm());
    const size_t nbuf_c_size = n_size * (vbo.num_vertices_cm());
    const size_t cbuf_size = c_size * (vbo.num_vertices_cm());
    const size_t vbuf_t_size = v_size * (vbo.num_vertices_t());
    const size_t nbuf_t_size = n_size * (vbo.num_vertices_t());
    const size_t tbuf_size = t_size * (vbo.num_vertices_t());
    
    if(vbuf_c_size) {
      unsigned char *p_verts = new unsigned char [vbuf_c_size];
      unsigned char *p_normals = new unsigned char [nbuf_c_size];
      unsigned char *p_colors = new unsigned char [cbuf_size];

      unsigned char *buffered_verts = p_verts;
      unsigned char *buffered_normals = p_normals;
      unsigned char *buffered_colors = p_colors;

      for(unsigned int i = 0; i < vbo.m_triangles_cm.size(); ++i)
        for(int j = 0; j < 3; ++j) {
          memcpy(buffered_verts, (*vbo.m_triangles_cm[i])[j].get_address(), v_size);
          buffered_verts += v_size;

          memcpy(buffered_normals, reinterpret_cast<float *>((*vbo.m_triangles_cm[i])[j].get_address())+3, n_size);
          buffered_normals += n_size;

          memcpy(buffered_colors, reinterpret_cast<float *>((*vbo.m_triangles_cm[i])[j].get_address())+6, c_size);
          std::swap(buffered_colors[0], buffered_colors[2]); /// HACK: Switch to BGRA order
          buffered_colors += c_size;
        }

      if(buffers_supported(vgl)) {
        for(int i = 0; i < 3; ++i)
          vgl.pglGenBuffersARB(1, &m_vbuf[i].vbo);

        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[0].vbo);
        vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, int(vbuf_c_size), p_verts, GL_STATIC_DRAW_ARB);
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[1].vbo);
        vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, int(nbuf_c_size), p_normals, GL_STATIC_DRAW_ARB);
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[2].vbo);
        vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, int(cbuf_size), p_colors, GL_STATIC_DRAW_ARB);

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

      for(unsigned int i = 0; i < vbo.m_triangles_t.size(); ++i)
        for(int j = 0; j < 3; ++j) {
          memcpy(buffered_verts, (*vbo.m_triangles_t[i])[j].get_address(), v_size);
          buffered_verts += v_size;

          memcpy(buffered_normals, reinterpret_cast<float *>((*vbo.m_triangles_t[i])[j].get_address())+3, n_size);
          buffered_normals += n_size;

          memcpy(buffered_texels, reinterpret_cast<float *>((*vbo.m_triangles_t[i])[j].get_address())+6, t_size);
          buffered_texels += t_size;
        }

      if(buffers_supported(vgl)) {
        for(int i = 3; i < 6; ++i)
          vgl.pglGenBuffersARB(1, &m_vbuf[i].vbo);

        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[3].vbo);
        vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, int(vbuf_t_size), p_verts, GL_STATIC_DRAW_ARB);
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[4].vbo);
        vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, int(nbuf_t_size), p_normals, GL_STATIC_DRAW_ARB);
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[5].vbo);
        vgl.pglBufferDataARB(GL_ARRAY_BUFFER_ARB, int(tbuf_size), p_texels, GL_STATIC_DRAW_ARB);

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
  }

  Vertex_Buffer_Renderer_GL::~Vertex_Buffer_Renderer_GL() {
    Video_GL &vgl = dynamic_cast<Video_GL &>(get_Video());

    if(buffers_supported(vgl)) {
      for(int i = 0; i < 6; ++i)
        if(m_vbuf[i].vbo)
          vgl.pglDeleteBuffersARB(1, &m_vbuf[i].vbo);
    }
    else {
      for(int i = 0; i < 6; ++i)
        delete [] m_vbuf[i].alt;
    }
  }

  class VB_Renderer_GL : public Vertex_Buffer_Microrenderer {
  public:
    VB_Renderer_GL(const GLint &first_, const GLsizei &count_)
      : first(first_),
      count(count_)
    {
    }

  private:
    void operator()() const {
      glDrawArrays(GL_TRIANGLES, first, count);
    }

    GLint first;
    GLsizei count;
  };

  static void render(const Vertex_Buffer_Macrorenderer &macrorenderer, std::vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors) {
    Video &vr = get_Video();

    for(size_t i = 0u; i < descriptors.size(); ++i) {
      if(descriptors[i]->material.get())
        vr.set_Material(*descriptors[i]->material);

      VB_Renderer_GL microrenderer(int(3u*descriptors[i]->start), int(3u*descriptors[i]->num_elements));
      macrorenderer(microrenderer);

      if(descriptors[i]->material.get())
        vr.unset_Material(*descriptors[i]->material);
    }
  }

  void Vertex_Buffer_Renderer_GL::render() {
    Video_GL &vgl = dynamic_cast<Video_GL &>(get_Video());
    const bool buffers_supported_ = buffers_supported(vgl);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    if(!m_vbo.m_descriptors_cm.empty()) {
      // Bind Vertex Buffer
      if(buffers_supported_)
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[0].vbo);
      glVertexPointer(3, GL_FLOAT, 0, buffers_supported_ ? 0 : m_vbuf[0].alt);
      // Bind Normal Buffer
      if(buffers_supported_)
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[1].vbo);
      glNormalPointer(GL_FLOAT, 0, buffers_supported_ ? 0 : m_vbuf[1].alt);
      // Bind Color Buffer
      glEnableClientState(GL_COLOR_ARRAY);
      if(buffers_supported_)
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[2].vbo);
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, buffers_supported_ ? 0 : m_vbuf[2].alt);

      Zeni::render(*m_vbo.m_macrorenderer, m_vbo.m_descriptors_cm);

      glDisableClientState(GL_COLOR_ARRAY);
    }

    if(!m_vbo.m_descriptors_t.empty()) {
      // Bind Vertex Buffer
      if(buffers_supported_)
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[3].vbo);
      glVertexPointer(3, GL_FLOAT, 0, buffers_supported_ ? 0 : m_vbuf[3].alt);
      // Bind Normal Buffer
      if(buffers_supported_)
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[4].vbo);
      glNormalPointer(GL_FLOAT, 0, buffers_supported_ ? 0 : m_vbuf[4].alt);
      // Bind Texel Buffer
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      if(buffers_supported_)
        vgl.pglBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf[5].vbo);
      glTexCoordPointer(2, GL_FLOAT, 0, buffers_supported_ ? 0 : m_vbuf[5].alt);

      Zeni::render(*m_vbo.m_macrorenderer, m_vbo.m_descriptors_t);

      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
  }

#endif
#ifndef DISABLE_DX9

  Vertex_Buffer_Renderer_DX9::Vertex_Buffer_Renderer_DX9(Vertex_Buffer &vbo)
    : Vertex_Buffer_Renderer(vbo)
  {
    memset(&m_buf_c, 0, sizeof(VBO_DX9));
    memset(&m_buf_t, 0, sizeof(VBO_DX9));

    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(get_Video());
    size_t vertex_size;
    char *buffered;

    if(!vbo.m_triangles_cm.empty()) {
      const size_t buf_size = vertex_c_size() * vbo.num_vertices_cm();

#ifndef DISABLE_VBO
      if(FAILED
        (vdx.get_d3d_device()->CreateVertexBuffer(
        UINT(buf_size),
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

      for(size_t i = 0u; i != vbo.m_triangles_cm.size(); ++i)
        for(int j = 0; j != 3; ++j) {
          memcpy(buffered, (*vbo.m_triangles_cm[i])[j].get_address(), vertex_size);
          buffered += vertex_size;
        }

      if(m_buf_c.is_vbo)
        m_buf_c.data.vbo->Unlock();
    }

    if(!vbo.m_triangles_t.empty()) {
      const size_t buf_size = vertex_t_size() * vbo.num_vertices_t();

#ifndef DISABLE_VBO
      if(FAILED
        (vdx.get_d3d_device()->CreateVertexBuffer(
        UINT(buf_size),
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

      for(size_t i = 0u; i != vbo.m_triangles_t.size(); ++i)
        for(int j = 0; j != 3; ++j) {
          memcpy(buffered, (*vbo.m_triangles_t[i])[j].get_address(), vertex_size);
          buffered += vertex_size;
        }

      if(m_buf_t.is_vbo)
        m_buf_t.data.vbo->Unlock();
    }
  }

  Vertex_Buffer_Renderer_DX9::~Vertex_Buffer_Renderer_DX9() {
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

  class VB_Renderer_DX9VBO : public Vertex_Buffer_Microrenderer {
  public:
    VB_Renderer_DX9VBO(Video_DX9 &vdx, const size_t &StartVertex_, const size_t &PrimitiveCount_)
      : d3d_device(vdx.get_d3d_device()),
      StartVertex(StartVertex_),
      PrimitiveCount(PrimitiveCount_)
    {
    }

  private:
    void operator()() const {
      d3d_device->DrawPrimitive(D3DPT_TRIANGLELIST,
                                UINT(StartVertex),
                                UINT(PrimitiveCount));
    }

    LPDIRECT3DDEVICE9 d3d_device;
    size_t StartVertex;
    size_t PrimitiveCount;
  };

  class VB_Renderer_DX9 : public Vertex_Buffer_Microrenderer {
  public:
    VB_Renderer_DX9(Video_DX9 &vdx, const size_t &PrimitiveCount_, const void * const &pVertexStreamZeroData_, const size_t &VertexStreamZeroStride_)
      : d3d_device(vdx.get_d3d_device()),
      PrimitiveCount(PrimitiveCount_),
      pVertexStreamZeroData(pVertexStreamZeroData_),
      VertexStreamZeroStride(VertexStreamZeroStride_)
    {
    }

  private:
    void operator()() const {
      d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
                                  UINT(PrimitiveCount),
                                  pVertexStreamZeroData,
                                  UINT(VertexStreamZeroStride));
    }

    LPDIRECT3DDEVICE9 d3d_device;
    size_t PrimitiveCount;
    const void * pVertexStreamZeroData;
    size_t VertexStreamZeroStride;
  };

  static void render(const Vertex_Buffer_Macrorenderer &macrorenderer,
    std::vector<Vertex_Buffer::Vertex_Buffer_Range *> &descriptors,
    Vertex_Buffer_Renderer_DX9::VBO_DX9 &vbo_dx9,
    const unsigned int &stride,
    Video_DX9 &vdx) {

      for(unsigned int i = 0; i < descriptors.size(); ++i) {
        if(descriptors[i]->material.get())
          vdx.set_Material(*descriptors[i]->material);

        if(vbo_dx9.is_vbo) {
          VB_Renderer_DX9VBO microrenderer(vdx, 3u * descriptors[i]->start, descriptors[i]->num_elements);
          macrorenderer(microrenderer);
        }
        else {
          VB_Renderer_DX9 microrenderer(vdx, descriptors[i]->num_elements, vbo_dx9.data.alt + 3u * descriptors[i]->start, stride);
          macrorenderer(microrenderer);
        }

        if(descriptors[i]->material.get())
          vdx.unset_Material(*descriptors[i]->material);
      }
  }

  void Vertex_Buffer_Renderer_DX9::render() {
    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(get_Video());

    vdx.set_fvf(true);

    if(m_buf_c.data.vbo || m_buf_c.data.alt) {
      if(m_buf_c.is_vbo)
        vdx.get_d3d_device()->SetStreamSource(0, m_buf_c.data.vbo, 0, UINT(vertex_c_size()));
      Zeni::render(*m_vbo.m_macrorenderer, m_vbo.m_descriptors_cm, m_buf_c, UINT(vertex_c_size()), vdx);
    }
    if(m_buf_t.data.vbo || m_buf_t.data.alt) {
      if(m_buf_t.is_vbo)
        vdx.get_d3d_device()->SetStreamSource(0, m_buf_t.data.vbo, 0, UINT(vertex_t_size()));
      Zeni::render(*m_vbo.m_macrorenderer, m_vbo.m_descriptors_t, m_buf_t, UINT(vertex_t_size()), vdx);
    }
  }

#endif

  Vertex_Buffer::Uninit Vertex_Buffer::g_uninit;

}

#include <Zeni/Undefine.h>
