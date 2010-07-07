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

#include <Zeni/Widget_Gamestate.h>

#include <Zeni/Video.hxx>
#include <Zeni/Widget.hxx>

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
    Video &vr = get_Video();

    m_hide_cursor = vr.is_mouse_hidden();
    if(m_hide_cursor)
      vr.mouse_hide(false);

    m_grab_input = vr.is_mouse_grabbed();
    if(m_grab_input)
      vr.mouse_grab(false);

    m_clear_color = vr.get_clear_color();
  }

  void Widget_Gamestate::on_pop() {
    Video &vr = get_Video();

    vr.set_clear_color(m_clear_color);

    if(m_hide_cursor)
      vr.mouse_hide(true);

    if(m_grab_input)
      vr.mouse_grab(true);
  }

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

  void Widget_Gamestate::perform_logic() {
    m_projector = Projector2D(m_virtual_window, get_Video().get_viewport());
  }

  void Widget_Gamestate::render() {
    Video &vr = get_Video();

    vr.set_2d(m_virtual_window, m_fix_aspect_ratio);

    m_widgets.render();
  }

}
