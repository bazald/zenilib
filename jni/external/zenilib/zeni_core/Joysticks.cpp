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

  Joysticks::Joysticks() {
    Core::remove_post_reinit(&g_reinit);

    /** Initialize SDL itself **/
    Core &cr = get_Core();

    /** Initialize Joysticks **/
    init();

    cr.lend_pre_uninit(&g_uninit);
    cr.lend_post_reinit(&g_reinit);
  }

  Joysticks::~Joysticks() {
    Core::remove_pre_uninit(&g_uninit);

    uninit();
  }

  Joysticks & get_Joysticks() {
    return Joysticks::get();
  }

  Sint32 Joysticks::get_joystick_id(const Sint32 &index) const {
    return SDL_JoystickInstanceID(m_joysticks.at(index)->joystick);
  }

  Sint32 Joysticks::get_joystick_index(const Sint32 &id) const {
    for(int i = 0, iend = int(m_joysticks.size()); i != iend; ++i) {
      if(SDL_JoystickInstanceID(m_joysticks[i]->joystick) == id)
        return i;
    }

    return -1;
  }

  size_t Joysticks::get_num_joysticks() const {
      return m_joysticks.size();
  }

  const char * Joysticks::get_joystick_name(const Sint32 &index) const {
    assert(index >= 0 && Uint32(index) < m_joysticks.size());
    return SDL_JoystickName(m_joysticks[index]->joystick);
  }
  
  int Joysticks::get_joystick_num_axes(const Sint32 &index) const {
#ifdef ANDROID
    return 0;
#else
    assert(index >= 0 && Uint32(index) < m_joysticks.size());
    return SDL_JoystickNumAxes(m_joysticks[index]->joystick);
#endif
  }

  int Joysticks::get_joystick_num_balls(const Sint32 &index) const {
#ifdef ANDROID
    return 0;
#else
    assert(index >= 0 && Uint32(index) < m_joysticks.size());
    return SDL_JoystickNumBalls(m_joysticks[index]->joystick);
#endif
  }

  int Joysticks::get_joystick_num_hats(const Sint32 &index) const {
#ifdef ANDROID
    return 0;
#else
    assert(index >= 0 && Uint32(index) < m_joysticks.size());
    return SDL_JoystickNumAxes(m_joysticks[index]->joystick);
#endif
  }

  int Joysticks::get_joystick_num_buttons(const Sint32 &index) const {
#ifdef ANDROID
    return 0;
#else
    assert(index >= 0 && Uint32(index) < m_joysticks.size());
    return SDL_JoystickNumAxes(m_joysticks[index]->joystick);
#endif
  }
  
  bool Joysticks::is_joystick_connected(const Sint32 &index) const {
    return SDL_JoystickGetAttached(m_joysticks[index]->joystick) != SDL_FALSE;
  }

  void Joysticks::reinit() {
    uninit();
    init();
  }

  void Joysticks::enable(const bool &
#ifndef ANDROID
    enable_
#endif
  ) {
#ifndef ANDROID
    SDL_JoystickEventState(enable_ ? SDL_ENABLE : SDL_DISABLE);
#endif
  }
  
  void Joysticks::poll() {
    for(int i = 0, iend = SDL_NumJoysticks(); i != iend; ++i) {
      SDL_Joystick * const joystick = SDL_JoystickOpen(i);
      if(!joystick) {
        uninit();
        throw Joystick_Init_Failure();
      }

      Joystick_Array::iterator found = m_joysticks.end();
      for(Joystick_Array::iterator it = m_joysticks.begin(), iend = m_joysticks.end(); it != iend; ++it) {
        if(SDL_JoystickInstanceID(joystick) == SDL_JoystickInstanceID((*it)->joystick)) {
          found = it;
          break;
        }
        else if(SDL_JoystickGetAttached((*it)->joystick) == SDL_FALSE)
          found = it;
      }

      SDL_Haptic * haptic = SDL_JoystickIsHaptic(joystick) == 1 ? SDL_HapticOpenFromJoystick(joystick) : nullptr;

      Joystick_Info * info = new Joystick_Info();
      info->joystick = joystick;
      info->haptic = haptic;
      if(SDL_HapticEffectSupported(info->haptic, &info->haptic_effect) == SDL_TRUE)
        info->haptic_effect_id = SDL_HapticNewEffect(info->haptic, &info->haptic_effect);

      if(found == m_joysticks.end())
        m_joysticks.push_back(info);
      else if(SDL_JoystickInstanceID(joystick) != SDL_JoystickInstanceID((*found)->joystick)) {
        delete *found;
        *found = info;
      }
    }
  }

  void Joysticks::set_vibration(const size_t &index, const float &left, const float &right) {
    if(index < m_joysticks.size()) {
      m_joysticks[index]->haptic_effect.leftright.large_magnitude = Uint16(left * 65535);
      m_joysticks[index]->haptic_effect.leftright.small_magnitude = Uint16(right * 65535);
      SDL_HapticUpdateEffect(m_joysticks[index]->haptic, 0, &m_joysticks[index]->haptic_effect);
      if(m_joysticks[index]->haptic_effect_id > -1) {
        switch(SDL_HapticGetEffectStatus(m_joysticks[index]->haptic, m_joysticks[index]->haptic_effect_id)) {
          case 0: SDL_HapticRunEffect(m_joysticks[index]->haptic, m_joysticks[index]->haptic_effect_id, 1);
                  break;
          case 1: SDL_HapticUpdateEffect(m_joysticks[index]->haptic, m_joysticks[index]->haptic_effect_id, &m_joysticks[index]->haptic_effect);
                  break;
          default: break;
        }
      }
    }
  }

  Joysticks::Joystick_Info::Joystick_Info()
    : joystick(nullptr),
    haptic(nullptr),
    haptic_effect_id(-1)
  {
    memset(&haptic_effect, 0, sizeof(haptic_effect));
    haptic_effect.type = SDL_HAPTIC_LEFTRIGHT;
    haptic_effect.leftright.length = SDL_HAPTIC_INFINITY;
  }

  Joysticks::Joystick_Info::~Joystick_Info() {
    SDL_HapticClose(haptic);
    SDL_JoystickClose(joystick);
  }
  
  void Joysticks::init() {
#ifndef ANDROID
    if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) == -1)
      throw Joystick_Init_Failure();

    SDL_JoystickEventState(SDL_ENABLE);
#endif
  }

  void Joysticks::uninit() {
#ifndef ANDROID
    SDL_JoystickEventState(SDL_DISABLE);

    for(Joystick_Array::iterator it = m_joysticks.begin(), iend = m_joysticks.end(); it != iend; ++it)
      delete *it;
    m_joysticks.clear();

    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
#endif
  }

#ifdef ENABLE_XINPUT
  Joysticks::XInputEnable_fcn Joysticks::g_XInputEnable = 0;
  Joysticks::XInputGetCapabilities_fcn Joysticks::g_XInputGetCapabilities = 0;
  Joysticks::XInputGetState_fcn Joysticks::g_XInputGetState = 0;
  Joysticks::XInputSetState_fcn Joysticks::g_XInputSetState = 0;
#endif

}
