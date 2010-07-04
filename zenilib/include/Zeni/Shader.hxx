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

#ifndef ZENI_SHADER_HXX
#define ZENI_SHADER_HXX

// HXXed below
#include <Zeni/Video.h>

#include <Zeni/Shader.h>

#ifndef DISABLE_CG

namespace Zeni {

  const CGcontext & Shader_System::get_context() const {
    return m_context;
  }

  const CGprofile & Shader_System::get_vertex_profile() const {
    return m_cg_vertex_profile;
  }

  const CGprofile & Shader_System::get_fragment_profile() const {
    return m_cg_fragment_profile;
  }

  Vertex_Shader::Vertex_Shader(const std::string &filename, const std::string &entry_function) {
    get_Video().initialize(*this, filename, entry_function);
  }

#ifndef DISABLE_GL
  void Vertex_Shader::set(Video_GL &screen) const {
    Shader::set(get_Shader_System().get_vertex_profile(), screen);
  }

  void Vertex_Shader::unset(Video_GL &screen) const {
    Shader::unset(get_Shader_System().get_vertex_profile(), screen);
  }
#endif

#ifndef DISABLE_DX9
  void Vertex_Shader::set(Video_DX9 &screen) const {
    Shader::set(get_Shader_System().get_vertex_profile(), screen);
  }

  void Vertex_Shader::unset(Video_DX9 &screen) const {
    Shader::unset(get_Shader_System().get_vertex_profile(), screen);
  }
#endif

  Fragment_Shader::Fragment_Shader(const std::string &filename, const std::string &entry_function) {
    get_Video().initialize(*this, filename, entry_function);
  }

#ifndef DISABLE_GL
  void Fragment_Shader::set(Video_GL &screen) const {
    Shader::set(get_Shader_System().get_fragment_profile(), screen);
  }

  void Fragment_Shader::unset(Video_GL &screen) const {
    Shader::unset(get_Shader_System().get_fragment_profile(), screen);
  }
#endif

#ifndef DISABLE_DX9
  void Fragment_Shader::set(Video_DX9 &screen) const {
    Shader::set(get_Shader_System().get_fragment_profile(), screen);
  }

  void Fragment_Shader::unset(Video_DX9 &screen) const {
    Shader::unset(get_Shader_System().get_fragment_profile(), screen);
  }
#endif

}

#endif

#include <Zeni/Video.hxx>

#endif
