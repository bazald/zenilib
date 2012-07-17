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

#ifndef DISABLE_DX9

#include <SDL/SDL_syswm.h>

#include <cassert>
#include <iostream>

#include <d3d9.h>
#include <d3dx9.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace Zeni {

  Video_DX9::Video_DX9()
    : m_d3d_capabilities(0),
    m_d3d_parameters(0),
    m_d3d(0),
    m_d3d_device(0),
    m_matrix_stack(0),
    m_begun_render(false),
    m_textured(false),
    m_fvf_3d(false),
    m_render_target(0),
    m_render_to_surface(0),
    m_back_buffer(0)
  {
    m_d3d9 = LoadLibrary("d3d9.dll");
    if(!m_d3d9) {
      std::cerr << "Loading d3d9.dll failed." << std::endl;

      throw Video_Init_Failure();
    }
    
    g_Direct3DCreate9 = (Direct3DCreate9_fcn)GetProcAddress(m_d3d9, "Direct3DCreate9");
    if(!g_Direct3DCreate9) {
      std::cerr << "Loading d3d9.dll failed." << std::endl;

      FreeLibrary(m_d3d9);

      zero_handles();

      throw Video_Init_Failure();
    }

    m_d3dx9 = LoadLibrary("D3DX9_43.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("D3DX9_42.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("D3DX9_41.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("D3DX9_40.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("D3DX9_39.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("D3DX9_38.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("D3DX9_37.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("d3dx9_36.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("d3dx9_35.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("d3dx9_34.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("d3dx9_33.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("d3dx9_32.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("d3dx9_31.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("d3dx9_30.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("d3dx9_29.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("d3dx9_28.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("d3dx9_27.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("d3dx9_26.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("d3dx9_25.dll");
    if(!m_d3dx9) m_d3dx9 = LoadLibrary("d3dx9_24.dll");
    if(!m_d3dx9) {
      std::cerr << "Loading d3dx9.dll failed." << std::endl;

      FreeLibrary(m_d3d9);

      zero_handles();

      throw Video_Init_Failure();
    }

    g_D3DXCreateRenderToSurface = (D3DXCreateRenderToSurface_fcn)GetProcAddress(m_d3dx9, "D3DXCreateRenderToSurface");
    g_D3DXCreateTexture = (D3DXCreateTexture_fcn)GetProcAddress(m_d3dx9, "D3DXCreateTexture");
    g_D3DXFilterTexture = (D3DXFilterTexture_fcn)GetProcAddress(m_d3dx9, "D3DXFilterTexture");
    g_D3DXCreateMatrixStack = (D3DXCreateMatrixStack_fcn)GetProcAddress(m_d3dx9, "D3DXCreateMatrixStack");
    if(!g_D3DXCreateRenderToSurface || !g_D3DXCreateTexture ||
       !g_D3DXFilterTexture || !g_D3DXCreateMatrixStack)
    {
      std::cerr << "Loading d3dx9.dll failed." << std::endl;

      FreeLibrary(m_d3dx9);
      FreeLibrary(m_d3d9);

      zero_handles();

      throw Video_Init_Failure();
    }

    m_d3d = Direct3DCreate9()(D3D_SDK_VERSION);
    if(!m_d3d) {
      FreeLibrary(m_d3dx9);
      FreeLibrary(m_d3d9);

      zero_handles();

      throw Video_Init_Failure();
    }

    try {
      init();
    }
    catch(...) {
      delete m_d3d_parameters;
      delete m_d3d_capabilities;

      FreeLibrary(m_d3dx9);
      FreeLibrary(m_d3d9);

      zero_handles();

      throw Video_Init_Failure();
    }
  }

  Video_DX9::~Video_DX9() {
    destroy_device();

    if(m_d3d) {
      m_d3d->Release();
      m_d3d = 0;
    }

    delete m_d3d_parameters;
    delete m_d3d_capabilities;

    FreeLibrary(m_d3dx9);
    FreeLibrary(m_d3d9);

    zero_handles();
  }

  static bool g_video_dx9_reset = false;

  bool Video_DX9::begin_prerender() {
    assert(!m_render_target && !m_render_to_surface);

    if(g_video_dx9_reset) {
      const HRESULT result = m_d3d_device->TestCooperativeLevel();
      
      if(result == D3DERR_DEVICELOST)
        return false;
      else if(result == D3DERR_DRIVERINTERNALERROR)
        throw Video_Device_Failure();
      
      if(result == D3DERR_DEVICENOTRESET) {
        if(m_back_buffer) {
          m_back_buffer->Release();
          m_back_buffer = 0;
        }

        if(FAILED(m_d3d_device->Reset(m_d3d_parameters)))
          throw Video_Device_Failure();

        g_video_dx9_reset = false;

        m_d3d_device->GetRenderTarget(0, &m_back_buffer);

        init_context();
      }
    }

    get_Textures().unlose_resources();
    get_Fonts().unlose_resources();

    return true;
  }

  bool Video_DX9::begin_render() {
    assert(!m_render_target && !m_render_to_surface);

    HRESULT result = m_begun_render ? m_d3d_device->Present(0, 0, 0, 0) : S_OK;

    if(result == S_OK) {
      m_begun_render = true;

      D3DVIEWPORT9 vp = {0, 0, DWORD(get_Window().get_width()), DWORD(get_Window().get_height()), 0, 1};
      m_d3d_device->SetViewport(&vp);
      m_d3d_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(get_clear_Color().r_ub(), get_clear_Color().g_ub(), get_clear_Color().b_ub()), 1.0f, 0);
      m_d3d_device->BeginScene();

      return true;
    }
    else if(result == D3DERR_DEVICELOST) {
      g_video_dx9_reset = true;
      return false;
    }
    else if(result == D3DERR_DEVICEREMOVED) {
      throw Video_Device_Failure();
    }

    return false;
  }

  void Video_DX9::end_render() {
    m_d3d_device->EndScene();
  }

  void Video_DX9::render(const Renderable &renderable) {
    class PrePostRenderActor {
      PrePostRenderActor & operator=(const PrePostRenderActor &) {return *this;}

    public:
      PrePostRenderActor(const Renderable &renderable_)
        : renderable(renderable_)
      {
        renderable.pre_render();
      }

      ~PrePostRenderActor() {
        renderable.post_render();
      }
    private:
      const Renderable &renderable;
    } ppra(renderable);

    set_fvf(renderable.is_3d());

    renderable.render_to(*this);
  }

  int Video_DX9::get_maximum_anisotropy() const {
    return int(m_d3d_capabilities->MaxAnisotropy);
  }

  bool Video_DX9::has_vertex_buffers() const {
    return true;
  }

  void Video_DX9::set_2d_view(const std::pair<Point2f, Point2f> &camera2d, const std::pair<Point2i, Point2i> &viewport, const bool &fix_aspect_ratio) {
    Video::set_2d_view(camera2d, viewport, fix_aspect_ratio);

    Matrix4f world = Matrix4f::Scale(Vector3f(1.0f, 1.0f, 0.5f)) *
                     Matrix4f::Translate(Vector3f(0.0f, 0.0f, 1.0f));
    D3DXMATRIX * const world_ptr = reinterpret_cast<D3DXMATRIX *>(&world);

    m_d3d_device->SetTransform(D3DTS_WORLD, world_ptr);
    m_matrix_stack->LoadMatrix(world_ptr);
  }

  void Video_DX9::set_3d_view(const Camera &camera, const std::pair<Point2i, Point2i> &viewport) {
    Video::set_3d_view(camera, viewport);

    Matrix4f world = Matrix4f::Identity();
    D3DXMATRIX * const world_ptr = reinterpret_cast<D3DXMATRIX *>(&world);

    m_d3d_device->SetTransform(D3DTS_WORLD, world_ptr);
    m_matrix_stack->LoadMatrix(world_ptr);
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

    destroy_device();
    init();
  }

  void Video_DX9::set_zwrite(const bool &enabled) {
    Video::set_zwrite(enabled);

    m_d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, DWORD(enabled));
  }

  void Video_DX9::set_ztest(const bool &enabled) {
    Video::set_ztest(enabled);

    if(enabled) {
      m_d3d_device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
      m_d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    }
    else
      m_d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
  }

  void Video_DX9::set_alpha_test(const bool &enabled,
                                 const TEST &test,
                                 const float &value)
  {
    Video::set_alpha_test(enabled, test, value);

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

  void Video_DX9::set_Color(const Color &color) {
    Video::set_Color(color);

    m_d3d_device->SetRenderState(D3DRS_TEXTUREFACTOR, color.get_argb());
  }

  void Video_DX9::apply_Texture(const unsigned long &id) {
    get_Textures().apply_Texture(id);

    m_textured = true;

    set_fvf();
  }

  void Video_DX9::apply_Texture(const Texture &texture) {
    texture.apply_Texture();

    m_textured = true;

    set_fvf();
  }

  void Video_DX9::unapply_Texture() {
    m_textured = false;

    m_d3d_device->SetTexture(0, 0);

    m_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    m_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    m_d3d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    m_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

    set_fvf();
  }

  void Video_DX9::set_lighting(const bool &on) {
    Video::set_lighting(on);

    m_d3d_device->SetRenderState(D3DRS_LIGHTING, DWORD(on));
    m_d3d_device->SetRenderState(D3DRS_SPECULARENABLE, DWORD(on));
  }

  void Video_DX9::set_ambient_lighting(const Color &color) {
    Video::set_ambient_lighting(color);

    m_d3d_device->SetRenderState(D3DRS_AMBIENT, color.get_argb());
  }

  void Video_DX9::set_Light(const int &number, const Light &light) {
    if(number < 0 || 7 < number)
      throw Light_Out_of_Range(); // Match OpenGL - Limit for both may actually be higher

    Video::set_Light(number, light);

    light.set(DWORD(number), *this);
  }

  void Video_DX9::unset_Light(const int &number) {
    if(number < 0 || 7 < number)
      throw Light_Out_of_Range(); // Match OpenGL - Limit for both may actually be higher

    Video::unset_Light(number);

    m_d3d_device->LightEnable(DWORD(number), FALSE);
  }

  void Video_DX9::set_Material(const Material &material) {
    material.set(*this);
  }

  void Video_DX9::unset_Material(const Material &material) {
    material.unset(*this);
  }

  void Video_DX9::set_Fog(const Fog &fog) {
    Video::set_Fog(fog);

    m_d3d_device->SetRenderState(D3DRS_FOGENABLE, true);
    fog.set(*this);
  }

  void Video_DX9::unset_Fog() {
    Video::unset_Fog();

    m_d3d_device->SetRenderState(D3DRS_FOGENABLE, false);
    m_d3d_device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
  }

#ifndef DISABLE_CG
  void Video_DX9::set_vertex_shader(const Vertex_Shader &shader) {
    shader.set(*this);
  }

  void Video_DX9::set_fragment_shader(const Fragment_Shader &shader) {
    shader.set(*this);
  }

  void Video_DX9::unset_vertex_shader(const Vertex_Shader &shader) {
    shader.unset(*this);
  }

  void Video_DX9::unset_fragment_shader(const Fragment_Shader &shader) {
    shader.unset(*this);
  }
#endif

  void Video_DX9::set_render_target(Texture &texture) {
    if(m_render_target)
      throw Video_Render_To_Texture_Error();

    Texture_DX9 &tdx = dynamic_cast<Texture_DX9 &>(texture);

    LPDIRECT3DSURFACE9 render_surface = 0;

    if(FAILED(tdx.m_texture->GetSurfaceLevel(0, &render_surface)))
      throw Video_Render_To_Texture_Error();

    m_render_to_surface = tdx.render_to_surface();

    if(!m_render_to_surface || FAILED(m_d3d_device->SetRenderTarget(0, render_surface)))
      throw Video_Render_To_Texture_Error();

    if(FAILED(m_d3d_device->BeginScene())) {
      m_d3d_device->SetRenderTarget(0, m_back_buffer);
      throw Video_Render_To_Texture_Error();
    }

    m_render_target = &tdx;
  }

  void Video_DX9::unset_render_target() {
    if(!m_render_target || !m_render_to_surface)
      throw Video_Render_To_Texture_Error();

    m_render_target = 0;

    m_d3d_device->EndScene();
    m_render_to_surface = 0;

    if(FAILED(m_d3d_device->SetRenderTarget(0, m_back_buffer)))
      throw Video_Render_To_Texture_Error();
  }

  void Video_DX9::clear_render_target(const Color &color) {
    const Point2i &render_target_size = get_render_target_size();
    set_viewport(std::make_pair(Point2i(0, 0), Point2i(render_target_size.x, render_target_size.y)));
    m_d3d_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(color.a_ub(), color.r_ub(), color.g_ub(), color.b_ub()), 1.0f, 0);
  }

  inline const Point2i & Video_DX9::get_render_target_size() const {
    if(m_render_target)
      return m_render_target->get_size();
    else
      return get_Window().get_size();
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

  Point2f Video_DX9::get_pixel_offset() const {
    return Point2f(0.5f, 0.5f);
  }

  void Video_DX9::set_view_matrix(const Matrix4f &view) {
    Video::set_view_matrix(view);

    m_d3d_device->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DXMATRIX *>(const_cast<Matrix4f *>(&view)));
  }

  void Video_DX9::set_projection_matrix(const Matrix4f &projection) {
    Video::set_projection_matrix(projection);

    m_d3d_device->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DXMATRIX *>(const_cast<Matrix4f *>(&projection)));
  }

  void Video_DX9::set_viewport(const std::pair<Point2i, Point2i> &viewport) {
    Video::set_viewport(viewport);

    D3DVIEWPORT9 vp = {DWORD(viewport.first.x), DWORD(viewport.first.y), DWORD(viewport.second.x - viewport.first.x), DWORD(viewport.second.y - viewport.first.y), 0u, 1u};
    m_d3d_device->SetViewport(&vp);
  }

  Texture * Video_DX9::load_Texture(const String &filename, const bool &repeat, const bool & /*lazy_loading*/ ) {
    return new Texture_DX9(filename, repeat);
  }

  Texture * Video_DX9::create_Texture(const Image &image) {
    return new Texture_DX9(image);
  }

  Texture * Video_DX9::create_Texture(const Point2i &size, const bool &repeat) {
    const Point2i &max = get_Window().get_size();

    Point2i corrected(size);
    while(corrected.x > max.x)
      corrected.x /= 2;
    while(corrected.y > max.y)
      corrected.y /= 2;

    return new Texture_DX9(corrected, repeat);
  }

  Font * Video_DX9::create_Font(const String &filename, const float &glyph_height, const float &virtual_screen_height) {
    return new Font_FT(filename, glyph_height, virtual_screen_height);
  }

  Vertex_Buffer_Renderer * Video_DX9::create_Vertex_Buffer_Renderer(Vertex_Buffer &vertex_buffer) {
    return new Vertex_Buffer_Renderer_DX9(vertex_buffer);
  }

#ifndef DISABLE_CG
  void Video_DX9::initialize(Shader_System &shader_system) {
    shader_system.init(*this);
  }

  void Video_DX9::initialize(Vertex_Shader &shader, const String &filename, const String &entry_function) {
    shader.init(filename, entry_function, get_Shader_System().get_vertex_profile(), *this);
  }

  void Video_DX9::initialize(Fragment_Shader &shader, const String &filename, const String &entry_function) {
    shader.init(filename, entry_function, get_Shader_System().get_fragment_profile(), *this);
  }
#endif

  const D3DCAPS9 & Video_DX9::get_d3d_capabilities() {
    return *m_d3d_capabilities;
  }

  const D3DPRESENT_PARAMETERS & Video_DX9::get_d3d_parameters() {
    return *m_d3d_parameters;
  }

  void Video_DX9::init() {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
#if SDL_VERSION_ATLEAST(1,3,0)
    SDL_GetWindowWMInfo(get_Window().get_window(), &wmInfo);
    HWND hWnd = wmInfo.win.window;
#else
    SDL_GetWMInfo(&wmInfo);
    HWND hWnd = wmInfo.window;
#endif

    std::cout << "Initializing DirectX 9" << std::endl;

    try {
      m_d3d_capabilities = new D3DCAPS9;
      m_d3d_parameters = new D3DPRESENT_PARAMETERS;
    }
    catch (...) {
      throw Video_Init_Failure();
    }

    m_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_d3d_capabilities);

    m_dpi = GetDeviceCaps(GetDC(hWnd), LOGPIXELSY);

    ZeroMemory(m_d3d_parameters, sizeof(m_d3d_parameters));

    m_d3d_parameters->hDeviceWindow = hWnd;
    
    m_d3d_parameters->Windowed = true;
    m_d3d_parameters->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

    m_d3d_parameters->BackBufferCount = 1;
    m_d3d_parameters->BackBufferWidth = UINT(get_Window().get_width());
    m_d3d_parameters->BackBufferHeight = UINT(get_Window().get_height());
    m_d3d_parameters->BackBufferFormat = D3DFMT_A8R8G8B8;
    m_d3d_parameters->SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_d3d_parameters->PresentationInterval = get_vertical_sync() ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    m_d3d_parameters->Flags = 0; //D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    m_d3d_parameters->EnableAutoDepthStencil = true;
    m_d3d_parameters->AutoDepthStencilFormat = D3DFMT_D16;

    if(get_multisampling() > 1)
      switch(get_multisampling()) {
      case 2: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_2_SAMPLES; break;
      case 3: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_3_SAMPLES; break;
      case 4: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_4_SAMPLES; break;
      case 5: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_5_SAMPLES; break;
      case 6: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_6_SAMPLES; break;
      case 7: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_7_SAMPLES; break;
      case 8: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_8_SAMPLES; break;
      case 9: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_9_SAMPLES; break;
      case 10: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_10_SAMPLES; break;
      case 11: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_11_SAMPLES; break;
      case 12: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_12_SAMPLES; break;
      case 13: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_13_SAMPLES; break;
      case 14: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_14_SAMPLES; break;
      case 15: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_15_SAMPLES; break;
      case 16:
      default: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_16_SAMPLES; break;
      }
    else if(get_multisampling() < 0)
      m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_16_SAMPLES;
    else
      m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_NONE;
    m_d3d_parameters->MultiSampleQuality = 0;

    // Initialize the D3D device
    if(!init_device())
      throw Video_Init_Failure();

    // Initialize the rendering context
    init_context();

    m_d3d_device->GetRenderTarget(0, &m_back_buffer);
  }
  
  bool Video_DX9::init_device() {
    DWORD num_quality_levels;
    while(FAILED(m_d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, m_d3d_parameters->Windowed, m_d3d_parameters->MultiSampleType, &num_quality_levels))) {
      switch(m_d3d_parameters->MultiSampleType) {
      case D3DMULTISAMPLE_2_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_NONE; preinit_multisampling(0); break;
      case D3DMULTISAMPLE_3_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_2_SAMPLES; preinit_multisampling(2); break;
      case D3DMULTISAMPLE_4_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_3_SAMPLES; preinit_multisampling(3); break;
      case D3DMULTISAMPLE_5_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_4_SAMPLES; preinit_multisampling(4); break;
      case D3DMULTISAMPLE_6_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_5_SAMPLES; preinit_multisampling(5); break;
      case D3DMULTISAMPLE_7_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_6_SAMPLES; preinit_multisampling(6); break;
      case D3DMULTISAMPLE_8_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_7_SAMPLES; preinit_multisampling(7); break;
      case D3DMULTISAMPLE_9_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_8_SAMPLES; preinit_multisampling(8); break;
      case D3DMULTISAMPLE_10_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_9_SAMPLES; preinit_multisampling(9); break;
      case D3DMULTISAMPLE_11_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_10_SAMPLES; preinit_multisampling(10); break;
      case D3DMULTISAMPLE_12_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_11_SAMPLES; preinit_multisampling(11); break;
      case D3DMULTISAMPLE_13_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_12_SAMPLES; preinit_multisampling(12); break;
      case D3DMULTISAMPLE_14_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_13_SAMPLES; preinit_multisampling(13); break;
      case D3DMULTISAMPLE_15_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_14_SAMPLES; preinit_multisampling(14); break;
      case D3DMULTISAMPLE_16_SAMPLES: m_d3d_parameters->MultiSampleType = D3DMULTISAMPLE_15_SAMPLES; preinit_multisampling(15); break;

      case D3DMULTISAMPLE_NONE:
      default:
        return false;
      }
    }

    const bool try_hardware = (m_d3d_capabilities->VertexProcessingCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0;

    if((!try_hardware || FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_d3d_parameters->hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, m_d3d_parameters, &m_d3d_device))) &&
       (!try_hardware || FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_d3d_parameters->hDeviceWindow, D3DCREATE_MIXED_VERTEXPROCESSING, m_d3d_parameters, &m_d3d_device))) &&
       FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_d3d_parameters->hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, m_d3d_parameters, &m_d3d_device)))
    {
      // HARDWARE, MIXED, and SOFTWARE all failed
      return false;
    }

    // Set Up Matrix Stack
    D3DXCreateMatrixStack()(0, &m_matrix_stack);
    m_matrix_stack->LoadIdentity();

    return true;
  }

  void Video_DX9::init_context() {
    // Enable Alpha Blitting
    m_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    m_d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_d3d_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    // Configure Texture Stages
    m_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    m_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    m_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    m_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

    // Set Lighting Variables
    m_d3d_device->SetRenderState(D3DRS_NORMALIZENORMALS, true);

    // Multisampling
    m_d3d_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, DWORD(get_multisampling() > 1));

    // More basic stuff
    set_2d();
    set_Color(get_Color());
    set_clear_Color(get_clear_Color());
    set_backface_culling(get_backface_culling());
    set_lighting(get_lighting());
    set_ambient_lighting(get_ambient_lighting());
    for(int i = 0; i != 8; ++i)
      if(const Light * const lp = get_Light(i))
        set_Light(i, *lp);
    if(const Fog * const fp = get_Fog())
      set_Fog(*fp);
    set_alpha_test(is_alpha_test_enabled(), get_alpha_test_function(), get_alpha_test_value());
    set_zwrite(is_zwrite_enabled());
    set_ztest(is_ztest_enabled());
  }

  void Video_DX9::destroy_device() {
    if(m_matrix_stack) {
      m_matrix_stack->Release();
      m_matrix_stack = 0;
    }

    if(m_back_buffer) {
      m_back_buffer->Release();
      m_back_buffer = 0;
    }

    if(m_d3d_device) {
      m_d3d_device->Release();
      m_d3d_device = 0;
    }
  }

  void Video_DX9::zero_handles() {
    g_Direct3DCreate9 = 0;

    g_D3DXCreateRenderToSurface = 0;
    g_D3DXCreateTexture = 0;
    g_D3DXFilterTexture = 0;
    g_D3DXCreateMatrixStack = 0;
  }

  void Video_DX9::set_fvf(const bool &is_3d) {
    m_fvf_3d = is_3d;

    set_fvf();
  }

  void Video_DX9::set_fvf() {
    DWORD fvf = D3DFVF_XYZ;

    if(m_fvf_3d)
      fvf |= D3DFVF_NORMAL;

    if(m_textured)
      fvf |= D3DFVF_TEX1;
    else
      fvf |= D3DFVF_DIFFUSE;

    m_d3d_device->SetFVF(fvf);
  }
  
  bool Video_DX9::is_fvf_3d() const {
    return m_fvf_3d;
  }

  void Video_DX9::set_fvf_3d(const bool &on) {
    m_fvf_3d = on;

    set_fvf();
  }

  Video_DX9::Direct3DCreate9_fcn Video_DX9::g_Direct3DCreate9 = 0;

  Video_DX9::D3DXCreateRenderToSurface_fcn Video_DX9::g_D3DXCreateRenderToSurface = 0;
  Video_DX9::D3DXCreateTexture_fcn Video_DX9::g_D3DXCreateTexture = 0;
  Video_DX9::D3DXFilterTexture_fcn Video_DX9::g_D3DXFilterTexture = 0;
  Video_DX9::D3DXCreateMatrixStack_fcn Video_DX9::g_D3DXCreateMatrixStack = 0;

}

#else

namespace Zeni {
  void * this_pointer_is_dead_beef = (void *)0xDEADBEEF;
}

#endif
