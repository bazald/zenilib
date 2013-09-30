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

typedef struct _XINPUT_CAPABILITIES XINPUT_CAPABILITIES;
typedef struct _XINPUT_STATE XINPUT_STATE;
typedef struct _XINPUT_VIBRATION XINPUT_VIBRATION;

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
    enum {
      AXIS_LEFT_THUMB_X = 0,
      AXIS_LEFT_THUMB_Y = 1,
      AXIS_RIGHT_THUMB_X = 2,
      AXIS_RIGHT_THUMB_Y = 3,
      AXIS_LEFT_TRIGGER = 4,
      AXIS_RIGHT_TRIGGER = 5
    } Xbox_360_Axis;

    enum {
      BUTTON_START = 4,
      BUTTON_BACK = 5,
      BUTTON_LEFT_THUMB = 6,
      BUTTON_RIGHT_THUMB = 7,
      BUTTON_LEFT_SHOULDER = 8,
      BUTTON_RIGHT_SHOULDER = 9,
      BUTTON_A = 10,
      BUTTON_B = 11,
      BUTTON_X = 12,
      BUTTON_Y = 13,
      //BUTTON_GUIDE = 14,
    } Xbox_360_Button;

    size_t get_num_joysticks() const; ///< Get the number of joysticks attached to the system

    Sint32 get_joystick_id(const Sint32 &index) const; ///< Get the id (as mapped by SDL) from the index
    Sint32 get_joystick_index(const Sint32 &id) const; ///< Get the index from the id (as mapped by SDL)

    const char * get_joystick_name(const Sint32 &index) const; ///< Get the name of a given joystick
    int get_joystick_num_axes(const Sint32 &index) const; ///< Get the number of axes for a joystick
    int get_joystick_num_balls(const Sint32 &index) const; ///< Get the number of balls for a joystick
    int get_joystick_num_hats(const Sint32 &index) const; ///< Get the number of hats for a joystick
    int get_joystick_num_buttons(const Sint32 &index) const; ///< Get the number of buttons for a joystick
    bool is_joystick_connected(const Sint32 &index) const; ///< Check to see if the joystick is currently connected

    void reinit(); ///< Reload all joysticks, flushing *all* SDL events and possibly changing 'which' values for joysticks
    void enable(const bool &enable_); ///< Temporarily turn joystick input on/off
    
    void poll(); ///< Poll for new input

    void set_vibration(const size_t &index, const float &left, const float &right);

  private:
    class Joystick_Info {
      Joystick_Info(const Joystick_Info &rhs);
      Joystick_Info & operator=(const Joystick_Info &rhs);

    public:
      Joystick_Info();
      ~Joystick_Info();

      SDL_Joystick * joystick;
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

  struct Joystick_Init_Failure : public Error {
    Joystick_Init_Failure() : Error("Zeni Joystick Failed to Initialize Correctly") {}
  };

}

#endif
