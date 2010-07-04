/* This file is part of the Zenipex Library.
* Copyleft (C) 2010 Mitchell Keith Bloch a.k.a. bazald
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

#include <Zeni/Chronometer.hxx>
#include <Zeni/Console_State.h>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Gamestate_One.h>
#include <Zeni/Random.h>
#include <Zeni/Sound_Source_Pool.h>
#include <Zeni/Timer.hxx>
#include <Zeni/Video.hxx>

#include <Zeni/Global.h>

namespace Zeni {

  Game::Game(const std::vector<std::string> * const args)
    : time(get_Timer().get_time()), ticks_passed(0), fps(END_OF_TIME), fps_next(0)
#ifndef NDEBUG
    , m_console_active(false)
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
    Random random;
    const float time_scale = NASTY_MIN_RATE + (NASTY_MAX_RATE - NASTY_MIN_RATE) * random.frand_lte();
    Time::Second_Type time_used = Time::Second_Type();
    Time start_time;
#endif

    Sound_Source_Pool &sspr = get_Sound_Source_Pool();
    
    for(;;) {
      for(SDL_Event event; SDL_PollEvent(&event);) {
        if(event.type == SDL_KEYDOWN ||
           event.type == SDL_KEYUP)
        {
          const SDL_keysym &s = event.key.keysym;
          const bool modifiers = 
#if defined(_MACOSX)
#if SDL_VERSION_ATLEAST(1,3,0)
                                (get_key_state(SDLK_LGUI) || get_key_state(SDLK_RGUI)) &&
#else
                                (get_key_state(SDLK_LMETA) || get_key_state(SDLK_RMETA) || get_key_state(SDLK_LSUPER) || get_key_state(SDLK_RSUPER)) &&
#endif
                                !get_key_state(SDLK_LALT) && !get_key_state(SDLK_LALT) &&
                                !get_key_state(SDLK_LCTRL) && !get_key_state(SDLK_RCTRL) &&
#elif defined(_WINDOWS)
                                (get_key_state(SDLK_LALT) || get_key_state(SDLK_RALT)) &&
                                !get_key_state(SDLK_LCTRL) && !get_key_state(SDLK_RCTRL) &&
#if SDL_VERSION_ATLEAST(1,3,0)
                                !get_key_state(SDLK_LGUI) && !get_key_state(SDLK_RGUI) &&
#else
                                !get_key_state(SDLK_LMETA) && !get_key_state(SDLK_RMETA) && !get_key_state(SDLK_LSUPER) && !get_key_state(SDLK_RSUPER) &&
#endif
#else
                                (get_key_state(SDLK_LCTRL) || get_key_state(SDLK_RCTRL)) &&
                                !get_key_state(SDLK_LALT) && !get_key_state(SDLK_LALT) &&
#if SDL_VERSION_ATLEAST(1,3,0)
                                !get_key_state(SDLK_LGUI) && !get_key_state(SDLK_RGUI) &&
#else
                                !get_key_state(SDLK_LMETA) && !get_key_state(SDLK_RMETA) && !get_key_state(SDLK_LSUPER) && !get_key_state(SDLK_RSUPER) &&
#endif
#endif
                                !get_key_state(SDLK_LSHIFT) && !get_key_state(SDLK_RSHIFT);

#ifndef NDEBUG
          if(s.sym == SDLK_BACKQUOTE && modifiers) {
            if(event.type == SDL_KEYDOWN) {
              if(m_console_active)
                deactivate_console();
              else
                activate_console();
            }

            continue;
          }
#endif

          on_event(event);

          if(event.type == SDL_KEYDOWN && modifiers &&
#ifdef _WINDOWS
             s.sym == SDLK_F4)
#else
             s.sym == SDLK_q)
#endif
          {
            throw Quit_Event();
          }
        }
#if SDL_VERSION_ATLEAST(1,3,0)
        else if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
          on_event(event);

          if(event.window.windowID == SDL_GetWindowID(get_Video().get_window())) {
            get_Video().alert_window_destroyed();
            throw Quit_Event();
          }
        }
#endif
        else {
          on_event(event);

          if(event.type == SDL_QUIT)
            throw Quit_Event();
        }
      }

#ifdef TEST_NASTY_CONDITIONS
      const Time current_time;
      const Time::Second_Type time_passed = time_scale * current_time.get_seconds_since(start_time);
      size_t step_count = 0u;
      while(time_used + (1 / 60.0f) < time_passed) {
        time_used += (1 / 60.0f);
        perform_logic();
        if(++step_count == NASTY_RATE_CUTOFF)
          time_used = time_passed;
      }
      if(!random.rand_lt(NASTY_ZERO_STEP_FREQUENCY))
        perform_logic();
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

    get_console().m_prompt.seek(get_console().m_prompt.get_max_seek());

    m_console_active = true;
  }

  void Game::deactivate_console() {
    m_console_active = false;
    get_console().clear_child();
  }

  Gamestate & Game::get_console_instance() {
    static Gamestate console_state(Gamestate(new Console_State()));
    return console_state;
  }
#endif

}

#include <Zeni/Global_Undef.h>
