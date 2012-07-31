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

namespace Zeni {

  Widget_Gamestate::Widget_Gamestate(const std::pair<Point2f, Point2f> &virtual_window_, const bool &fix_aspect_ratio_)
    : m_virtual_window(virtual_window_),
    m_fix_aspect_ratio(fix_aspect_ratio_)
  {
  }

  const std::pair<Point2f, Point2f> & Widget_Gamestate::get_virtual_window() const {
    return m_virtual_window;
  }

  const bool & Widget_Gamestate::fix_aspect_ratio() const {
    return m_fix_aspect_ratio;
  }

  void Widget_Gamestate::on_push() {
#ifndef ANDROID
    Window &wr = get_Window();

    m_hide_cursor = wr.is_mouse_hidden();
    if(m_hide_cursor)
      wr.mouse_hide(false);

    m_grab_input = wr.is_mouse_grabbed();
    if(m_grab_input)
      wr.mouse_grab(false);
#endif

    m_clear_color = get_Video().get_clear_Color();
  }

  void Widget_Gamestate::on_pop() {
    get_Video().set_clear_Color(m_clear_color);

#ifndef ANDROID
    Window &wr = get_Window();

    if(m_hide_cursor)
      wr.mouse_hide(true);

    if(m_grab_input)
      wr.mouse_grab(true);
#endif
  }

#ifndef ANDROID
  void Widget_Gamestate::on_key(const SDL_KeyboardEvent &event) {
    m_widgets.on_event(event);
    if(!m_widgets.is_busy())
      Gamestate_Base::on_key(event);
  }

  void Widget_Gamestate::on_mouse_button(const SDL_MouseButtonEvent &event) {
    m_widgets.on_event(event, m_projector);
  }

  void Widget_Gamestate::on_mouse_motion(const SDL_MouseMotionEvent &event) {
    m_widgets.on_event(event, m_projector);
  }
#endif

  void Widget_Gamestate::perform_logic() {
    m_viewport = get_Video().calculate_viewport(m_virtual_window, std::make_pair(Point2i(), get_Window().get_size()), m_fix_aspect_ratio);
    m_projector = Projector2D(m_virtual_window, m_viewport);

    m_widgets.perform_logic();
  }

  void Widget_Gamestate::render() {
    Video &vr = get_Video();

    vr.set_2d(m_virtual_window, m_fix_aspect_ratio);

    m_widgets.render();
  }

}
