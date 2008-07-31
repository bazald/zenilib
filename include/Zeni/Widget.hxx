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

#include <Zeni/Fonts.h>
#include <Zeni/Timer.hxx>

namespace Zeni {

  void Widget::on_event(const SDL_KeyboardEvent &event) {
    on_key(event.keysym, event.type == SDL_KEYDOWN);
  }

  void Widget::on_event(const SDL_MouseButtonEvent &event) {
    on_mouse_button(Point2i(event.x, event.y), event.type == SDL_MOUSEBUTTONDOWN);
  }

  void Widget::on_event(const SDL_MouseMotionEvent &event) {
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
    font.render_text(m_text, Point2f(x, y), m_color, ZENI_CENTER);
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

    return *this;
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

    return *this;
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
  
  Check_Box::Check_Box(const Point2f &upper_left_, const Point2f &lower_right_,
                       const Color &border_color_, const Color &check_color_,
                       const bool &checked_, const bool &toggleable_)
    : Widget_Button(upper_left_, lower_right_),
    m_border_color(border_color_),
    m_check_color(check_color_),
    m_checked(checked_),
    m_toggleable(toggleable_)
  {
  }

  const Color & Check_Box::get_border_color() const {return m_border_color;}
  const Color & Check_Box::get_check_color() const {return m_check_color;}
  const bool & Check_Box::is_checked() const {return m_checked;}

  void Check_Box::set_border_color(const Color &border_color_) {m_border_color = border_color_;}
  void Check_Box::set_check_color(const Color &check_color_) {m_check_color = check_color_;}
  const void Check_Box::set_checked(const bool &checked_) {m_checked = checked_;}

  Radio_Button::Radio_Button(Radio_Button_Set &radio_button_set_,
                             const Point2f &upper_left_, const Point2f &lower_right_,
                             const Color &border_color_, const Color &check_color_,
                             const bool &checked_, const bool &toggleable_)
    : Check_Box(upper_left_, lower_right_, border_color_, check_color_, checked_, toggleable_),
    m_radio_button_set(&radio_button_set_)
  {
    radio_button_set_.add_Radio_Button(*this);
  }

  void Radio_Button_Set::accept(Radio_Button &radio_button) {
    for(std::set<Radio_Button *>::iterator it = m_radio_buttons.begin(); it != m_radio_buttons.end(); ++it)
      (*it)->set_checked((*it) == &radio_button);
  }

  void Radio_Button_Set::clear() {
    for(std::set<Radio_Button *>::iterator it = m_radio_buttons.begin(); it != m_radio_buttons.end(); ++it)
      (*it)->set_checked(false);
  }

  void Radio_Button_Set::add_Radio_Button(Radio_Button &radio_button) {
    m_radio_buttons.insert(&radio_button);
  }

  void Radio_Button_Set::remove_Radio_Button(Radio_Button &radio_button) {
    m_radio_buttons.erase(&radio_button);
  }

  Point2f Slider::get_end_point_a() const {
    return Point2f(m_line_segment.get_end_point_a());
  }

  Point2f Slider::get_end_point_b() const {
    return Point2f(m_line_segment.get_end_point_b());
  }

  const float & Slider::get_slider_radius() const {
    return m_slider_radius;
  }

  const Color & Slider::get_line_color() const {
    return m_line_color;
  }

  const Color & Slider::get_slider_color() const {
    return m_slider_color;
  }

  const float & Slider::get_slider_position() const {
    return m_slider_position;
  }

  void Slider::set_line_color(const Color &line_color_) {
    m_line_color = line_color_;

    m_line_segment_r.set_vertex(0, Vertex2f_Color(get_end_point_a(), m_line_color));
    m_line_segment_r.set_vertex(1, Vertex2f_Color(get_end_point_b(), m_line_color));
  }

  void Slider::set_slider_color(const Color &slider_color_) {
    m_slider_color = slider_color_;

    regenerate_slider_r();
  }

  void Slider::set_slider_position(const float &slider_position_) {
    if(slider_position_ < 0.0f)
      m_slider_position = 0.0f;
    else if(slider_position_ > 1.0f)
      m_slider_position = 1.0f;
    else
      m_slider_position = slider_position_;

    regenerate_slider_r();
  }
  
  void Slider::regenerate_slider_r() {
    const Point3f &p0 = m_line_segment.get_end_point_a();
    const Point3f &p1 = m_line_segment.get_end_point_b();
    const Vector3f v = p1 - p0;
    const Vector3f n(-v.j, v.i, 0.0f); // or (v.j, -v.i, 0.0f)

    const Point3f &midpt = p0 + m_slider_position * v;
    const Vector3f &n2 = m_slider_radius * n.normalized();

    m_slider_r.set_vertex(0, Vertex2f_Color(Point2f(midpt - n2), m_slider_color));
    m_slider_r.set_vertex(1, Vertex2f_Color(Point2f(midpt + n2), m_slider_color));
  }
  
  const Color & Text_Box::get_bg_color() const {
    return m_bg.get_color();
  }

  const std::string & Text_Box::get_font_name() const {
    return m_text.get_font_name();
  }

  const Font & Text_Box::get_font() const {
    return Fonts::get_reference().get_font(m_text.get_font_name());
  }

  const std::string & Text_Box::get_text() const {
    return m_text.get_text();
  }
  
  const Color & Text_Box::get_text_color() const {
    return m_text.get_color();
  }

  const bool & Text_Box::is_editable() const {
    return m_editable;
  }

  const JUSTIFY & Text_Box::get_justify() const {
    return m_justify;
  }

  int Text_Box::get_num_lines() const {
    return int(m_lines.size());
  }

  int Text_Box::get_max_lines() const {
    return int(get_lower_right().y - get_upper_left().y) / get_font().get_text_height();
  }

  void Text_Box::set_bg_color(const Color &bg_color_) {
    m_bg.set_color(bg_color_);
  }

  void Text_Box::set_font_name(const std::string &font_name_) {
    m_text.set_font_name(font_name_);
    format();
    seek(m_edit_pos);
  }

  void Text_Box::set_text(const std::string &text_) {
    m_text.set_text(text_);
    format();
    seek(min(m_edit_pos, this->get_max_seek()));
  }
  
  void Text_Box::set_text_color(const Color &text_color_) {
    m_text.set_color(text_color_);
  }

  void Text_Box::set_editable(const bool &editable_) {
    m_editable = editable_;
    format();
    invalidate_edit_pos();
  }
  
  void Text_Box::set_justify(const JUSTIFY &justify_) {
    m_justify = justify_;
  }

  void Text_Box::erase_lines(const int &before_index, const int &after_and_including_index) {
    std::string new_text;
    for(int i = 0, iend = int(m_lines.size()); i != iend; ++i)
      if(before_index <= i && i < after_and_including_index)
        new_text += m_lines[i].unformatted;
    if(!new_text.empty() && new_text[0] == '\n')
      new_text = new_text.substr(1, new_text.size() - 1);

    m_text.set_text(new_text);

    format();
    invalidate_edit_pos();
  }

  void Text_Box::invalidate_edit_pos() {
    m_edit_pos = -1;
    m_cursor_index.x = -1;
    m_cursor_index.y = -1;
  }

  Widget_Input_Repeater::Widget_Input_Repeater(Widget &widget_,
                                               const int &repeat_delay_,
                                               const int &repeat_interval_)
  : m_widget(&widget_),
  m_repeat_delay(repeat_delay_),
  m_repeat_interval(repeat_interval_),
  m_last_repeated(0),
  m_active(false)
  {
  }

  Widget * const & Widget_Input_Repeater::get_widget() const {return m_widget;}
  const int & Widget_Input_Repeater::get_repeat_delay() const {return m_repeat_delay;}
  const int & Widget_Input_Repeater::get_repeat_interval() const  {return m_repeat_interval;}

  void Widget_Input_Repeater::set_widget(Widget &widget_) {m_widget = &widget_;}
  void Widget_Input_Repeater::set_repeat_delay(const int &repeat_delay_) {m_repeat_delay = repeat_delay_;}
  void Widget_Input_Repeater::set_repeat_interval(const int &repeat_interval_) {m_repeat_interval = repeat_interval_;}

  void Widget_Input_Repeater::perform_logic() {
    if(!m_active)
      return;

    const Time current_time = Timer::get_reference().get_time();
    const int ticks = current_time.get_ticks_since(m_last_repeated);

    if(m_delay_finished && ticks > m_repeat_interval ||
      !m_delay_finished && ticks > m_repeat_delay) {
      m_delay_finished = true;
      m_last_repeated = current_time;
      m_widget->on_key(m_keysym, m_down);
    }
  }

  void Widgets::add_Widget(Widget &widget) {
    m_widgets.insert(&widget);
  }

  void Widgets::remove_Widget(Widget &widget) {
    m_widgets.erase(&widget);
  }

}

#endif
