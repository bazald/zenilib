/* This file is part of the Zenipex Library.
* Copyleft (C) 2010 Mitchell Keith Bloch a.k.a. bazald
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
#include <Zeni/Color.h>
#include <Zeni/Game.h>
#include <Zeni/Light.h>
#include <Zeni/Material.h>
#include <Zeni/Renderable.h>
#include <Zeni/Shader.h>
#include <Zeni/Texture.h>
#include <Zeni/Textures.h>
#include <Zeni/Video.h>

#include <Zeni/Video_DX9.h>

// Not HXXed
#include <Zeni/Fog.h>
#include <Zeni/Fonts.h>
#include <Zeni/Vertex_Buffer.h>

#include <Zeni/Global.h>

#ifndef DISABLE_DX9

namespace Zeni {

  void Video_DX9::render_impl(const Renderable &renderable) {
    set_fvf(renderable.is_3d());

    renderable.render_to(*this);
  }

  int Video_DX9::get_maximum_anisotropy_impl() const {
    return int(m_d3d_capabilities.MaxAnisotropy);
  }

  bool Video_DX9::has_vertex_buffers_impl() const {
    return true;
  }

  void Video_DX9::set_2d_view_impl(const std::pair<Point2f, Point2f> & /*camera2d*/, const std::pair<Point2i, Point2i> & /*viewport*/, const bool & /*fix_aspect_ratio*/) {
    Matrix4f world = Matrix4f::Scale(Vector3f(1.0f, 1.0f, 0.5f)) *
                     Matrix4f::Translate(Vector3f(0.0f, 0.0f, 1.0f));
    D3DXMATRIX * const world_ptr = reinterpret_cast<D3DXMATRIX *>(&world);

    m_d3d_device->SetTransform(D3DTS_WORLD, world_ptr);
    m_matrix_stack->LoadMatrix(world_ptr);
  }

  void Video_DX9::set_3d_view_impl(const Camera & /*camera*/, const std::pair<Point2i, Point2i> & /*viewport*/) {
    Matrix4f world = Matrix4f::Identity();
    D3DXMATRIX * const world_ptr = reinterpret_cast<D3DXMATRIX *>(&world);

    m_d3d_device->SetTransform(D3DTS_WORLD, world_ptr);
    m_matrix_stack->LoadMatrix(world_ptr);
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
    m_d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, DWORD(enabled));
  }

  void Video_DX9::set_ztest_impl(const bool &enabled) {
    if(enabled) {
      m_d3d_device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
      m_d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    }
    else
      m_d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
  }

  void Video_DX9::set_alpha_test_impl(const bool &enabled,
                                      const TEST &test,
                                      const float &value) {
    D3DCMPFUNC func;

    switch(test) {
      case ZENI_NEVER:            func = D3DCMP_NEVER;        break;
      case ZENI_LESS:             func = D3DCMP_LESS;         break;
      case ZENI_EQUAL:            func = D3DCMP_EQUAL;        break;
      case ZENI_GREATER:          func = D3DCMP_GREATER;      break;
      case ZENI_NOT_EQUAL:        func = D3DCMP_NOTEQUAL;     break;
      case ZENI_LESS_OR_EQUAL:    func = D3DCMP_LESSEQUAL;    break;
      case ZENI_GREATER_OR_EQUAL: func = D3DCMP_GREATEREQUAL; break;
      case ZENI_ALWAYS:           func = D3DCMP_ALWAYS;       break;
      default:
        assert(false);
        return;
    }

    int ref = static_cast<int>(255.0f * value + 0.5f);
    if(ref < 0)
      ref = 0;
    else if(ref > 0xFF)
      ref = 0xFF;

    m_d3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, DWORD(enabled));
    m_d3d_device->SetRenderState(D3DRS_ALPHAREF, DWORD(ref));
    m_d3d_device->SetRenderState(D3DRS_ALPHAFUNC, func);
  }

  void Video_DX9::set_color_impl(const Color &color) {
    m_d3d_device->SetRenderState(D3DRS_TEXTUREFACTOR, color.get_argb());
  }

  void Video_DX9::apply_texture_impl(const unsigned long &id) {
    get_Textures().apply_texture(id);

    m_textured = true;

    set_fvf();
  }

  void Video_DX9::apply_texture_impl(const Texture &texture) {
    texture.apply_texture();

    m_textured = true;

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
    m_d3d_device->SetRenderState(D3DRS_LIGHTING, DWORD(on));
    m_d3d_device->SetRenderState(D3DRS_SPECULARENABLE, DWORD(on));
  }

  void Video_DX9::set_ambient_lighting_impl(const Color &color) {
    m_d3d_device->SetRenderState(D3DRS_AMBIENT, color.get_argb());
  }

  void Video_DX9::set_light_impl(const int &number, const Light &light) {
    if(number < 0 || 7 < number)
      throw Light_Out_of_Range(); // Match OpenGL - Limit for both may actually be higher

    light.set(DWORD(number), *this);
  }

  void Video_DX9::unset_light_impl(const int &number) {
    if(number < 0 || 7 < number)
      throw Light_Out_of_Range(); // Match OpenGL - Limit for both may actually be higher

    m_d3d_device->LightEnable(DWORD(number), FALSE);
  }

  void Video_DX9::set_material_impl(const Material &material) {
    material.set(*this);
  }

  void Video_DX9::unset_material_impl(const Material &material) {
    material.unset(*this);
  }

  void Video_DX9::set_fog_impl(const Fog &fog) {
    m_d3d_device->SetRenderState(D3DRS_FOGENABLE, true);
    fog.set(*this);
  }

  void Video_DX9::unset_fog_impl() {
    m_d3d_device->SetRenderState(D3DRS_FOGENABLE, false);
    m_d3d_device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
  }

#ifndef DISABLE_CG
  void Video_DX9::set_vertex_shader_impl(const Vertex_Shader &shader) {
    shader.set(*this);
  }

  void Video_DX9::set_fragment_shader_impl(const Fragment_Shader &shader) {
    shader.set(*this);
  }

  void Video_DX9::unset_vertex_shader_impl(const Vertex_Shader &shader) {
    shader.unset(*this);
  }

  void Video_DX9::unset_fragment_shader_impl(const Fragment_Shader &shader) {
    shader.unset(*this);
  }
#endif

  void Video_DX9::set_render_target_impl(Texture &texture) {
    if(m_render_target)
      throw Video_Render_To_Texture_Error();

    Texture_DX9 &tdx = dynamic_cast<Texture_DX9 &>(texture);

    LPDIRECT3DSURFACE9 render_surface = 0;

    if(FAILED(tdx.m_texture->GetSurfaceLevel(0, &render_surface)))
      throw Video_Render_To_Texture_Error();

    m_render_to_surface = tdx.render_to_surface();

    if(!m_render_to_surface || FAILED(m_render_to_surface->BeginScene(render_surface, 0)))
      throw Video_Render_To_Texture_Error();

    m_render_target = &tdx;
  }

  void Video_DX9::unset_render_target_impl() {
    if(!m_render_target || !m_render_to_surface)
      throw Video_Render_To_Texture_Error();

    m_render_target = 0;

    m_render_to_surface->EndScene(D3DX_FILTER_NONE);
    m_render_to_surface = 0;
  }

  void Video_DX9::clear_render_target_impl(const Color &color) {
    const Point2i &render_target_size = get_render_target_size();
    set_viewport(std::make_pair(Point2i(0, 0), Point2i(render_target_size.x, render_target_size.y)));
    m_d3d_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(color.a_ub(), color.r_ub(), color.g_ub(), color.b_ub()), 1.0f, 0);
  }

  inline const Point2i & Video_DX9::get_render_target_size_impl() const {
    if(m_render_target)
      return m_render_target->get_size();
    else
      return get_screen_size();
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
    D3DVIEWPORT9 vp = {DWORD(viewport.first.x), DWORD(viewport.first.y), DWORD(viewport.second.x - viewport.first.x), DWORD(viewport.second.y - viewport.first.y), 0u, 1u};
    m_d3d_device->SetViewport(&vp);
  }

  Texture * Video_DX9::load_Texture_impl(const std::string &filename, const bool &repeat, const bool & /*lazy_loading*/ ) {
    return new Texture_DX9(filename, repeat);
  }

  Texture * Video_DX9::create_Texture_impl(SDL_Surface * const &surface, const bool &repeat) {
    return new Texture_DX9(surface, repeat);
  }

  Texture * Video_DX9::create_Texture_impl(const Point2i &size, const bool &repeat) {
    return new Texture_DX9(size, repeat);
  }

  Font * Video_DX9::create_Font_impl(const std::string &filename, const bool &bold, const bool &italic, const float &glyph_height, const float &virtual_screen_height) {
    return new Font_FT(filename, bold, italic, glyph_height, virtual_screen_height);
  }

  Vertex_Buffer_Renderer * Video_DX9::create_Vertex_Buffer_Renderer_impl(Vertex_Buffer &vertex_buffer) {
    return new Vertex_Buffer_Renderer_DX9(vertex_buffer);
  }

#ifndef DISABLE_CG
  void Video_DX9::initialize_impl(Shader_System &shader_system) {
    shader_system.init(*this);
  }

  void Video_DX9::initialize_impl(Vertex_Shader &shader, const std::string &filename, const std::string &entry_function) {
    shader.init(filename, entry_function, get_Shader_System().get_vertex_profile(), *this);
  }

  void Video_DX9::initialize_impl(Fragment_Shader &shader, const std::string &filename, const std::string &entry_function) {
    shader.init(filename, entry_function, get_Shader_System().get_fragment_profile(), *this);
  }
#endif

  void Video_DX9::uninit_impl() {
    destroy_device();
  }

  const D3DCAPS9 & Video_DX9::get_d3d_capabilities() {
    return m_d3d_capabilities;
  }

  const D3DPRESENT_PARAMETERS & Video_DX9::get_d3d_parameters() {
    return m_d3d_parameters;
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
    return m_dpi / ZENI_STANDARD_DPI;
  }

}

#endif

#include <Zeni/Global_Undef.h>

#include <Zeni/Color.hxx>
#include <Zeni/Game.hxx>
#include <Zeni/Light.hxx>
#include <Zeni/Material.hxx>
#include <Zeni/Renderable.hxx>
#include <Zeni/Shader.hxx>
#include <Zeni/Texture.hxx>
#include <Zeni/Textures.hxx>
#include <Zeni/Video.hxx>

#endif
