/* This file is part of the Zenipex Library.
* Copyleft (C) 2007 Mitchell Keith Bloch a.k.a. bazald
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

#ifndef ZENI_VIDEO_GL_HXX
#define ZENI_VIDEO_GL_HXX

#include "Video_GL.h"

#ifndef DISABLE_GL

namespace Zeni {

  void Video_GL::pglBindBufferARB(const GLenum target, const GLuint buffer) const {
    m_pglBindBufferARB(target, buffer);
  }

  void Video_GL::pglDeleteBuffersARB(const GLsizei n, GLuint * const buffers) const {
    m_pglDeleteBuffersARB(n, buffers);
  }

  void Video_GL::pglGenBuffersARB(const GLsizei n, GLuint * const buffers) const {
    m_pglGenBuffersARB(n, buffers);
  }

  void Video_GL::pglBufferDataARB(const GLenum target, const int size, const GLvoid * const data, const GLenum usage) const {
    m_pglBufferDataARB(target, size, data, usage);
  }

}

#endif

#endif
