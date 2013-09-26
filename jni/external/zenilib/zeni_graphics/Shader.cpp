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
#include <d3dx9shader.h>
#endif

#include <iostream>

namespace Zeni {
  
#ifndef DISABLE_GL_FIXED
  Shader_GL_Fixed::Shader_GL_Fixed(const String &shader_src, const Type &type)
    : m_shader(glCreateShaderObjectARB(type == VERTEX ? GL_VERTEX_SHADER_ARB : GL_FRAGMENT_SHADER_ARB))
  {
    const char * src_ptr = shader_src.c_str();
    glShaderSourceARB(m_shader, 1, reinterpret_cast<const GLchar **>(&src_ptr), 0);
    if(!m_shader)
      throw Shader_Init_Failure();
    glCompileShaderARB(m_shader);
  }

  Shader_GL_Fixed::~Shader_GL_Fixed() {
    glDeleteShader(m_shader);
  }
  
  Program_GL_Fixed::Program_GL_Fixed()
    : m_program(glCreateProgram())
  {
  }
  
  Program_GL_Fixed::~Program_GL_Fixed() {
    for(std::list<Shader_GL_Fixed *>::iterator it = m_shaders.begin(), iend = m_shaders.end(); it != iend; ++it)
      glDetachShader(m_program, (*it)->get());
    m_shaders.clear();

    glDeleteProgram(m_program);
  }
  
  void Program_GL_Fixed::attach(Shader &shader) {
    Shader_GL_Fixed &shader_gl_fixed = dynamic_cast<Shader_GL_Fixed &>(shader);
    m_shaders.push_back(&shader_gl_fixed);
    glAttachShader(m_program, shader_gl_fixed.get());
  }
  
  void Program_GL_Fixed::link() {
    glLinkProgramARB(m_program);
    
    GLint rv = GL_TRUE;
    glGetProgramivARB(m_program, GL_LINK_STATUS, &rv);
    if(rv != GL_TRUE)
      throw Shader_Link_Failure();
  }
#endif

#ifndef DISABLE_GL_SHADER
  Shader_GL_Shader::Shader_GL_Shader(const String &shader_src, const Type &type)
    : m_shader(glCreateShader(type == VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER))
  {
    const char * src_ptr = shader_src.c_str();
    glShaderSource(m_shader, 1, reinterpret_cast<const GLchar **>(&src_ptr), 0);
    if(!m_shader)
      throw Shader_Init_Failure();
    glCompileShader(m_shader);

    // Check vertex shader
    GLint rv = GL_TRUE, len;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &rv);
    if(rv != GL_TRUE) {
      glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &len);
      Zeni::String log(len > 0 ? len - 1 : 0, '\0');
      glGetShaderInfoLog(m_shader, len, 0, reinterpret_cast<GLchar *>(const_cast<char *>(log.c_str())));
      std::cerr << "Shader initialization failure: " << log << std::endl;
      throw Shader_Init_Failure();
    }
  }

  Shader_GL_Shader::~Shader_GL_Shader() {
    glDeleteShader(m_shader);
  }
  
  Program_GL_Shader::Program_GL_Shader()
    : m_program(glCreateProgram())
  {
  }
  
  Program_GL_Shader::~Program_GL_Shader() {
    for(std::list<Shader_GL_Shader *>::iterator it = m_shaders.begin(), iend = m_shaders.end(); it != iend; ++it)
      glDetachShader(m_program, (*it)->get());
    m_shaders.clear();

    glDeleteProgram(m_program);
  }
  
  void Program_GL_Shader::attach(Shader &shader) {
    Shader_GL_Shader &shader_gl_shader = dynamic_cast<Shader_GL_Shader &>(shader);
    m_shaders.push_back(&shader_gl_shader);
    glAttachShader(m_program, shader_gl_shader.get());
  }
  
  void Program_GL_Shader::link() {
    glLinkProgram(m_program);
    
    GLint rv = GL_TRUE, len;
    glGetProgramiv(m_program, GL_LINK_STATUS, &rv);
    if(rv != GL_TRUE) {
      glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);
      Zeni::String log(len > 0 ? len - 1 : 0, '\0');
      glGetProgramInfoLog(m_program, len, 0, reinterpret_cast<GLchar *>(const_cast<char *>(log.c_str())));
      std::cerr << "Shader link failure: " << log << std::endl;
      throw Shader_Link_Failure();
    }
  }
#endif

#ifndef DISABLE_DX9
  Shader_DX9::Shader_DX9(const String &shader_src, const Type &type, Video_DX9 &vdx)
    : m_constant_table(0),
    m_vertex_shader(0)
  {
    String shader_src_full = shader_src;
    if(type == VERTEX) {
      String uniforms;
      int pos, pos2;
      
      pos = shader_src_full.find("// Attributes");
      shader_src_full = shader_src_full.substr(0, pos) + "struct VS_INPUT {" + shader_src_full.substr(pos + 13);
      pos = shader_src_full.find("// Varyings");
      shader_src_full = shader_src_full.substr(0, pos) + "};\nstruct VS_OUTPUT {\nfloat4 gl_Position : POSITION;" + shader_src_full.substr(pos + 11);
      pos = shader_src_full.rfind("static");
      pos = shader_src_full.find(';', pos);
      shader_src_full = shader_src_full.substr(0, pos + 1) + "\n}; " + shader_src_full.substr(pos + 1);
      pos = shader_src_full.find("static float4 gl_Position");
      pos2 = shader_src_full.find(';', pos);
      shader_src_full = shader_src_full.substr(0, pos) + shader_src_full.substr(pos2 + 1);

      std::set<String> ins;
      std::set<String> outs;
      outs.insert("gl_Position");
      
      for(pos2 = shader_src_full.find("uniform ");; pos2 = pos) {
        pos = shader_src_full.rfind(" = ", pos2);
        if(pos == String::npos)
          break;
        pos2 = shader_src_full.find(';', pos);
        shader_src_full = shader_src_full.substr(0, pos) + shader_src_full.substr(pos2);
      }
      for(pos = 0;;) {
        pos = shader_src_full.find("static ", pos);
        if(pos == String::npos)
          break;
        pos = shader_src_full.find(' ', pos + 7) + 1;
        pos2 = shader_src_full.find(';', pos);
        if(shader_src_full.rfind("VS_OUTPUT", pos) != String::npos)
          outs.insert(shader_src_full.substr(pos, pos2 - pos));
        else
          ins.insert(shader_src_full.substr(pos, pos2 - pos));
        pos = pos2;
      }
      for(pos = 0;;) {
        pos = shader_src_full.find("static ", pos);
        if(pos == String::npos)
          break;
        pos2 = shader_src_full.find(';', pos);
        String sub = shader_src_full.substr(pos + 7, pos2 - (pos + 7));
        if(sub.find("position") != String::npos)
          sub = " : POSITION";
        else if(sub.find("color") != String::npos)
          sub = " : COLOR";
        else
          sub.clear();
        shader_src_full = shader_src_full.substr(0, pos) + shader_src_full.substr(pos + 7, pos2 - (pos + 7))
                        + sub + shader_src_full.substr(pos2);
      }
      for(pos = 0;;) {
        pos = shader_src_full.find("uniform ", pos);
        if(pos == String::npos)
          break;
        pos2 = shader_src_full.find(": register(", pos);
        uniforms += ", " + shader_src_full.substr(pos, pos2 - pos);
        shader_src_full = shader_src_full.substr(0, pos) + shader_src_full.substr(pos2 + 14);
      }

      pos = shader_src_full.find("void gl_main()");
      pos2 = shader_src_full.find('{', pos) + 1;
      for(int count = 1; count; ++pos2) {
        if(shader_src_full[pos2] == '}')
          --count;
        else if(shader_src_full[pos2] == '{')
          ++count;
      }
      String shader_front = shader_src_full.substr(0, pos);
      String shader_back = shader_src_full.substr(pos2 + 1);
      String main_body = shader_src_full.substr(pos + 14, (pos2 + 1) - (pos + 14));
      
      for(std::set<String>::iterator it = ins.begin(), iend = ins.end(); it != iend; ++it) {
        for(pos = main_body.size();;) {
          pos = main_body.rfind(*it, pos);
          if(pos == String::npos)
            break;
          main_body = main_body.substr(0, pos) + "v." + main_body.substr(pos);
        }
      }
      for(std::set<String>::iterator it = outs.begin(), iend = outs.end(); it != iend; ++it) {
        for(pos = main_body.size();;) {
          pos = main_body.rfind(*it, pos);
          if(pos == String::npos)
            break;
          main_body = main_body.substr(0, pos) + "o." + main_body.substr(pos);
        }
      }

      shader_src_full = shader_front + "VS_OUTPUT vs_main(const VS_INPUT v" + uniforms + ") {VS_OUTPUT o; " + main_body + "return o;}";
    }
    else {
      String uniforms;
      int pos, pos2;
      
      pos = shader_src_full.find("// Varyings");
      shader_src_full = shader_src_full.substr(0, pos) + "struct PS_INPUT {\nfloat4 gl_Position : POSITION;" + shader_src_full.substr(pos + 11);
      pos = shader_src_full.rfind("static");
      pos = shader_src_full.find(';', pos);
      shader_src_full = shader_src_full.substr(0, pos + 1) + "\n}; " + shader_src_full.substr(pos + 1);
      pos = shader_src_full.find("static float4 gl_Color[1]");
      pos2 = shader_src_full.find(';', pos);
      shader_src_full = shader_src_full.substr(0, pos) + shader_src_full.substr(pos2 + 1);

      std::set<String> ins;
      
      for(pos2 = shader_src_full.find("#define GL_USES_FRAG_COLOR");; pos2 = pos) {
        pos = shader_src_full.rfind(" = ", pos2);
        if(pos == String::npos)
          break;
        pos2 = shader_src_full.find(';', pos);
        shader_src_full = shader_src_full.substr(0, pos) + shader_src_full.substr(pos2);
      }
      for(pos = 0;;) {
        pos = shader_src_full.find("static ", pos);
        if(pos == String::npos)
          break;
        pos = shader_src_full.find(' ', pos + 7) + 1;
        pos2 = shader_src_full.find(';', pos);
        ins.insert(shader_src_full.substr(pos, pos2 - pos));
        pos = pos2;
      }
      for(pos = 0;;) {
        pos = shader_src_full.find("static ", pos);
        if(pos == String::npos)
          break;
        pos2 = shader_src_full.find(';', pos);
        String sub = shader_src_full.substr(pos + 7, pos2 - (pos + 7));
        if(sub.find("position") != String::npos)
          sub = " : POSITION";
        else if(sub.find("color") != String::npos)
          sub = " : COLOR";
        else
          sub.clear();
        shader_src_full = shader_src_full.substr(0, pos) + shader_src_full.substr(pos + 7, pos2 - (pos + 7))
                        + sub + shader_src_full.substr(pos2);
      }

      pos = shader_src_full.find("void gl_main()");
      pos2 = shader_src_full.find('{', pos) + 1;
      for(int count = 1; count; ++pos2) {
        if(shader_src_full[pos2] == '}')
          --count;
        else if(shader_src_full[pos2] == '{')
          ++count;
      }
      String shader_front = shader_src_full.substr(0, pos);
      String shader_back = shader_src_full.substr(pos2 + 1);
      String main_body = shader_src_full.substr(pos + 14, (pos2 + 1) - (pos + 14));
      
      for(std::set<String>::iterator it = ins.begin(), iend = ins.end(); it != iend; ++it) {
        for(pos = main_body.size();;) {
          pos = main_body.rfind(*it, pos);
          if(pos == String::npos)
            break;
          main_body = main_body.substr(0, pos) + "p." + main_body.substr(pos);
        }
      }
      for(pos = main_body.size();;) {
        pos = main_body.rfind("gl_Color[0]", pos);
        if(pos == String::npos)
          break;
        main_body = main_body.substr(0, pos) + "gl_Color" + main_body.substr(pos + 11);
      }

      shader_src_full = shader_front + "float4 ps_main(const PS_INPUT p" + uniforms + ") : COLOR {\nfloat4 gl_Color;" + main_body + "return gl_Color;}";
    }

    std::cerr << "Shader Source Full:" << std::endl << shader_src_full << std::endl;

    LPD3DXBUFFER code;
    LPD3DXBUFFER errors;

    HRESULT result = vdx.D3DXCompileShader()(shader_src_full.c_str(),
                                             shader_src_full.size(),
                                             0,
                                             0,
                                             type == VERTEX ? "vs_main" : "ps_main",
                                             type == VERTEX ? "vs_3_0" : "ps_3_0",
                                             0,
                                             &code,
                                             &errors,
                                             type == VERTEX ? &m_constant_table : 0);

    if(FAILED(result)) {
      std::cerr << "Errors:" << std::endl << reinterpret_cast<char *>(errors->GetBufferPointer());
      errors->Release();
      throw Shader_Init_Failure();
    }

    if(type == VERTEX)
      vdx.get_d3d_device()->CreateVertexShader((DWORD*)code->GetBufferPointer(), &m_vertex_shader);
    else
      vdx.get_d3d_device()->CreatePixelShader((DWORD*)code->GetBufferPointer(), &m_pixel_shader);

    code->Release();
  }

  Shader_DX9::~Shader_DX9() {
    if(m_constant_table) {
      m_constant_table->Release();
      if(m_vertex_shader)
        m_vertex_shader->Release();
    }
    else if(m_pixel_shader)
      m_pixel_shader->Release();
  }
  
  Program_DX9::Program_DX9()
    : m_vertex_shader(0),
    m_fragment_shader(0)
  {
  }
  
  Program_DX9::~Program_DX9() {
  }
  
  void Program_DX9::attach(Shader &shader) {
    Shader_DX9 &sdx = dynamic_cast<Shader_DX9 &>(shader);

    if(sdx.type() == Shader::VERTEX) {
      if(m_vertex_shader)
        throw Shader_Link_Failure();
      m_vertex_shader = &sdx;
    }
    else {
      if(m_fragment_shader)
        throw Shader_Link_Failure();
      m_fragment_shader = &sdx;
    }
  }

  void Program_DX9::link() {
  }
#endif

}
