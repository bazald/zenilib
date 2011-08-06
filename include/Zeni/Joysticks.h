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
 * \class Zeni::Joysticks
 *
 * \ingroup zenilib
 *
 * \brief The Joysticks Singleton
 *
 * The Joysticks singleton is responsible for managing joysticks.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_JOYSTICKS_H
#define ZENI_JOYSTICKS_H

#include <Zeni/Error.h>
#include <Zeni/Singleton.h>

#include <SDL/SDL.h>

#include <vector>

namespace Zeni {

  class ZENI_CORE_DLL Joysticks;

#ifdef _WINDOWS
  ZENI_CORE_EXT template class ZENI_CORE_DLL Singleton<Joysticks>;
#endif

  class ZENI_CORE_DLL Joysticks : public Singleton<Joysticks> {
    friend class Singleton<Joysticks>;

    static Joysticks * create();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    static Uninit g_uninit;
    static Reinit g_reinit;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

    Joysticks();
    ~Joysticks();

    // Undefined
    Joysticks(const Joysticks &);
    Joysticks & operator=(const Joysticks &);

  public:
    size_t get_num_joysticks() const; ///< Get the number of joysticks attached to the system

    const String & get_joystick_name(const size_t &index) const; ///< Get the name of a given joystick
    int get_joystick_num_axes(const size_t &index) const; ///< Get the number of axes for a joystick
    int get_joystick_num_balls(const size_t &index) const; ///< Get the number of balls for a joystick
    int get_joystick_num_hats(const size_t &index) const; ///< Get the number of hats for a joystick
    int get_joystick_num_buttons(const size_t &index) const; ///< Get the number of buttons for a joystick

    void reinit(); ///< Reload all joysticks, flusing *all* SDL events and possibly changing 'which' values for joysticks

  private:
    void init();
    void uninit();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::vector<std::pair<SDL_Joystick *, String> > m_joystick;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  };

  ZENI_CORE_DLL Joysticks & get_Joysticks(); ///< Get access to the singleton.

  struct Joystick_Init_Failure : public Error {
    Joystick_Init_Failure() : Error("Zeni Joystick Failed to Initialize Correctly") {}
  };

}

#endif
