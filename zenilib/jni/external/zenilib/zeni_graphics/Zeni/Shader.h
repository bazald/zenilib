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

/**
 * \class Zeni::Shader_System
 *
 * \ingroup zenilib
 *
 * \brief A rendering context for Cg
 *
 * This Singleton is used by Shaders and should not need direct interaction.
 * As this functionality is currently incomplete, its design will need to
 * be reassessed later.
 * Perhaps it is not even correct to use the Singleton patter.  (Though it
 * has made initial development simpler.)
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Shader
 *
 * \ingroup zenilib
 *
 * \brief A base class for Vertex_Shader and Fragment_Shader
 *
 * Functionality shared between Vertex_Shader and Fragment_Shader is stored here.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Vertex_Shader
 *
 * \ingroup zenilib
 *
 * \brief A shader for manipulating vertex data as it is streamed through the GPU.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Fragment_Shader
 *
 * \ingroup zenilib
 *
 * \brief A shader for manipulating fragment/pixel data as it is streamed through the GPU.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_SHADER_H
#define ZENI_SHADER_H

#include <Zeni/Core.h>
#include <Zeni/Hash_Map.h>

#ifndef DISABLE_GL
#if defined(REQUIRE_GL_ES)
#include <GLES/gl.h>
#else
#include <GL/glew.h>
#endif

typedef struct ID3DXConstantTable *LPD3DXCONSTANTTABLE;
typedef struct IDirect3DVertexShader9 *LPDIRECT3DVERTEXSHADER9;
typedef struct IDirect3DPixelShader9 *LPDIRECT3DPIXELSHADER9;

namespace Zeni {

  class ZENI_GRAPHICS_DLL Video_DX9;
  class ZENI_GRAPHICS_DLL Video_GL_Fixed;
  class ZENI_GRAPHICS_DLL Video_GL_Shader;

  class ZENI_GRAPHICS_DLL Shader {
    Shader(const Shader &);
    Shader & operator=(const Shader &);

  public:
    enum Type {VERTEX, FRAGMENT};

    Shader() {}
    virtual ~Shader() {}
  };

  class ZENI_GRAPHICS_DLL Program {
    Program(const Program &);
    Program & operator=(const Program &);

  public:
    Program() {}
    virtual ~Program() {}

    virtual void attach(Shader &shader) = 0;
    virtual void link() = 0;
  };
  
#ifndef DISABLE_GL_FIXED
  class ZENI_GRAPHICS_DLL Shader_GL_Fixed : public Shader {
    Shader_GL_Fixed(const Shader_GL_Fixed &);
    Shader_GL_Fixed & operator=(const Shader_GL_Fixed &);

  public:
    Shader_GL_Fixed(const String &shader_src, const Type &type);
    virtual ~Shader_GL_Fixed();

    inline GLuint get() const;

  private:
    GLuint m_shader;
  };

  class ZENI_GRAPHICS_DLL Program_GL_Fixed : public Program {
    Program_GL_Fixed(const Program_GL_Fixed &);
    Program_GL_Fixed & operator=(const Program_GL_Fixed &);

  public:
    Program_GL_Fixed();
    ~Program_GL_Fixed();

    void attach(Shader &shader);
    void link();

    inline GLuint get() const;

  private:
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::list<Shader_GL_Fixed *> m_shaders;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
    GLuint m_program;
  };
#endif
  
#ifndef DISABLE_GL_SHADER
  class ZENI_GRAPHICS_DLL Shader_GL_Shader : public Shader {
    Shader_GL_Shader(const Shader_GL_Shader &);
    Shader_GL_Shader & operator=(const Shader_GL_Shader &);

  public:
    Shader_GL_Shader(const String &shader_src, const Type &type);
    ~Shader_GL_Shader();

    inline GLuint get() const;

  private:
    GLuint m_shader;
  };

  class ZENI_GRAPHICS_DLL Program_GL_Shader : public Program {
    Program_GL_Shader(const Program_GL_Shader &);
    Program_GL_Shader & operator=(const Program_GL_Shader &);

  public:
    Program_GL_Shader();
    virtual ~Program_GL_Shader();

    void attach(Shader &shader);
    void link();

    inline GLuint get() const;

  private:
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::list<Shader_GL_Shader *> m_shaders;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
    GLuint m_program;
  };
#endif
  
#ifndef DISABLE_DX9
  class ZENI_GRAPHICS_DLL Shader_DX9 : public Shader {
    Shader_DX9(const Shader_DX9 &);
    Shader_DX9 & operator=(const Shader_DX9 &);

  public:
    Shader_DX9(const String &shader_src, const Type &type, Video_DX9 &vdx);
    ~Shader_DX9();

    inline Shader::Type type() const;
    inline LPD3DXCONSTANTTABLE get_constant_table() const;
    inline LPDIRECT3DVERTEXSHADER9 get_vertex_shader() const;
    inline LPDIRECT3DPIXELSHADER9 get_pixel_shader() const;

  private:
    LPD3DXCONSTANTTABLE m_constant_table;
    union {
      LPDIRECT3DVERTEXSHADER9 m_vertex_shader;
      LPDIRECT3DPIXELSHADER9 m_pixel_shader;
    };
  };

  class ZENI_GRAPHICS_DLL Program_DX9 : public Program {
    Program_DX9(const Program_DX9 &);
    Program_DX9 & operator=(const Program_DX9 &);

  public:
    Program_DX9();
    virtual ~Program_DX9();

    void attach(Shader &shader);
    void link();
    
    inline const Shader_DX9 * get_vertex_shader() const;
    inline const Shader_DX9 * get_fragment_shader() const;

  private:
    Shader_DX9 * m_vertex_shader;
    Shader_DX9 * m_fragment_shader;
  };
#endif

  struct ZENI_GRAPHICS_DLL Shader_Init_Failure : public Error {
    Shader_Init_Failure() : Error("Zeni Shader Failed to Initialize Correctly") {}
  };

  struct ZENI_GRAPHICS_DLL Shader_Link_Failure : public Error {
    Shader_Link_Failure() : Error("Zeni Shader Failed to Link Correctly") {}
  };

}

#endif

#endif
