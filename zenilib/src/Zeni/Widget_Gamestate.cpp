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

#include <Zeni/Widget_Gamestate.h>

#include <Zeni/Video.hxx>
#include <Zeni/Widget.hxx>

using namespace std;

namespace Zeni {

  Widget_Gamestate::Widget_Gamestate(const std::pair<Zeni::Point2f, Zeni::Point2f> &virtual_window_)
    : m_virtual_window(virtual_window_)
  {
  }

  const std::pair<Point2i, Point2i> & Widget_Gamestate::get_crop_window() const {
    return m_crop_window;
  }

  const std::pair<Point2f, Point2f> & Widget_Gamestate::get_virtual_window() const {
    return m_virtual_window;
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

  void Widget_Gamestate::render() {
    resize();

    get_Video().set_2d_view(m_virtual_window, m_crop_window);

    m_widgets.render();
  }

  void Widget_Gamestate::resize() {
    const Point2i resolution(get_Video().get_screen_width(), get_Video().get_screen_height());

    if(resolution.x == m_prev_resolution.x && resolution.y == m_prev_resolution.y)
      return;
    else
      m_prev_resolution = resolution;

    m_crop_window = make_pair(Point2i(), resolution);

    /** Build virtual window**/

    const float desired_ratio = m_virtual_window.second.x / m_virtual_window.second.y;
    const float given_ratio = float(m_crop_window.second.x) / m_crop_window.second.y;

    if(given_ratio > desired_ratio) {
      const int new_width = int(m_crop_window.second.x * desired_ratio / given_ratio);
      const int cut_side = (m_crop_window.second.x - new_width) / 2;

      m_crop_window.first.x += cut_side;
      m_crop_window.second.x -= cut_side;
    }
    else if(desired_ratio > given_ratio) {
      const int new_height = int(m_crop_window.second.y * given_ratio / desired_ratio);
      const int cut_side = (m_crop_window.second.y - new_height) / 2;

      m_crop_window.first.y += cut_side;
      m_crop_window.second.y -= cut_side;
    }

    /** Build projector **/

    m_projector = Projector2D(m_virtual_window, m_crop_window);
  }

}
