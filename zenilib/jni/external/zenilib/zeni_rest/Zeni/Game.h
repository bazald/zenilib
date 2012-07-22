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
 * \class Zeni::Game
 *
 * \ingroup zenilib
 *
 * \brief The Gamestate Stack
 *
 * This singleton is a stack of Gamestates.  The main function
 * continually refers to the Game singleton to get the current 
 * Gamestate.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_GAME_H
#define ZENI_GAME_H

#include <Zeni/Gamestate.h>
#include <Zeni/Hash_Map.h>
#include <Zeni/Singleton.h>
#include <Zeni/String.h>
#include <Zeni/Timer.h>

/* \cond */
#include <stack>
#include <vector>
/* \endcond */

namespace Zeni {

  class Console_State;
  class Game;

  class ZENI_REST_DLL Gamestate_Zero_Initializer {
  public:
	virtual ~Gamestate_Zero_Initializer() {}

    virtual Gamestate_Base * operator()() = 0;
  };

  ZENI_REST_DLL extern Gamestate_Zero_Initializer * g_gzi;
  ZENI_REST_DLL extern int g_argc;
  ZENI_REST_DLL extern const char * const * g_argv;
#ifdef _WINDOWS
  ZENI_REST_EXT template class ZENI_REST_DLL Singleton<Game>;
#endif

  class ZENI_REST_DLL Game : public Singleton<Game> {
    friend class Singleton<Game>;

    static Game * create();

    Game();

    // Undefined
    Game(const Game &);
    Game & operator=(const Game &);

  public:
    inline Gamestate get_top(); ///< Get a reference to the current Gamestate.

#if !defined(ANDROID) && !defined(NDEBUG)
    Console_State & get_console(); ///< Get a reference to the Console_State
#endif
    void write_to_console(const String &text); ///< Write text to the console when in Debug mode

    inline size_t size() const; ///< Get the current size of the Gamestate stack.
    void push_state(const Gamestate &state); ///< Push a new Gamestate onto the stack.
    Gamestate pop_state(); ///< Pop a Gamestate off the stack. Pop the current Gamestate with caution. All members will instantly become invalid.

#ifdef ANDROID
    void on_event(android_app &app, const AInputEvent &event); ///< Called in main, calls the function by the same name in the current Gamestate.
#else
    void on_event(const SDL_Event &event); ///< Called in main, calls the function by the same name in the current Gamestate.
#endif
    void perform_logic(); ///< Called in main, calls the function by the same name in the current Gamestate.
    void prerender(); ///< Called in main, calls the function by the same name in the current Gamestate.
    void render(); ///< Called in main, calls the function by the same name in the current Gamestate.

    inline size_t get_fps() const; ///< Get the current approximation of the frames displayed per second.
    bool get_key_state(const int &key) const; ///< Get the state of a key.
    bool get_mouse_button_state(const int &button) const; ///< Get the state of a mouse button.
    bool get_joy_button_state(const int &which, const int &button) const; ///< Get the state of a joystick button.

    void run();

#ifndef ANDROID
    struct ZENI_REST_DLL Joy_Mouse {
      Joy_Mouse()
        : enabled(false),
        joy_axes(Joysticks::AXIS_LEFT_THUMB_X, Joysticks::AXIS_LEFT_THUMB_Y),
        noise_zone(8192, 4096),
        pixels_per_second(1000.0f, 1000.0f),
        left_click(Joysticks::BUTTON_A),
        escape(Joysticks::BUTTON_START),
        scroll_hat(0)
      {
      }

      bool enabled;

      Point2i joy_axes;
      Point2i noise_zone;
      Point2i velocity;
      Point2f pixels_per_second;

      int left_click;
      int escape;

      int scroll_hat;
    } joy_mouse;
#endif

  private:
    void calculate_fps();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::stack<Gamestate> m_states;
    Unordered_Map<int, bool> m_keys;
    Unordered_Map<int, bool> m_mouse_buttons;
    Unordered_Map<int, Unordered_Map<int, bool> > m_joy_buttons;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

    Time time;
    Time::Tick_Type ticks_passed, fps, fps_next;

#if !defined(ANDROID) && !defined(NDEBUG)
    void activate_console();
    void deactivate_console();
    Gamestate & get_console_instance();

    bool m_console_active;
#endif

  public:
    bool is_console_active() const;
  };

  ZENI_REST_DLL Game & get_Game(); ///< Get access to the singleton.

  struct ZENI_REST_DLL Zero_Gamestate : public Error {
    Zero_Gamestate() : Error("Zeni Gamestate State is Empty") {}
  };

}

#endif
