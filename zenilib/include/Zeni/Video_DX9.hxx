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


#ifndef ZENI_VIDEO_DX9_HXX
#define ZENI_VIDEO_DX9_HXX

// HXXed below
#include <Zeni/Fog.h>
#include <Zeni/Game.h>
#include <Zeni/Light.h>
#include <Zeni/Render_Wrapper.h>
#include <Zeni/Textures.h>
#include <Zeni/Video.h>

#include <Zeni/Video_DX9.h>

// Not HXXed
#include <Zeni/Fonts.h>
#include <Zeni/Vertex_Buffer.h>

#ifndef DISABLE_DX9

namespace Zeni {

  void Video_DX9::render_impl(const Renderable &renderable) {
    renderable.render_to(*this);
  }

  int Video_DX9::get_maximum_anisotropy_impl() const {
    return m_d3d_capabilities.MaxAnisotropy;
  }

  bool Video_DX9::has_vertex_buffers_impl() const {
    return true;
  }

  void Video_DX9::set_2d_view_impl(const std::pair<Point2f, Point2f> & /*camera2d*/, const std::pair<Point2i, Point2i> & /*viewport*/) {
    m_3d = false;

    Matrix4f world = Matrix4f::Identity();
    D3DXMATRIX * const world_ptr = reinterpret_cast<D3DXMATRIX *>(&world);

    m_d3d_device->SetTransform(D3DTS_WORLD, world_ptr);
    m_matrix_stack->LoadMatrix(world_ptr);

    set_fvf();
  }

  void Video_DX9::set_3d_view_impl(const Camera & /*camera*/, const std::pair<Point2i, Point2i> & /*viewport*/) {
    m_3d = true;

    Matrix4f world = Matrix4f::Identity();
    D3DXMATRIX * const world_ptr = reinterpret_cast<D3DXMATRIX *>(&world);

    m_d3d_device->SetTransform(D3DTS_WORLD, world_ptr);
    m_matrix_stack->LoadMatrix(world_ptr);

    set_fvf();
  }

  void Video_DX9::set_backface_culling_impl(const bool &on) {
    if(on)
      m_d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    else
      m_d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  }

  void Video_DX9::set_vertical_sync_impl(const bool & /*on*/) {
    uninit();
    init();
  }

  void Video_DX9::set_zwrite_impl(const bool &enabled) {
    m_d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, enabled);
  }

  void Video_DX9::set_ztest_impl(const bool &enabled) {
    if(enabled) {
      m_d3d_device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
      m_d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    }
    else
      m_d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
  }

  void Video_DX9::set_color_impl(const Color &color) {
    m_d3d_device->SetRenderState(D3DRS_TEXTUREFACTOR, color.get_argb());
  }

  void Video_DX9::apply_texture_impl(const unsigned long &id) {
    Textures::get_reference().apply_texture(id);

    m_textured = true;

    set_fvf();
  }

  void Video_DX9::apply_texture_impl(const Texture &texture) {
    m_textured = true;

    texture.apply_texture();

    set_fvf();
  }

  void Video_DX9::unapply_texture_impl() {
    m_textured = false;

    m_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    m_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    m_d3d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    m_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

    set_fvf();
  }

  void Video_DX9::set_lighting_impl(const bool &on) {
    m_d3d_device->SetRenderState(D3DRS_LIGHTING, on);
    m_d3d_device->SetRenderState(D3DRS_SPECULARENABLE, on);

    set_fvf();
  }

  void Video_DX9::set_normal_interpolation_impl(const bool & /*on*/) {
    /* !!TODO!! */
  }

  void Video_DX9::set_ambient_lighting_impl(const Color &color) {
    m_ambient_color = color;
    m_d3d_device->SetRenderState(D3DRS_AMBIENT, color.get_argb());
  }

  void Video_DX9::set_light_impl(const int &number, const Light *light) {
    if(light)
      light->set(number, *this);
    else
      m_d3d_device->LightEnable(number, FALSE);
  }

  void Video_DX9::set_material_impl(const Material &material, const int &optimization) {
    material.set(*this, optimization);
  }

  void Video_DX9::unset_material_impl(const Material &material, const int &optimization) {
    material.unset(*this, optimization);
  }

  void Video_DX9::set_fog_impl(const Fog * const fog) {
    if(fog) {
      m_d3d_device->SetRenderState(D3DRS_FOGENABLE, true);
      fog->set(*this);
    }
    else {
      m_d3d_device->SetRenderState(D3DRS_FOGENABLE, false);
      m_d3d_device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
    }
  }

  void Video_DX9::push_world_stack_impl() {
    get_matrix_stack()->Push();
  }

  void Video_DX9::pop_world_stack_impl() {
    get_matrix_stack()->Pop();
    m_d3d_device->SetTransform(D3DTS_WORLD, m_matrix_stack->GetTop());
  }

  void Video_DX9::translate_scene_impl(const Vector3f &direction) {
    m_matrix_stack->TranslateLocal(direction.i, direction.j, direction.k);
    m_d3d_device->SetTransform(D3DTS_WORLD, m_matrix_stack->GetTop());
  }

  void Video_DX9::rotate_scene_impl(const Vector3f &about, const float &radians) {
    m_matrix_stack->RotateAxisLocal(reinterpret_cast<const D3DXVECTOR3 *>(&about), radians);
    m_d3d_device->SetTransform(D3DTS_WORLD, m_matrix_stack->GetTop());
  }

  void Video_DX9::scale_scene_impl(const Vector3f &factor) {
    m_matrix_stack->ScaleLocal(factor.i, factor.j, factor.k);
    m_d3d_device->SetTransform(D3DTS_WORLD, m_matrix_stack->GetTop());
  }

  void Video_DX9::transform_scene_impl(const Matrix4f &transformation) {
    m_matrix_stack->MultMatrixLocal(reinterpret_cast<const D3DXMATRIX * const>(&transformation));
    m_d3d_device->SetTransform(D3DTS_WORLD, m_matrix_stack->GetTop());
  }

  Point2f Video_DX9::get_pixel_offset_impl() const {
    return Point2f(0.5f, 0.5f);
  }

  void Video_DX9::set_view_matrix_impl(const Matrix4f &view) {
    m_d3d_device->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DXMATRIX *>(const_cast<Matrix4f *>(&view)));
  }

  void Video_DX9::set_projection_matrix_impl(const Matrix4f &projection) {
    m_d3d_device->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DXMATRIX *>(const_cast<Matrix4f *>(&projection)));
  }

  void Video_DX9::set_viewport_impl(const std::pair<Point2i, Point2i> &viewport) {
    D3DVIEWPORT9 vp = {viewport.first.x, viewport.first.y, viewport.second.x - viewport.first.x, viewport.second.y - viewport.first.y, 0, 1};
    m_d3d_device->SetViewport(&vp);
  }

  Texture * Video_DX9::load_Texture_impl(const std::string &filename, const bool &repeat) {
    return new Texture_DX9(filename, repeat);
  }

  Texture * Video_DX9::create_Texture_impl(SDL_Surface * const &surface, const bool &repeat) {
    return new Texture_DX9(surface, repeat);
  }

  Font * Video_DX9::create_Font_impl(const std::string &filename, const bool &bold, const bool &italic, const int &glyph_height) {
    return new Font_FT(filename, bold, italic, glyph_height);
  }

  Vertex_Buffer * Video_DX9::create_Vertex_Buffer_impl() {
    return new Vertex_Buffer_DX9();
  }

  void Video_DX9::uninit_impl() {
    destroy_device();

    if(m_d3d)
      m_d3d->Release();
    m_d3d = 0;
  }

  const D3DCAPS9 & Video_DX9::get_d3d_capabilities() {
    return m_d3d_capabilities;
  }

  LPDIRECT3D9 & Video_DX9::get_d3d() {
    return m_d3d;
  }

  LPDIRECT3DDEVICE9 & Video_DX9::get_d3d_device() {
    return m_d3d_device;
  }

  LPD3DXMATRIXSTACK & Video_DX9::get_matrix_stack() {
    return m_matrix_stack;
  }
  
  const int & Video_DX9::get_dpi() {
    return m_dpi;
  }

  float Video_DX9::get_dpi_ratio() {
    return m_dpi / 96.0f;
  }

}

#endif

#include <Zeni/Fog.hxx>
#include <Zeni/Fonts.h>
#include <Zeni/Game.hxx>
#include <Zeni/Light.hxx>
#include <Zeni/Render_Wrapper.hxx>
#include <Zeni/Textures.hxx>
#include <Zeni/Video.hxx>

#endif
