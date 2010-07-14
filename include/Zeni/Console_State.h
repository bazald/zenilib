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

#ifndef CONSOLE_STATE_H
#define CONSOLE_STATE_H

#include <Zeni/Gamestate.h>
#include <Zeni/Widget.h>

namespace Zeni {

  class Console_State;

  struct Console_Function {
    virtual ~Console_Function() {}

    virtual void operator()(Console_State &console,
                            const std::string &name,
                            const std::vector<std::string> &args);
  };

  class Console_State : public Gamestate_Base {
    Console_State(const Console_State &);
    Console_State & operator=(const Console_State &);

    friend class Game;

  private:
    Console_State();
    ~Console_State();

    Gamestate & get_child();
    void set_child(Gamestate child);
    void clear_child();

  public:
    void write_to_log(const std::string &text);
    void clear_log();

    void give_function(const std::string &name, Console_Function * const &function);

  private:
    void fire_command(const std::string &text);

    void on_event(const SDL_Event &event);

    void on_key(const SDL_KeyboardEvent &event);
    void on_mouse_button(const SDL_MouseButtonEvent &event);
    void on_mouse_motion(const SDL_MouseMotionEvent &event);

    void perform_logic();
    void prerender();
    void render();

    std::pair<Point2f, Point2f> m_virtual_screen;
    Projector2D m_projector;

    Quadrilateral<Vertex2f_Color> m_bg;
    Text_Box m_log;
    Text_Box m_prompt;

    std::map<std::string, Console_Function *> m_functions;
    std::string m_text;
    bool m_log_dirty;

    Gamestate * m_child;
    bool m_handled_event;
  };

}

#endif
