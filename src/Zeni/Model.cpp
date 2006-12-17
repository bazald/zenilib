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

#include <Zeni/Model.hxx>
#include <Zeni/Vertex_Buffer.h>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Video_DX9.hxx>

#include <lib3ds/mesh.h>
#include <lib3ds/material.h>
#include <lib3ds/matrix.h>
#include <lib3ds/vector.h>
#include <string>

#ifndef DISABLE_GL
#include <GL/gl.h>
#endif
#ifndef DISABLE_DX9
#include <d3dx9.h>
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
    virtual void operator()(const Model &model, Lib3dsNode *node) = 0;

    void create_vertex_buffer(Vertex_Buffer *user_d, const Model &model, Lib3dsNode *node, Lib3dsMesh *mesh);
  };

  void Model_Renderer::create_vertex_buffer(Vertex_Buffer *user_d, const Model &model, Lib3dsNode *node, Lib3dsMesh *mesh) {
    node->user.d = reinterpret_cast<long int>(user_d);

    struct l3dsv {
      ~l3dsv() {delete [] normalL;}
      Lib3dsVector *normalL;
    } ptr = {new Lib3dsVector[3 * mesh->faces]};
    Lib3dsVector *normal = ptr.normalL;

    lib3ds_mesh_calculate_normals(mesh, normal);

    for(unsigned int i = 0; i < mesh->faces; ++i) {
      Lib3dsFace *face = &mesh->faceL[i];

      Lib3dsMaterial *material = 0;
      if(face->material[0]) {
        material=lib3ds_file_material_by_name(model.get_file(), face->material);
        if(!material)
          throw Model_Render_Failure();
      }

      Material mat;
      Color pseudo_color;

      if(material) {
        mat = Material(Color(material->ambient[0], material->ambient[1], material->ambient[2], material->ambient[3]),
          Color(material->diffuse[0], material->diffuse[1], material->diffuse[2], material->diffuse[3]),
          Color(material->specular[0], material->specular[1], material->specular[2], material->specular[3]),
          Color(0, 0, 0, 1.0f));
        mat.set_shininess(material->shininess);

        pseudo_color = Color(material->diffuse[0], material->diffuse[1], material->diffuse[2], material->diffuse[3]).interpolate_to(0.5f, Color(material->ambient[0], material->ambient[1], material->ambient[2], material->ambient[3]));
      }

      if(mesh->texels)
        user_d->add_triangle
        (new Triangle<Vertex3f_Texture>
        (
        Vertex3f_Texture(Point3f(mesh->pointL[face->points[0]].pos[0], mesh->pointL[face->points[0]].pos[1], mesh->pointL[face->points[0]].pos[2]), Vector3f(normal[0][0], normal[0][1], normal[0][2]), Point2f(mesh->texelL[face->points[0]][0], mesh->texelL[face->points[0]][1])),
        Vertex3f_Texture(Point3f(mesh->pointL[face->points[1]].pos[0], mesh->pointL[face->points[1]].pos[1], mesh->pointL[face->points[1]].pos[2]), Vector3f(normal[1][0], normal[1][1], normal[1][2]), Point2f(mesh->texelL[face->points[1]][0], mesh->texelL[face->points[1]][1])),
        Vertex3f_Texture(Point3f(mesh->pointL[face->points[2]].pos[0], mesh->pointL[face->points[2]].pos[1], mesh->pointL[face->points[2]].pos[2]), Vector3f(normal[2][0], normal[2][1], normal[2][2]), Point2f(mesh->texelL[face->points[2]][0], mesh->texelL[face->points[2]][1])),
        (material ? reinterpret_cast<Render_Wrapper *>(new Multiple_Render_Wrapper(model.get_render_wrapper()->get_duplicate(), new Material_Render_Wrapper(mat))) : reinterpret_cast<Render_Wrapper *>(model.get_render_wrapper()->get_duplicate()))
        ));
      else
        user_d->add_triangle
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
    virtual void operator()(const Model &model, Lib3dsNode *node);
  };

  void Model_Unrenderer::operator()(const Model &, Lib3dsNode *node) {
    if(node->type != LIB3DS_OBJECT_NODE ||
      !strcmp(node->name, "$$$DUMMY"))
      return;

    if(node->user.d) {
      Vertex_Buffer *user_d = reinterpret_cast<Vertex_Buffer *>(node->user.d);
      delete user_d;
      node->user.d = 0;
    }
  }

  static void vflip_texels(const Model &model, Lib3dsNode *node = 0) {
    if(!node) {
      for(node=model.get_file()->nodes; node; node=node->next)
        vflip_texels(model, node);
      return;
    }

    for(Lib3dsNode *child=node->childs; child; child=child->next)
      vflip_texels(model, child);

    // End recursion // Begin vflip

    Lib3dsMesh *mesh=lib3ds_file_mesh_by_name(model.get_file(), node->name);
    if(!mesh)
      return;

    for(Lib3dsTexel *texel=&mesh->texelL[0], *end=texel+mesh->texels; texel != end; ++texel)
      (*texel)[1] *= -1;
  }

  Model::Model(const std::string &filename, Render_Wrapper *render_wrapper)
    : m_file(0), 
    m_render_wrapper(render_wrapper),
    m_unrenderer(0), 
    m_scale(1, 1, 1), 
    m_rotate(0, 0, 1), 
    m_translate(0, 0, 0), 
    m_rotate_angle(0)
  {
    m_file = lib3ds_file_load(filename.c_str());
    if(!m_file)
      throw Model_Init_Failure();

    set_keyframe(0);

    visit_nodes(m_extents);

    m_position = m_extents.upper_bound.interpolate_to(0.5f, m_extents.lower_bound);

    // Flip Textures Vertically - !!HACK!!
    vflip_texels(*this);
  }

  Model::~Model() {
    if(m_unrenderer)
      visit_nodes(*m_unrenderer);
    delete m_unrenderer;

    lib3ds_file_free(m_file);
  }

  Point3f Model::get_position() const {
    return Point3f(m_translate.x + m_position.x,
      m_translate.y + m_position.y,
      m_translate.z + m_position.z);
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

#ifndef DISABLE_GL

  class Model_Renderer_GL : public Model_Renderer {
  public:
    virtual void operator()(const Model &model, Lib3dsNode *node);
  };

  void Model::render_to(Video_GL &) const {
    if(!m_unrenderer)
      m_unrenderer = new Model_Unrenderer();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    GLfloat m[16]; glGetFloatv (GL_MODELVIEW_MATRIX, m);

    glTranslatef(m_translate.x, m_translate.y, m_translate.z);
    glRotatef(m_rotate_angle*180.0f/pi, m_rotate.x, m_rotate.y, m_rotate.z);
    glScalef(m_scale.x, m_scale.y, m_scale.z);

    Model_Renderer_GL mrGL;
    visit_nodes(mrGL);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }

  void Model_Renderer_GL::operator()(const Model &model, Lib3dsNode *node) {
    if(node->type != LIB3DS_OBJECT_NODE ||
      !strcmp(node->name, "$$$DUMMY"))
      return;

    Lib3dsMesh *mesh=lib3ds_file_mesh_by_name(model.get_file(), node->name);
    if(!mesh)
      throw Model_Render_Failure();

    if(!node->user.d)
      if(mesh->texels)
        create_vertex_buffer(new Vertex_Buffer_3FT_GL(), model, node, mesh);
      else
        create_vertex_buffer(new Vertex_Buffer_3FC_GL(), model, node, mesh);

    Vertex_Buffer *user_d = reinterpret_cast<Vertex_Buffer *>(node->user.d);
    if(!user_d)
      throw Model_Render_Failure();

    Lib3dsObjectData *data=&node->data.object;

    glPushMatrix();

    glMultMatrixf(&node->matrix[0][0]);

    {//glTranslatef(-data->pivot[0], -data->pivot[1], -data->pivot[2]);
      if (lib3ds_matrix_det(node->matrix) < 0.0) {
        glScalef(-1.0, 1.0, 1.0);
        glTranslatef(data->pivot[0], -data->pivot[1], -data->pivot[2]);
      }
      else
        glTranslatef(-data->pivot[0], -data->pivot[1], -data->pivot[2]);
    }

    Lib3dsMatrix M;
    lib3ds_matrix_copy(M, mesh->matrix);
    lib3ds_matrix_inv(M);
    glMultMatrixf(&M[0][0]);

    user_d->render();

    glPopMatrix();
  }

#endif
#ifndef DISABLE_DX9

  class Model_Renderer_DX9 : public Model_Renderer {
  public:
    virtual void operator()(const Model &model, Lib3dsNode *node);
  };

  void Model::render_to(Video_DX9 &) const {
    if(!m_unrenderer)
      m_unrenderer = new Model_Unrenderer();

    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(Video::get_reference());

    vdx.get_matrix_stack()->Push();

    D3DXMATRIX world;
    vdx.get_d3d_device()->GetTransform(D3DTS_WORLD, &world);
    vdx.get_matrix_stack()->LoadMatrix(&world);

    vdx.get_matrix_stack()->TranslateLocal(m_translate.x, m_translate.y, m_translate.z);
    vdx.get_matrix_stack()->RotateAxisLocal(reinterpret_cast<const D3DXVECTOR3 *>(&m_rotate), m_rotate_angle);
    vdx.get_matrix_stack()->ScaleLocal(m_scale.x, m_scale.y, m_scale.z);

    Model_Renderer_DX9 mrDX9;
    visit_nodes(mrDX9);

    vdx.get_matrix_stack()->Pop();
    vdx.get_d3d_device()->SetTransform(D3DTS_WORLD, vdx.get_matrix_stack()->GetTop());
  }

  void Model_Renderer_DX9::operator()(const Model &model, Lib3dsNode *node) {
    if(node->type != LIB3DS_OBJECT_NODE ||
      !strcmp(node->name, "$$$DUMMY"))
      return;

    Lib3dsMesh *mesh=lib3ds_file_mesh_by_name(model.get_file(), node->name);
    if(!mesh)
      throw Model_Render_Failure();

    if(!node->user.d)
      if(mesh->texels)
        create_vertex_buffer(new Vertex_Buffer_3FT_DX9(), model, node, mesh);
      else
        create_vertex_buffer(new Vertex_Buffer_3FC_DX9(), model, node, mesh);

    Vertex_Buffer *user_d = reinterpret_cast<Vertex_Buffer *>(node->user.d);
    if(!mesh)
      throw Model_Render_Failure();

    Lib3dsObjectData *data=&node->data.object;
    Video_DX9 &vdx = dynamic_cast<Video_DX9 &>(Video::get_reference());

    vdx.get_matrix_stack()->Push();

    vdx.get_matrix_stack()->MultMatrixLocal(reinterpret_cast<D3DXMATRIX *>(node->matrix));

    {//vdx.get_matrix_stack()->TranslateLocal(-data->pivot[0], -data->pivot[1], -data->pivot[2]);
      if (lib3ds_matrix_det(node->matrix) < 0.0) {
        vdx.get_matrix_stack()->ScaleLocal(-1.0, 1.0, 1.0);
        vdx.get_matrix_stack()->TranslateLocal(data->pivot[0], -data->pivot[1], -data->pivot[2]);
      }
      else
        vdx.get_matrix_stack()->TranslateLocal(-data->pivot[0], -data->pivot[1], -data->pivot[2]);
    }

    Lib3dsMatrix M;
    lib3ds_matrix_copy(M, mesh->matrix);
    lib3ds_matrix_inv(M);
    vdx.get_matrix_stack()->MultMatrixLocal(reinterpret_cast<D3DXMATRIX *>(M));

    vdx.get_d3d_device()->SetTransform(D3DTS_WORLD, vdx.get_matrix_stack()->GetTop());

    user_d->render();

    vdx.get_matrix_stack()->Pop();
    vdx.get_d3d_device()->SetTransform(D3DTS_WORLD, vdx.get_matrix_stack()->GetTop());
  }

#endif

  void Model_Extents::operator()(const Model &model, Lib3dsNode *node) {
    if(node->type != LIB3DS_OBJECT_NODE ||
      !strcmp(node->name, "$$$DUMMY"))
      return;

    Lib3dsMesh *mesh=lib3ds_file_mesh_by_name(model.get_file(), node->name);

    if(!mesh)
      return;

    for(Lib3dsPoint *point=&mesh->pointL[0], *end=point+mesh->points; point != end; ++point) {
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

}
