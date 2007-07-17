/* This file is part of the Zenipex Library.
* Copyleft (C) 2007 Mitchell Keith Bloch a.k.a. bazald
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

#include <Zeni/Coordinate.h>
#include <Zeni/Texture.h>
#include <Zeni/Textures.h>
#include <Zeni/Quadrilateral.hxx>
#include <Zeni/Vertex2f.h>
#include <Zeni/Video.h>

namespace Zeni {

  Widget::Widget()
    : m_clicked_down(0),
    m_clicked_elsewhere(0)
  {
  }
  
  Widget::Widget(const Point2f &upper_left, const Point2f &lower_right)
    : m_upper_left(upper_left),
    m_lower_right(lower_right),
    m_clicked_down(0),
    m_clicked_elsewhere(0)
  {
  }

  Widget::~Widget()
  {
  }

  Widget::MOUSE_STATE Widget::mouse_move(const Point2f &mouse_pos) {
    if(m_clicked_elsewhere ||
      mouse_pos.x < m_upper_left.x ||
      mouse_pos.y < m_upper_left.y ||
      mouse_pos.x > m_lower_right.x ||
      mouse_pos.y > m_lower_right.y)
      return ZENI_MOUSE_NORMAL;

    return m_clicked_down ? ZENI_MOUSE_CLICKED : ZENI_MOUSE_HOVERING;
  }

  Widget::MOUSE_STATE Widget::mouse_click(const Point2f &mouse_pos, const bool &down) {
    if(mouse_move(mouse_pos) == ZENI_MOUSE_NORMAL) {
      m_clicked_elsewhere = down;
      m_clicked_down = false;
      return ZENI_MOUSE_NORMAL;
    }

    m_clicked_down = down;
    m_clicked_elsewhere = false;
    return m_clicked_down ? ZENI_MOUSE_CLICKED : ZENI_MOUSE_UNCLICKED;
  }

  Button::Button()
    : m_quad(0)
  {
  }

  Button::Button(const Point2f &upper_left, const Point2f &lower_right,
    const std::string &normal, const std::string &mouse_over, const std::string &click_down)
    : Widget(upper_left, lower_right),
    m_quad(0)
  {
    m_sprite.append_frame(normal);
    m_sprite.append_frame(mouse_over);
    m_sprite.append_frame(click_down);

    char buf[64];
#ifdef WIN32
    sprintf_s
#else
    sprintf
#endif
      (buf, "Zeni_%d", Resource::get_reference().assign());
    m_sprite_name = buf;
    Textures::get_reference().set_texture(m_sprite_name, new Sprite(m_sprite));

    m_quad = new Quadrilateral<Vertex2f_Texture>(
      Vertex2f_Texture(Point2f(get_upper_left().x, get_upper_left().y), Point2f(0.0f, 0.0f)),
      Vertex2f_Texture(Point2f(get_upper_left().x, get_lower_right().y), Point2f(0.0f, 1.0f)),
      Vertex2f_Texture(Point2f(get_lower_right().x, get_lower_right().y), Point2f(1.0f, 1.0f)),
      Vertex2f_Texture(Point2f(get_lower_right().x, get_upper_left().y), Point2f(1.0f, 0.0f)),
      new Material_Render_Wrapper(Material(m_sprite_name)));
  }

  Button::Button(const Button &rhs) {
    m_sprite = rhs.m_sprite;
    m_sprite_name = rhs.m_sprite_name;
    m_quad = rhs.m_quad ? rhs.m_quad->get_duplicate() : 0;
  }

  Button::~Button() {
    delete m_quad;
  }

  Widget::MOUSE_STATE Button::mouse_move(const Point2f &mouse_pos) {
    MOUSE_STATE ms = Widget::mouse_move(mouse_pos);

    switch(ms) {
      case ZENI_MOUSE_NORMAL:
        m_sprite.set_current_frame(0);
        break;

      case ZENI_MOUSE_HOVERING:
        m_sprite.set_current_frame(1);
        break;

      case ZENI_MOUSE_CLICKED:
        m_sprite.set_current_frame(2);
        break;

      default:
        throw Error("Internal Error in Button::mouse_move.");
    }

    return ms;
  }

  Widget::MOUSE_STATE Button::mouse_click(const Point2f &mouse_pos, const bool &down) {
    MOUSE_STATE ms = Widget::mouse_click(mouse_pos, down);

    switch(ms) {
      case ZENI_MOUSE_NORMAL:
        m_sprite.set_current_frame(0);
        break;

      case ZENI_MOUSE_HOVERING:
      case ZENI_MOUSE_UNCLICKED:
        m_sprite.set_current_frame(1);
        break;

      case ZENI_MOUSE_CLICKED:
        m_sprite.set_current_frame(2);
        break;

      default:
        throw Error("Internal Error in Button::mouse_move.");
    }

    return ms;
  }

  void Button::render() {
    if(m_quad) {
      Video &vr = Video::get_reference();
      Textures &tex = Textures::get_reference();

      try {
        tex.set_current_frame(tex.get_texture_id(m_sprite_name), m_sprite.get_current_frame());
        vr.render(*m_quad);
      }
      catch(Texture_Not_Found &) {
        tex.set_texture(m_sprite_name, new Sprite(m_sprite));
        vr.render(*m_quad);
      }
    }
  }

  Button & Button::operator=(const Button &rhs) {
    static_cast<Widget &>(*this) = static_cast<Widget>(rhs);

    Button lhs(rhs);

    std::swap(m_sprite, lhs.m_sprite);
    std::swap(m_sprite_name, lhs.m_sprite_name);
    std::swap(m_quad, lhs.m_quad);

    return *this;
  }

}
