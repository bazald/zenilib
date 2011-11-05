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

typedef struct _XINPUT_CAPABILITIES XINPUT_CAPABILITIES;
typedef struct _XINPUT_STATE XINPUT_STATE;
typedef struct _XINPUT_VIBRATION XINPUT_VIBRATION;

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

#ifdef ENABLE_XINPUT
    typedef void (WINAPI *XInputEnable_fcn)(BOOL enable);
    typedef DWORD (WINAPI *XInputGetCapabilities_fcn)(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities);
    typedef DWORD (WINAPI *XInputGetState_fcn)(DWORD dwUserIndex, XINPUT_STATE* pState);
    typedef DWORD (WINAPI *XInputSetState_fcn)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);

    // DLL Functions
    static XInputEnable_fcn XInputEnable() {return g_XInputEnable;}
    static XInputGetCapabilities_fcn XInputGetCapabilities() {return g_XInputGetCapabilities;}
    static XInputGetState_fcn XInputGetState() {return g_XInputGetState;}
    static XInputSetState_fcn XInputSetState() {return g_XInputSetState;}
#endif
  public:
    enum {
      AXIS_LEFT_THUMB_X = 0,
      AXIS_LEFT_THUMB_Y = 1,
#if defined(_MACOSX)
      AXIS_RIGHT_THUMB_X = 2,
      AXIS_RIGHT_THUMB_Y = 3,
      AXIS_LEFT_TRIGGER = 4,
      AXIS_RIGHT_TRIGGER = 5
#elif defined(_WINDOWS)
      AXIS_RIGHT_TRIGGER = 2,
      AXIS_RIGHT_THUMB_Y = 3,
      AXIS_RIGHT_THUMB_X = 4,
      AXIS_LEFT_TRIGGER = 5
#else
      AXIS_LEFT_TRIGGER = 2,
      AXIS_RIGHT_THUMB_X = 3,
      AXIS_RIGHT_THUMB_Y = 4,
      AXIS_RIGHT_TRIGGER = 5
#endif
    } Xbox_360_Axis;

    enum {
#ifdef _MACOSX
      BUTTON_START = 4,
      BUTTON_BACK = 5,
      BUTTON_LEFT_THUMB = 6,
      BUTTON_RIGHT_THUMB = 7,
      BUTTON_LEFT_SHOULDER = 8,
      BUTTON_RIGHT_SHOULDER = 9,
      //BUTTON_RESERVED = 10,
      BUTTON_A = 11,
      BUTTON_B = 12,
      BUTTON_X = 13,
      BUTTON_Y = 14
#else
      BUTTON_A = 0,
      BUTTON_B = 1,
      BUTTON_X = 2,
      BUTTON_Y = 3,
      BUTTON_LEFT_SHOULDER = 4,
      BUTTON_RIGHT_SHOULDER = 5,
      BUTTON_BACK = 6,
      BUTTON_START = 7,
#ifdef _WINDOWS
      BUTTON_LEFT_THUMB = 8,
      BUTTON_RIGHT_THUMB = 9
#else
      //BUTTON_RESERVED = 8,
      BUTTON_LEFT_THUMB = 9,
      BUTTON_RIGHT_THUMB = 10
#endif
#endif
    } Xbox_360_Button;

    size_t get_num_joysticks() const; ///< Get the number of joysticks attached to the system

    const char * const get_joystick_name(const size_t &index) const; ///< Get the name of a given joystick
    int get_joystick_num_axes(const size_t &index) const; ///< Get the number of axes for a joystick
    int get_joystick_num_balls(const size_t &index) const; ///< Get the number of balls for a joystick
    int get_joystick_num_hats(const size_t &index) const; ///< Get the number of hats for a joystick
    int get_joystick_num_buttons(const size_t &index) const; ///< Get the number of buttons for a joystick

    void reinit(); ///< Reload all joysticks, flushing *all* SDL events and possibly changing 'which' values for joysticks
    void reinit(const bool &try_xinput = true); ///< Reload all joysticks, flushing *all* SDL events and possibly changing 'which' values for joysticks
    void enable(const bool &enable_); ///< Temporarily turn joystick input on/off

#ifdef ENABLE_XINPUT
    void poll(); ///< Poll for new input

    bool is_xinput_connected(const size_t &index) const;
    const XINPUT_CAPABILITIES & get_xinput_capabilities(const size_t &index) const;
    const XINPUT_STATE & get_xinput_state(const size_t &index) const;
    void set_xinput_vibration(const size_t &index, const float &left, const float &right);
#endif

  private:
#ifdef ENABLE_XINPUT
    void zero_handles();

    HMODULE m_xinput;
    static XInputEnable_fcn g_XInputEnable;
    static XInputGetCapabilities_fcn g_XInputGetCapabilities;
    static XInputGetState_fcn g_XInputGetState;
    static XInputSetState_fcn g_XInputSetState;

    struct ZENI_CORE_DLL XInput {
      XInput();
      ~XInput();

      void poll();

      int index;
      bool connected;
      XINPUT_CAPABILITIES *capabilities;
      XINPUT_STATE *state;
      XINPUT_STATE *state_prev;
      XINPUT_VIBRATION *vibration;
    } m_xinput_controller[4];
#endif

    void init(const bool &try_xinput);
    void uninit();

    bool m_using_xinput;

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
