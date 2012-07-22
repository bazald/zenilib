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

#include <zeni_rest.h>

#include <Zeni/Define.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Singleton.hxx>

namespace Zeni {

  Gamestate_Zero_Initializer * g_gzi = 0;
  int g_argc = 0;
  const char * const * g_argv = 0;
  template class Singleton<Game>;

  Game * Game::create() {
    return new Game;
  }

  Game::Game()
    : time(get_Timer().get_time()), ticks_passed(0), fps(END_OF_TIME), fps_next(0)
#if !defined(ANDROID) && !defined(NDEBUG)
    , m_console_active(false)
#endif
  {
  }

  Game & get_Game() {
    return Singleton<Game>::get();
  }

#if !defined(ANDROID) && !defined(NDEBUG)
  Console_State & Game::get_console() {
    return static_cast<Console_State &>(get_console_instance().get());
  }
#endif

#if !defined(ANDROID) && !defined(NDEBUG)
  void Game::write_to_console(const String &text) {
    get_console().write_to_log(text);
#else
  void Game::write_to_console(const String & /*text*/) {
#endif
  }

  void Game::push_state(const Gamestate &state) {
    if(!m_states.empty())
      m_states.top().on_cover();

    m_states.push(state);

#if !defined(ANDROID) && !defined(NDEBUG)
    if(m_console_active)
      get_console().set_child(m_states.top());
#endif

    m_states.top().on_push();
  }

  Gamestate Game::pop_state() {
    Gamestate gs;

    {
      if(m_states.empty())
        throw Zero_Gamestate();

      gs = m_states.top();
      m_states.pop();

#if !defined(ANDROID) && !defined(NDEBUG)
      if(m_console_active) {
        Console_State &csr = get_console();

        if(!m_states.empty())
          csr.set_child(m_states.top());
        else
          csr.clear_child();
      }
#endif
    }

    gs.on_pop();

    if(!m_states.empty())
      m_states.top().on_uncover();

    return gs;
  }

#ifdef ANDROID
  void Game::on_event(android_app &app, const AInputEvent &event) {
#else
  void Game::on_event(const SDL_Event &event) {
    switch(event.type) {
      case SDL_KEYDOWN:
        m_keys[event.key.keysym.sym] = true;
        break;

      case SDL_KEYUP:
        m_keys[event.key.keysym.sym] = false;
        break;

      case SDL_MOUSEBUTTONDOWN:
        m_mouse_buttons[event.button.button] = true;
        break;

      case SDL_MOUSEBUTTONUP:
        m_mouse_buttons[event.button.button] = false;
        break;

      case SDL_JOYBUTTONDOWN:
        m_joy_buttons[event.jbutton.which][event.jbutton.button] = true;
        break;

      case SDL_JOYBUTTONUP:
        m_joy_buttons[event.jbutton.which][event.jbutton.button] = false;
        break;

      default:
        break;
    }
#endif

    Gamestate gs;
#if !defined(ANDROID) && !defined(NDEBUG)
    Gamestate console_child;
#endif

    {
      if(m_states.empty())
        throw Zero_Gamestate();

#if !defined(ANDROID) && !defined(NDEBUG)
      if(m_console_active) {
        gs = get_console_instance();
        console_child = get_console().get_child();
      }
      else
#endif
      {
        gs = m_states.top();
      }
    }

#ifdef ANDROID
    gs.on_event(app, event);
#else
    gs.on_event(event);
#endif
  }

  void Game::perform_logic() {
    Gamestate gs;
#if !defined(ANDROID) && !defined(NDEBUG)
    Gamestate console_child;
#endif

    {
      if(m_states.empty())
        throw Zero_Gamestate();

#if !defined(ANDROID) && !defined(NDEBUG)
      if(m_console_active) {
        gs = get_console_instance();
        console_child = get_console().get_child();
      }
      else
#endif
      {
        gs = m_states.top();
      }
    }

    gs.perform_logic();
  }

  void Game::prerender() {
    Gamestate gs;
#if !defined(ANDROID) && !defined(NDEBUG)
    Gamestate console_child;
#endif

    {
      if(m_states.empty())
        throw Zero_Gamestate();

#if !defined(ANDROID) && !defined(NDEBUG)
      if(m_console_active) {
        gs = get_console_instance();
        console_child = get_console().get_child();
      }
      else
#endif
      {
        gs = m_states.top();
      }
    }

    gs.prerender();
  }

  void Game::render() {
    Gamestate gs;
#if !defined(ANDROID) && !defined(NDEBUG)
    Gamestate console_child;
#endif

    {
      if(m_states.empty())
        throw Zero_Gamestate();

#if !defined(ANDROID) && !defined(NDEBUG)
      if(m_console_active) {
        gs = get_console_instance();
        console_child = get_console().get_child();
      }
      else
#endif
      {
        gs = m_states.top();
      }
    }

    gs.render();

    calculate_fps();
  }

  bool Game::get_key_state(const int &button) const {
    const Unordered_Map<int, bool>::const_iterator it = m_keys.find(button);
    if(it != m_keys.end())
      return it->second;
    return false;
  }

  bool Game::get_mouse_button_state(const int &button) const {
    const Unordered_Map<int, bool>::const_iterator it = m_mouse_buttons.find(button);
    if(it != m_mouse_buttons.end())
      return it->second;
    return false;
  }

  bool Game::get_joy_button_state(const int &which, const int &button) const {
    const Unordered_Map<int, Unordered_Map<int, bool> >::const_iterator jt = m_joy_buttons.find(which);

    if(jt != m_joy_buttons.end()) {
      const Unordered_Map<int, bool>::const_iterator it = jt->second.find(button);
      if(it != jt->second.end())
        return it->second;
    }

    return false;
  }

  void Game::run() {
#ifdef TEST_NASTY_CONDITIONS
    Random random;
    const float time_scale = NASTY_MIN_RATE + (NASTY_MAX_RATE - NASTY_MIN_RATE) * random.frand_lte();
    Time::Second_Type time_used = Time::Second_Type();
    Time start_time;
#endif

    Sound_Source_Pool &sspr = get_Sound_Source_Pool();
    Time time_processed;

    for(;;) {
      const Time time_passed;
      float time_step = time_passed.get_seconds_since(time_processed);
      time_processed = time_passed;

#ifdef ENABLE_XINPUT
      get_Joysticks().poll();
#endif

#ifndef ANDROID
      if(joy_mouse.enabled && (joy_mouse.velocity.x != 0 || joy_mouse.velocity.y != 0)) {
        Point2f adjusted_vel(joy_mouse.velocity.x + 0.5f, joy_mouse.velocity.y + 0.5f);
        if(adjusted_vel.x < 0.0f)
          adjusted_vel.x = std::min(0.0f, adjusted_vel.x + joy_mouse.noise_zone.x);
        else
          adjusted_vel.x = std::max(0.0f, adjusted_vel.x - joy_mouse.noise_zone.x);
        if(adjusted_vel.y < 0.0f)
          adjusted_vel.y = std::min(0.0f, adjusted_vel.y + joy_mouse.noise_zone.y);
        else
          adjusted_vel.y = std::max(0.0f, adjusted_vel.y - joy_mouse.noise_zone.y);
        adjusted_vel.x /= 32767.5f - joy_mouse.noise_zone.x;
        adjusted_vel.y /= 32767.5f - joy_mouse.noise_zone.y;

        int xrel = int(adjusted_vel.x * joy_mouse.pixels_per_second.x * time_step);
        int yrel = int(adjusted_vel.y * joy_mouse.pixels_per_second.y * time_step);

        if(xrel || yrel) {
          int x, y;
          SDL_GetMouseState(&x, &y);

          x += xrel;
          y += yrel;
          if(x < 0)
            x = 0;
          else if(x >= get_Window().get_width())
            x = get_Window().get_width() - 1;
          if(y < 0)
            y = 0;
          else if(y >= get_Window().get_width())
            y = get_Window().get_height() - 1;

          SDL_WarpMouse(Uint16(x), Uint16(y));
        }
      }

      for(SDL_Event event; SDL_PollEvent(&event);) {
        if(event.type == SDL_KEYDOWN ||
           event.type == SDL_KEYUP)
        {
          const SDL_keysym &s = event.key.keysym;
          const bool alt = get_key_state(SDLK_LALT) || get_key_state(SDLK_RALT);
          const bool ctrl = get_key_state(SDLK_LCTRL) || get_key_state(SDLK_RCTRL);
          const bool shift = get_key_state(SDLK_LSHIFT) || get_key_state(SDLK_RSHIFT);
          const bool gui = 
#if SDL_VERSION_ATLEAST(1,3,0)
                           get_key_state(SDLK_LGUI) || get_key_state(SDLK_RGUI);
#else
                           get_key_state(SDLK_LMETA) || get_key_state(SDLK_RMETA) || get_key_state(SDLK_LSUPER) || get_key_state(SDLK_RSUPER);
#endif

#ifndef NDEBUG
          if(s.sym == SDLK_BACKQUOTE && alt && !ctrl && !gui && !shift) {
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

          if(event.type == SDL_KEYDOWN && (
#if defined(_MACOSX)
              (!alt && !ctrl &&  gui && !shift && s.sym == SDLK_q) ||
#endif
              (!alt &&  ctrl && !gui && !shift && s.sym == SDLK_q) ||
              ( alt && !ctrl && !gui && !shift && s.sym == SDLK_F4)))
          {
            throw Quit_Event();
          }
        }
#if SDL_VERSION_ATLEAST(1,3,0)
        else if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
          on_event(event);

          if(event.window.windowID == SDL_GetWindowID(get_Window().get_window())) {
            get_Window().alert_window_destroyed();
            throw Quit_Event();
          }
        }
#endif
        else if(event.type == SDL_JOYAXISMOTION) {
          if(joy_mouse.enabled && (joy_mouse.joy_axes.x == event.jaxis.axis || joy_mouse.joy_axes.y == event.jaxis.axis)) {
            if(joy_mouse.joy_axes.x == event.jaxis.axis)
              joy_mouse.velocity.x = event.jaxis.value;
            else
              joy_mouse.velocity.y = event.jaxis.value;
          }
          else
            on_event(event);
        }
        else if(event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP) {
#ifdef _MACOSX
          if(event.jbutton.which < 4 && event.jbutton.button < 4) {
            static bool values[4][4] = {{false, false, false, false},
                                        {false, false, false, false},
                                        {false, false, false, false},
                                        {false, false, false, false}};
            values[event.jbutton.which][event.jbutton.button] = event.jbutton.state == SDL_PRESSED;

            SDL_Event e;

            e.type = Uint8(SDL_JOYHATMOTION);
            e.jhat.which = event.jbutton.which;
            e.jhat.hat = 0;

            if(values[event.jbutton.which][0]) {
              if(values[event.jbutton.which][2])
                e.jhat.value = SDL_HAT_LEFTUP;
              else if(values[event.jbutton.which][3])
                e.jhat.value = SDL_HAT_RIGHTUP;
              else
                e.jhat.value = SDL_HAT_UP;
            }
            else if(values[event.jbutton.which][1]) {
              if(values[event.jbutton.which][2])
                e.jhat.value = SDL_HAT_LEFTDOWN;
              else if(values[event.jbutton.which][3])
                e.jhat.value = SDL_HAT_RIGHTDOWN;
              else
                e.jhat.value = SDL_HAT_DOWN;
            }
            else {
              if(values[event.jbutton.which][2])
                e.jhat.value = SDL_HAT_LEFT;
              else if(values[event.jbutton.which][3])
                e.jhat.value = SDL_HAT_RIGHT;
              else
                e.jhat.value = SDL_HAT_CENTERED;
            }

            SDL_PushEvent(&e);
          }
          else
#endif

          if(joy_mouse.enabled && joy_mouse.left_click == event.jbutton.button) {
            SDL_Event e;

            e.type = Uint8(event.type == SDL_JOYBUTTONDOWN ? SDL_MOUSEBUTTONDOWN : SDL_MOUSEBUTTONUP);
            e.button.which = Uint8(event.jbutton.which + 1);
            e.button.state = event.jbutton.state;
            e.button.button = SDL_BUTTON_LEFT;

            int x, y;
            SDL_GetMouseState(&x, &y);
            e.button.x = Uint16(x);
            e.button.y = Uint16(y);

            on_event(e);
          }
          else if(joy_mouse.enabled && joy_mouse.escape == event.jbutton.button) {
            SDL_Event e;

            e.type = Uint8(event.type == SDL_JOYBUTTONDOWN ? SDL_KEYDOWN : SDL_KEYUP);
            e.key.which = Uint8(event.jbutton.which + 1);
            e.key.state = event.jbutton.state;
            e.key.keysym.mod = SDL_GetModState();
            e.key.keysym.scancode = 0;
            e.key.keysym.sym = SDLK_ESCAPE;
            e.key.keysym.unicode = 0;

            on_event(e);
          }
          else
            on_event(event);
        }
        else if(event.type == SDL_JOYHATMOTION) {
          if(joy_mouse.enabled && joy_mouse.scroll_hat == event.jhat.hat && (event.jhat.value == SDL_HAT_DOWN || event.jhat.value == SDL_HAT_UP)) {
            SDL_Event e;

            e.type = SDL_MOUSEBUTTONDOWN;
            e.button.which = Uint8(event.jhat.which + 1);
            e.button.state = SDL_PRESSED;
            e.button.button = Uint8(event.jhat.value == SDL_HAT_DOWN ? SDL_BUTTON_WHEELDOWN : SDL_BUTTON_WHEELUP);

            int x, y;
            SDL_GetMouseState(&x, &y);
            e.button.x = Uint16(x);
            e.button.y = Uint16(y);

            on_event(e);

            e.type = SDL_MOUSEBUTTONUP;
            e.button.state = SDL_RELEASED;

            on_event(e);
          }
          else
            on_event(event);
        }
        else {
          on_event(event);

          if(event.type == SDL_QUIT)
            throw Quit_Event();
        }
      }
#endif

#ifdef TEST_NASTY_CONDITIONS
      {
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
      }
#else
      perform_logic();
#endif

      sspr.update();

      if(Window::is_enabled()) {
        Video &vr = get_Video();

#ifndef DISABLE_DX9
        try
#endif
        {
          if(vr.begin_prerender()) {
            prerender();

            if(vr.begin_render()) {
              try {
                render();
              }
              catch(...) {
                vr.end_render();
                throw;
              }

              vr.end_render();
            }
          }
        }
#ifndef DISABLE_DX9
        catch(Video_Device_Failure &) {
          Video::destroy();
        }
#endif
      }
    }
  }

  void Game::calculate_fps() {
    ++fps_next;
    const Time::Tick_Type tp = time.get_ticks_passed();
    if(tp < ticks_passed)
      return;
    ticks_passed = tp + 1000;
    fps = fps_next;
    fps_next = 0;
  }

#if !defined(ANDROID) && !defined(NDEBUG)
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

  bool Game::is_console_active() const {
#if !defined(ANDROID) && !defined(NDEBUG)
    return m_console_active;
#else
    return false;
#endif
  }
}

#include <Zeni/Undefine.h>
