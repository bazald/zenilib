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
    
    GLint rv;
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
    GLint rv, len;
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
    
    GLint rv, len;
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
  Shader_DX9::Shader_DX9(const String &shader_src, const Type &type) {
  }

  Shader_DX9::~Shader_DX9() {
  }
  
  Program_DX9::Program_DX9() {
  }
  
  Program_DX9::~Program_DX9() {
  }
  
  void Program_DX9::attach(Shader &shader) {
    m_shaders.push_back(&dynamic_cast<Shader_DX9 &>(shader));
  }

  void Program_DX9::link() {
  }
#endif

}
