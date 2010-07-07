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
* \class Zeni::Widget_Gamestate
*
* \ingroup zenilib
*
* \brief Gamestate with some helpers for dealing with Widgets
*
* \author bazald
*
* Contact: bazald@zenipex.com
*/

#ifndef WIDGET_GAMESTATE_H
#define WIDGET_GAMESTATE_H

#include <Zeni/Gamestate.h>
#include <Zeni/Widget.h>

namespace Zeni {

  class Widget_Gamestate : public Gamestate_Base {
    Widget_Gamestate(const Widget_Gamestate &);
    Widget_Gamestate & operator=(const Widget_Gamestate &);

  public:
    Widget_Gamestate(const std::pair<Point2f, Point2f> &virtual_window_, const bool &fix_aspect_ratio_ = true);

    const std::pair<Point2f, Point2f> & get_virtual_window() const;
    const bool & fix_aspect_ratio() const;

    void on_push();
    void on_pop();

    void on_key(const SDL_KeyboardEvent &event);
    void on_mouse_button(const SDL_MouseButtonEvent &event);
    void on_mouse_motion(const SDL_MouseMotionEvent &event);
    void perform_logic();
    void render();

  protected:
    Widgets m_widgets;

  private:
    std::pair<Point2f, Point2f> m_virtual_window;
    bool m_fix_aspect_ratio;

    Zeni::Projector2D m_projector;

    bool m_hide_cursor;
    bool m_grab_input;
    Color m_clear_color;
  };

}

#endif
