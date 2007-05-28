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

#ifndef ZENI_VIDEO_DX9_HXX
#define ZENI_VIDEO_DX9_HXX

#include "Video_DX9.h"

#ifndef DISABLE_DX9

namespace Zeni {

  const D3DCAPS9 & Video_DX9::get_d3d_capabilities() {
    return m_d3d_capabilities;
  }

  LPDIRECT3D9 & Video_DX9::get_d3d() {
    return m_d3d;
  }

  LPDIRECT3DDEVICE9 & Video_DX9::get_d3d_device() {
    return m_d3d_device;
  }

  LPD3DXMATRIXSTACK & Video_DX9::get_matrix_stack() {
    return m_matrix_stack;
  }

}

#endif

#endif
