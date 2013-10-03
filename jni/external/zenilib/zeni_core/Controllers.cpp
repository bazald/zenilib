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

  template class Singleton<Controllers>;

  Controllers * Controllers::create() {
    return new Controllers;
  }

  Singleton<Controllers>::Uninit Controllers::g_uninit;
  Singleton<Controllers>::Reinit Controllers::g_reinit;

  Controllers::Controllers() {
    Core::remove_post_reinit(&g_reinit);

    /** Initialize SDL itself **/
    Core &cr = get_Core();

    /** Initialize Controllers **/
    init();

    cr.lend_pre_uninit(&g_uninit);
    cr.lend_post_reinit(&g_reinit);
  }

  Controllers::~Controllers() {
    Core::remove_pre_uninit(&g_uninit);

    uninit();
  }

  Controllers & get_Controllers() {
    return Controllers::get();
  }

  size_t Controllers::get_num_controllers() const {
      return m_joysticks.size();
  }

  Sint32 Controllers::get_controller_index(const Sint32 &id) const {
    for(size_t i = 0, iend = m_joysticks.size(); i != iend; ++i) {
      if(m_joysticks[i] && m_joysticks[i]->joystick_id == id)
        return Sint32(i);
    }

    return -1;
  }

  const char * Controllers::get_controller_name(const Sint32 &index) const {
    assert(index >= 0 && Uint32(index) < m_joysticks.size());
    return SDL_JoystickName(m_joysticks[index]->joystick);
  }
  
  bool Controllers::is_controller_connected(const Sint32 &index) const {
    if(m_joysticks[index]->gamecontroller)
      return SDL_GameControllerGetAttached(m_joysticks[index]->gamecontroller) == SDL_TRUE;
    return false;
  }

  void Controllers::reinit() {
    uninit();
    init();
  }

  void Controllers::enable(const bool &
#ifndef ANDROID
    enable_
#endif
  ) {
#ifndef ANDROID
    SDL_GameControllerEventState(enable_ ? SDL_ENABLE : SDL_DISABLE);
#endif
  }
  
  void Controllers::device_added(const Sint32 &index) {
    SDL_Joystick * const joystick = SDL_JoystickOpen(index);
    if(!joystick)
      return;

    Joystick_Info * info = new Joystick_Info;
    info->joystick = joystick;
    info->joystick_id = SDL_JoystickInstanceID(joystick);

    for(Joystick_Array::iterator it = m_joysticks.begin(), iend = m_joysticks.end(); ; ++it) {
      if(it != iend) {
        if(!*it) {
          *it = info;
          break;
        }
      }
      else {
        m_joysticks.push_back(info);
        break;
      }
    }

    char szGUID[33];
    SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(info->joystick), szGUID, sizeof(szGUID));

    if(SDL_IsGameController(index))
      info->gamecontroller = SDL_GameControllerOpen(index);
    if(info->gamecontroller) {
      char * mapping = SDL_GameControllerMapping(info->gamecontroller);
      std::cerr << "Loaded game controller " << info->joystick_id << ": " << mapping << std::endl;
      SDL_free(mapping);
    }
    else {
      std::cerr << "Failed to load game controller (" << szGUID << "): " << SDL_GetError() << std::endl;
      SDL_ClearError();
    }

    if(SDL_JoystickIsHaptic(info->joystick) == 1)
      info->haptic = SDL_HapticOpenFromJoystick(info->joystick);
    if(info->haptic) {
      if(SDL_HapticEffectSupported(info->haptic, &info->haptic_effect) == SDL_TRUE) {
        info->haptic_effect_id = SDL_HapticNewEffect(info->haptic, &info->haptic_effect);
        if(SDL_HapticRunEffect(info->haptic, info->haptic_effect_id, 1) == -1) {
          std::cerr << "Failed to run effect on joystick (" << szGUID << ")." << std::endl;
          SDL_ClearError();
        }
      }
    }
    else {
      std::cerr << "Joystick (" << szGUID << ") not recognized as haptic." << std::endl;
      SDL_ClearError();
    }
  }

  void Controllers::device_removed(const Sint32 &id) {
    for(Joystick_Array::iterator it = m_joysticks.begin(), iend = m_joysticks.end(); it != iend; ++it) {
      if(*it && (*it)->joystick_id == id) {
        delete *it;
        *it = 0;
        break;
      }
    }
  }

  void Controllers::detect_removed() {
    for(Joystick_Array::reverse_iterator it = m_joysticks.rbegin(), iend = m_joysticks.rend(); it != iend; ++it) {
      if(*it && SDL_JoystickGetAttached((*it)->joystick) == SDL_FALSE)
        device_removed((*it)->joystick_id);
    }

    //for(int i = SDL_NumJoysticks() - 1; i > -1; --i) {
    //  SDL_Joystick * joystick = SDL_JoystickOpen(i);
    //  Sint32 joystick_id = SDL_JoystickInstanceID(joystick);
    //  SDL_JoystickClose(joystick);
    //  bool found = false;
    //  for(Joystick_Array::reverse_iterator it = m_joysticks.rbegin(), iend = m_joysticks.rend(); it != iend; ++it) {
    //    if((*it) && (*it)->joystick_id == joystick_id) {
    //      found = true;
    //      break;
    //    }
    //  }
    //  if(!found)
    //    device_added(i);
    //}
  }
  
  void Controllers::device_add_all() {
    for(int i = 0, iend = SDL_NumJoysticks(); i != iend; ++i)
      device_added(i);
  }

  void Controllers::set_vibration(const size_t &index, const float &left, const float &right) {
    if(index < m_joysticks.size() && m_joysticks[index]) {
      m_joysticks[index]->haptic_effect.leftright.large_magnitude = Uint16(left * 0xFFFF);
      m_joysticks[index]->haptic_effect.leftright.small_magnitude = Uint16(right * 0xFFFF);
      if(m_joysticks[index]->haptic_effect_id > -1) {
        if(SDL_HapticUpdateEffect(m_joysticks[index]->haptic, m_joysticks[index]->haptic_effect_id, &m_joysticks[index]->haptic_effect) == -1)
          SDL_ClearError();
      }
    }
  }

  void Controllers::reset_vibration_all() {
    for(int i = 0, iend = m_joysticks.size(); i != iend; ++i)
      set_vibration(i, 0.0f, 0.0f);
  }

  Controllers::Joystick_Info::Joystick_Info()
    : joystick(0),
    gamecontroller(0),
    haptic(0),
    haptic_effect_id(-1)
  {
    memset(&haptic_effect, 0, sizeof(haptic_effect));
    haptic_effect.type = SDL_HAPTIC_LEFTRIGHT;
    haptic_effect.leftright.length = SDL_HAPTIC_INFINITY;
  }

  Controllers::Joystick_Info::~Joystick_Info() {
    get_Core().assert_no_error();
    if(haptic) {
      if(haptic_effect_id > -1)
        SDL_HapticDestroyEffect(haptic, haptic_effect_id);
      SDL_HapticClose(haptic);
      SDL_ClearError(); ///< HACK
    }
    if(gamecontroller)
      SDL_GameControllerClose(gamecontroller);
    SDL_JoystickClose(joystick);
    get_Core().assert_no_error();
  }
  
  void Controllers::init() {
#ifndef ANDROID
    if(SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) == -1)
      throw Controllers_Init_Failure();

    const String appdata_path = get_File_Ops().get_appdata_path();
    const String user_normal = appdata_path + "config/zenilib.xml";
    const String local_normal = "config/zenilib.xml";
    XML_Document file;

    if(file.try_load(local_normal)) {
      XML_Element_c zenilib = file["Zenilib"];
      XML_Element_c joysticks = zenilib["Controllers"];
      if(joysticks.good()) {
        for(XML_Element_c joystick = joysticks.first(); joystick.good(); joystick = joystick.next()) {
          if(joystick.value() == "GameController") {
            if(SDL_GameControllerAddMapping(joystick.to_string().c_str()) < 0)
              std::cerr << "Joystick mapping " << joystick.to_string().c_str() << " failed." << std::endl;
            else
              std::cerr << "Added/Updating joystick mapping: " << joystick.to_string().c_str() << std::endl;
          }
        }
      }
    }
    
    if(file.try_load(user_normal)) {
      XML_Element_c zenilib = file["Zenilib"];
      XML_Element_c joysticks = zenilib["Controllers"];
      if(joysticks.good()) {
        for(XML_Element_c joystick = joysticks.first(); joystick.good(); joystick = joystick.next()) {
          if(joystick.value() == "GameController") {
            if(SDL_GameControllerAddMapping(joystick.to_string().c_str()) < 0)
              std::cerr << "Joystick mapping " << joystick.to_string().c_str() << " failed." << std::endl;
            else
              std::cerr << "Added/Updating joystick mapping: " << joystick.to_string().c_str() << std::endl;
          }
        }
      }
    }

    device_add_all();

    SDL_JoystickEventState(SDL_ENABLE);
    SDL_GameControllerEventState(SDL_ENABLE);
#endif
  }

  void Controllers::uninit() {
#ifndef ANDROID
    SDL_GameControllerEventState(SDL_DISABLE);
    SDL_JoystickEventState(SDL_DISABLE);

    for(Joystick_Array::iterator it = m_joysticks.begin(), iend = m_joysticks.end(); it != iend; ++it)
      delete *it;
    m_joysticks.clear();

    SDL_QuitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);

    get_Core().assert_no_error();
#endif
  }

#ifdef ENABLE_XINPUT
  Controllers::XInputEnable_fcn Controllers::g_XInputEnable = 0;
  Controllers::XInputGetCapabilities_fcn Controllers::g_XInputGetCapabilities = 0;
  Controllers::XInputGetState_fcn Controllers::g_XInputGetState = 0;
  Controllers::XInputSetState_fcn Controllers::g_XInputSetState = 0;
#endif

}
