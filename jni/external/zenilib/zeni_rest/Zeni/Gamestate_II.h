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
 * \class Zeni::Gamestate_II
 *
 * \ingroup zenilib
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

#ifndef ZENI_GAMESTATE_II_H
#define ZENI_GAMESTATE_II_H

#include <Zeni/Gamestate.h>

namespace Zeni {

  struct ZENI_REST_DLL Zeni_Input_ID {
#ifndef ANDROID
    Zeni_Input_ID(const Uint16 &type_ = SDL_KEYDOWN, const int &subid_ = 0, const int &which_ = 0);
#endif

    Uint16 type; ///< directly copied from SDL_Event; UP types are converted to DOWN types
    int subid; ///< event.keysym.sym, event.button, event.axis, mouse axis (x==0, y==1)
    int which; ///< Joystick Identifier; ignored for other events (should be 0)
    mutable float previous_confidence;

    bool operator<(const Zeni_Input_ID &rhs) const;
  };

  class ZENI_REST_DLL Gamestate_II : public Gamestate_Base {
    Gamestate_II(const Gamestate_II &rhs);
    Gamestate_II & operator=(const Gamestate_II &rhs);

  public:
    Gamestate_II();

    // The control loop

    /// First check for events. Called by Game as part of the main gameloop.
#ifndef ANDROID
    virtual void on_event(const SDL_Event &event);
#endif
    virtual void on_event(const Zeni_Input_ID &id, const float &confidence, const int &action);
    virtual void on_push();
    virtual void on_uncover();

    inline const int & get_joyball_min() const;
    inline const int & get_joyball_max() const;
    inline const float & get_joystick_min() const;
    inline const float & get_joystick_max() const;
    inline const int & get_mouse_min() const;
    inline const int & get_mouse_max() const;
    inline const bool & is_firing_missed_events() const;

    inline void set_joyball_min(const int &min); ///< Should be (0, inf) and less than max
    inline void set_joyball_max(const int &max); ///< Should be (1, inf) and greater than min
    inline void set_joystick_min(const float &min); ///< Should be [0.0, 1.0) and less than max
    inline void set_joystick_max(const float &max); ///< Should be (0.0, 1.0] and greater than min
    inline void set_mouse_min(const int &min); ///< Should be (0, inf) and less than max
    inline void set_mouse_max(const int &max); ///< Should be (1, inf) and greater than min
    inline void set_firing_missed_events(const bool &firing); ///< true if missed events should be handled normally; false if they should be discarded

    virtual int get_action(const Zeni_Input_ID &event);
    virtual Zeni_Input_ID get_event(const int &action);
    virtual void set_action(const Zeni_Input_ID &event, const int &action);

    void fire_missed_events();

  private:
    void fire_event(const Zeni_Input_ID &id, const float &confidence);

    int m_joyball_min;
    int m_joyball_max;
    float m_joystick_min;
    float m_joystick_max;
    int m_mouse_min;
    int m_mouse_max;

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::map<Zeni_Input_ID, int> m_ii;
    std::map<int, Zeni_Input_ID> m_rii;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

    bool m_firing_missed_events;
  };

}

#endif
