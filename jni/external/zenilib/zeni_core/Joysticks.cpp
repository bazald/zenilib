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

#ifdef ENABLE_XINPUT
#include <Xinput.h>
#endif

#include <cassert>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Singleton.hxx>

namespace Zeni {

  template class Singleton<Joysticks>;

  Joysticks * Joysticks::create() {
    return new Joysticks;
  }

  Singleton<Joysticks>::Uninit Joysticks::g_uninit;
  Singleton<Joysticks>::Reinit Joysticks::g_reinit;

  Joysticks::Joysticks()
    : m_using_xinput(true),
    m_joystick(0)
  {
    Core::remove_post_reinit(&g_reinit);

    /** Initialize SDL itself **/
    Core &cr = get_Core();

#ifdef ENABLE_XINPUT
    /** Initialize Xinput **/

    m_xinput = LoadLibrary("xinput1_3.dll");
    if(!m_xinput) m_xinput = LoadLibrary("xinput1_2.dll");
    if(!m_xinput) m_xinput = LoadLibrary("xinput1_1.dll");
    if(!m_xinput) {
      std::cerr << "Loading xinput1.dll failed." << std::endl;

      zero_handles();
    }
    else {
      g_XInputEnable = (XInputEnable_fcn)GetProcAddress(m_xinput, "XInputEnable");
      g_XInputGetCapabilities = (XInputGetCapabilities_fcn)GetProcAddress(m_xinput, "XInputGetCapabilities");
      g_XInputGetState = (XInputGetState_fcn)GetProcAddress(m_xinput, "XInputGetState");
      g_XInputSetState = (XInputSetState_fcn)GetProcAddress(m_xinput, "XInputSetState");
      if(!g_XInputEnable || !g_XInputGetCapabilities || !g_XInputGetState || !g_XInputSetState) {
        std::cerr << "Loading xinput1.dll failed." << std::endl;

        FreeLibrary(m_xinput);

        zero_handles();
      }
      else {
        for(int i = 0; i != 4; ++i)
          m_xinput_controller[i].index = i;
      }
    }
#endif

    /** Initialize Joysticks **/
    init(m_using_xinput);

    cr.lend_pre_uninit(&g_uninit);
    cr.lend_post_reinit(&g_reinit);
  }

  Joysticks::~Joysticks() {
    Core::remove_pre_uninit(&g_uninit);

    uninit();
    
#ifdef ENABLE_XINPUT
    FreeLibrary(m_xinput);

    zero_handles();
#endif
  }

  Joysticks & get_Joysticks() {
    return Joysticks::get();
  }

  size_t Joysticks::get_num_joysticks() const {
    if(m_using_xinput)
      return 4;
    else
      return m_joystick.size();
  }

  const char * const Joysticks::get_joystick_name(const size_t &index) const {
    if(m_using_xinput) {
      assert(index < 4);
      return "Controller (XBOX 360 For Windows)";
    }
    else {
      assert(index < m_joystick.size());
      return m_joystick[index].second.c_str();
    }
  }
  
  int Joysticks::get_joystick_num_axes(const size_t &index) const {
#ifdef ANDROID
    return 0;
#else
    if(m_using_xinput) {
      assert(index < 4);
      return 6;
    }
    else {
      assert(index < m_joystick.size());
      return SDL_JoystickNumAxes(m_joystick[index].first);
    }
#endif
  }

  int Joysticks::get_joystick_num_balls(const size_t &index) const {
#ifdef ANDROID
    return 0;
#else
    if(m_using_xinput) {
      assert(index < 4);
      return 0;
    }
    else {
      assert(index < m_joystick.size());
      return SDL_JoystickNumBalls(m_joystick[index].first);
    }
#endif
  }

  int Joysticks::get_joystick_num_hats(const size_t &index) const {
#ifdef ANDROID
    return 0;
#else
    if(m_using_xinput) {
      assert(index < 4);
      return 1;
    }
    else {
      assert(index < m_joystick.size());
      return SDL_JoystickNumAxes(m_joystick[index].first);
    }
#endif
  }

  int Joysticks::get_joystick_num_buttons(const size_t &index) const {
#ifdef ANDROID
    return 0;
#else
    if(m_using_xinput) {
      assert(index < 4);
      return 10;
    }
    else {
      assert(index < m_joystick.size());
      return SDL_JoystickNumAxes(m_joystick[index].first);
    }
#endif
  }

  void Joysticks::reinit() {
    uninit();
    init(m_using_xinput);
  }

  void Joysticks::reinit(const bool &try_xinput) {
    uninit();
    init(try_xinput);
  }

  void Joysticks::enable(const bool &
#ifndef ANDROID
    enable_
#endif
  ) {
#ifndef ANDROID
#ifdef ENABLE_XINPUT
    if(m_using_xinput)
      XInputEnable()(enable_);
    else
#endif
      SDL_JoystickEventState(enable_ ? SDL_ENABLE : SDL_DISABLE);
#endif
  }

#ifdef ENABLE_XINPUT
  void Joysticks::zero_handles() {
    g_XInputEnable = 0;
    g_XInputGetCapabilities = 0;
    g_XInputGetState = 0;
    g_XInputSetState = 0;
  }

  void Joysticks::poll() {
    if(!m_using_xinput)
      return;

    for(int i = 0; i != 4; ++i)
      m_xinput_controller[i].poll();
  }

  bool Joysticks::is_xinput_connected(const size_t &index) const {
    assert(index < 4lu);
    return m_xinput_controller[index].connected;
  }

  const XINPUT_CAPABILITIES & Joysticks::get_xinput_capabilities(const size_t &index) const {
    assert(index < 4lu);
    return *m_xinput_controller[index].capabilities;
  }

  const XINPUT_STATE & Joysticks::get_xinput_state(const size_t &index) const {
    assert(index < 4lu);
    return *m_xinput_controller[index].state;
  }

  void Joysticks::set_xinput_vibration(const size_t &index, const float &left, const float &right) {
    assert(index < 4lu);
    m_xinput_controller[index].vibration->wLeftMotorSpeed = WORD(left * 65535);
    m_xinput_controller[index].vibration->wRightMotorSpeed = WORD(right * 65535);
  }

  Joysticks::XInput::XInput()
    : index(-1),
    connected(false),
    capabilities(0),
    state(0),
    state_prev(0),
    vibration(0)
  {
    try {
      capabilities = new XINPUT_CAPABILITIES;
      state = new XINPUT_STATE;
      state_prev = new XINPUT_STATE;
      vibration = new XINPUT_VIBRATION;
    }
    catch(...) {
      delete capabilities;
      delete state;
      delete state_prev;
      delete vibration;
      throw;
    }

    ZeroMemory(state, sizeof(XINPUT_STATE));
    ZeroMemory(state_prev, sizeof(XINPUT_STATE));

    vibration->wLeftMotorSpeed = 0;
    vibration->wRightMotorSpeed = 0;
  }

  Joysticks::XInput::~XInput() {
    delete capabilities;
    delete state;
    delete state_prev;
    delete vibration;
  }

  void Joysticks::XInput::poll() {
    ZeroMemory(capabilities, sizeof(XINPUT_CAPABILITIES));

    DWORD rv = XInputGetCapabilities()(DWORD(index), XINPUT_FLAG_GAMEPAD, capabilities);
    if(rv == ERROR_DEVICE_NOT_CONNECTED || FAILED(rv))
      connected = false;
    else {
      rv = XInputSetState()(DWORD(index), vibration);
      if(rv == ERROR_DEVICE_NOT_CONNECTED || FAILED(rv))
        connected = false;
      else {
        if(connected)
          memcpy(state_prev, state, sizeof(XINPUT_STATE));

        rv = XInputGetState()(DWORD(index), state);
        connected = rv != ERROR_DEVICE_NOT_CONNECTED && !FAILED(rv);

        if(connected && state->dwPacketNumber != state_prev->dwPacketNumber) {
          if(state->Gamepad.sThumbLX != state_prev->Gamepad.sThumbLX) {
            SDL_Event e;

            e.type = SDL_JOYAXISMOTION;
            e.jaxis.which = Uint8(index);
            e.jaxis.axis = AXIS_LEFT_THUMB_X;
            e.jaxis.value = state->Gamepad.sThumbLX;

            SDL_PushEvent(&e);
          }
          if(state->Gamepad.sThumbLY != state_prev->Gamepad.sThumbLY) {
            SDL_Event e;

            e.type = SDL_JOYAXISMOTION;
            e.jaxis.which = Uint8(index);
            e.jaxis.axis = AXIS_LEFT_THUMB_Y;
            e.jaxis.value = -state->Gamepad.sThumbLY - 1;

            SDL_PushEvent(&e);
          }
          if(state->Gamepad.bRightTrigger != state_prev->Gamepad.bRightTrigger) {
            SDL_Event e;

            e.type = SDL_JOYAXISMOTION;
            e.jaxis.which = Uint8(index);
            e.jaxis.axis = AXIS_RIGHT_TRIGGER;
            e.jaxis.value = state->Gamepad.bRightTrigger * 128;

            SDL_PushEvent(&e);
          }
          if(state->Gamepad.sThumbRY != state_prev->Gamepad.sThumbRY) {
            SDL_Event e;

            e.type = SDL_JOYAXISMOTION;
            e.jaxis.which = Uint8(index);
            e.jaxis.axis = AXIS_RIGHT_THUMB_Y;
            e.jaxis.value = -state->Gamepad.sThumbRY - 1;

            SDL_PushEvent(&e);
          }
          if(state->Gamepad.sThumbRX != state_prev->Gamepad.sThumbRX) {
            SDL_Event e;

            e.type = SDL_JOYAXISMOTION;
            e.jaxis.which = Uint8(index);
            e.jaxis.axis = AXIS_RIGHT_THUMB_X;
            e.jaxis.value = state->Gamepad.sThumbRX;

            SDL_PushEvent(&e);
          }
          if(state->Gamepad.bLeftTrigger != state_prev->Gamepad.bLeftTrigger) {
            SDL_Event e;

            e.type = SDL_JOYAXISMOTION;
            e.jaxis.which = Uint8(index);
            e.jaxis.axis = AXIS_LEFT_TRIGGER;
            e.jaxis.value = state->Gamepad.bLeftTrigger * 128;

            SDL_PushEvent(&e);
          }
          if((state->Gamepad.wButtons & 0xF) != (state_prev->Gamepad.wButtons & 0xF)) {
            SDL_Event e;

            e.type = SDL_JOYHATMOTION;
            e.jhat.which = Uint8(index);
            e.jhat.hat = 0;

            switch(state->Gamepad.wButtons & 0xF) {
            case 0x1:
              e.jhat.value = SDL_HAT_UP;
              break;

            case 0x2:
              e.jhat.value = SDL_HAT_DOWN;
              break;

            case 0x4:
              e.jhat.value = SDL_HAT_LEFT;
              break;

            case 0x5:
              e.jhat.value = SDL_HAT_LEFTUP;
              break;

            case 0x6:
              e.jhat.value = SDL_HAT_LEFTDOWN;
              break;

            case 0x8:
              e.jhat.value = SDL_HAT_RIGHT;
              break;

            case 0x9:
              e.jhat.value = SDL_HAT_RIGHTUP;
              break;

            case 0xA:
              e.jhat.value = SDL_HAT_RIGHTDOWN;
              break;

            default:
              e.jhat.value = SDL_HAT_CENTERED;
              break;
            }

            SDL_PushEvent(&e);
          }
          if((state->Gamepad.wButtons & XINPUT_GAMEPAD_A) != (state_prev->Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
            SDL_Event e;

            e.type = Uint8((state->Gamepad.wButtons & XINPUT_GAMEPAD_A) ? SDL_JOYBUTTONDOWN : SDL_JOYBUTTONUP);
            e.jbutton.which = Uint8(index);
            e.jbutton.button = BUTTON_A;
            e.jbutton.state = Uint8(e.type == SDL_JOYBUTTONDOWN ? SDL_PRESSED : SDL_RELEASED);

            SDL_PushEvent(&e);
          }
          if((state->Gamepad.wButtons & XINPUT_GAMEPAD_B) != (state_prev->Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
            SDL_Event e;
            
            e.type = Uint8((state->Gamepad.wButtons & XINPUT_GAMEPAD_B) ? SDL_JOYBUTTONDOWN : SDL_JOYBUTTONUP);
            e.jbutton.which = Uint8(index);
            e.jbutton.button = BUTTON_B;
            e.jbutton.state = Uint8(e.type == SDL_JOYBUTTONDOWN ? SDL_PRESSED : SDL_RELEASED);

            SDL_PushEvent(&e);
          }
          if((state->Gamepad.wButtons & XINPUT_GAMEPAD_X) != (state_prev->Gamepad.wButtons & XINPUT_GAMEPAD_X)) {
            SDL_Event e;
            
            e.type = Uint8((state->Gamepad.wButtons & XINPUT_GAMEPAD_X) ? SDL_JOYBUTTONDOWN : SDL_JOYBUTTONUP);
            e.jbutton.which = Uint8(index);
            e.jbutton.button = BUTTON_X;
            e.jbutton.state = Uint8(e.type == SDL_JOYBUTTONDOWN ? SDL_PRESSED : SDL_RELEASED);

            SDL_PushEvent(&e);
          }
          if((state->Gamepad.wButtons & XINPUT_GAMEPAD_Y) != (state_prev->Gamepad.wButtons & XINPUT_GAMEPAD_Y)) {
            SDL_Event e;
            
            e.type = Uint8((state->Gamepad.wButtons & XINPUT_GAMEPAD_Y) ? SDL_JOYBUTTONDOWN : SDL_JOYBUTTONUP);
            e.jbutton.which = Uint8(index);
            e.jbutton.button = BUTTON_Y;
            e.jbutton.state = Uint8(e.type == SDL_JOYBUTTONDOWN ? SDL_PRESSED : SDL_RELEASED);

            SDL_PushEvent(&e);
          }
          if((state->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != (state_prev->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)) {
            SDL_Event e;
            
            e.type = Uint8((state->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? SDL_JOYBUTTONDOWN : SDL_JOYBUTTONUP);
            e.jbutton.which = Uint8(index);
            e.jbutton.button = BUTTON_LEFT_SHOULDER;
            e.jbutton.state = Uint8(e.type == SDL_JOYBUTTONDOWN ? SDL_PRESSED : SDL_RELEASED);

            SDL_PushEvent(&e);
          }
          if((state->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != (state_prev->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
            SDL_Event e;
            
            e.type = Uint8((state->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? SDL_JOYBUTTONDOWN : SDL_JOYBUTTONUP);
            e.jbutton.which = Uint8(index);
            e.jbutton.button = BUTTON_RIGHT_SHOULDER;
            e.jbutton.state = Uint8(e.type == SDL_JOYBUTTONDOWN ? SDL_PRESSED : SDL_RELEASED);

            SDL_PushEvent(&e);
          }
          if((state->Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != (state_prev->Gamepad.wButtons & XINPUT_GAMEPAD_BACK)) {
            SDL_Event e;
            
            e.type = Uint8((state->Gamepad.wButtons & XINPUT_GAMEPAD_BACK) ? SDL_JOYBUTTONDOWN : SDL_JOYBUTTONUP);
            e.jbutton.which = Uint8(index);
            e.jbutton.button = BUTTON_BACK;
            e.jbutton.state = Uint8(e.type == SDL_JOYBUTTONDOWN ? SDL_PRESSED : SDL_RELEASED);

            SDL_PushEvent(&e);
          }
          if((state->Gamepad.wButtons & XINPUT_GAMEPAD_START) != (state_prev->Gamepad.wButtons & XINPUT_GAMEPAD_START)) {
            SDL_Event e;
            
            e.type = Uint8((state->Gamepad.wButtons & XINPUT_GAMEPAD_START) ? SDL_JOYBUTTONDOWN : SDL_JOYBUTTONUP);
            e.jbutton.which = Uint8(index);
            e.jbutton.button = BUTTON_START;
            e.jbutton.state = Uint8(e.type == SDL_JOYBUTTONDOWN ? SDL_PRESSED : SDL_RELEASED);

            SDL_PushEvent(&e);
          }
          if((state->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != (state_prev->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)) {
            SDL_Event e;
            
            e.type = Uint8((state->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? SDL_JOYBUTTONDOWN : SDL_JOYBUTTONUP);
            e.jbutton.which = Uint8(index);
            e.jbutton.button = BUTTON_LEFT_THUMB;
            e.jbutton.state = Uint8(e.type == SDL_JOYBUTTONDOWN ? SDL_PRESSED : SDL_RELEASED);

            SDL_PushEvent(&e);
          }
          if((state->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != (state_prev->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)) {
            SDL_Event e;
            
            e.type = Uint8((state->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) ? SDL_JOYBUTTONDOWN : SDL_JOYBUTTONUP);
            e.jbutton.which = Uint8(index);
            e.jbutton.button = BUTTON_RIGHT_THUMB;
            e.jbutton.state = Uint8(e.type == SDL_JOYBUTTONDOWN ? SDL_PRESSED : SDL_RELEASED);

            SDL_PushEvent(&e);
          }
        }
      }
    }
  }
#endif

  void Joysticks::init(const bool &
#ifdef ENABLE_XINPUT
    try_xinput
#endif
    ) {
#ifdef ENABLE_XINPUT
    if(try_xinput && g_XInputEnable) {
      m_using_xinput = true;

      XInputEnable()(true);

      poll();
    }
    else
#endif
    {
      m_using_xinput = false;

#ifndef ANDROID
      if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) == -1)
        throw Joystick_Init_Failure();

      for(int i = 0, end = SDL_NumJoysticks(); i < end; ++i) {
        m_joystick.push_back(std::make_pair(SDL_JoystickOpen(i),
                                            SDL_JoystickName(i)));

        if(!m_joystick[size_t(i)].first) {
          m_joystick.pop_back();
          uninit();

          throw Joystick_Init_Failure();
        }
      }

      SDL_JoystickEventState(SDL_ENABLE);
#endif
    }
  }

  void Joysticks::uninit() {
#ifndef ANDROID
    if(m_using_xinput) {
#ifdef ENABLE_XINPUT
      XInputEnable()(false);
#endif
    }
    else {
      SDL_JoystickEventState(SDL_DISABLE);

      for(int i = 0, end = SDL_NumJoysticks(); i < end; ++i)
        SDL_JoystickClose(m_joystick[size_t(i)].first);

      m_joystick.clear();

      SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    }
#endif
  }

#ifdef ENABLE_XINPUT
  Joysticks::XInputEnable_fcn Joysticks::g_XInputEnable = 0;
  Joysticks::XInputGetCapabilities_fcn Joysticks::g_XInputGetCapabilities = 0;
  Joysticks::XInputGetState_fcn Joysticks::g_XInputGetState = 0;
  Joysticks::XInputSetState_fcn Joysticks::g_XInputSetState = 0;
#endif

}
