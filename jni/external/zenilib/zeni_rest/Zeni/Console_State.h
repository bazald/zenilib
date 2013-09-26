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
* \class Zeni::Console_State
*
* \ingroup zenilib
*
* \brief Provide a text console to process commands
*
* \author bazald
*
* Contact: bazald@zenipex.com
*/

#if !defined(ANDROID) && !defined(CONSOLE_STATE_H)
#define CONSOLE_STATE_H

#include <Zeni/Gamestate.h>
#include <Zeni/Widget.h>

namespace Zeni {

  class ZENI_REST_DLL Console_State;

  struct ZENI_REST_DLL Console_Function {
    virtual ~Console_Function() {}

    virtual void operator()(Console_State &console,
                                             const String &name,
                                             const std::vector<String> &args);
  };

  class ZENI_REST_DLL Console_State : public Gamestate_Base {
    Console_State(const Console_State &);
    Console_State & operator=(const Console_State &);

    friend class ZENI_REST_DLL Game;

  private:
    Console_State();
    ~Console_State();

    Gamestate & get_child();
    void set_child(Gamestate child);
    void clear_child();

  public:
    void write_to_log(const String &text);
    void clear_log();

    void give_function(const String &name, Console_Function * const &function);

  private:
    void fire_command(const String &text);

    void on_event(const SDL_Event &event);

    void on_key(const SDL_KeyboardEvent &event);
    void on_mouse_button(const SDL_MouseButtonEvent &event);
    void on_mouse_motion(const SDL_MouseMotionEvent &event);

    void perform_logic();
    void prerender();
    void render();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::pair<Point2f, Point2f> m_virtual_screen;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
    Projector2D m_projector;

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    Quadrilateral<Vertex2f_Color> m_bg;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
    Text_Box m_log;
    Text_Box m_prompt;

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::map<String, Console_Function *> m_functions;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
    String m_text;
    bool m_log_dirty;

    Gamestate * m_child;
    bool m_handled_event;
  };

}

#endif
