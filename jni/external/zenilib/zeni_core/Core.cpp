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

#include <zeni_core.h>

#ifdef ANDROID
#include <android/window.h>
#endif

#include <SDL/SDL.h>

#include <iostream>
#include <fstream>
#include <cassert>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Singleton.hxx>

namespace Zeni {

  template class Singleton<Core>;

  Core * Core::create() {
    return new Core;
  }

  Core::Core()
#ifdef ANDROID
   : m_screen_saver_enabled(true)
#endif
  {
#ifdef ANDROID
    if(!g_state)
      throw Core_Init_Failure();
#endif

    get_File_Ops(); // Redirect File I/O

    /** Initialize SDL itself **/

#ifndef ANDROID
    if(SDL_Init(SDL_INIT_TIMER) == -1)
      throw Core_Init_Failure();
#endif
  }

  Core::~Core() {
#ifndef ANDROID
    SDL_Quit();
#endif
  }

  Core & get_Core() {
    return Core::get();
  }

#ifdef ANDROID
  void Core::preinit(android_app * const &state_) {
    g_state = state_;
  }

  android_app & Core::get_state() {
    return *g_state;
  }
#endif

  bool Core::is_screen_saver_enabled() {
#ifdef ANDROID
    return m_screen_saver_enabled;
#elif SDL_VERSION_ATLEAST(1,3,0)
    return SDL_IsScreenSaverEnabled() != 0;
#elif defined(_WINDOWS)
    BOOL is_active;
    SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &is_active, 0);
    return is_active != 0;
#else
    return true;
#endif
  }

  void Core::set_screen_saver(const bool &
#if defined(ANDROID) || SDL_VERSION_ATLEAST(1,3,0) || defined(_WINDOWS)
    enabled
#endif
    )
  {
#if defined(ANDROID)
    m_screen_saver_enabled = enabled;

    if(enabled)
      ANativeActivity_setWindowFlags(g_state->activity, 0, AWINDOW_FLAG_KEEP_SCREEN_ON);
    else
      ANativeActivity_setWindowFlags(g_state->activity, AWINDOW_FLAG_KEEP_SCREEN_ON, 0);
#elif SDL_VERSION_ATLEAST(1,3,0)
    if(enabled)
      SDL_EnableScreenSaver();
    else
      SDL_DisableScreenSaver();
#elif defined(_WINDOWS)
    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, UINT(enabled), 0, SPIF_SENDCHANGE);
#endif
  }

  void Core::assert_no_error() {
#ifndef ANDROID
    if(strlen(SDL_GetError())) {
      std::cerr << "SDL       : " << SDL_GetError() << std::endl;
      SDL_ClearError();
      assert(false);
    }
#endif
  }

  bool Core::print_error() {
#ifndef ANDROID
    if(strlen(SDL_GetError())) {
      std::cerr << "SDL       : " << SDL_GetError() << std::endl;
      SDL_ClearError();
      return true;
    }
#endif

    return false;
  }

#ifdef ANDROID
  android_app *Core::g_state = 0;
#endif

}
