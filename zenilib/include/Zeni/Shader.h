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

#ifdef ZENI_INLINES
#include <Zeni/Shader.hxx>
#endif

#ifndef ZENI_SHADER_H
#define ZENI_SHADER_H

#include <Zeni/Core.h>

#include <Cg/cg.h>

#ifndef DISABLE_DX9
#include <Cg/cgD3D9.h>
#endif

#ifndef DISABLE_GL
#include <Cg/cgGL.h>
#endif

namespace Zeni {

  class Video_DX9;
  class Video_GL;

  class Shader_System {
    // Get reference to only instance;
    friend Shader_System & get_Shader_System(); ///< Get access to the singleton.

  public:
    Shader_System();
    ~Shader_System();

    inline const CGcontext & get_context() const;
    inline const CGprofile & get_vertex_profile() const;
    inline const CGprofile & get_fragment_profile() const;

#ifndef DISABLE_GL
    void init(Video_GL &screen);
#endif

#ifndef DISABLE_DX9
    void init(Video_DX9 &screen);
#endif

  private:
    CGcontext m_context;
    CGprofile m_cg_vertex_profile;
    CGprofile m_cg_fragment_profile;
  };

  class Vertex_Shader {
    Vertex_Shader(const Vertex_Shader &);
    Vertex_Shader & operator=(const Vertex_Shader &);

  public:
#ifndef DISABLE_GL
    Vertex_Shader(const std::string &filename, const std::string &entry_function, Video_GL &screen);
    void set(Video_GL &screen) const;
    void unset(Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    Vertex_Shader(const std::string &filename, const std::string &entry_function, Video_DX9 &screen);
    void reload(Video_DX9 &screen);
    void set(Video_DX9 &screen) const;
    void unset(Video_DX9 &screen) const;
#endif

    ~Vertex_Shader();

  private:
    CGprogram m_vertex_program;
  };

  class Fragment_Shader {
    Fragment_Shader(const Fragment_Shader &);
    Fragment_Shader & operator=(const Fragment_Shader &);

  public:
#ifndef DISABLE_GL
    Fragment_Shader(const std::string &filename, const std::string &entry_function, Video_GL &screen);
    void set(Video_GL &screen) const;
    void unset(Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    Fragment_Shader(const std::string &filename, const std::string &entry_function, Video_DX9 &screen);
    void reload(Video_DX9 &screen);
    void set(Video_DX9 &screen) const;
    void unset(Video_DX9 &screen) const;
#endif

    ~Fragment_Shader();

  private:
    CGprogram m_fragment_program;
  };

  struct Shader_System_Init_Failure : public Error {
    Shader_System_Init_Failure() : Error("Zeni Shader System Failed to Initialize Correctly") {}
  };

  struct Vertex_Shader_Init_Failure : public Error {
    Vertex_Shader_Init_Failure() : Error("Zeni Vertex Shader Failed to Initialize Correctly") {}
  };

  struct Fragment_Shader_Init_Failure : public Error {
    Fragment_Shader_Init_Failure() : Error("Zeni Fragment Shader Failed to Initialize Correctly") {}
  };

  struct Vertex_Shader_Bind_Failure : public Error {
    Vertex_Shader_Bind_Failure() : Error("Zeni Vertex Shader Failed to Bind Correctly") {}
  };

  struct Fragment_Shader_Bind_Failure : public Error {
    Fragment_Shader_Bind_Failure() : Error("Zeni Fragment Shader Failed to Bind Correctly") {}
  };

}

#endif
