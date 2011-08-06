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

#ifndef ZENI_VIDEO_GL_HXX
#define ZENI_VIDEO_GL_HXX

// HXXed below
#include <Zeni/Light.h>
#include <Zeni/Material.h>
#include <Zeni/Renderable.h>
#include <Zeni/Shader.h>
#include <Zeni/Texture.h>
#include <Zeni/Textures.h>
#include <Zeni/Video.h>

#include <Zeni/Video_GL.h>

// Not HXXed
#include <Zeni/Fog.h>
#include <Zeni/Shader.h>
#include <Zeni/Vertex_Buffer.h>

#ifndef DISABLE_GL

namespace Zeni {

  void Video_GL::pglBindBufferARB(const GLenum target, const GLuint buffer) const {
#ifdef REQUIRE_GL_ES
    glBindBuffer(target, buffer);
#else
    m_pglBindBufferARB(target, buffer);
#endif
  }

  void Video_GL::pglDeleteBuffersARB(const GLsizei n, GLuint * const buffers) const {
#ifdef REQUIRE_GL_ES
    glDeleteBuffers(n, buffers);
#else
    m_pglDeleteBuffersARB(n, buffers);
#endif
  }

  void Video_GL::pglGenBuffersARB(const GLsizei n, GLuint * const buffers) const {
#ifdef REQUIRE_GL_ES
    glGenBuffers(n, buffers);
#else
    m_pglGenBuffersARB(n, buffers);
#endif
  }

  void Video_GL::pglBufferDataARB(const GLenum target, const int size, const GLvoid * const data, const GLenum usage) const {
#ifdef REQUIRE_GL_ES
    glBufferData(target, size, data, usage);
#else
    m_pglBufferDataARB(target, size, data, usage);
#endif
  }

}

#endif

#include <Zeni/Fonts.h>
#include <Zeni/Light.hxx>
#include <Zeni/Material.hxx>
#include <Zeni/Renderable.hxx>
#include <Zeni/Shader.hxx>
#include <Zeni/Texture.hxx>
#include <Zeni/Textures.hxx>
#include <Zeni/Video.hxx>

#endif
