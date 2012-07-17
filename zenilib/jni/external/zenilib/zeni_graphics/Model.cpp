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

#include <lib3ds.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace Zeni {

  Model_Extents::Model_Extents()
    : lower_bound(0, 0, 0),
    upper_bound(0, 0, 0),
    started(false)
  {
  }

  class Model_Renderer : public Model_Visitor {
  public:
    virtual void operator()(const Model &model, Lib3dsMeshInstanceNode * const &node, Lib3dsMesh * const &mesh);

    void create_vertex_buffer(Vertex_Buffer * const &user_p, const Model &model, Lib3dsMeshInstanceNode * const &node, Lib3dsMesh * const &mesh);
  };

  void Model_Renderer::create_vertex_buffer(Vertex_Buffer * const &user_p, const Model &model, Lib3dsMeshInstanceNode * const &node, Lib3dsMesh * const &mesh) {
#ifndef TEMP_DISABLE
    assert(mesh);
    assert(user_p);

    mesh->user_ptr = user_p;
    user_p->do_normal_alignment(model.will_do_normal_alignment());

    struct l3dsv {
      ~l3dsv() {free(normals);}
      float (*normals)[3];
    } ptr = {(float(*)[3])malloc(sizeof(float) * 9 * mesh->nfaces)};
    float (*normal)[3] = ptr.normals;

    lib3ds_mesh_calculate_vertex_normals(mesh, normal);

    /*** BEGIN NEW FLIP-TEST ***/

    const Vector3f scl_track(node ? node->scl_track.keys->value[0] : 1.0f,
                             node ? node->scl_track.keys->value[1] : 1.0f,
                             node ? node->scl_track.keys->value[2] : 1.0f);
    const bool flip_order = scl_track.i * scl_track.j * scl_track.k < 0.0f;

    /*** END NEW FLIP-TEST ***/

    for(unsigned int i = 0; i < mesh->nfaces; ++i) {
      Lib3dsFace *face = &mesh->faces[i];

      Lib3dsMaterial *material = 0;
      if(face->material != -1) {
        material = model.thun_get_file()->materials[face->material];
        if(!material)
          throw Model_Render_Failure();
      }

      Material mat;
      bool textured = false;

      if(material) {///HACK
        const float opacity = 1.0f - material->transparency;
        textured = material->texture1_map.name[0] != '\0' && mesh->texcos;

        if(textured) {
          mat.ambient.a = opacity;
          mat.diffuse.a = opacity;
          mat.set_Texture(material->texture1_map.name);
        }
        else {
          mat.ambient = Color(opacity, material->ambient[0], material->ambient[1], material->ambient[2]);
          mat.diffuse = Color(opacity, material->diffuse[0], material->diffuse[1], material->diffuse[2]);
        }

        mat.specular = Color(opacity, material->specular[0], material->specular[1], material->specular[2]);
        mat.emissive = Color(opacity, 0.0f, 0.0f, 0.0f).interpolate_to(material->self_illum, mat.diffuse);
        mat.set_shininess(material->shininess);
      }

      const Point2f tex_offset((material ? material->texture1_map.offset[0] : 0.0f) + 0.5f,
                               (material ? material->texture1_map.offset[1] : 0.0f) + 0.5f);
      const Point2f tex_scale(material ? material->texture1_map.scale[0] : 1.0f,
                              material ? material->texture1_map.scale[1] : 1.0f);

      Point3f pa(mesh->vertices[face->index[0]][0], mesh->vertices[face->index[0]][1], mesh->vertices[face->index[0]][2]);
      const Point3f pb(mesh->vertices[face->index[1]][0], mesh->vertices[face->index[1]][1], mesh->vertices[face->index[1]][2]);
      Point3f pc(mesh->vertices[face->index[2]][0], mesh->vertices[face->index[2]][1], mesh->vertices[face->index[2]][2]);

      Vector3f na(normal[0][0], normal[0][1], normal[0][2]);
      Vector3f nb(normal[1][0], normal[1][1], normal[1][2]);
      Vector3f nc(normal[2][0], normal[2][1], normal[2][2]);

      if(flip_order) {
        std::swap(pa, pc);
        std::swap(na, nc);

        na *= -1;
        nb *= -1;
        nc *= -1;
      }

      if(textured) {
        Point2f ta(0.5f - tex_scale.x * (tex_offset.x - mesh->texcos[face->index[0]][0]), 0.5f - tex_scale.y * (tex_offset.y + mesh->texcos[face->index[0]][1]));
        const Point2f tb(0.5f - tex_scale.x * (tex_offset.x - mesh->texcos[face->index[1]][0]), 0.5f - tex_scale.y * (tex_offset.y + mesh->texcos[face->index[1]][1]));
        Point2f tc(0.5f - tex_scale.x * (tex_offset.x - mesh->texcos[face->index[2]][0]), 0.5f - tex_scale.y * (tex_offset.y + mesh->texcos[face->index[2]][1]));
      
        if(flip_order)
          std::swap(ta, tc);

        Triangle<Vertex3f_Texture> triangle(Vertex3f_Texture(pa, na, ta),
                                            Vertex3f_Texture(pb, nb, tb),
                                            Vertex3f_Texture(pc, nc, tc));
        triangle.fax_Material(&mat);

        user_p->fax_Triangle(&triangle);
      }
      else {
        const Uint32 argb = mat.diffuse.get_argb();

        Triangle<Vertex3f_Color> triangle(Vertex3f_Color(pa, na, argb),
                                          Vertex3f_Color(pb, nb, argb),
                                          Vertex3f_Color(pc, nc, argb));
        triangle.fax_Material(&mat);

        user_p->fax_Triangle(&triangle);
      }

      normal+=3;
    }
#endif
  }

  class Model_Unrenderer : public Model_Visitor {
  public:
    virtual void operator()(const Model &model, Lib3dsMeshInstanceNode * const &node, Lib3dsMesh * const &mesh);
  };

  void Model_Unrenderer::operator()(const Model & /*model*/ , Lib3dsMeshInstanceNode * const & /*node*/ , Lib3dsMesh * const &mesh) {
    assert(mesh);

    if(mesh->user_ptr) {
      Vertex_Buffer *user_p = reinterpret_cast<Vertex_Buffer *>(mesh->user_ptr);
      delete user_p;
      mesh->user_ptr = 0;
    }
  }

#ifndef TEMP_DISABLE
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif
  Model::Model(const String &filename, const bool align_normals_)
    : m_filename(filename),
    m_file(0), 
    m_keyframe(0.0f),
    m_align_normals(align_normals_),
    m_unrenderer(0), 
    m_scale(1.0f, 1.0f, 1.0f), 
    m_rotate(0.0f, 0.0f, 1.0f), 
    m_translate(0.0f, 0.0f, 0.0f), 
    m_rotate_angle(0.0f)
//     m_loader(*this),
//     m_loader_op(m_loader)
  {
    load();
  }

  Model::Model(const Model &rhs)
    : m_filename(rhs.m_filename),
    m_file(0),
    m_keyframe(rhs.m_keyframe),
    m_align_normals(rhs.m_align_normals),
    m_unrenderer(0),
    m_scale(rhs.m_scale),
    m_rotate(rhs.m_rotate),
    m_translate(rhs.m_translate),
    m_rotate_angle(rhs.m_rotate_angle)
//     m_loader(*this),
//     m_loader_op(m_loader)
  {
    load();
  }
#ifdef _WINDOWS
#pragma warning( pop )
#endif

  Model::~Model() {
    if(m_unrenderer)
      visit_meshes(*m_unrenderer);
    delete m_unrenderer;

    if(m_file)
      lib3ds_file_free(m_file);
  }

  Model & Model::operator =(const Model &rhs) {
    Model *lhs = 0;
    
    try {
//       GUARANTEED_FINISHED_BEGIN(rhs.m_loader);
      lhs = new Model(rhs);
//       GUARANTEED_FINISHED_END();

//       GUARANTEED_FINISHED_BEGIN(lhs->m_loader);
//       GUARANTEED_FINISHED_BEGIN(m_loader);
      std::swap(m_filename, lhs->m_filename);
      std::swap(m_file, lhs->m_file);
      std::swap(m_keyframe, lhs->m_keyframe);
      std::swap(m_unrenderer, lhs->m_unrenderer);
      std::swap(m_scale, lhs->m_scale);
      std::swap(m_rotate, lhs->m_rotate);
      std::swap(m_translate, lhs->m_translate);
      std::swap(m_rotate_angle, lhs->m_rotate_angle);
//       GUARANTEED_FINISHED_END();
//       GUARANTEED_FINISHED_END();
    }
    catch(...) {
      delete lhs;
      throw;
    }
    
    delete lhs;

    return *this;
  }

  Point3f Model::get_position() const {
//     GUARANTEED_FINISHED_BEGIN(m_loader);
    return m_translate + m_scale.multiply_by(Quaternion::Axis_Angle(m_rotate, m_rotate_angle) * Vector3f(m_position));
//     GUARANTEED_FINISHED_END();
  }

  float Model::get_keyframes() const {
//     GUARANTEED_FINISHED_BEGIN(m_loader);
    return float(m_file->frames);
//     GUARANTEED_FINISHED_END();
  }
  
  void Model::set_keyframe(const float &keyframe) {
//     GUARANTEED_FINISHED_BEGIN(m_loader);
    m_keyframe = keyframe;
    lib3ds_file_eval(m_file, keyframe);
//     GUARANTEED_FINISHED_END();
  }

  void Model::visit_nodes(Model_Visitor &mv, Lib3dsNode *node) const {
    if(!node) {
      for(node=m_file->nodes; node; node=node->next)
        visit_nodes(mv, node);
      return;
    }

    for(Lib3dsNode *child=node->childs; child; child=child->next)
      visit_nodes(mv, child);

    mv(*this, node);
  }

  void Model::visit_meshes(Model_Visitor &mv, Lib3dsNode * node, Lib3dsMesh * const &mesh) const {
    if(!node) {
      if(!mesh) {
        if(m_file->nodes)
          for(node = m_file->nodes; node; node = node->next)
            visit_meshes(mv, node);
        else
          for(Lib3dsMesh *mesh = *m_file->meshes, *end = mesh + m_file->nmeshes;
              mesh != end;
              ++mesh)
          {
            visit_meshes(mv, node, mesh);
          }

        return;
      }
    }
    else
      for(Lib3dsNode *child=node->childs; child; child=child->next)
        visit_meshes(mv, child);

    if(!node) {
      if(!mesh)
        throw Model_Render_Failure();

      mv(*this, 0, mesh);
    }
    else if(node->type == LIB3DS_NODE_MESH_INSTANCE &&
            strcmp(node->name, "$$$DUMMY"))
    {
      Lib3dsMesh * node_mesh = m_file->meshes[lib3ds_file_mesh_by_name(m_file, node->name)];

      if(!node_mesh)
        throw Model_Render_Failure();

      mv(*this, reinterpret_cast<Lib3dsMeshInstanceNode * const>(node), node_mesh);
    }
  }

  void Model::render() const {
//     GUARANTEED_FINISHED_BEGIN(m_loader);
    
    if(!m_unrenderer)
      m_unrenderer = new Model_Unrenderer();

    Video &vr = get_Video();

    vr.push_world_stack();

    vr.translate_scene(Vector3f(m_translate));
    vr.rotate_scene(m_rotate, m_rotate_angle);
    vr.scale_scene(m_scale);

    Model_Renderer mr;
    visit_meshes(mr);

    vr.pop_world_stack();
    
//     GUARANTEED_FINISHED_END();
  }
#endif

  void Model_Renderer::operator()(const Model &model, Lib3dsMeshInstanceNode * const &node, Lib3dsMesh * const &mesh) {
    Video &vr = get_Video();

    if(!mesh || !mesh->user_ptr)
      create_vertex_buffer(new Vertex_Buffer(), model, node, mesh);

    Vertex_Buffer * user_p = reinterpret_cast<Vertex_Buffer *>(mesh->user_ptr);
    if(!user_p)
      throw Model_Render_Failure();

    vr.push_world_stack();

    if(node) {
      vr.transform_scene(reinterpret_cast<const Matrix4f &>(node->base.matrix));
      vr.translate_scene(Vector3f(-node->pivot[0],
                                  -node->pivot[1],
                                  -node->pivot[2]));
    }
    vr.transform_scene(reinterpret_cast<const Matrix4f &>(mesh->matrix).inverted());

    //user_p->debug_render(); ///HACK
    user_p->render();

    vr.pop_world_stack();
  }

  void Model_Extents::operator()(const Model & /*model*/ , Lib3dsMeshInstanceNode * const & /*node*/ , Lib3dsMesh * const &mesh) {
    for(float (*vertex)[3] = &mesh->vertices[0], (*end)[3] = vertex + mesh->nvertices; vertex != end; ++vertex)
      if(vertex) {
        float (&point)[3] = *vertex;

        if(lower_bound.x > point[0] || !started)
          lower_bound.x = point[0];
        if(lower_bound.y > point[1] || !started)
          lower_bound.y = point[1];
        if(lower_bound.z > point[2] || !started)
          lower_bound.z = point[2];

        if(upper_bound.x < point[0] || !started)
          upper_bound.x = point[0];
        if(upper_bound.y < point[1] || !started)
          upper_bound.y = point[1];
        if(upper_bound.z < point[2] || !started)
          upper_bound.z = point[2];

        started = true;
      }
  }
  
//   int Model::Loader::function() {
//     m_model.load();
//     return 0;
//   }
  
#ifndef TEMP_DISABLE
  void Model::load() {
    m_file = lib3ds_file_open(m_filename.c_str());
    if(!m_file)
      throw Model_Init_Failure();

    lib3ds_file_eval(m_file, m_keyframe);

    visit_meshes(m_extents);

    m_position = m_extents.upper_bound.interpolate_to(0.5f, m_extents.lower_bound);
  }
#endif

}
