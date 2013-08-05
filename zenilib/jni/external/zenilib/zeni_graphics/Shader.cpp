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

#include <iostream>

#ifndef DISABLE_CG

static CGerror cgCheckError() {
  const CGerror err = cgGetError();
  if(err != CG_NO_ERROR)
    std::cerr << cgGetErrorString(err) << std::endl;
  return err;
}

#include <Zeni/Singleton.hxx>

namespace Zeni {

  template class Singleton<Shader_System>;

  Shader_System * Shader_System::create() {
    return new Shader_System;
  }

  Singleton<Shader_System>::Uninit Shader_System::g_uninit;
  Singleton<Shader_System>::Reinit Shader_System::g_reinit;

  Shader_System::Shader_System()
    : m_context(cgCreateContext())
  {
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();

    get_Video().initialize(*this);

    cgSetAutoCompile(m_context, CG_COMPILE_MANUAL);

    Video::remove_post_reinit(&g_reinit);

    Video &vr = get_Video();

    vr.lend_pre_uninit(&g_uninit);
    vr.lend_post_reinit(&g_reinit);
  }

  Shader_System::~Shader_System() {
    cgD3D9SetDevice(0);
    cgDestroyContext(m_context);
  }

  Shader_System & get_Shader_System() {
    return Singleton<Shader_System>::get();
  }
  
#ifndef DISABLE_GL_FIXED
  void Shader_System::init(Video_GL_Fixed &) {
    m_cg_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();

    m_cg_fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();
  }
#endif
  
#ifndef DISABLE_GL_SHADER
  void Shader_System::init(Video_GL_Shader &) {
    m_cg_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();

    m_cg_fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();
  }
#endif

#ifndef DISABLE_DX9
  void Shader_System::init(Video_DX9 &screen) {
    cgD3D9SetDevice(screen.get_d3d_device());

    m_cg_vertex_profile = cgD3D9GetLatestVertexProfile();
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();

    m_cg_fragment_profile = cgD3D9GetLatestPixelProfile();
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();
  }
#endif

  void Shader::compile() {
    if(!cgIsProgramCompiled(m_program))
      cgCompileProgram(m_program);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Init_Failure();
  }
  
  Shader::Shader()
    : m_program(0)
  {
  }

  Shader::~Shader() {
    cgDestroyProgram(m_program);
  }
  
#ifndef DISABLE_GL_FIXED
  void Shader::init(const String &filename, const String &entry_function, const CGprofile &profile, Video_GL_Fixed &) {
    Shader_System &shader_system = get_Shader_System();
    const CGcontext &context = shader_system.get_context();

    cgGLSetOptimalOptions(profile);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Init_Failure();

    m_program =
      cgCreateProgramFromFile(
        context,                /* Cg runtime context */
        CG_SOURCE,              /* Program in human-readable form */
        filename.c_str(),       /* Name of file containing program */
        profile,                /* Profile: OpenGL ARB vertex program */
        entry_function.c_str(), /* Entry function name */
        NULL);                  /* Probably no extra compiler options */
    if(cgCheckError() != CG_NO_ERROR) {
      std::cerr << "Error initializing '" << filename.c_str() << "'\n";
      std::cerr << "Cg error: " << cgGetLastListing(context) << std::endl;
      throw Shader_Init_Failure();
    }
  }

  void Shader::load(Video_GL_Fixed &) {
    compile();

    cgGLLoadProgram(m_program);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Init_Failure();
  }

  void Shader::set(const CGprofile &profile, Video_GL_Fixed &screen) const {
    if(!cgGLIsProgramLoaded(m_program))
      const_cast<Shader &>(*this).load(screen);

    cgGLBindProgram(m_program);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();
    cgGLEnableProfile(profile);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();
  }

  void Shader::unset(const CGprofile &profile, Video_GL_Fixed &) const {
    cgGLUnbindProgram(profile);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();
    cgGLDisableProfile(profile);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();
  }
#endif

#ifndef DISABLE_GL_SHADER
  void Shader::init(const String &filename, const String &entry_function, const CGprofile &profile, Video_GL_Shader &) {
    Shader_System &shader_system = get_Shader_System();
    const CGcontext &context = shader_system.get_context();

    cgGLSetOptimalOptions(profile);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Init_Failure();

    m_program =
      cgCreateProgramFromFile(
        context,                /* Cg runtime context */
        CG_SOURCE,              /* Program in human-readable form */
        filename.c_str(),       /* Name of file containing program */
        profile,                /* Profile: OpenGL ARB vertex program */
        entry_function.c_str(), /* Entry function name */
        NULL);                  /* Probably no extra compiler options */
    if(cgCheckError() != CG_NO_ERROR) {
      std::cerr << "Error initializing '" << filename.c_str() << "'\n";
      std::cerr << "Cg error: " << cgGetLastListing(context) << std::endl;
      throw Shader_Init_Failure();
    }
  }

  void Shader::load(Video_GL_Shader &) {
    compile();

    cgGLLoadProgram(m_program);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Init_Failure();
  }

  void Shader::set(const CGprofile &profile, Video_GL_Shader &screen) const {
    if(!cgGLIsProgramLoaded(m_program))
      const_cast<Shader &>(*this).load(screen);

    cgGLBindProgram(m_program);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();
    cgGLEnableProfile(profile);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();
  }

  void Shader::unset(const CGprofile &profile, Video_GL_Shader &) const {
    cgGLUnbindProgram(profile);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();
    cgGLDisableProfile(profile);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();
  }
#endif

#ifndef DISABLE_DX9
  void Shader::init(const String &filename, const String &entry_function, const CGprofile &profile, Video_DX9 &) {
    Shader_System &shader_system = get_Shader_System();
    const CGcontext &context = shader_system.get_context();

    const char **options = cgD3D9GetOptimalOptions(profile);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Init_Failure();

    m_program =
      cgCreateProgramFromFile(
        context,                /* Cg runtime context */
        CG_SOURCE,              /* Program in human-readable form */
        filename.c_str(),       /* Name of file containing program */
        profile,                /* Profile: OpenGL ARB vertex program */
        entry_function.c_str(), /* Entry function name */
        options);               /* Probably no extra compiler options */
    if(cgCheckError() != CG_NO_ERROR) {
      std::cerr << "Error initializing '" << filename.c_str() << "'\n";
      std::cerr << "Cg error: " << cgGetLastListing(context) << std::endl;
      throw Shader_Init_Failure();
    }
  }

  void Shader::load(Video_DX9 &) {
    compile();

    cgD3D9LoadProgram(m_program, false, 0);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Init_Failure();
  }

  void Shader::set(const CGprofile &, Video_DX9 &screen) const {
    if(!cgD3D9IsProgramLoaded(m_program))
      const_cast<Shader &>(*this).load(screen);

    cgD3D9BindProgram(m_program);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();
  }

  void Shader::unset(const CGprofile &, Video_DX9 &) const {
    cgD3D9UnbindProgram(m_program);
    if(cgCheckError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();
  }
#endif

  //void Shader::initialize_parameter(const String &parameter_name) {
  //  if(m_parameters.find(parameter_name) != m_parameters.end())
  //    return;

  //  const CGparameter to = cgGetNamedParameter(m_program, parameter_name.c_str());
  //  if(cgCheckError() != CG_NO_ERROR)
  //    throw Shader_Parameter_Error();

  //  const CGtype type = cgGetParameterType(to);
  //  if(cgCheckError() != CG_NO_ERROR)
  //    throw Shader_Parameter_Error();

  //  const CGparameter from = cgCreateParameter(get_Shader_System().get_context(), type);
  //  if(cgCheckError() != CG_NO_ERROR)
  //    throw Shader_Parameter_Error();

  //  m_parameters[parameter_name] = std::make_pair(from, to);
  //}

  //CGparameter Shader::get_from_parameter(const String &parameter_name) {
  //  Parameters::iterator it = m_parameters.find(parameter_name);

  //  if(it == m_parameters.end()) {
  //    initialize_parameter(parameter_name);
  //    it = m_parameters.find(parameter_name);
  //  }

  //  return it->second.first;
  //}

  //void Shader::connect_parameter(const String &parameter_name) {
  //  Parameters::iterator it = m_parameters.find(parameter_name);

  //  assert(it != m_parameters.end());

  //  cgConnectParameter(it->second.first, it->second.second);
  //  if(cgCheckError() != CG_NO_ERROR)
  //    throw Shader_Parameter_Error();
  //}

}

#else

namespace Zeni {
  void * this_pointer_is_cg_dead_beef = (void *)0xDEADBEEF;
}

#endif
