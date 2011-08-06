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

#ifndef ZENI_WINDOW_HXX
#define ZENI_WINDOW_HXX

#include <Zeni/Window.h>

namespace Zeni {

  const bool & Window::is_enabled() {
    return g_enabled;
  }

  const Point2i & Window::get_size() {
    return g_screen_size;
  }

  const int & Window::get_width() {
    return g_screen_size.x;
  }

  const int & Window::get_height() {
    return g_screen_size.y;
  }

  const bool & Window::is_full_screen() {
    return g_screen_full;
  }

  const bool & Window::is_frame_visible() {
    return g_screen_show_frame;
  }

  const bool & Window::is_resizable() {
    return g_screen_resizable;
  }

  const std::vector<Point2i> & Window::get_resolutions() const {
    return m_modes;
  }

#if SDL_VERSION_ATLEAST(1,3,0)
  SDL_Window * Window::get_window() {
    return m_window;
  }
#else
  SDL_Surface * Window::get_display_surface() {
    return m_display_surface;
  }
#endif

  bool Window::is_mouse_grabbed() const {
#if SDL_VERSION_ATLEAST(1,3,0)
    SDL_Window * const window = get_Window().get_window();
    return window && SDL_GetWindowGrab(window) == SDL_GRAB_ON;
#else
    return SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON;
#endif
  }

  bool Window::is_mouse_hidden() const {
    return SDL_ShowCursor(SDL_QUERY) != SDL_ENABLE;
  }

  void Window::mouse_grab(const bool &grab) {
#if SDL_VERSION_ATLEAST(1,3,0)
    SDL_Window * const window = get_Window().get_window();
    if(window)
      SDL_SetWindowGrab(window, grab ? SDL_GRAB_ON : SDL_GRAB_OFF);
#else
    SDL_WM_GrabInput(grab ? SDL_GRAB_ON : SDL_GRAB_OFF);
#endif
  }

  void Window::mouse_hide(const bool &hide) {
    SDL_ShowCursor(hide ? SDL_DISABLE : SDL_ENABLE);
  }

  const String & Window::get_title() const {
    return get_m_title();
  }

  const String & Window::get_taskmsg() const {
    return get_m_taskmsg();
  }

}

#endif
