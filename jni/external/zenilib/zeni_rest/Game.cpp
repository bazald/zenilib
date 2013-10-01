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
    : time(get_Timer().get_time()),
    ticks_passed(0),
    fps(END_OF_TIME),
    fps_next(0),
    m_popup_menu_state_factory(new Popup_Menu_State_Factory),
    m_popup_pause_state_factory(new Popup_Pause_State_Factory)
#if !defined(ANDROID) && !defined(NDEBUG)
    , m_console_active(false)
#endif
  {
  }

  Game::~Game() {
    delete m_popup_menu_state_factory;
    delete m_popup_pause_state_factory;
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
    SDL_Event event2;
    memcpy(&event2, &event, sizeof(SDL_Event));

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

      case SDL_CONTROLLERAXISMOTION:
        {
          event2.caxis.which = get_Controllers().get_controller_index(event.caxis.which);
          m_controller_axes[event2.caxis.which][event2.caxis.axis] = event.caxis.value;
        }
        break;
        
      case SDL_CONTROLLERBUTTONDOWN:
        {
          event2.cbutton.which = get_Controllers().get_controller_index(event.cbutton.which);
          m_controller_buttons[event2.cbutton.which][event2.cbutton.button] = true;
        }
        break;
        
      case SDL_CONTROLLERBUTTONUP:
        {
          event2.cbutton.which = get_Controllers().get_controller_index(event.cbutton.which);
          m_controller_buttons[event2.cbutton.which][event2.cbutton.button] = false;
        }
        break;

      case SDL_CONTROLLERDEVICEADDED:
        get_Controllers().device_added(event.cdevice.which);
        break;

      //case SDL_CONTROLLERDEVICEREMAPPED:
      //  get_Controllers().device_remapped(event.cdevice.which);
      //  break;

      case SDL_CONTROLLERDEVICEREMOVED:
        //get_Controllers().device_removed(event.cdevice.which);
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
    gs.on_event(app, event2);
#else
    gs.on_event(event2);
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
  
  Sint16 Game::get_controller_axis_state(const int &which, const SDL_GameControllerAxis &axis) const {
    const Unordered_Map<int, Unordered_Map<int, Sint16> >::const_iterator jt = m_controller_axes.find(which);

    if(jt != m_controller_axes.end()) {
      const Unordered_Map<int, Sint16>::const_iterator it = jt->second.find(axis);
      if(it != jt->second.end())
        return it->second;
    }

    return 0;
  }

  bool Game::get_controller_button_state(const int &which, const SDL_GameControllerButton &button) const {
    const Unordered_Map<int, Unordered_Map<int, bool> >::const_iterator jt = m_controller_buttons.find(which);

    if(jt != m_controller_buttons.end()) {
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

    Time time_processed;

    for(;;) {
      const Time time_passed;
      float time_step = time_passed.get_seconds_since(time_processed);
      time_processed = time_passed;

#ifndef ANDROID
      if(controller_mouse.enabled && (controller_mouse.velocity.x != 0 || controller_mouse.velocity.y != 0)) {
        Point2f adjusted_vel(controller_mouse.velocity.x + 0.5f, controller_mouse.velocity.y + 0.5f);
        if(adjusted_vel.x < 0.0f)
          adjusted_vel.x = std::min(0.0f, adjusted_vel.x + controller_mouse.noise_zone.x);
        else
          adjusted_vel.x = std::max(0.0f, adjusted_vel.x - controller_mouse.noise_zone.x);
        if(adjusted_vel.y < 0.0f)
          adjusted_vel.y = std::min(0.0f, adjusted_vel.y + controller_mouse.noise_zone.y);
        else
          adjusted_vel.y = std::max(0.0f, adjusted_vel.y - controller_mouse.noise_zone.y);
        adjusted_vel.x /= 32767.5f - controller_mouse.noise_zone.x;
        adjusted_vel.y /= 32767.5f - controller_mouse.noise_zone.y;

        int xrel = int(adjusted_vel.x * controller_mouse.pixels_per_second.x * time_step);
        int yrel = int(adjusted_vel.y * controller_mouse.pixels_per_second.y * time_step);

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

#if SDL_VERSION_ATLEAST(2,0,0)
          SDL_WarpMouseInWindow(get_Window().get_window(), x, y);
#else
          SDL_WarpMouse(Uint16(x), Uint16(y));
#endif
        }
      }

      get_Controllers().detect_removed();

      for(SDL_Event event; SDL_PollEvent(&event);) {
        if(event.type == SDL_KEYDOWN ||
           event.type == SDL_KEYUP)
        {
          const SDL_Keysym &s = event.key.keysym;
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
        else if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
          on_event(event);

          if(event.window.windowID == SDL_GetWindowID(get_Window().get_window())) {
            get_Window().alert_window_destroyed();
            throw Quit_Event();
          }
        }
        else if(event.type == SDL_CONTROLLERAXISMOTION) {
          if(controller_mouse.enabled && get_Controllers().get_controller_index(event.caxis.which) == 0 && (controller_mouse.controller_axes.x == event.caxis.axis || controller_mouse.controller_axes.y == event.caxis.axis)) {
            if(controller_mouse.controller_axes.x == event.caxis.axis)
              controller_mouse.velocity.x = event.caxis.value;
            else
              controller_mouse.velocity.y = event.caxis.value;
          }
          else
            on_event(event);
        }
        else if(event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP) {
          if(controller_mouse.enabled && get_Controllers().get_controller_index(event.cbutton.which) == 0 && controller_mouse.left_click == event.cbutton.button) {
            SDL_Event e;

            e.common.type = event.common.type == SDL_CONTROLLERBUTTONDOWN ? SDL_MOUSEBUTTONDOWN : SDL_MOUSEBUTTONUP;
            e.common.timestamp = event.common.timestamp;
            e.button.which = event.cbutton.which + 1;
            e.button.state = event.cbutton.state;
            e.button.button = SDL_BUTTON_LEFT;

            Sint32 x, y;
            SDL_GetMouseState(&x, &y);
            e.button.x = x;
            e.button.y = y;

            on_event(e);
          }
          else if(controller_mouse.enabled && get_Controllers().get_controller_index(event.cbutton.which) == 0 && controller_mouse.escape == event.jbutton.button) {
            SDL_Event e;

            e.common.type = event.common.type == SDL_CONTROLLERBUTTONDOWN ? SDL_KEYDOWN : SDL_KEYUP;
            e.common.timestamp = event.common.timestamp;
            e.key.keysym.mod = Uint16(SDL_GetModState());
            e.key.keysym.scancode = SDL_SCANCODE_ESCAPE;
            e.key.state = event.cbutton.state;
            e.key.keysym.sym = SDLK_ESCAPE;
            //e.key.keysym.unicode = 0;

            on_event(e);
          }
          else if(controller_mouse.enabled && event.cbutton.which == 0 && (controller_mouse.scroll_down == event.jbutton.button || controller_mouse.scroll_up == event.jbutton.button)) {
            SDL_Event e;

            e.common.type = SDL_MOUSEWHEEL;
            e.common.timestamp = event.common.timestamp;
            e.wheel.which = event.cbutton.which + 1;
            e.wheel.windowID = 0;
            e.wheel.x = 0;
            e.wheel.y = controller_mouse.scroll_down == event.cbutton.button ? -1 : 1;

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

      get_Sound().update();
      get_Sound_Source_Pool().update();

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

  void Game::push_Popup_Menu_State() {
    push_state((*m_popup_menu_state_factory)());
  }

  void Game::push_Popup_Pause_State() {
    push_state((*m_popup_pause_state_factory)());
  }

  void Game::replace_Popup_Menu_State_Factory(Popup_Menu_State_Factory * const popup_menu_state_factory) {
    assert(popup_menu_state_factory);
    delete m_popup_menu_state_factory;
    m_popup_menu_state_factory = popup_menu_state_factory;
  }

  void Game::replace_Popup_Pause_State_Factory(Popup_Pause_State_Factory * const popup_pause_state_factory) {
    assert(popup_pause_state_factory);
    delete m_popup_pause_state_factory;
    m_popup_pause_state_factory = popup_pause_state_factory;
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
