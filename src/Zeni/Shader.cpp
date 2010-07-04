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

#include <Zeni/Shader.hxx>

#include <Zeni/Video.hxx>
#include <iostream>

#ifndef DISABLE_CG

namespace Zeni {

  Shader_System::Shader_System()
    : m_context(cgCreateContext())
  {
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();

    get_Video().initialize(*this);

    cgSetAutoCompile(m_context, CG_COMPILE_MANUAL);
  }

  Shader_System::~Shader_System() {
    cgDestroyContext(m_context);
  }

  Shader_System & get_Shader_System() {
    static Shader_System e_shader_system;
    return e_shader_system;
  }
  
#ifndef DISABLE_GL
  void Shader_System::init(Video_GL &) {
    m_cg_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();

    m_cg_fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();
  }
#endif

#ifndef DISABLE_DX9
  void Shader_System::init(Video_DX9 &screen) {
    cgD3D9SetDevice(screen.get_d3d_device());

    m_cg_vertex_profile = cgD3D9GetLatestVertexProfile();
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();

    m_cg_fragment_profile = cgD3D9GetLatestPixelProfile();
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();
  }
#endif

  void Shader::compile() {
    if(!cgIsProgramCompiled(m_program))
      cgCompileProgram(m_program);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Init_Failure();
  }
  
  Shader::Shader()
    : m_program(0)
  {
  }

  Shader::~Shader() {
    cgDestroyProgram(m_program);
  }

#ifndef DISABLE_GL
  void Shader::init(const std::string &filename, const std::string &entry_function, const CGprofile &profile, Video_GL &) {
    Shader_System &shader_system = get_Shader_System();
    const CGcontext &context = shader_system.get_context();

    cgGLSetOptimalOptions(profile);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Init_Failure();

    m_program =
      cgCreateProgramFromFile(
        context,                /* Cg runtime context */
        CG_SOURCE,              /* Program in human-readable form */
        filename.c_str(),       /* Name of file containing program */
        profile,                /* Profile: OpenGL ARB vertex program */
        entry_function.c_str(), /* Entry function name */
        NULL);                  /* Probably no extra compiler options */
    if(cgGetError() != CG_NO_ERROR) {
      std::cerr << "Error initializing '" << filename.c_str() << "'\n";
      throw Shader_Init_Failure();
    }
  }

  void Shader::load(Video_GL &) {
    compile();

    cgGLLoadProgram(m_program);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Init_Failure();
  }

  void Shader::set(const CGprofile &profile, Video_GL &screen) const {
#define GLSET cgGLBindProgram(m_program); \
              if(cgGetError() != CG_NO_ERROR) \
                throw Shader_Bind_Failure(); \
              cgGLEnableProfile(profile); \
              if(cgGetError() != CG_NO_ERROR) \
                throw Shader_Bind_Failure();

    try
    {
      GLSET;
    }
    catch(Shader_Bind_Failure &)
    {
      const_cast<Shader &>(*this).load(screen);

      GLSET;
    }

#undef GLSET
  }

  void Shader::unset(const CGprofile &profile, Video_GL &) const {
    cgGLDisableProfile(profile);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();
  }
#endif

#ifndef DISABLE_DX9
  void Shader::init(const std::string &filename, const std::string &entry_function, const CGprofile &profile, Video_DX9 &) {
    Shader_System &shader_system = get_Shader_System();
    const CGcontext &context = shader_system.get_context();

    const char **options = cgD3D9GetOptimalOptions(profile);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Init_Failure();

    m_program =
      cgCreateProgramFromFile(
        context,                /* Cg runtime context */
        CG_SOURCE,              /* Program in human-readable form */
        filename.c_str(),       /* Name of file containing program */
        profile,                /* Profile: OpenGL ARB vertex program */
        entry_function.c_str(), /* Entry function name */
        options);               /* Probably no extra compiler options */
    if(cgGetError() != CG_NO_ERROR) {
      std::cerr << "Error initializing '" << filename.c_str() << "'\n";
      throw Shader_Init_Failure();
    }
  }

  void Shader::load(Video_DX9 &) {
    compile();

    cgD3D9LoadProgram(m_program, false, 0);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Init_Failure();
  }

  void Shader::set(const CGprofile &, Video_DX9 &screen) const {
#define DXSET cgD3D9BindProgram(m_program); \
              if(cgGetError() != CG_NO_ERROR) \
                throw Shader_Bind_Failure();

    try
    {
      DXSET;
    }
    catch(Shader_Bind_Failure &)
    {
      const_cast<Shader &>(*this).load(screen);

      DXSET;
    }

#undef DXSET
  }

  void Shader::unset(const CGprofile &, Video_DX9 &) const {
  }
#endif

  void Shader::initialize_parameter(const std::string &parameter_name) {
    if(m_parameters.find(parameter_name) != m_parameters.end())
      return;

    const CGparameter to = cgGetNamedParameter(m_program, parameter_name.c_str());
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Parameter_Error();

    const CGtype type = cgGetParameterType(to);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Parameter_Error();

    const CGparameter from = cgCreateParameter(get_Shader_System().get_context(), type);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Parameter_Error();

    m_parameters[parameter_name] = std::make_pair(from, to);
  }

  CGparameter Shader::get_from_parameter(const std::string &parameter_name) {
    Parameters::iterator it = m_parameters.find(parameter_name);

    if(it == m_parameters.end()) {
      initialize_parameter(parameter_name);
      it = m_parameters.find(parameter_name);
    }

    return it->second.first;
  }

  void Shader::connect_parameter(const std::string &parameter_name) {
    Parameters::iterator it = m_parameters.find(parameter_name);

    assert(it != m_parameters.end());

    cgConnectParameter(it->second.first, it->second.second);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Parameter_Error();
  }

}

#else

namespace Zeni {
  void * this_pointer_is_cg_dead_beef = (void *)0xDEADBEEF;
}

#endif
