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

#include <Zeni/Video_DX9.hxx>

#ifndef DISABLE_DX9

#include <Zeni/Font.h>
#include <Zeni/Light.h>
#include <Zeni/Render_Wrapper.h>
#include <Zeni/Textures.h>
#include <Zeni/Vertex_Buffer.h>

#include <Zeni/Camera.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Game.hxx>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Video.hxx>

#include <iostream>

using namespace std;

namespace Zeni {

  Video_DX9::Video_DX9()
    : m_d3d(0),
    m_d3d_device(0),
    m_matrix_stack(0), 
    m_color(0.0f, 0.0f, 0.0f, 1.0f),
    m_clear_color(0.0f, 0.0f, 0.0f, 1.0f),
    m_textured(false),
    m_3d(false)
  {
    init();
  }

  Video_DX9::~Video_DX9() {
    uninit();
  }

  void Video_DX9::render_all() {
    D3DVIEWPORT9 vp = {0, 0, get_screen_width(), get_screen_height(), 0, 1};
    m_d3d_device->SetViewport(&vp);
    m_d3d_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(m_clear_color.r_ub(), m_clear_color.g_ub(), m_clear_color.b_ub()), 1.0f, 0);
    m_d3d_device->BeginScene();

    Game::get_reference().render();

    m_d3d_device->EndScene();
    m_d3d_device->Present(NULL,NULL,NULL,NULL);
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

  void Video_DX9::set_color_to(const Color &color) {
    m_color = color;
  }

  void Video_DX9::set_clear_color_to(const Color &color) {
    m_clear_color = color;
  }

  void Video_DX9::apply_texture(const std::string &name) {
    Textures::get_reference().apply_texture(name);

    m_textured = true;

    set_fvf();
  }

  void Video_DX9::unapply_texture() {
    m_textured = false;

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
        &i = forward.i(), &j = forward.j(), &k = forward.k();

      D3DXMATRIX world, scale, rot, trans;
      D3DXMatrixIdentity(&world);
      m_d3d_device->SetTransform(D3DTS_WORLD, &world);

      D3DXVECTOR3 initial(x, y, z), 
        terminal(x + i, y + j, z + k),
        cross(up.i(), up.j(), up.k());
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
    }
    else {
      m_d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
      m_d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

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
    m_d3d_device->SetRenderState(D3DRS_AMBIENT, color.get_argb());
  }

  void Video_DX9::set_light(const int &number, const Light *light) {
    if(light)
      light->set(number, *this);
    else
      m_d3d_device->LightEnable(number, FALSE);
  }

  void Video_DX9::set_material(const Material &material) {
    material.set(*this);
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
    m_matrix_stack->TranslateLocal(direction.i(), direction.j(), direction.k());
    m_d3d_device->SetTransform(D3DTS_WORLD, m_matrix_stack->GetTop());
  }

  void Video_DX9::rotate_scene(const Vector3f &about, const float &radians) {
    m_matrix_stack->RotateAxisLocal(reinterpret_cast<const D3DXVECTOR3 *>(&about), radians);
    m_d3d_device->SetTransform(D3DTS_WORLD, m_matrix_stack->GetTop());
  }

  void Video_DX9::scale_scene(const Vector3f &factor) {
    m_matrix_stack->ScaleLocal(factor.i(), factor.j(), factor.k());
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

  Texture * Video_DX9::load_Texture(const std::string &name, const std::string &filename) {
    return new Texture_DX9(name, filename, *this);
  }

  Font * Video_DX9::create_Font(const std::string &filename, const bool &bold, const bool &italic, const int &glyph_height) {
    return new Font_DX9(filename, bold, italic, glyph_height);
  }

  Vertex_Buffer_3FC * Video_DX9::create_Vertex_Buffer_3FC() {
    return new Vertex_Buffer_3FC_DX9();
  }

  Vertex_Buffer_3FT * Video_DX9::create_Vertex_Buffer_3FT() {
    return new Vertex_Buffer_3FT_DX9();
  }

  void Video_DX9::init() {
    set_opengl_flag(false);
    Video::init();

    cout << "Initializing DirectX 9" << endl;

    m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if(!m_d3d)
      throw Video_Init_Failure();

    m_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_d3d_capabilities);

    D3DPRESENT_PARAMETERS d3d_parameters;
    ZeroMemory(&d3d_parameters, sizeof(d3d_parameters));

    d3d_parameters.hDeviceWindow = GetActiveWindow();
    d3d_parameters.Windowed = !is_fullscreen();
    d3d_parameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

    d3d_parameters.BackBufferCount = 1;
    d3d_parameters.BackBufferWidth = get_screen_width();
    d3d_parameters.BackBufferHeight = get_screen_height();
    d3d_parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3d_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3d_parameters.PresentationInterval = get_vertical_sync() ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    d3d_parameters.Flags = 0;//D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    d3d_parameters.EnableAutoDepthStencil = true;
    d3d_parameters.AutoDepthStencilFormat = D3DFMT_D16;

    if(get_multisampling() > 1)
      switch(get_multisampling()) {
      case 2: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES; break;
      case 3: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_3_SAMPLES; break;
      case 4: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES; break;
      case 5: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_5_SAMPLES; break;
      case 6: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_6_SAMPLES; break;
      case 7: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_7_SAMPLES; break;
      case 8: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES; break;
      case 9: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_9_SAMPLES; break;
      case 10: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_10_SAMPLES; break;
      case 11: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_11_SAMPLES; break;
      case 12: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_12_SAMPLES; break;
      case 13: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_13_SAMPLES; break;
      case 14: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_14_SAMPLES; break;
      case 15: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_15_SAMPLES; break;
      case 16:
      default: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES; break;
      }
    else if(get_multisampling() == -1)
      d3d_parameters.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES;
    else
      d3d_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3d_parameters.MultiSampleQuality = 0;

    DWORD num_quality_levels;
    while(FAILED(m_d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, d3d_parameters.Windowed, d3d_parameters.MultiSampleType, &num_quality_levels))) {
      if(d3d_parameters.MultiSampleType <= D3DMULTISAMPLE_2_SAMPLES) {
        d3d_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
        break;
      }

      switch(d3d_parameters.MultiSampleType) {
      case D3DMULTISAMPLE_3_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES; break;
      case D3DMULTISAMPLE_4_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_3_SAMPLES; break;
      case D3DMULTISAMPLE_5_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES; break;
      case D3DMULTISAMPLE_6_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_5_SAMPLES; break;
      case D3DMULTISAMPLE_7_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_6_SAMPLES; break;
      case D3DMULTISAMPLE_8_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_7_SAMPLES; break;
      case D3DMULTISAMPLE_9_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES; break;
      case D3DMULTISAMPLE_10_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_9_SAMPLES; break;
      case D3DMULTISAMPLE_11_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_10_SAMPLES; break;
      case D3DMULTISAMPLE_12_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_11_SAMPLES; break;
      case D3DMULTISAMPLE_13_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_12_SAMPLES; break;
      case D3DMULTISAMPLE_14_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_13_SAMPLES; break;
      case D3DMULTISAMPLE_15_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_14_SAMPLES; break;
      case D3DMULTISAMPLE_16_SAMPLES: d3d_parameters.MultiSampleType = D3DMULTISAMPLE_15_SAMPLES; break;
      default: throw Video_Init_Failure();
      }
    }

    if(FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3d_parameters.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3d_parameters, &m_d3d_device)) &&
      FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3d_parameters.hDeviceWindow, D3DCREATE_MIXED_VERTEXPROCESSING, &d3d_parameters, &m_d3d_device)) &&
      FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3d_parameters.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3d_parameters, &m_d3d_device))) {

        // HARDWARE, MIXED, and SOFTWARE all failed

        uninit();

        throw Video_Init_Failure();
    }

    // Set Up Matrix Stack
    D3DXCreateMatrixStack(0, &m_matrix_stack);
    m_matrix_stack->LoadIdentity();

    // Enable Alpha Blitting
    m_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    m_d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_d3d_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    // Set Lighting Variables
    m_d3d_device->SetRenderState(D3DRS_NORMALIZENORMALS, true);

    // Multisampling
    m_d3d_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, get_multisampling() > 1);

    // Finish with a few function calls
    set_3d(Camera());
    set_backface_culling();
    set_lighting(false);
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

}

#else

namespace Zeni {
  void * this_pointer_is_dead_beef = (void *)0xDEADBEEF;
}

#endif
