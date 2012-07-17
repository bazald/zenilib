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
 * \class Zeni::Gamestate_Base
 *
 * \ingroup zenilib
 *
 * \brief The base class for all gamestates.
 *
 * Gamestate_Base provides virtual functions to be overridden by
 * derived classes.  It is expected that all input handling will 
 * be done in the provided callback functions.  It is required that 
 * all rendering be done in the render function.
 *
 * \note You can ignore any callback functions you aren't using.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Gamestate
 *
 * \ingroup zenilib
 *
 * \brief A smartpointer for a Gamestate_Base
 *
 * A Gamestate also allows one to call the same functions provided 
 * by Gamestate_Base indirectly.  Any functions provided by derivations 
 * of Gamestate_Base will, of course, require you to get the internal 
 * pointer and dynamic cast it to the correct type.  (Or static cast if 
 * you're absolutely sure...)
 *
 * \note A Gamestate is returned by Game::pop_state().  You can later push the state back onto the stack if you so desire.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_GAMESTATE_H
#define ZENI_GAMESTATE_H

#include <Zeni/Core.h>

#include <SDL/SDL.h>

#ifndef ANDROID
#include <SDL/SDL_keysym.h>
#endif

#include <map>

struct android_app;
struct AInputEvent;

namespace Zeni {

  class ZENI_REST_DLL Gamestate;

  // Derive from this class
  class ZENI_REST_DLL Gamestate_Base {
    friend class Gamestate;

    Gamestate_Base(const Gamestate_Base &rhs);
    Gamestate_Base & operator=(const Gamestate_Base &rhs);

  public:
    Gamestate_Base() : m_count(0), m_pausable(false) {}
    virtual ~Gamestate_Base() {}

    // The control loop

    /// First check for events. Called by Game as part of the main gameloop.
#ifdef ANDROID
    virtual void on_event(android_app &app, const AInputEvent &event);
#else
    virtual void on_event(const SDL_Event &event);
#endif
    /// Then perform logic.  Called by Game as part of the main gameloop.
    virtual void perform_logic() {}
    /// No actual rendering, but any logic strictly necessary for rendering.
    virtual void prerender() {}
    /// Then render.  Called by Game as part of the main gameloop.
    virtual void render();

    /// Called when the Gamestate is pushed onto the stack in Game
    virtual void on_push();
    /// Called when a Gamestate is pushed on top of this Gamestate in Game
    virtual void on_cover();
    /// Called when a Gamestate is popped off Game, making this Gamestate on top
    virtual void on_uncover();
    /// Called when the Gamestate is popped off the stack in Game
    virtual void on_pop();

    inline const bool & is_pausable() const;
    inline void set_pausable(const bool &pausable_);

    // Converters

#ifndef ANDROID
    static char to_char(const SDL_keysym &ks); ///< Returns a character key corresponding to the current combination of keys pressed or the null character (0).
    static SDLKey to_sym(const String &text_version); ///< Convert a text representation to an actual sym
    static String to_text(const SDLKey &sym); ///< Convert a sym to a text representation
#endif

  protected:
    // Event handlers

#ifndef ANDROID
    // Keyboard events
    virtual void on_key(const SDL_KeyboardEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    // Mouse events
    virtual void on_mouse_motion(const SDL_MouseMotionEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    virtual void on_mouse_button(const SDL_MouseButtonEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
#if SDL_VERSION_ATLEAST(1,3,0)
    virtual void on_mouse_wheel(const SDL_MouseWheelEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
#endif
    // "Joystick" events
    virtual void on_joy_axis(const SDL_JoyAxisEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    virtual void on_joy_ball(const SDL_JoyBallEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    virtual void on_joy_hat(const SDL_JoyHatEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    virtual void on_joy_button(const SDL_JoyButtonEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    // Quit events
    virtual void on_quit(const SDL_QuitEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    // Annoying events
#if SDL_VERSION_ATLEAST(1,3,0)
    virtual void on_window_event(const SDL_WindowEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
#endif
    virtual void on_system_wm_event(const SDL_SysWMEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    virtual void on_active(const SDL_ActiveEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    virtual void on_video_resize(const SDL_ResizeEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
#if !SDL_VERSION_ATLEAST(1,3,0)
    virtual void on_video_expose(const SDL_ExposeEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
#endif
    // Custom events
    virtual void on_user_event(const SDL_UserEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
#endif

  private:
    inline void increment();
    inline void decrement();
    int m_count;

    bool m_pausable;
  };

  class ZENI_REST_DLL Gamestate {
  public:
    inline Gamestate();
    inline Gamestate(Gamestate_Base * const &state);
    ~Gamestate();

    inline Gamestate(const Gamestate &state);
    inline Gamestate & operator=(const Gamestate &state);

#ifdef ANDROID
    inline void on_event(android_app &app, const AInputEvent &event);
#else
    inline void on_event(const SDL_Event &event);
#endif
    inline void perform_logic();
    inline void prerender();
    inline void render();

    inline void on_push();
    inline void on_cover();
    inline void on_uncover();
    inline void on_pop();

    inline const bool & is_pausable() const;

    inline Gamestate_Base & get();

  private:
    Gamestate_Base *m_state;
  };

}

#endif
