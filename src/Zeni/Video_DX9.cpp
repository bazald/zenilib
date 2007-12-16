/* This file is part of the Zenipex Library.
* Copyleft (C) 2007 Mitchell Keith Bloch a.k.a. bazald
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

#include <Zeni/Video_DX9.hxx>

#ifndef DISABLE_DX9

#include <Zeni/Fonts.h>
#include <Zeni/Light.h>
#include <Zeni/Render_Wrapper.h>
#include <Zeni/Textures.h>
#include <Zeni/Vertex_Buffer.h>

#include <Zeni/Camera.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Game.hxx>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Matrix4f.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Video.hxx>

#include <iostream>

using namespace std;

namespace Zeni {

  Video_DX9::Video_DX9()
    : m_d3d(0),
    m_d3d_device(0),
    m_matrix_stack(0), 
    m_color(1.0f, 1.0f, 1.0f, 1.0f),
    m_clear_color(1.0f, 0.0f, 0.0f, 0.0f),
    m_ambient_color(1.0f, 1.0f, 1.0f, 1.0f),
    m_textured(false),
    m_3d(false)
  {
    init();
  }

  Video_DX9::~Video_DX9() {
    uninit();
  }

  void Video_DX9::render_all() {
    static bool reset = false;

    if(reset) {
      if(FAILED(m_d3d_device->Reset(&m_d3d_parameters)))
        return;
      reset = false;

      reinit();

      Textures::get_reference().reload();
      Fonts::get_reference().reload();
    }
    else if(m_d3d_device->Present(0, 0, 0, 0) == D3DERR_DEVICELOST) {
      reset = true;
      Fonts::get_reference().lose_resources();
      Textures::get_reference().lose_resources();
      return;
    }

    D3DVIEWPORT9 vp = {0, 0, get_screen_width(), get_screen_height(), 0, 1};
    m_d3d_device->SetViewport(&vp);
    m_d3d_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(m_clear_color.r_ub(), m_clear_color.g_ub(), m_clear_color.b_ub()), 1.0f, 0);
    m_d3d_device->BeginScene();

    Game::get_reference().render();

    m_d3d_device->EndScene();
  }

  void Video_DX9::render(const Renderable &renderable) {
    renderable.render_to(*this);
  }

  int Video_DX9::get_maximum_anisotropy() const {
    return m_d3d_capabilities.MaxAnisotropy;
  }

  bool Video_DX9::has_vertex_buffers() const {
    return true;
  }

  bool Video_DX9::zwrite_enabled() const {
    return m_zwrite;
  }

  void Video_DX9::set_color_to(const Color &color) {
    m_color = color;
  }

  void Video_DX9::set_clear_color_to(const Color &color) {
    m_clear_color = color;
  }

  void Video_DX9::apply_texture(const std::string &name) {
    Video_DX9::apply_texture(Textures::get_reference().get_texture_id(name));
  }

  void Video_DX9::apply_texture(const unsigned long &id) {
    Textures::get_reference().apply_texture(id);

    m_textured = true;

    set_fvf();
  }

  void Video_DX9::apply_texture(const Texture &texture) {
    texture.apply_texture();

    m_textured = true;

    set_fvf();
  }

  void Video_DX9::unapply_texture() {
    m_textured = false;

    m_d3d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    m_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

    set_fvf();
  }

  void Video_DX9::set_3d_view(const Camera &camera, const bool &on, const std::pair<Point2i, Point2i> &viewport) {
    m_3d = on;
    if(m_3d) {
      // Set Camera
      const Point3f &position = camera.get_position();
      const Vector3f &forward = camera.get_forward().normalized(),
        up = camera.get_up().normalized();
      const float
        &x = position.x, &y = position.y, &z = position.z,
        &i = forward.i, &j = forward.j, &k = forward.k;

      D3DXMATRIX world, scale, rot, trans;
      D3DXMatrixIdentity(&world);
      m_d3d_device->SetTransform(D3DTS_WORLD, &world);

      D3DXVECTOR3 initial(x, y, z), 
        terminal(x + i, y + j, z + k),
        cross(up.i, up.j, up.k);
      D3DXMATRIX view;
      D3DXMatrixLookAtRH(&view, &initial, &terminal, &cross);
      m_d3d_device->SetTransform(D3DTS_VIEW, &view);

      D3DXMATRIX projection;
      D3DXMatrixPerspectiveFovRH(&projection, camera.get_fov_rad(), float(viewport.second.x - viewport.first.x) / (viewport.second.y - viewport.first.y), camera.get_near_clip(), camera.get_far_clip());
      m_d3d_device->SetTransform(D3DTS_PROJECTION, &projection);

      // Enable Depth Buffer
      m_d3d_device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
      m_d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
      m_d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
      m_zwrite = true;
    }
    else {
      m_d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
      m_d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
      m_zwrite = false;

      D3DXMATRIX identity;
      D3DXMatrixIdentity(&identity);

      m_d3d_device->SetTransform(D3DTS_WORLD, &identity);

      m_d3d_device->SetTransform(D3DTS_VIEW, &identity);

      D3DXMatrixOrthoOffCenterRH(&identity, 0, float(get_screen_width()), float(get_screen_height()), 0, 0, 1);
      m_d3d_device->SetTransform(D3DTS_PROJECTION, &identity);
    }

    D3DVIEWPORT9 vp = {viewport.first.x, viewport.first.y, viewport.second.x - viewport.first.x, viewport.second.y - viewport.first.y, 0, 1};
    m_d3d_device->SetViewport(&vp);

    D3DXMATRIX world;
    m_d3d_device->GetTransform(D3DTS_WORLD, &world);
    m_matrix_stack->LoadMatrix(&world);

    set_fvf();
  }

  void Video_DX9::set_lighting(const bool &on) {
    Video::set_lighting(on);

    m_d3d_device->SetRenderState(D3DRS_LIGHTING, on);
    m_d3d_device->SetRenderState(D3DRS_SPECULARENABLE, on);

    set_fvf();
  }

  void Video_DX9::set_normal_interpolation(const bool &on) {
    Video::set_normal_interpolation(on);

    /* !!TODO!! */
  }

  void Video_DX9::set_ambient_lighting(const Color &color) {
    m_ambient_color = color;
    m_d3d_device->SetRenderState(D3DRS_AMBIENT, color.get_argb());
  }

  void Video_DX9::set_light(const int &number, const Light *light) {
    if(light)
      light->set(number, *this);
    else
      m_d3d_device->LightEnable(number, FALSE);
  }

  void Video_DX9::set_material(const Material &material, const int &optimization) {
    material.set(*this, optimization);
  }

  void Video_DX9::unset_material(const Material &material, const int &optimization) {
    material.unset(*this, optimization);
  }

  void Video_DX9::select_world_matrix() {
  }

  void Video_DX9::push_world_stack() {
    get_matrix_stack()->Push();
  }

  void Video_DX9::pop_world_stack() {
    get_matrix_stack()->Pop();
    m_d3d_device->SetTransform(D3DTS_WORLD, m_matrix_stack->GetTop());
  }

  void Video_DX9::translate_scene(const Vector3f &direction) {
    m_matrix_stack->TranslateLocal(direction.i, direction.j, direction.k);
    m_d3d_device->SetTransform(D3DTS_WORLD, m_matrix_stack->GetTop());
  }

  void Video_DX9::rotate_scene(const Vector3f &about, const float &radians) {
    m_matrix_stack->RotateAxisLocal(reinterpret_cast<const D3DXVECTOR3 *>(&about), radians);
    m_d3d_device->SetTransform(D3DTS_WORLD, m_matrix_stack->GetTop());
  }

  void Video_DX9::scale_scene(const Vector3f &factor) {
    m_matrix_stack->ScaleLocal(factor.i, factor.j, factor.k);
    m_d3d_device->SetTransform(D3DTS_WORLD, m_matrix_stack->GetTop());
  }

  void Video_DX9::transform_scene(const Matrix4f &transformation) {
    m_matrix_stack->MultMatrixLocal(reinterpret_cast<const D3DXMATRIX * const>(&transformation));
    m_d3d_device->SetTransform(D3DTS_WORLD, m_matrix_stack->GetTop());
  }

  void Video_DX9::set_backface_culling(const bool &on) {
    Video::set_backface_culling(on);

    if(on)
      m_d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    else
      m_d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  }

  void Video_DX9::set_vertical_sync(const bool &on) {
    Video::set_vertical_sync(on);

    uninit();
    init();
  }

  void Video_DX9::set_zwrite(const bool &enabled) {
    m_zwrite = enabled;

    if(m_zwrite)
      m_d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    else 
      m_d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
  }

  Texture * Video_DX9::load_Texture(const std::string &filename, const bool &repeat) {
    return new Texture_DX9(filename, repeat, *this);
  }

  Font * Video_DX9::create_Font(const std::string &filename, const bool &bold, const bool &italic, const int &glyph_height) {
    return new Font_DX9(filename, bold, italic, glyph_height);
  }

  Vertex_Buffer * Video_DX9::create_Vertex_Buffer() {
    return new Vertex_Buffer_DX9();
  }

  void Video_DX9::init() {
    set_opengl_flag(false);
    Video::init();

    cout << "Initializing DirectX 9" << endl;

    m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if(!m_d3d)
      throw Video_Init_Failure();

    m_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_d3d_capabilities);

    ZeroMemory(&m_d3d_parameters, sizeof(m_d3d_parameters));

    m_d3d_parameters.hDeviceWindow = GetActiveWindow();
    m_d3d_parameters.Windowed = !is_fullscreen();
    m_d3d_parameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

    m_d3d_parameters.BackBufferCount = 1;
    m_d3d_parameters.BackBufferWidth = get_screen_width();
    m_d3d_parameters.BackBufferHeight = get_screen_height();
    m_d3d_parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
    m_d3d_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_d3d_parameters.PresentationInterval = get_vertical_sync() ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    m_d3d_parameters.Flags = 0;//D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    m_d3d_parameters.EnableAutoDepthStencil = true;
    m_d3d_parameters.AutoDepthStencilFormat = D3DFMT_D16;

    if(get_multisampling() > 1)
      switch(get_multisampling()) {
      case 2: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES; break;
      case 3: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_3_SAMPLES; break;
      case 4: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES; break;
      case 5: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_5_SAMPLES; break;
      case 6: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_6_SAMPLES; break;
      case 7: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_7_SAMPLES; break;
      case 8: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES; break;
      case 9: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_9_SAMPLES; break;
      case 10: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_10_SAMPLES; break;
      case 11: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_11_SAMPLES; break;
      case 12: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_12_SAMPLES; break;
      case 13: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_13_SAMPLES; break;
      case 14: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_14_SAMPLES; break;
      case 15: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_15_SAMPLES; break;
      case 16:
      default: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES; break;
      }
    else if(get_multisampling() == -1)
      m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES;
    else
      m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
    m_d3d_parameters.MultiSampleQuality = 0;

    DWORD num_quality_levels;
    while(FAILED(m_d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, m_d3d_parameters.Windowed, m_d3d_parameters.MultiSampleType, &num_quality_levels))) {
      if(m_d3d_parameters.MultiSampleType <= D3DMULTISAMPLE_2_SAMPLES) {
        m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
        break;
      }

      switch(m_d3d_parameters.MultiSampleType) {
      case D3DMULTISAMPLE_3_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES; break;
      case D3DMULTISAMPLE_4_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_3_SAMPLES; break;
      case D3DMULTISAMPLE_5_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES; break;
      case D3DMULTISAMPLE_6_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_5_SAMPLES; break;
      case D3DMULTISAMPLE_7_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_6_SAMPLES; break;
      case D3DMULTISAMPLE_8_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_7_SAMPLES; break;
      case D3DMULTISAMPLE_9_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES; break;
      case D3DMULTISAMPLE_10_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_9_SAMPLES; break;
      case D3DMULTISAMPLE_11_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_10_SAMPLES; break;
      case D3DMULTISAMPLE_12_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_11_SAMPLES; break;
      case D3DMULTISAMPLE_13_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_12_SAMPLES; break;
      case D3DMULTISAMPLE_14_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_13_SAMPLES; break;
      case D3DMULTISAMPLE_15_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_14_SAMPLES; break;
      case D3DMULTISAMPLE_16_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_15_SAMPLES; break;
      default: throw Video_Init_Failure();
      }
    }

    if(FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_d3d_parameters.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3d_parameters, &m_d3d_device)) &&
      FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_d3d_parameters.hDeviceWindow, D3DCREATE_MIXED_VERTEXPROCESSING, &m_d3d_parameters, &m_d3d_device)) &&
      FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_d3d_parameters.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3d_parameters, &m_d3d_device))) {

        // HARDWARE, MIXED, and SOFTWARE all failed

        uninit();

        throw Video_Init_Failure();
    }

    // Set Up Matrix Stack
    D3DXCreateMatrixStack(0, &m_matrix_stack);
    m_matrix_stack->LoadIdentity();

    // Finish with a few function calls
    set_backface_culling(false);
    set_lighting(false);

    reinit();
  }

  void Video_DX9::reinit() {
    // Enable Alpha Blitting
    m_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    m_d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_d3d_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    // Set Lighting Variables
    m_d3d_device->SetRenderState(D3DRS_NORMALIZENORMALS, true);

    // Multisampling
    m_d3d_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, get_multisampling() > 1);

    // More basic stuff
    set_2d();
    set_color_to(m_color);
    set_clear_color_to(m_clear_color);
    set_ambient_lighting(m_ambient_color);
    set_backface_culling(get_backface_culling());
  }

  void Video_DX9::uninit() {
    if(m_matrix_stack)
      m_matrix_stack->Release();
    m_matrix_stack = 0;

    if(m_d3d_device)
      m_d3d_device->Release();
    m_d3d_device = 0;

    if(m_d3d)
      m_d3d->Release();
    m_d3d = 0;

    Video::uninit();
  }

  void Video_DX9::set_fvf() {
    DWORD fvf = D3DFVF_XYZ;

    if(m_3d)
      fvf |= D3DFVF_NORMAL;

    if(m_textured)
      fvf |= D3DFVF_TEX1;
    else if(!get_lighting())
      fvf |= D3DFVF_DIFFUSE;

    m_d3d_device->SetFVF(fvf);
  }
  
  bool Video_DX9::get_3d() const {
    return m_3d;
  }

  void Video_DX9::set_3d(const bool &on) {
    m_3d = on;

    set_fvf();
  }

}

#else

namespace Zeni {
  void * this_pointer_is_dead_beef = (void *)0xDEADBEEF;
}

#endif
