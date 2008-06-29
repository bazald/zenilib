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

#include <Zeni/Widget.h>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Resource.hxx>
#include <Zeni/Texture.h>
#include <Zeni/Textures.h>
#include <Zeni/Quadrilateral.hxx>
#include <Zeni/Vertex2f.h>
#include <Zeni/Video.h>

namespace Zeni {
  
  const Point2f & Widget_Rectangle::get_upper_left() const {
    return m_upper_left;
  }

  const Point2f & Widget_Rectangle::get_lower_right() const {
    return m_lower_right;
  }

  void Widget_Rectangle_Color::render() const {
    Video::get_reference().render(*m_quad);
  }

  void Widget_Rectangle_Texture::render() const {
    Video::get_reference().render(*m_quad);
  }

  const Point2f & Widget_Button::get_upper_left() const {
    return m_upper_left;
  }

  const Point2f & Widget_Button::get_lower_right() const {
    return m_lower_right;
  }
  
  void Widget_Button::on_mouse_button(const Point2i &pos, const bool &down) {
    const bool inside = is_inside(pos);

    if(down)
      if(inside) {
        m_clicked_inside = true;
        m_transient = false;
        on_click();
      }
      else
        m_clicked_outside = true;
    else
      if(inside)
        if(m_clicked_inside) {
          m_clicked_inside = false;
          on_accept();
          on_mouse_motion(pos);
        }
        else
          m_clicked_outside = false;
      else
        if(m_clicked_inside) {
          m_clicked_inside = false;
          m_transient = false;
          on_reject();
        }
        else
          m_clicked_outside = false;
  }
  
  void Widget_Button::on_mouse_motion(const Point2i &pos) {
    const bool inside = is_inside(pos);

    if(!m_clicked_outside)
      if(inside) {
        if(m_clicked_inside) {
          if(m_transient) {
            m_transient = false;
            on_unstray();
          }
        }
        else if(!m_transient) {
          m_transient = true;
          on_hover();
        }
      }
      else if(m_clicked_inside) {
        if(!m_transient) {
          m_transient = true;
          on_stray();
        }
      }
      else if(m_transient) {
        m_transient = false;
        on_unhover();
      }
  }

  void Text_Button::render() const {
    m_bg.render();
    m_text.render(get_center());
  }

  void Widgets::on_mouse_button(const Point2i &pos, const bool &down) {
    for(std::set<Widget *>::iterator it = m_widgets.begin(); it != m_widgets.end(); ++it)
      (*it)->on_mouse_button(pos, down);
  }
    
  void Widgets::on_mouse_motion(const Point2i &pos) {
    for(std::set<Widget *>::iterator it = m_widgets.begin(); it != m_widgets.end(); ++it)
      (*it)->on_mouse_motion(pos);
  }

  void Widgets::render() const {
    for(std::set<Widget *>::const_iterator it = m_widgets.begin(); it != m_widgets.end(); ++it)
      (*it)->render();
  }

}
