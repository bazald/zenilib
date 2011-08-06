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

  template class ZENI_CORE_DLL Singleton<Core>;

  Core * Core::create() {
    return new Core;
  }

  Core::Core() {
    get_File_Ops(); // Redirect File I/O

    /** Initialize SDL itself **/

    if(SDL_Init(SDL_INIT_TIMER) == -1)
      throw Core_Init_Failure();
  }

  Core::~Core() {
    SDL_Quit();
  }

  Core & get_Core() {
    return Core::get();
  }

  bool Core::is_screen_saver_enabled() {
#if SDL_VERSION_ATLEAST(1,3,0)
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
#if SDL_VERSION_ATLEAST(1,3,0) || defined(_WINDOWS)
    enabled
#endif
    ) {
#if SDL_VERSION_ATLEAST(1,3,0)
    if(enabled)
      SDL_EnableScreenSaver();
    else
      SDL_DisableScreenSaver();
#elif defined(_WINDOWS)
    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, UINT(enabled), 0, SPIF_SENDCHANGE);
#endif
  }

  void Core::assert_no_error() {
    if(strlen(SDL_GetError())) {
      std::cerr << "SDL       : " << SDL_GetError() << std::endl;
      SDL_ClearError();
      assert(false);
    }
  }

  bool Core::print_error() {
    if(strlen(SDL_GetError())) {
      std::cerr << "SDL       : " << SDL_GetError() << std::endl;
      SDL_ClearError();
      return true;
    }

    return false;
  }

}
