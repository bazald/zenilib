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

#ifndef ZENI_VIDEO_HXX
#define ZENI_VIDEO_HXX

#include <Zeni/Video.h>

namespace Zeni {

  const VIDEO_MODE & Video::get_video_mode() {
    return g_video_mode;
  }

  const int & Video::get_screen_width() {
    return g_screen_width;
  }

  const int & Video::get_screen_height() {
    return g_screen_height;
  }

  const bool & Video::is_fullscreen() {
    return g_screen_full;
  }

  const bool & Video::is_frame_visible() {
    return g_screen_show_frame;
  }

  const bool & Video::get_backface_culling() {
    return g_backface_culling;
  }

  const bool & Video::get_lighting() {
    return g_lighting;
  }

  const bool & Video::get_normal_interpolation() {
    return g_normal_interp;
  }

  const bool & Video::get_vertical_sync() {
    return g_vertical_sync;
  }

  const int & Video::get_multisampling() {
    return g_multisampling;
  }

  const bool & Video::is_initialized() {
    return g_initialized;
  }

  SDL_Surface * Video::get_display_surface() {
    return m_display_surface;
  }

  const bool & Video::get_opengl_flag() const {
    return m_opengl_flag;
  }

  void Video::set_opengl_flag(const bool &on) {
    m_opengl_flag = on;
  }

}

#endif
