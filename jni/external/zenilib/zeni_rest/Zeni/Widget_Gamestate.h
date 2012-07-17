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

  class ZENI_REST_DLL Widget_Gamestate : public Gamestate_Base {
    Widget_Gamestate(const Widget_Gamestate &);
    Widget_Gamestate & operator=(const Widget_Gamestate &);

  public:
    Widget_Gamestate(const std::pair<Point2f, Point2f> &virtual_window_, const bool &fix_aspect_ratio_ = true);

    const std::pair<Point2f, Point2f> & get_virtual_window() const;
    const bool & fix_aspect_ratio() const;

    void on_push();
    void on_pop();

#ifndef ANDROID
    void on_key(const SDL_KeyboardEvent &event);
    void on_mouse_button(const SDL_MouseButtonEvent &event);
    void on_mouse_motion(const SDL_MouseMotionEvent &event);
#endif
    void perform_logic();
    void render();

  protected:
    Widgets m_widgets;

  private:
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::pair<Point2i, Point2i> m_viewport;
    std::pair<Point2f, Point2f> m_virtual_window;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
    bool m_fix_aspect_ratio;

    Zeni::Projector2D m_projector;

    bool m_hide_cursor;
    bool m_grab_input;
    Color m_clear_color;
  };

}

#endif
