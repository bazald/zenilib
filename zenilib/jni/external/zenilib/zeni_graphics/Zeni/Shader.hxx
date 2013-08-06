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

#ifndef ZENI_SHADER_HXX
#define ZENI_SHADER_HXX

// HXXed below
#include <Zeni/Video.h>

#include <Zeni/Shader.h>

namespace Zeni {
  
#ifndef DISABLE_GL_FIXED
  GLuint Shader_GL_Fixed::get() const {
    return m_shader;
  }

  GLuint Program_GL_Fixed::get() const {
    return m_program;
  }
#endif

#ifndef DISABLE_GL_SHADER
  GLuint Shader_GL_Shader::get() const {
    return m_shader;
  }

  GLuint Program_GL_Shader::get() const {
    return m_program;
  }
#endif

}

#include <Zeni/Video.hxx>

#endif
