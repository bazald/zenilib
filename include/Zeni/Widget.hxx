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

#ifndef ZENI_WIDGET_HXX
#define ZENI_WIDGET_HXX

#include <Zeni/Widget.h>

namespace Zeni {

  void Widget::on_mouse_event(const SDL_MouseButtonEvent &event) {
    on_mouse_button(Point2i(event.x, event.y), event.type == SDL_MOUSEBUTTONDOWN);
  }

  void Widget::on_mouse_event(const SDL_MouseMotionEvent &event) {
    on_mouse_motion(Point2i(event.x, event.y));
  }

  Point2f Widget_Positioned::get_lower_left() const {
    return Point2f(get_upper_left().x, get_lower_right().y);
  }

  Point2f Widget_Positioned::get_upper_right() const {
    return Point2f(get_lower_right().x, get_upper_left().y);
  }

  Point2f Widget_Positioned::get_center() const {
    const Point2f &upper_left = get_upper_left();
    const Point2f &lower_right = get_lower_right();

    return Point2f(0.5f * (upper_left.x + lower_right.x),
                   0.5f * (upper_left.y + lower_right.y));
  }
  
  Widget_Text::Widget_Text(const std::string &font_name_, const std::string &text_, const Color &color_)
    : m_font_name(font_name_),
    m_text(text_),
    m_color(color_)
  {
  }

  const std::string & Widget_Text::get_font_name() const {return m_font_name;}
  const std::string & Widget_Text::get_text() const {return m_text;}
  const Color & Widget_Text::get_color() const {return m_color;}

  void Widget_Text::set_font_name(const std::string &font_name_) {m_font_name = font_name_;}
  void Widget_Text::set_text(const std::string &text_) {m_text = text_;}
  void Widget_Text::set_color(const Color &color_) {m_color = color_;}

  void Widget_Text::render(const Point2f &center) const {
    const Font &font = Fonts::get_reference().get_font(m_font_name);

    const float x = center.x;
    const float y = center.y - 0.5f * font.get_text_height();
    font.render_text(m_text, int(x), int(y), m_color, ZENI_CENTER);
  }
  
  Widget_Rectangle::Widget_Rectangle(const Point2f &upper_left_, const Point2f &lower_right_)
    : m_upper_left(upper_left_),
    m_lower_right(lower_right_)
  {
  }

  Widget_Rectangle_Color::Widget_Rectangle_Color(const Point2f &upper_left_, const Point2f &lower_right_,
                                                 const Color &color_)
    : Widget_Rectangle(upper_left_, lower_right_),
    m_color(color_),
    m_quad(0)
  {
    generate_quadrilateral();
  }

  Widget_Rectangle_Color::Widget_Rectangle_Color(const Widget_Rectangle_Color &rhs)
    : Widget_Rectangle(rhs),
    m_color(rhs.m_color),
    m_quad(rhs.m_quad->get_duplicate())
  {
  }

  Widget_Rectangle_Color::~Widget_Rectangle_Color() {
    delete m_quad;
  }

  const Color & Widget_Rectangle_Color::get_color() const {return m_color;}

  void Widget_Rectangle_Color::set_color(const Color &color_) {
    m_color = color_;
    generate_quadrilateral();
  }

  Widget_Rectangle_Color & Widget_Rectangle_Color::operator=(const Widget_Rectangle_Color &rhs) {
    if(this != &rhs) {
      delete m_quad;
      m_quad = 0;

      reinterpret_cast<Widget_Rectangle &>(*this) = rhs;
      m_color = rhs.m_color;
      m_quad = rhs.m_quad->get_duplicate();
    }
  }

  void Widget_Rectangle_Color::generate_quadrilateral() {
    delete m_quad;
    m_quad = 0;

    Vertex2f_Color ul(get_upper_left(), m_color);
    Vertex2f_Color ll(get_lower_left(), m_color);
    Vertex2f_Color lr(get_lower_right(), m_color);
    Vertex2f_Color ur(get_upper_right(), m_color);

    m_quad = new Quadrilateral<Vertex2f_Color>(ul, ll, lr, ur);
  }

  Widget_Rectangle_Texture::Widget_Rectangle_Texture(const Point2f &upper_left_, const Point2f &lower_right_,
                                                     const std::string &texture_name_)
    : Widget_Rectangle(upper_left_, lower_right_),
    m_texture_name(texture_name_),
    m_quad(0)
  {
    generate_quadrilateral();
  }

  Widget_Rectangle_Texture::Widget_Rectangle_Texture(const Widget_Rectangle_Texture &rhs)
    : Widget_Rectangle(rhs),
    m_texture_name(rhs.m_texture_name),
    m_quad(rhs.m_quad->get_duplicate())
  {
  }

  Widget_Rectangle_Texture::~Widget_Rectangle_Texture() {
    delete m_quad;
  }

  const std::string Widget_Rectangle_Texture::get_texture_name() const {return m_texture_name;}
  void Widget_Rectangle_Texture::set_texture_name(const std::string &texture_name_) {
    m_texture_name = texture_name_;
    generate_quadrilateral();
  }

  Widget_Rectangle_Texture & Widget_Rectangle_Texture::operator=(const Widget_Rectangle_Texture &rhs) {
    if(this != &rhs) {
      delete m_quad;
      m_quad = 0;

      reinterpret_cast<Widget_Rectangle_Texture &>(*this) = rhs;
      m_texture_name = rhs.m_texture_name;
      m_quad = rhs.m_quad->get_duplicate();
    }
  }

  void Widget_Rectangle_Texture::generate_quadrilateral() {
    delete m_quad;
    m_quad = 0;

    Vertex2f_Texture ul(get_upper_left(), Point2f(0.0f, 0.0f));
    Vertex2f_Texture ll(get_lower_left(), Point2f(0.0f, 1.0f));
    Vertex2f_Texture lr(get_lower_right(), Point2f(1.0f, 1.0f));
    Vertex2f_Texture ur(get_upper_right(), Point2f(1.0f, 0.0f));

    m_quad = new Quadrilateral<Vertex2f_Texture>(ul, ll, lr, ur, new Material_Render_Wrapper(Material(m_texture_name)));
  }

  Widget_Button::Widget_Button(const Point2f &upper_left_, const Point2f &lower_right_)
    : m_upper_left(upper_left_),
    m_lower_right(lower_right_),
    m_clicked_inside(false),
    m_clicked_outside(false),
    m_transient(false)
  {
  }

  bool Widget_Button::is_inside(const Point2i &pos) const {
    return
      m_upper_left.x < pos.x && pos.x < m_lower_right.x &&
      m_upper_left.y < pos.y && pos.y < m_lower_right.y;
  }

  Text_Button::Text_Button(const Point2f &upper_left_, const Point2f &lower_right_,
                           const Color &bg_color_,
                           const std::string &font_name_, const std::string &text_, const Color &text_color_)
    : Widget_Button(upper_left_, lower_right_),
    m_bg(upper_left_, lower_right_, bg_color_),
    m_text(font_name_, text_, text_color_)
  {
  }

  void Widgets::add_Widget(Widget * const &widget) {
    m_widgets.insert(widget);
  }

  void Widgets::remove_Widget(Widget * const &widget) {
    m_widgets.erase(widget);
  }

}

#endif
