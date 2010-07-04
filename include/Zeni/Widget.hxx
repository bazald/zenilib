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

#ifndef ZENI_WIDGET_HXX
#define ZENI_WIDGET_HXX

// HXXed below
#include <Zeni/Font.h>
#include <Zeni/Material.h>
#include <Zeni/Projector.h>
#include <Zeni/Quadrilateral.h>
#include <Zeni/Timer.h>
#include <Zeni/Video.h>

#include <Zeni/Widget.h>

// Not HXXed
#include <Zeni/Colors.h>
#include <Zeni/Fonts.h>

#include <cassert>
#include <algorithm>

namespace Zeni {

  Widget::Widget()
    : m_layer(0.0f),
    m_busy(false),
    m_editable(true),
    m_renderer(0),
    delete_m_renderer(false)
  {
  }

  const bool & Widget::is_busy() const {
    return m_busy;
  }

  const bool & Widget::is_editable() const {
    return m_editable;
  }

  const float & Widget::get_layer() const {
    return m_layer;
  }

  void Widget::set_busy(const bool &busy_) {
    m_busy = busy_;
  }

  void Widget::set_layer(const float &layer_) {
    m_layer = layer_;
  }

  void Widget::on_event(const SDL_KeyboardEvent &event) {
    on_key(event.keysym, event.type == SDL_KEYDOWN);
  }

  void Widget::on_event(const SDL_MouseButtonEvent &event) {
    on_mouse_button(Point2i(event.x, event.y),
                    event.type == SDL_MOUSEBUTTONDOWN,
                    event.button);
  }

  void Widget::on_event(const SDL_MouseButtonEvent &event, const Projector2D &projector) {
    const Point2f projected = projector.unproject(Point2f(float(event.x), float(event.y)));
    on_mouse_button(Point2i(int(projected.x), int(projected.y)),
                    event.type == SDL_MOUSEBUTTONDOWN,
                    event.button);
  }

  void Widget::on_event(const SDL_MouseMotionEvent &event) {
    on_mouse_motion(Point2i(event.x, event.y));
  }

  void Widget::on_event(const SDL_MouseMotionEvent &event, const Projector2D &projector) {
    const Point2f projected = projector.unproject(Point2f(float(event.x), float(event.y)));
    on_mouse_motion(Point2i(int(projected.x), int(projected.y)));
  }

  void Widget::render() const {
    Video &vr = get_Video();

    vr.push_world_stack();
    vr.translate_scene(Vector3f(0.0f, 0.0f, m_layer));

    render_impl();

    vr.pop_world_stack();
  }

  const Widget_Render_Function * Widget::get_Renderer() const {
    return m_renderer;
  }

  void Widget::give_Renderer(Widget_Render_Function * const &renderer) {
    if(delete_m_renderer)
      delete m_renderer;
    m_renderer = renderer;
    delete_m_renderer = true;
  }

  void Widget::lend_Renderer(const Widget_Render_Function * const &renderer) {
    give_Renderer(const_cast<Widget_Render_Function * const &>(renderer));
    delete_m_renderer = false;
  }

  void Widget::fax_Renderer(const Widget_Render_Function * const &renderer) {
    give_Renderer(renderer->get_duplicate());
  }
  
  Widget_Rectangle::Widget_Rectangle(const Point2f &upper_left_, const Point2f &lower_right_)
    : m_upper_left(upper_left_),
    m_lower_right(lower_right_)
  {
  }

  const Point2f & Widget_Rectangle::get_upper_left() const {
    return m_upper_left;
  }

  Point2f Widget_Rectangle::get_lower_left() const {
    return Point2f(m_upper_left.x, m_lower_right.y);
  }

  const Point2f & Widget_Rectangle::get_lower_right() const {
    return m_lower_right;
  }

  Point2f Widget_Rectangle::get_upper_right() const {
    return Point2f(m_lower_right.x, m_upper_left.y);
  }

  float Widget_Rectangle::get_height() const {
    return m_lower_right.y - m_upper_left.y;
  }

  float Widget_Rectangle::get_width() const {
    return m_lower_right.x - m_upper_left.x;
  }

  Point2f Widget_Rectangle::get_center() const {
    return Point2f(0.5f * (m_upper_left.x + m_lower_right.x),
                   0.5f * (m_upper_left.y + m_lower_right.y));
  }

  bool Widget_Rectangle::is_inside(const Point2i &pos) const {
    return m_upper_left.x < pos.x && pos.x < m_lower_right.x &&
           m_upper_left.y < pos.y && pos.y < m_lower_right.y;
  }

  template <typename T1, typename T2>
  Widget_Renderer_Pair<T1, T2>::Widget_Renderer_Pair(const T1 * const &first_, const bool &delete_first_,
                                                     const T2 * const &second_, const bool &delete_second_)
    : m_first(const_cast<T1 * const &>(first_)),
    delete_m_first(delete_first_),
    m_second(const_cast<T2 * const &>(second_)),
    delete_m_second(delete_second_)
  {
  }

  template <typename T1, typename T2>
  Widget_Renderer_Pair<T1, T2>::~Widget_Renderer_Pair() {
    if(delete_m_first)
      delete m_first;
    if(delete_m_second)
      delete m_second;
  }

  template <typename T1, typename T2>
  const T1 * const & Widget_Renderer_Pair<T1, T2>::first() const {
    return m_first;
  }

  template <typename T1, typename T2>
  const T2 * const & Widget_Renderer_Pair<T1, T2>::second() const {
    return m_second;
  }

  template <typename T1, typename T2>
  T1 * const & Widget_Renderer_Pair<T1, T2>::first() {
    return m_first;
  }

  template <typename T1, typename T2>
  T2 * const & Widget_Renderer_Pair<T1, T2>::second() {
    return m_second;
  }

  template <typename T1, typename T2>
  void Widget_Renderer_Pair<T1, T2>::render_to(const Widget &rect) {
    if(m_first)
      m_first->render_to(rect);
    if(m_second)
      m_second->render_to(rect);
  }

  template <typename T1, typename T2>
  Widget_Renderer_Pair<T1, T2> * Widget_Renderer_Pair<T1, T2>::get_duplicate() const {
    return new Widget_Renderer_Pair<T1, T2>(new T1(*m_first), true, new T2(*m_second), true);
  }

  template <typename T1, typename T2>
  Widget_Renderer_Pair<T1, T2> * make_Widget_Renderer_Pair(const T1 * const &first_, const bool &delete_first_,
                                                           const T2 * const &second_, const bool &delete_second_)
  {
    return new Widget_Renderer_Pair<T1, T2>(first_, delete_first_, second_, delete_second_);
  }

  Widget_Renderer_Text::Widget_Renderer_Text(const std::string &font_name_, const std::string &text_, const Color &color_)
    : font_name(font_name_),
    text(text_),
    color(color_)
  {
  }

  Widget_Renderer_Color::Widget_Renderer_Color(const Color &color_)
    : color(color_)
  {
  }

  Widget_Renderer_Texture::Widget_Renderer_Texture(const std::string &texture_)
    : texture(texture_),
    tex_coord_ul(0.0f, 0.0f),
    tex_coord_ll(0.0f, 1.0f),
    tex_coord_lr(1.0f, 1.0f),
    tex_coord_ur(1.0f, 0.0f)
  {
  }

  Widget_Renderer_Texture::Widget_Renderer_Texture(const std::string &texture_,
                                                   const Point2f &tex_coord_ul_,
                                                   const Point2f &tex_coord_ll_,
                                                   const Point2f &tex_coord_lr_,
                                                   const Point2f &tex_coord_ur_)
    : texture(texture_),
    tex_coord_ul(tex_coord_ul_),
    tex_coord_ll(tex_coord_ll_),
    tex_coord_lr(tex_coord_lr_),
    tex_coord_ur(tex_coord_ur_)
  {
  }

  Widget_Renderer_Tricolor::Widget_Renderer_Tricolor()
    : Widget_Renderer_Color(Color()),
    bg_normal(get_Colors()["default_button_bg_normal"]),
    bg_clicked(get_Colors()["default_button_bg_clicked"]),
    bg_hovered_strayed(get_Colors()["default_button_bg_hovered_strayed"]),
    text_normal(get_Colors()["default_button_text_normal"]),
    text_clicked(get_Colors()["default_button_text_clicked"]),
    text_hovered_strayed(get_Colors()["default_button_text_hovered_strayed"])
  {
  }

  Widget_Renderer_Tricolor::Widget_Renderer_Tricolor(const Color &bg_normal_, const Color &bg_clicked_, const Color &bg_hovered_strayed_,
                                                     const Color &text_normal_, const Color &text_clicked_, const Color &text_hovered_strayed_)
    : Widget_Renderer_Color(Color()),
    bg_normal(bg_normal_),
    bg_clicked(bg_clicked_),
    bg_hovered_strayed(bg_hovered_strayed_),
    text_normal(text_normal_),
    text_clicked(text_clicked_),
    text_hovered_strayed(text_hovered_strayed_)
  {
  }

  Widget_Renderer_Check_Box::Widget_Renderer_Check_Box()
    : border_color(get_Colors()["default_button_bg_normal"]),
    check_color(get_Colors()["default_button_bg_normal"])
  {
  }

  Widget_Renderer_Check_Box::Widget_Renderer_Check_Box(const Color &border_color_, const Color &check_color_)
    : border_color(border_color_),
    check_color(check_color_)
  {
  }

  Widget_Renderer_Slider::Widget_Renderer_Slider()
    : line_color(get_Colors()["default_button_bg_normal"]),
    slider_color(get_Colors()["default_button_bg_normal"])
  {
  }

  Widget_Renderer_Slider::Widget_Renderer_Slider(const Color &line_color_, const Color &slider_color_)
    : line_color(line_color_),
    slider_color(slider_color_)
  {
  }

  Widget_Button::Widget_Button(const Point2f &upper_left_, const Point2f &lower_right_)
    : Widget_Rectangle(upper_left_, lower_right_),
    m_state(NORMAL)
  {
  }

  const Widget_Button::State & Widget_Button::get_State() const {
    return m_state;
  }

  Text_Button::Text_Button(const Point2f &upper_left_, const Point2f &lower_right_,
                           const std::string &font_name_, const std::string &text_)
    : Widget_Button(upper_left_, lower_right_),
    Widget_Renderer_Text(font_name_, text_, Color())
  {
    give_Renderer(new Widget_Renderer_Tricolor);
  }

  Check_Box::Check_Box(const Point2f &upper_left_, const Point2f &lower_right_,
                       const bool &checked_, const bool &toggleable_)
    : Widget_Button(upper_left_, lower_right_),
    m_checked(checked_),
    m_toggling(false)
  {
    set_editable(toggleable_);

    give_Renderer(new Widget_Renderer_Check_Box());
  }

  const bool & Check_Box::is_checked() const {return m_checked;}
  const void Check_Box::set_checked(const bool &checked_) {m_checked = checked_;}
  const bool & Check_Box::is_toggling() const {return m_toggling;}

  Radio_Button::Radio_Button(Radio_Button_Set &radio_button_set_,
                             const Point2f &upper_left_, const Point2f &lower_right_,
                             const bool &checked_, const bool &toggleable_)
    : Check_Box(upper_left_, lower_right_, checked_, toggleable_),
    m_radio_button_set(&radio_button_set_)
  {
    radio_button_set_.lend_Radio_Button(*this);
  }

  Radio_Button::~Radio_Button() {
    if(m_radio_button_set)
      m_radio_button_set->unlend_Radio_Button(*this);
  }

  void Radio_Button_Set::accept(Radio_Button &radio_button) {
    for(std::set<Radio_Button *>::iterator it = m_radio_buttons.begin(); it != m_radio_buttons.end(); ++it)
      (*it)->set_checked((*it) == &radio_button);
  }

  void Radio_Button_Set::clear() {
    for(std::set<Radio_Button *>::iterator it = m_radio_buttons.begin(); it != m_radio_buttons.end(); ++it)
      (*it)->set_checked(false);
  }

  void Radio_Button_Set::lend_Radio_Button(Radio_Button &radio_button) {
    m_radio_buttons.insert(&radio_button);
  }

  void Radio_Button_Set::unlend_Radio_Button(Radio_Button &radio_button) {
    m_radio_buttons.erase(&radio_button);
    radio_button.m_radio_button_set = 0;
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

  const float & Slider::get_slider_position() const {
    return m_slider_position;
  }

  void Slider::set_end_points(const Point2f &end_point_a_, const Point2f &end_point_b_) {
    m_line_segment = Collision::Line_Segment(Point3f(end_point_a_), Point3f(end_point_b_));
  }

  void Slider::set_slider_radius(const float &radius_) {
    m_slider_radius = radius_;
  }

  void Slider::set_slider_position(const float &slider_position_) {
    if(slider_position_ < 0.0f)
      m_slider_position = 0.0f;
    else if(slider_position_ > 1.0f)
      m_slider_position = 1.0f;
    else
      m_slider_position = slider_position_;
  }

  const Collision::Line_Segment & Slider::get_line_segment() const {
    return m_line_segment;
  }

  const Slider_Int::Range & Slider_Int::get_range() const {
    return m_range;
  }

  void Slider_Int::set_range(const Range &range_) {
    assert(range_.first <= range_.second);
    m_range = range_;
  }

  int Slider_Int::get_value() const {
    return int(get_slider_position() * (m_range.second - m_range.first) + 0.5f) + m_range.first;
  }

  void Slider_Int::set_value(const int &value) {
    const int clamped = value < m_range.first ? m_range.first : value > m_range.second ? m_range.second : value;
    set_slider_position(float(clamped - m_range.first) / (m_range.second - m_range.first));
  }

  const bool & Slider_Int::is_mouse_wheel_inverted() const {
    return m_mouse_wheel_inverted;
  }

  void Slider_Int::invert_mouse_wheel(const bool &invert) {
    m_mouse_wheel_inverted = invert;
  }

  const Widget_Render_Function * Selector::get_Text_Button_Renderer() const {
    return m_button_renderer;
  }

  void Selector::give_Text_Button_Renderer(Widget_Render_Function * const &renderer) {
    if(delete_m_button_renderer)
      delete m_button_renderer;
    m_button_renderer = renderer;
    delete_m_button_renderer = true;

    m_normal_button.lend_Renderer(m_button_renderer);
    for(std::vector<Selector_Button *>::iterator it = m_selector_buttons.begin(); it != m_selector_buttons.end(); ++it)
      (*it)->lend_Renderer(m_button_renderer);
  }

  void Selector::lend_Text_Button_Renderer(const Widget_Render_Function * const &renderer) {
    give_Text_Button_Renderer(const_cast<Widget_Render_Function * const &>(renderer));
    delete_m_button_renderer = false;
  }

  void Selector::fax_Text_Button_Renderer(const Widget_Render_Function * const &renderer) {
    give_Text_Button_Renderer(renderer->get_duplicate());
  }

  const Widget_Render_Function * Selector::get_Slider_Renderer() const {
    return m_slider_renderer;
  }

  void Selector::give_Slider_Renderer(Widget_Render_Function * const &renderer) {
    if(delete_m_slider_renderer)
      delete m_slider_renderer;
    m_slider_renderer = renderer;
    delete_m_slider_renderer = true;

    m_selector_slider.lend_Renderer(m_slider_renderer);
  }

  void Selector::lend_Slider_Renderer(const Widget_Render_Function * const &renderer) {
    give_Slider_Renderer(const_cast<Widget_Render_Function * const &>(renderer));
    delete_m_button_renderer = false;
  }

  void Selector::fax_Slider_Renderer(const Widget_Render_Function * const &renderer) {
    give_Slider_Renderer(renderer->get_duplicate());
  }

  const Widget_Render_Function * Selector::get_Slider_BG_Renderer() const {
    return m_slider_bg_renderer;
  }

  void Selector::give_Slider_BG_Renderer(Widget_Render_Function * const &renderer) {
    if(delete_m_slider_bg_renderer)
      delete m_slider_bg_renderer;
    m_slider_bg_renderer = renderer;
    delete_m_slider_bg_renderer = true;
  }

  void Selector::lend_Slider_BG_Renderer(const Widget_Render_Function * const &renderer) {
    give_Slider_BG_Renderer(const_cast<Widget_Render_Function * const &>(renderer));
    delete_m_slider_bg_renderer = false;
  }

  void Selector::fax_Slider_BG_Renderer(const Widget_Render_Function * const &renderer) {
    give_Slider_BG_Renderer(renderer->get_duplicate());
  }

  const std::string & Selector::get_font() const {
    return m_font;
  }

  void Selector::set_font(const std::string &font_) {
    m_font = font_;

    m_normal_button.font_name = m_font;
    for(std::vector<Selector_Button *>::iterator it = m_selector_buttons.begin(); it != m_selector_buttons.end(); ++it)
      (*it)->font_name = m_font;
  }

  const std::string & Text_Box::get_font_name() const {
    return m_text.font_name;
  }

  const Font & Text_Box::get_font() const {
    return get_Fonts()[m_text.font_name];
  }

  const std::string & Text_Box::get_text() const {
    return m_text.text;
  }
  
  const Color & Text_Box::get_text_color() const {
    return m_text.color;
  }

  const JUSTIFY & Text_Box::get_justify() const {
    return m_justify;
  }

  int Text_Box::get_num_lines() const {
    return int(m_lines.size());
  }

  int Text_Box::get_max_lines() const {
    return int(get_height() / get_font().get_text_height());
  }

  void Text_Box::set_font_name(const std::string &font_name_) {
    m_text.font_name = font_name_;
    format();
    seek(m_edit_pos);
  }

  void Text_Box::set_text(const std::string &text_) {
    m_text.text = text_;
    format();
    seek(std::min(m_edit_pos, this->get_max_seek()));
  }
  
  void Text_Box::set_text_color(const Color &text_color_) {
    m_text.color = text_color_;
  }
  
  void Text_Box::set_justify(const JUSTIFY &justify_) {
    m_justify = justify_;
  }

  void Text_Box::erase_lines(const int &begin, const int &end) {
    std::string new_text;

    assert(-1 < begin);
    assert(begin <= end);
    assert(end <= get_num_lines());

    for(int i = 0; i != begin; ++i)
      new_text += m_lines[size_t(i)].unformatted;
    if(end != get_num_lines()) {
      if(m_lines[size_t(end)].endled)
        new_text += m_lines[size_t(end)].unformatted.substr(1u);
      else
        new_text += m_lines[size_t(end)].unformatted;

      for(int i = end + 1; i != get_num_lines(); ++i)
        new_text += m_lines[size_t(i)].unformatted;
    }

    m_text.text = new_text;

    format();
    invalidate_edit_pos();
  }

  const Widget_Render_Function * Text_Box::get_BG_Renderer() const {
    return m_bg_renderer;
  }

  void Text_Box::give_BG_Renderer(Widget_Render_Function * const &renderer) {
    if(delete_m_bg_renderer)
      delete m_bg_renderer;
    m_bg_renderer = renderer;
    delete_m_bg_renderer = true;
  }

  void Text_Box::lend_BG_Renderer(const Widget_Render_Function * const &renderer) {
    give_BG_Renderer(const_cast<Widget_Render_Function * const &>(renderer));
    delete_m_bg_renderer = false;
  }

  void Text_Box::fax_BG_Renderer(const Widget_Render_Function * const &renderer) {
    give_BG_Renderer(renderer->get_duplicate());
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

  void Widget_Input_Repeater::set_widget(Widget &widget_) {m_widget = &widget_; set_busy(widget_.is_busy());}
  void Widget_Input_Repeater::set_repeat_delay(const int &repeat_delay_) {m_repeat_delay = repeat_delay_;}
  void Widget_Input_Repeater::set_repeat_interval(const int &repeat_interval_) {m_repeat_interval = repeat_interval_;}

  void Widget_Input_Repeater::perform_logic() {
    if(!m_active)
      return;

    const Time current_time = get_Timer().get_time();
    const int ticks = int(current_time.get_ticks_since(m_last_repeated));

    if((m_delay_finished && ticks > m_repeat_interval) ||
       (!m_delay_finished && ticks > m_repeat_delay))
    {
      m_delay_finished = true;
      m_last_repeated = current_time;
      m_widget->on_key(m_keysym, m_down);
    }
  }

  Widgets::Widgets()
    : m_busy_one(0)
  {
  }

  void Widgets::lend_Widget(Widget &widget) {
    m_widgets.push_back(&widget);

    if(widget.is_busy()) {
      assert(!m_busy_one);
      m_busy_one = &widget;
      set_busy(true);
    }
  }

  void Widgets::unlend_Widget(Widget &widget) {
    std::vector<Widget *>::iterator it = std::find(m_widgets.begin(), m_widgets.end(), &widget);
    if(it != m_widgets.end())
      m_widgets.erase(it);

    if(m_busy_one == &widget) {
      m_busy_one = 0;
      set_busy(false);
    }
  }

}

#include <Zeni/Font.hxx>
#include <Zeni/Material.hxx>
#include <Zeni/Projector.hxx>
#include <Zeni/Quadrilateral.hxx>
#include <Zeni/Timer.hxx>
#include <Zeni/Video.hxx>

#endif
