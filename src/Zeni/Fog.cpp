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

#include <Zeni/Fog.h>

#include <Zeni/Video_DX9.hxx>

#ifndef DISABLE_GL
#ifdef _MACOSX
#include <GLEW/glew.h>
#else
#include <GL/glew.h>
#endif
#endif

namespace Zeni {

  Fog::Fog(const Color &color_, 
    const float &density_,
    const FOG_TYPE &type_,
    const float &start_,
    const float &end_)
    : type(type_),
    color(color_),
    density(density_),
    start(start_),
    end(end_)
  {
  }

#ifndef DISABLE_GL
  void Fog::set(Video_GL &) const {
    const int mode = type == FOG_EXP ? GL_EXP
      : type == FOG_EXP2 ? GL_EXP2
      : GL_LINEAR;

    glFogi(GL_FOG_MODE, mode);

    if(type == FOG_LINEAR) {
      glFogf(GL_FOG_START, start);
      glFogf(GL_FOG_END, end);
    }
    else
      glFogf(GL_FOG_DENSITY, density);

    glFogfv(GL_FOG_COLOR, reinterpret_cast<const GLfloat *>(&color));
    glHint(GL_FOG_HINT, GL_NICEST);
  }
#endif

#ifndef DISABLE_DX9
  void Fog::set(Video_DX9 &screen) const {
    const DWORD mode = type == FOG_EXP ? D3DFOG_EXP
      : type == FOG_EXP2 ? D3DFOG_EXP2
      : D3DFOG_LINEAR;

    screen.get_d3d_device()->SetRenderState(D3DRS_FOGTABLEMODE, mode);

    if(type == FOG_LINEAR) {
      screen.get_d3d_device()->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&start));
      screen.get_d3d_device()->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&end));
    }
    else
      screen.get_d3d_device()->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&density));

    screen.get_d3d_device()->SetRenderState(D3DRS_FOGCOLOR, color.get_argb());
  }
#endif

}
