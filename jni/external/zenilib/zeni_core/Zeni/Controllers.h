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
 * \class Zeni::Controllers
 *
 * \ingroup zenilib
 *
 * \brief The Controllers Singleton
 *
 * The Controllers singleton is responsible for managing joysticks.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_CONTROLLERS_H
#define ZENI_CONTROLLERS_H

#include <Zeni/Error.h>
#include <Zeni/Singleton.h>

#include <SDL/SDL.h>

#include <vector>

namespace Zeni {

  class ZENI_CORE_DLL Controllers;

#ifdef _WINDOWS
  ZENI_CORE_EXT template class ZENI_CORE_DLL Singleton<Controllers>;
#endif

  class ZENI_CORE_DLL Controllers : public Singleton<Controllers> {
    friend class Singleton<Controllers>;

    static Controllers * create();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    static Uninit g_uninit;
    static Reinit g_reinit;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

    Controllers();
    ~Controllers();

    // Undefined
    Controllers(const Controllers &);
    Controllers & operator=(const Controllers &);

  public:
    size_t get_num_controllers() const; ///< Get the number of controllers attached to the system
    
    Sint32 get_controller_index(const Sint32 &id) const; ///< Get the index of a given controller from the true SDL_JoystickInstanceID
    const char * get_controller_name(const Sint32 &index) const; ///< Get the name of a given controller
    bool is_controller_connected(const Sint32 &index) const; ///< Check to see if the controller is currently connected

    void reinit(); ///< Reload all joysticks, flushing *all* SDL events and possibly changing 'which' values for controllers
    void enable(const bool &enable_); ///< Temporarily turn controller input on/off
    
    void device_added(const Sint32 &index); ///< Register a new device
    void device_removed(const Sint32 &id); ///< Remove a device
    void detect_removed(); ///< Fix to broken SDL device removal detection

    void set_vibration(const size_t &index, const float &left, const float &right); ///< Set vibration for controller [0,...] to <[0,1],[0,1]>
    void reset_vibration_all(); ///< Set vibration for all controllers to <0,0>

  private:
    void device_add_all(); ///< Register all devices

    class Joystick_Info {
      Joystick_Info(const Joystick_Info &rhs);
      Joystick_Info & operator=(const Joystick_Info &rhs);

    public:
      Joystick_Info();
      ~Joystick_Info();

      SDL_Joystick * joystick;
      Sint32 joystick_id;

      SDL_GameController * gamecontroller;
      SDL_Haptic * haptic;

      SDL_HapticEffect haptic_effect;
      int haptic_effect_id;
    };

    void init();
    void uninit();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    typedef std::vector<Joystick_Info *> Joystick_Array;
    Joystick_Array m_joysticks;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  };

  ZENI_CORE_DLL Controllers & get_Controllers(); ///< Get access to the singleton.

  struct Controllers_Init_Failure : public Error {
    Controllers_Init_Failure() : Error("Zeni Controllers Failed to Initialize Correctly") {}
  };

}

#endif
