/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* The Zenipex Library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU General Public License as 
* published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
*
* The Zenipex Library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with the Zenipex Library; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
* 02110-1301 USA.
*
* As a special exception, you may use this file as part of a free software
* library without restriction.  Specifically, if other files instantiate
* templates or use macros or inline functions from this file, or you compile
* this file and link it with other files to produce an executable, this
* file does not by itself cause the resulting executable to be covered by
* the GNU General Public License.  This exception does not however
* invalidate any other reasons why the executable file might be covered by
* the GNU General Public License.
*/

#include <Zeni/Game.hxx>

#include <Zeni/Console_State.h>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Gamestate_One.h>
#include <Zeni/Sound_Source_Pool.h>
#include <Zeni/Timer.hxx>
#include <Zeni/Video.hxx>

#include <Zeni/Global.h>

namespace Zeni {

  Game::Game(const std::vector<std::string> * const args)
    : time(get_Timer().get_time()), ticks_passed(0), fps(END_OF_TIME), fps_next(0)
#ifndef NDEBUG
    , m_console(Gamestate(new Console_State())), m_console_active(false)
#endif
  {
    m_states.push(Gamestate(new Gamestate_One(args)));
  }

  Game & get_Game(const std::vector<std::string> * const &args) {
    static Game e_game(args);
    return e_game;
  }

  void Game::run() {
#ifdef TEST_NASTY_CONDITIONS
    Chronometer<Time> chrono;
    Time::Second_Type time_used = Time::Second_Type();
    chrono.scale(0.5f + 1.5f * Random().frand_lte());
    chrono.start();
#endif

    Sound_Source_Pool &sspr = get_Sound_Source_Pool();
    
    for(;;) {
      for(SDL_Event event; SDL_PollEvent(&event);) {
        if(event.type == SDL_KEYDOWN) {
          const SDL_keysym &s = event.key.keysym;
          const bool alt_only = (get_key_state(SDLK_LALT) || get_key_state(SDLK_RALT)) &&
                                !get_key_state(SDLK_LCTRL) && !get_key_state(SDLK_RCTRL) &&
                                !get_key_state(SDLK_LMETA) && !get_key_state(SDLK_RMETA) &&
                                !get_key_state(SDLK_LSHIFT) && !get_key_state(SDLK_RSHIFT) &&
                                !get_key_state(SDLK_LSUPER) && !get_key_state(SDLK_RSUPER);
          if(s.sym == SDLK_F4 && alt_only)
            throw Quit_Event();
#ifndef NDEBUG
          else if(s.sym == SDLK_BACKQUOTE && alt_only) {
            if(m_console_active)
              deactivate_console();
            else
              activate_console();
            continue;
          }
#endif
        }
#ifndef NDEBUG
        else if(event.type == SDL_KEYUP) {
          const SDL_keysym &s = event.key.keysym;
          const bool alt_only = (get_key_state(SDLK_LALT) || get_key_state(SDLK_RALT)) &&
                                !get_key_state(SDLK_LCTRL) && !get_key_state(SDLK_RCTRL) &&
                                !get_key_state(SDLK_LMETA) && !get_key_state(SDLK_RMETA) &&
                                !get_key_state(SDLK_LSHIFT) && !get_key_state(SDLK_RSHIFT) &&
                                !get_key_state(SDLK_LSUPER) && !get_key_state(SDLK_RSUPER);
          if(s.sym == SDLK_BACKQUOTE && alt_only)
            continue;
        }
#endif

        on_event(event);
      }

#ifdef TEST_NASTY_CONDITIONS
      const Time::Second_Type time_passed = chrono.seconds();
      while(time_used + (1 / 60.0f) < time_passed) {
        time_used += (1 / 60.0f);
        perform_logic();
      }
#else
      perform_logic();
#endif

      sspr.update();

      if(Video::is_enabled())
        get_Video().render_all();
    }
  }

  void Game::calculate_fps() {
    ++fps_next;
    if(time.get_ticks_passed() < ticks_passed)
      return;
    ticks_passed = time.get_ticks_passed() + 1000;
    fps = fps_next;
    fps_next = 0;
  }

#ifndef NDEBUG
  void Game::activate_console() {
    if(!m_states.empty())
      get_console().set_child(m_states.top());
    m_console_active = true;
  }

  void Game::deactivate_console() {
    m_console_active = false;
    get_console().clear_child();
  }
#endif

}

#include <Zeni/Global_Undef.h>
