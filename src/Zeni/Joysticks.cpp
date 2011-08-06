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

  template class ZENI_CORE_DLL Singleton<Joysticks>;

  Joysticks * Joysticks::create() {
    return new Joysticks;
  }

  Singleton<Joysticks>::Uninit Joysticks::g_uninit;
  Singleton<Joysticks>::Reinit Joysticks::g_reinit;

  Joysticks::Joysticks()
    : m_joystick(0)
  {
    Core::remove_post_reinit(&g_reinit);

    /** Initialize SDL itself **/
    Core &cr = get_Core();

    /** Initialize Joysticks **/
    init();

    cr.lend_pre_uninit(&g_uninit);
    cr.lend_post_reinit(&g_reinit);
  }

  Joysticks::~Joysticks() {
    uninit();
  }

  Joysticks & get_Joysticks() {
    return Joysticks::get();
  }

  size_t Joysticks::get_num_joysticks() const {
    return m_joystick.size();
  }

  const String & Joysticks::get_joystick_name(const size_t &index) const {
    assert(index < m_joystick.size());
    return m_joystick[index].second;
  }
  
  int Joysticks::get_joystick_num_axes(const size_t &index) const {
    assert(index < m_joystick.size());
    return SDL_JoystickNumAxes(m_joystick[index].first);
  }

  int Joysticks::get_joystick_num_balls(const size_t &index) const {
    assert(index < m_joystick.size());
    return SDL_JoystickNumBalls(m_joystick[index].first);
  }

  int Joysticks::get_joystick_num_hats(const size_t &index) const {
    assert(index < m_joystick.size());
    return SDL_JoystickNumAxes(m_joystick[index].first);
  }

  int Joysticks::get_joystick_num_buttons(const size_t &index) const {
    assert(index < m_joystick.size());
    return SDL_JoystickNumAxes(m_joystick[index].first);
  }

  void Joysticks::reinit() {
    uninit();
    init();
  }

  void Joysticks::init() {
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
  }

  void Joysticks::uninit() {
    SDL_JoystickEventState(SDL_DISABLE);

    for(int i = 0, end = SDL_NumJoysticks(); i < end; ++i)
      SDL_JoystickClose(m_joystick[size_t(i)].first);

    m_joystick.clear();

    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
  }

}
