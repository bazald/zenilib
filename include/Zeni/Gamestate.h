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

/**
 * \class Zeni::Gamestate_Base
 *
 * \ingroup Zenilib
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
 * \ingroup Zenilib
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

/**
 * \class Zeni::Gamestate_II
 *
 * \ingroup Zenilib
 *
 * \brief A new Gamestate base with an Integrated Input technique
 *
 * All user input excluding mouse motion and joyhat events are abstracted away.
 * A keydown, mousebuttondown, or joybuttondown event will yield a specfied action identifier with confidence 1.0f.
 * The respective up event will yield the same action identifier with confidence 0.0f.
 * Similarly, joyaxis movement will yield an action identifier with confidence -1.0f to 1.0f.
 * This makes it easier to treat different types of input nearly identically, and to make it possible to
 * use different control schemes.  If no action id has been specified for a given input, it will report an
 * action id of 0.  It is then possible to tie an action to the input or to ignore it.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifdef ZENI_INLINES
#include <Zeni/Gamestate.hxx>
#endif

#ifndef ZENI_GAMESTATE_H
#define ZENI_GAMESTATE_H

#include <Zeni/Core.h>

#include <SDL/SDL.h>
#include <SDL/SDL_keysym.h>
#include <map>

namespace Zeni {

  class Gamestate;

  // Derive from this class
  class Gamestate_Base {
    friend class Gamestate;

    Gamestate_Base(const Gamestate_Base &rhs);
    Gamestate_Base & operator=(const Gamestate_Base &rhs);

  public:
    Gamestate_Base() : m_count(0) {}
    virtual ~Gamestate_Base() {}

    // The control loop

    /// First check for events. Called by Game as part of the main gameloop.
    virtual void on_event(const SDL_Event &event);
    /// Then perform logic.  Called by Game as part of the main gameloop.
    virtual void perform_logic() {}
    /// Then render.  Called by Game as part of the main gameloop.
    virtual void render() {}

    // Converters

    static char to_char(const SDL_keysym &ks); ///< Returns a character key corresponding to the current combination of keys pressed or NULL (0 or '\0').
    static SDLKey to_sym(const std::string &text_version); ///< Convert a text representation to an actual sym
    static std::string to_text(const SDLKey &sym); ///< Convert a sym to a text representation

  protected:
    // Event handlers

    // Keyboard events
    virtual void on_key(const SDL_KeyboardEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    // Mouse events
    virtual void on_mouse_motion(const SDL_MouseMotionEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    virtual void on_mouse_button(const SDL_MouseButtonEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    // "Joystick" events
    virtual void on_joy_axis(const SDL_JoyAxisEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    virtual void on_joy_ball(const SDL_JoyBallEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    virtual void on_joy_hat(const SDL_JoyHatEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    virtual void on_joy_button(const SDL_JoyButtonEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    // Quit events
    virtual void on_quit(const SDL_QuitEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    // Annoying events
    virtual void on_system_wm_event(const SDL_SysWMEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    virtual void on_active(const SDL_ActiveEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    virtual void on_video_resize(const SDL_ResizeEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    virtual void on_video_expose(const SDL_ExposeEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.
    // Custom events
    virtual void on_user_event(const SDL_UserEvent &event); ///< Override this input callback in your Gamestates. See SDL documentation for details.

  private:
    inline void increment();
    inline void decrement();
    int m_count;
  };

  class Gamestate {
  public:
    inline Gamestate();
    inline Gamestate(Gamestate_Base *state);
    ~Gamestate();

    inline Gamestate(const Gamestate &state);
    inline Gamestate & operator=(const Gamestate &state);

    inline void on_event(const SDL_Event &event);
    inline void perform_logic();
    inline void render();

    inline Gamestate_Base & get();

  private:
    Gamestate_Base *m_state;
  };

  struct Zeni_Input_ID {
    Zeni_Input_ID(const Uint8 &type_ = SDL_KEYDOWN, const int &subid_ = 0, const int &which_ = 0);

    Uint8 type; ///< directly copied from SDL_Event; UP types are converted to DOWN types
    int subid; ///< event.keysym.sym, event.button, event.axis; ignored for mouse motion (should be 0)
    int which; ///< Joystick Identifier; ignored for other events (should be 0)
    mutable float previous_confidence;

    bool operator<(const Zeni_Input_ID &rhs) const;
  };

  class Gamestate_II : public Gamestate_Base {
  public:
    Gamestate_II();

    // The control loop

    /// First check for events. Called by Game as part of the main gameloop.
    virtual void on_event(const SDL_Event &event);
    virtual void on_event(const Zeni_Input_ID &id, const float &confidence, const int &action);

    inline const float & get_min_confidence() const;
    inline const float & get_max_confidence() const;

    inline void set_min_confidence(const float &min);
    inline void set_max_confidence(const float &max);

    virtual int get_action(const Zeni_Input_ID &event);
    virtual Zeni_Input_ID get_event(const int &action);
    virtual void set_action(const Zeni_Input_ID &event, const int &action);

  private:
    float m_min_confidence;
    float m_max_confidence;

    std::map<Zeni_Input_ID, int> m_ii;
    std::map<int, Zeni_Input_ID> m_rii;
  };

  struct Quit_Event {
    Quit_Event() {fired = true;}

    static void fire() {throw Quit_Event();}
    static bool has_fired() {return fired;}

  private:
    static bool fired;
  };

}

#endif
