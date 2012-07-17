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

/**
 * \class Zeni::Core
 *
 * \ingroup zenilib
 *
 * \brief The Core Singleton
 *
 * The Core singleton essentially initializes and uninitializes SDL.  
 * It is depended upon by most other classes in some way.
 *
 * \note You almost definitely don't need to interact directly with this class.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_CORE_H
#define ZENI_CORE_H

#include <Zeni/Error.h>
#include <Zeni/Singleton.h>

#ifdef ANDROID
#include <android_native_app_glue.h>
#endif

namespace Zeni {

  class ZENI_CORE_DLL Core;

#ifdef _WINDOWS
  ZENI_CORE_EXT template class ZENI_CORE_DLL Singleton<Core>;
#endif

  class ZENI_CORE_DLL Core : public Singleton<Core> {
    friend class Singleton<Core>;

    static Core * create();

    Core();
    ~Core();

    // Undefined
    Core(const Core &);
    Core & operator=(const Core &);

  public:
#ifdef ANDROID
    static void preinit(android_app * const &state);

    android_app & get_state();
#endif

    bool is_screen_saver_enabled(); ///< Check to see if the screen saver is enabled
    void set_screen_saver(const bool &enabled); ///< Enable/Disable the screen saver

    static void assert_no_error(); ///< If there is an SDL error, print it and assert(false)
    static bool print_error(); ///< If there is an SDL error, print it and then clear it; returns true iff there was an error

#ifdef ANDROID
  private:
    static android_app *g_state;
    bool m_screen_saver_enabled;
#endif
  };

  // Get reference to only instance; Might throw Core_Init_Failure
  ZENI_CORE_DLL Core & get_Core(); ///< Get access to the singleton.

  struct ZENI_CORE_DLL Core_Init_Failure : public Error {
    Core_Init_Failure() : Error("Zeni Core Failed to Initialize Correctly") {}
  };

}

#endif
