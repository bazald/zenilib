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

#include <Zeni/Model.hxx>

#include <Zeni/Vertex_Buffer.h>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Material.hxx>
#include <Zeni/Mutex.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Video.hxx>

#include <lib3ds/mesh.h>
#include <lib3ds/material.h>
#include <lib3ds/matrix.h>
#include <lib3ds/vector.h>
#include <string>

namespace Zeni {

  Model_Extents::Model_Extents()
    : lower_bound(0, 0, 0),
    upper_bound(0, 0, 0),
    started(false)
  {
  }

  class Model_Renderer : public Model_Visitor {
  public:
    virtual void operator()(const Model &model, Lib3dsNode * const &node, Lib3dsMesh * const &mesh);

    void create_vertex_buffer(Vertex_Buffer * const &user_p, const Model &model, Lib3dsNode * const &node, Lib3dsMesh * const &mesh);
  };

  void Model_Renderer::create_vertex_buffer(Vertex_Buffer * const &user_p, const Model &model, Lib3dsNode * const & /*node*/, Lib3dsMesh * const &mesh) {
    mesh->user.p = user_p;

    struct l3dsv {
      ~l3dsv() {delete [] normalL;}
      Lib3dsVector *normalL;
    } ptr = {new Lib3dsVector[3 * mesh->faces]};
    Lib3dsVector *normal = ptr.normalL;

    lib3ds_mesh_calculate_normals(mesh, normal);

    /*** BEGIN NEW FLIP-TEST ***/

    const Matrix4f &mesh_transform = reinterpret_cast<const Matrix4f &>(mesh->matrix);
    const Vector3f ti = mesh_transform * Vector3f(1.0f, 0.0f, 0.0f);
    const Vector3f tj = mesh_transform * Vector3f(0.0f, 1.0f, 0.0f);
    const Vector3f tk = mesh_transform * Vector3f(0.0f, 0.0f, 1.0f);
    const bool flip = (ti % tj) * tk < 0.0f;

    /*** END NEW FLIP-TEST ***/

    /*** BEGIN OLD FLIP-TEST ***/

    //// Get scaling vector
    //Lib3dsObjectData * const data=&node->data.object;
    //Vector3f scl(data->scl[0], data->scl[1], data->scl[2]);
    //const bool flip = (data->scl[0] < 0.0f) ^
    //                  (data->scl[1] < 0.0f) ^
    //                  (data->scl[2] < 0.0f);

    /*** END OLD FLIP-TEST ***/

    for(unsigned int i = 0; i < mesh->faces; ++i) {
      Lib3dsFace *face = &mesh->faceL[i];

      Lib3dsMaterial *material = 0;
      if(face->material[0]) {
        material=lib3ds_file_material_by_name(model.thun_get_file(), face->material);
        if(!material)
          throw Model_Render_Failure();
      }

      Material mat;
      Color pseudo_color;

      if(material) {///HACK
        mat = Material(Color(material->ambient[3], material->ambient[0], material->ambient[1], material->ambient[2]),
          Color(material->diffuse[3], material->diffuse[0], material->diffuse[1], material->diffuse[2]),
          Color(material->specular[3], material->specular[0], material->specular[1], material->specular[2]),
          Color(1.0f, 0.0f, 0.0f, 0.0f),
          1.0f, mesh->texels ? material->texture1_map.name : "");
        mat.set_shininess(material->shininess);

        pseudo_color = Color(material->diffuse[3], material->diffuse[0], material->diffuse[1], material->diffuse[2]);
      }

      const Point2f &tex_offset = reinterpret_cast<Point2f &>(material->texture1_map.offset);
      const Point2f &tex_scale = reinterpret_cast<Point2f &>(material->texture1_map.scale);

      if(flip)
        if(mat.get_texture().size())
          user_p->add_triangle
          (new Triangle<Vertex3f_Texture>
          (
          Vertex3f_Texture(Point3f(mesh->pointL[face->points[2]].pos[0], mesh->pointL[face->points[2]].pos[1], mesh->pointL[face->points[2]].pos[2]), Vector3f(normal[2][0], normal[2][1], normal[2][2]), Point2f(tex_offset.x + tex_scale.x * mesh->texelL[face->points[2]][0], tex_offset.y + tex_scale.y * mesh->texelL[face->points[2]][1])),
          Vertex3f_Texture(Point3f(mesh->pointL[face->points[1]].pos[0], mesh->pointL[face->points[1]].pos[1], mesh->pointL[face->points[1]].pos[2]), Vector3f(normal[1][0], normal[1][1], normal[1][2]), Point2f(tex_offset.x + tex_scale.x * mesh->texelL[face->points[1]][0], tex_offset.y + tex_scale.y * mesh->texelL[face->points[1]][1])),
          Vertex3f_Texture(Point3f(mesh->pointL[face->points[0]].pos[0], mesh->pointL[face->points[0]].pos[1], mesh->pointL[face->points[0]].pos[2]), Vector3f(normal[0][0], normal[0][1], normal[0][2]), Point2f(tex_offset.x + tex_scale.x * mesh->texelL[face->points[0]][0], tex_offset.y + tex_scale.y * mesh->texelL[face->points[0]][1])),
          (reinterpret_cast<Render_Wrapper *>(new Material_Render_Wrapper(mat)))
          ));
        else
          user_p->add_triangle
          (new Triangle<Vertex3f_Color>
          (
          Vertex3f_Color(Point3f(mesh->pointL[face->points[2]].pos[0], mesh->pointL[face->points[2]].pos[1], mesh->pointL[face->points[2]].pos[2]), Vector3f(normal[2][0], normal[2][1], normal[2][2]), pseudo_color.get_argb()),
          Vertex3f_Color(Point3f(mesh->pointL[face->points[1]].pos[0], mesh->pointL[face->points[1]].pos[1], mesh->pointL[face->points[1]].pos[2]), Vector3f(normal[1][0], normal[1][1], normal[1][2]), pseudo_color.get_argb()),
          Vertex3f_Color(Point3f(mesh->pointL[face->points[0]].pos[0], mesh->pointL[face->points[0]].pos[1], mesh->pointL[face->points[0]].pos[2]), Vector3f(normal[0][0], normal[0][1], normal[0][2]), pseudo_color.get_argb()),
          (material ? reinterpret_cast<Render_Wrapper *>(new Material_Render_Wrapper(mat)) : new Render_Wrapper())
          ));
      else
        if(mat.get_texture().size())
          user_p->add_triangle
          (new Triangle<Vertex3f_Texture>
          (
          Vertex3f_Texture(Point3f(mesh->pointL[face->points[0]].pos[0], mesh->pointL[face->points[0]].pos[1], mesh->pointL[face->points[0]].pos[2]), Vector3f(normal[0][0], normal[0][1], normal[0][2]), Point2f(tex_offset.x + tex_scale.x * mesh->texelL[face->points[0]][0], tex_offset.y + tex_scale.y * mesh->texelL[face->points[0]][1])),
          Vertex3f_Texture(Point3f(mesh->pointL[face->points[1]].pos[0], mesh->pointL[face->points[1]].pos[1], mesh->pointL[face->points[1]].pos[2]), Vector3f(normal[1][0], normal[1][1], normal[1][2]), Point2f(tex_offset.x + tex_scale.x * mesh->texelL[face->points[1]][0], tex_offset.y + tex_scale.y * mesh->texelL[face->points[1]][1])),
          Vertex3f_Texture(Point3f(mesh->pointL[face->points[2]].pos[0], mesh->pointL[face->points[2]].pos[1], mesh->pointL[face->points[2]].pos[2]), Vector3f(normal[2][0], normal[2][1], normal[2][2]), Point2f(tex_offset.x + tex_scale.x * mesh->texelL[face->points[2]][0], tex_offset.y + tex_scale.y * mesh->texelL[face->points[2]][1])),
          (reinterpret_cast<Render_Wrapper *>(new Material_Render_Wrapper(mat)))
          ));
        else
          user_p->add_triangle
          (new Triangle<Vertex3f_Color>
          (
          Vertex3f_Color(Point3f(mesh->pointL[face->points[0]].pos[0], mesh->pointL[face->points[0]].pos[1], mesh->pointL[face->points[0]].pos[2]), Vector3f(normal[0][0], normal[0][1], normal[0][2]), pseudo_color.get_argb()),
          Vertex3f_Color(Point3f(mesh->pointL[face->points[1]].pos[0], mesh->pointL[face->points[1]].pos[1], mesh->pointL[face->points[1]].pos[2]), Vector3f(normal[1][0], normal[1][1], normal[1][2]), pseudo_color.get_argb()),
          Vertex3f_Color(Point3f(mesh->pointL[face->points[2]].pos[0], mesh->pointL[face->points[2]].pos[1], mesh->pointL[face->points[2]].pos[2]), Vector3f(normal[2][0], normal[2][1], normal[2][2]), pseudo_color.get_argb()),
          (material ? reinterpret_cast<Render_Wrapper *>(new Material_Render_Wrapper(mat)) : new Render_Wrapper())
          ));

      normal+=3;
    }
  }

  class Model_Unrenderer : public Model_Visitor {
  public:
    virtual void operator()(const Model &model, Lib3dsNode * const &node, Lib3dsMesh * const &mesh);
  };

  void Model_Unrenderer::operator()(const Model &model, Lib3dsNode * const &node, Lib3dsMesh * const &node_mesh) {
    Lib3dsMesh * mesh = 0;

    if(node) {
      if(node->type != LIB3DS_OBJECT_NODE ||
        !strcmp(node->name, "$$$DUMMY"))
        return;

      mesh = lib3ds_file_mesh_by_name(model.thun_get_file(), node->name);
    }
    else if(node_mesh)
      mesh = node_mesh;
    else
      return;

    if(mesh->user.p) {
      Vertex_Buffer *user_p = reinterpret_cast<Vertex_Buffer *>(mesh->user.p);
      delete user_p;
      mesh->user.p = 0;
    }
  }

  void vflip_texels(const Model &model, Lib3dsNode *node = 0) {
    if(!node) {
      for(node=model.thun_get_file()->nodes; node; node=node->next)
        vflip_texels(model, node);
      return;
    }

    for(Lib3dsNode *child=node->childs; child; child=child->next)
      vflip_texels(model, child);

    // End recursion // Begin vflip

    Lib3dsMesh *mesh=lib3ds_file_mesh_by_name(model.thun_get_file(), node->name);
    if(!mesh)
      return;

    for(Lib3dsTexel *texel=&mesh->texelL[0], *end=texel+mesh->texels; texel != end; ++texel)
      (*texel)[1] = 1.0f - (*texel)[1];
  }

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif
  Model::Model(const std::string &filename)
    : m_filename(filename),
    m_file(0), 
    m_keyframe(0.0f),
    m_unrenderer(0), 
    m_scale(1.0f, 1.0f, 1.0f), 
    m_rotate(0.0f, 0.0f, 1.0f), 
    m_translate(0.0f, 0.0f, 0.0f), 
    m_rotate_angle(0.0f),
    m_loader(*this),
    m_loader_op(m_loader)
  {
  }

  Model::Model(const Model &rhs)
    : m_filename(rhs.m_filename),
    m_file(0),
    m_keyframe(rhs.m_keyframe),
    m_unrenderer(0),
    m_scale(rhs.m_scale),
    m_rotate(rhs.m_rotate),
    m_translate(rhs.m_translate),
    m_rotate_angle(rhs.m_rotate_angle),
    m_loader(*this),
    m_loader_op(m_loader)
  {
  }
#ifdef _WINDOWS
#pragma warning( pop )
#endif

  Model::~Model() {
    m_loader.finish();
    
    if(m_unrenderer)
      visit_meshes(*m_unrenderer);
    delete m_unrenderer;

    if(m_file)
      lib3ds_file_free(m_file);
  }

  Model & Model::operator =(const Model &rhs) {
    Model *lhs = 0;
    
    try {
      GUARANTEED_FINISHED_BEGIN(rhs.m_loader);
      lhs = new Model(rhs);
      GUARANTEED_FINISHED_END();

      GUARANTEED_FINISHED_BEGIN(lhs->m_loader);
      GUARANTEED_FINISHED_BEGIN(m_loader);
      std::swap(m_filename, lhs->m_filename);
      std::swap(m_file, lhs->m_file);
      std::swap(m_keyframe, lhs->m_keyframe);
      std::swap(m_unrenderer, lhs->m_unrenderer);
      std::swap(m_scale, lhs->m_scale);
      std::swap(m_rotate, lhs->m_rotate);
      std::swap(m_translate, lhs->m_translate);
      std::swap(m_rotate_angle, lhs->m_rotate_angle);
      GUARANTEED_FINISHED_END();
      GUARANTEED_FINISHED_END();
    }
    catch(...) {
      delete lhs;
      throw;
    }
    
    delete lhs;

    return *this;
  }

  Point3f Model::get_position() const {
    return m_translate + m_scale.multiply_by(Quaternion(m_rotate, m_rotate_angle) * Vector3f(m_position));
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

  void Model::visit_meshes(Model_Visitor &mv, Lib3dsNode *node, Lib3dsMesh *mesh) const {
    if(!node) {
      if(!mesh) {
        if(m_file->nodes)
          for(node=m_file->nodes; node; node=node->next)
            visit_meshes(mv, node);
        else
          for(mesh=m_file->meshes; mesh; mesh=mesh->next)
            visit_meshes(mv, node, mesh);

        return;
      }
    }
    else
      for(Lib3dsNode *child=node->childs; child; child=child->next)
        visit_meshes(mv, child);

    mv(*this, node, mesh);
  }

  void Model::render() const {
    GUARANTEED_FINISHED_BEGIN(m_loader);
    
    if(!m_unrenderer)
      m_unrenderer = new Model_Unrenderer();

    Video &vr = Video::get_reference();

    vr.push_world_stack();

    vr.translate_scene(Vector3f(m_translate));
    vr.rotate_scene(m_rotate, m_rotate_angle);
    vr.scale_scene(m_scale);

    Model_Renderer mr;
    visit_meshes(mr);

    vr.pop_world_stack();
    
    GUARANTEED_FINISHED_END();
  }

  void Model_Renderer::operator()(const Model &model, Lib3dsNode * const &node, Lib3dsMesh * const &node_mesh) {
    Lib3dsMesh * mesh = 0;

    if(node) {
      if(node->type != LIB3DS_OBJECT_NODE ||
        !strcmp(node->name, "$$$DUMMY"))
        return;

      mesh = lib3ds_file_mesh_by_name(model.thun_get_file(), node->name);
    }
    else if(node_mesh)
      mesh = node_mesh;
    else
      throw Model_Render_Failure();

    Video &vr = Video::get_reference();

    if(!mesh->user.p)
      create_vertex_buffer(vr.create_Vertex_Buffer(), model, node, mesh);

    Vertex_Buffer * user_p = reinterpret_cast<Vertex_Buffer *>(mesh->user.p);
    if(!user_p)
      throw Model_Render_Failure();

    vr.push_world_stack();

    /*** BEGIN PART 1 ***/

    if(node) {
      Lib3dsObjectData * data = &node->data.object;

      vr.transform_scene(reinterpret_cast<const Matrix4f &>(node->matrix));

      vr.translate_scene(-reinterpret_cast<const Vector3f &>(data->pivot));
    }
    
    /*** END PART 1 ***/

    /*** BEGIN PART 2 ***/

    Lib3dsMatrix M;
    lib3ds_matrix_copy(M, mesh->matrix);
    lib3ds_matrix_inv(M);

    vr.transform_scene(reinterpret_cast<const Matrix4f &>(M));
    
    /*** END PART 2 ***/

    //user_p->debug_render(); ///HACK
    user_p->render();

    vr.pop_world_stack();
  }

  void Model_Extents::operator()(const Model &model, Lib3dsNode * const &node, Lib3dsMesh * const &node_mesh) {
    Lib3dsMesh * mesh = 0;

    if(node) {
      if(node->type != LIB3DS_OBJECT_NODE ||
        !strcmp(node->name, "$$$DUMMY"))
        return;

      mesh = lib3ds_file_mesh_by_name(model.thun_get_file(), node->name);
    }
    else if(node_mesh)
      mesh = node_mesh;
    else
      return;

    for(Lib3dsPoint *point=&mesh->pointL[0], *end=point+mesh->points; point != end; ++point)
      if(point) {
        if(lower_bound.x > point->pos[0] || !started)
          lower_bound.x = point->pos[0];
        if(lower_bound.y > point->pos[1] || !started)
          lower_bound.y = point->pos[1];
        if(lower_bound.z > point->pos[2] || !started)
          lower_bound.z = point->pos[2];

        if(upper_bound.x < point->pos[0] || !started)
          upper_bound.x = point->pos[0];
        if(upper_bound.y < point->pos[1] || !started)
          upper_bound.y = point->pos[1];
        if(upper_bound.z < point->pos[2] || !started)
          upper_bound.z = point->pos[2];

        started = true;
      }
  }
  
  int Model::Loader::function() {
    m_model.load();
    return 0;
  }
  
  void Model::load() {
    m_file = lib3ds_file_load(m_filename.c_str());
    if(!m_file)
      throw Model_Init_Failure();

    lib3ds_file_eval(m_file, m_keyframe);

    visit_meshes(m_extents);

    m_position = m_extents.upper_bound.interpolate_to(0.5f, m_extents.lower_bound);

    // Flip Textures Vertically - !!HACK!!
    vflip_texels(*this);
  }

}
