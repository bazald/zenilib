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

#include <Zeni/Widget.hxx>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Line_Segment.hxx>
#include <Zeni/Texture.h>
#include <Zeni/Quadrilateral.hxx>
#include <Zeni/Vertex2f.hxx>
#include <Zeni/Video.h>

#include <list>

#include <Zeni/Global.h>

using namespace std;

namespace Zeni {

  void Widget::set_editable(const bool &editable_) {
    m_editable = editable_;
  }
  
  const Point2f & Widget_Rectangle::get_upper_left() const {
    return m_upper_left;
  }

  const Point2f & Widget_Rectangle::get_lower_right() const {
    return m_lower_right;
  }

  void Widget_Rectangle_Color::render_impl() const {
    get_Video().render(*m_quad);
  }

  void Widget_Rectangle_Texture::render_impl() const {
    get_Video().render(*m_quad);
  }

  const Point2f & Widget_Button::get_upper_left() const {
    return m_upper_left;
  }

  const Point2f & Widget_Button::get_lower_right() const {
    return m_lower_right;
  }
  
  void Widget_Button::on_mouse_button(const Point2i &pos, const bool &down, const int &button) {
    if(!is_editable() || button != SDL_BUTTON_LEFT)
      return;

    const bool inside = is_inside(pos);

    if(down)
      if(inside) {
        m_transient = false;
        m_clicked_inside = true;
        on_click();

        set_busy(true);
      }
      else {
        m_transient = true;
        m_clicked_outside = true;

        set_busy(false);
      }
    else {
      if(inside) {
        m_transient = true;

        if(m_clicked_inside) {
          m_clicked_inside = false;
          on_accept();
          on_mouse_motion(pos);
        }
        else
          m_clicked_outside = false;
      }
      else {
        m_transient = false;

        if(m_clicked_inside) {
          m_clicked_inside = false;
          on_reject();
        }
        else
          m_clicked_outside = false;
      }

      set_busy(false);
    }
  }
  
  void Widget_Button::on_mouse_motion(const Point2i &pos) {
    if(!is_editable())
      return;

    const bool inside = is_inside(pos);
    m_clicked_outside &= get_Game().get_mouse_button_state(SDL_BUTTON_LEFT);

    if(!m_clicked_outside) {
      if(inside)
        if(m_clicked_inside) {
          if(m_transient) {
            m_transient = false;
            on_unstray();
          }
        }
        else {
          if(!m_transient) {
            m_transient = true;
            on_hover();
          }
        }
      else
        if(m_clicked_inside) {
          if(!m_transient) {
            m_transient = true;
            on_stray();
          }
        }
        else {
          if(m_transient) {
            m_transient = false;
            on_unhover();
          }
        }
    }
  }

  void Text_Button::render_impl() const {
    m_bg.render_impl();
    m_text.render(get_center());
  }

  void Text_Button_3C::on_unhover() {
    m_bg.set_color(m_bg_normal);
    m_text.set_color(m_text_normal);
    m_state = NORMAL;
  }

  void Text_Button_3C::on_click() {
    m_bg.set_color(m_bg_clicked);
    m_text.set_color(m_text_clicked);
    m_state = CLICKED;
  }

  void Text_Button_3C::on_unstray() {
    m_bg.set_color(m_bg_clicked);
    m_text.set_color(m_text_clicked);
    m_state = CLICKED;
  }

  void Text_Button_3C::on_hover() {
    m_bg.set_color(m_bg_hovered_strayed);
    m_text.set_color(m_text_hovered_strayed);
    m_state = HOVERED_STRAYED;
  }

  void Text_Button_3C::on_stray() {
    m_bg.set_color(m_bg_hovered_strayed);
    m_text.set_color(m_text_hovered_strayed);
    m_state = HOVERED_STRAYED;
  }

  void Text_Button_3C::on_accept() {
    m_bg.set_color(m_bg_hovered_strayed);
    m_text.set_color(m_text_hovered_strayed);
    m_state = HOVERED_STRAYED;
  }

  void Text_Button_3C::on_reject() {
    m_bg.set_color(m_bg_normal);
    m_text.set_color(m_text_normal);
    m_state = NORMAL;
  }

  void Check_Box::on_accept() {
    m_checked = !m_checked;
    m_toggling = false;
  }

  void Check_Box::on_click() {
    m_toggling = true;
  }

  void Check_Box::on_unstray() {
    m_toggling = true;
  }

  void Check_Box::on_reject() {
    m_toggling = false;
  }

  void Check_Box::on_stray() {
    m_toggling = false;
  }

  void Check_Box::render_impl() const {
    Video &vr = get_Video();

    Vertex2f_Color ul(get_upper_left(), m_border_color);
    Vertex2f_Color ll(get_lower_left(), m_border_color);
    Vertex2f_Color lr(get_lower_right(), m_border_color);
    Vertex2f_Color ur(get_upper_right(), m_border_color);

    Line_Segment<Vertex2f_Color> line_seg(ul, ll);
    vr.render(line_seg);

    line_seg.a = lr;
    vr.render(line_seg);

    line_seg.b = ur;
    vr.render(line_seg);

    line_seg.a = ul;
    vr.render(line_seg);

    if(m_checked || m_toggling) {
      Color check_color = m_check_color;
      if(m_toggling) {
        if(m_checked)
          check_color.a /= 3.0f;
        else
          check_color.a *= 2.0f / 3.0f;
      }

      ul.set_color(check_color);
      ll.set_color(check_color);
      lr.set_color(check_color);
      ur.set_color(check_color);

      line_seg.a = ul;
      line_seg.b = lr;
      vr.render(line_seg);

      line_seg.a = ll;
      line_seg.b = ur;
      vr.render(line_seg);
    }
  }

  void Radio_Button::on_accept() {
    if(is_checked())
      return;

    Check_Box::on_accept();

    m_radio_button_set->accept(*this);
  }

  void Radio_Button_Set::on_mouse_button(const Point2i &pos, const bool &down, const int &button) {
    if(!is_editable())
      return;

    for(std::set<Radio_Button *>::iterator it = m_radio_buttons.begin(); it != m_radio_buttons.end(); ++it)
      (*it)->on_mouse_button(pos, down, button);
  }
    
  void Radio_Button_Set::on_mouse_motion(const Point2i &pos) {
    if(!is_editable())
      return;

    for(std::set<Radio_Button *>::iterator it = m_radio_buttons.begin(); it != m_radio_buttons.end(); ++it)
      (*it)->on_mouse_motion(pos);
  }

  void Radio_Button_Set::render_impl() const {
    for(std::set<Radio_Button *>::const_iterator it = m_radio_buttons.begin(); it != m_radio_buttons.end(); ++it)
      (*it)->render_impl();
  }

  Slider::Slider(const Point2f &end_point_a_, const Point2f &end_point_b_,
                 const float &slider_radius_,
                 const Color &line_color_,
                 const Color &slider_color_,
                 const float &slider_position_)
  : m_line_segment(Point3f(end_point_a_), Point3f(end_point_b_)),
    m_line_color(line_color_),
    m_line_segment_r(Vertex2f_Color(Point2f(m_line_segment.get_end_point_a()), m_line_color),
                     Vertex2f_Color(Point2f(m_line_segment.get_end_point_b()), m_line_color)),
    m_slider_radius(slider_radius_),
    m_slider_color(slider_color_),
    m_slider_position(slider_position_),
    m_down(false)
  {
    regenerate_slider_r();
  }

  void Slider::on_mouse_button(const Zeni::Point2i &pos, const bool &down, const int &button) {
    if(!is_editable() || button != SDL_BUTTON_LEFT)
      return;

    if(down) {
      const Point3f mouse_pos(float(pos.x), float(pos.y), 0.0f);

      const std::pair<float, float> test = m_line_segment.nearest_point(mouse_pos);
      if(test.first < m_slider_radius) {
        m_down = true;
        m_backup_position = m_slider_position;
        m_slider_position = test.second;
        regenerate_slider_r();
        on_slide();

        set_busy(true);
      }
      else
        m_down = false;
    }
    else {
      if(m_down)
        on_accept();

      set_busy(false);
    }
  }

  void Slider::on_mouse_motion(const Zeni::Point2i &pos) {
    if(m_down) {
      const Point3f mouse_pos(float(pos.x), float(pos.y), 0.0f);

      const std::pair<float, float> test = m_line_segment.nearest_point(mouse_pos);
      if(test.first < m_slider_radius) {
        m_slider_position = test.second;
        regenerate_slider_r();
        on_slide();
      }
      else {
        m_slider_position = m_backup_position;
        regenerate_slider_r();
        on_slide();
      }
    }
  }

  void Slider::on_slide() {
  }

  void Slider::on_accept() {
    m_down = false;
  }

  void Slider::render_impl() const {
    Video &vr = get_Video();

    vr.render(m_line_segment_r);
    vr.render(m_slider_r);
  }

  Slider_Int::Slider_Int(const Range &range,
                         const Point2f &end_point_a_, const Point2f &end_point_b_,
                         const float &slider_radius_,
                         const Color &line_color_,
                         const Color &slider_color_,
                         const float &slider_position_)
  : Slider(end_point_a_, end_point_b_, slider_radius_, line_color_, slider_color_, slider_position_),
  m_range(range)
  {
    assert(range.first < range.second);
    set_value(get_value());
  }

  void Slider_Int::on_slide() {
    Slider::on_slide();
    set_value(get_value());
  }

  Text_Box::Text_Box(const Point2f &upper_left_, const Point2f &lower_right_,
                     const Color &bg_color_,
                     const std::string &font_name_, const std::string &text_, const Color &text_color_,
                     const bool &editable_, const JUSTIFY &justify_, const int &tab_spaces_)
  : Widget_Button(upper_left_, lower_right_),
    m_bg(upper_left_, lower_right_, bg_color_),
    m_text(font_name_, clean_string(text_), text_color_),
    m_edit_pos(-1),
    m_last_seek(0),
    m_justify(justify_),
    m_tab_spaces(tab_spaces_),
    m_cursor_index(-1, -1)
  {
    set_editable(editable_);
    format();
  }

  Selector::Normal_Button::Normal_Button(Selector &selector,
                                         const Point2f &upper_left_,
                                         const Point2f &lower_right_,
                                         const Color &bg_normal_, const Color &bg_clicked_, const Color &bg_hovered_strayed_,
                                         const std::string &font_,
                                         const Color &text_normal_, const Color &text_clicked_, const Color &text_hovered_strayed_)
    : Text_Button_3C(upper_left_, lower_right_, bg_normal_, bg_clicked_, bg_hovered_strayed_, font_, "", text_normal_, text_clicked_, text_hovered_strayed_),
    m_selector(&selector)
  {
  }

  void Selector::Normal_Button::on_accept() {
    Text_Button::on_accept();
    m_selector->decide_visible(m_selector->m_option);
    m_selector->m_selected = true;

    m_selector->set_busy(true);
    m_selector->set_layer(-0.5f);
  }

  Selector::Selector_Button::Selector_Button(Selector &selector,
                                             const std::string &option,
                                             const Point2f &upper_left_,
                                             const Point2f &lower_right_,
                                             const Color &bg_normal_, const Color &bg_clicked_, const Color &bg_hovered_strayed_,
                                             const std::string &font_,
                                             const Color &text_normal_, const Color &text_clicked_, const Color &text_hovered_strayed_)
    : Text_Button_3C(upper_left_, lower_right_, bg_normal_, bg_clicked_, bg_hovered_strayed_, font_, option, text_normal_, text_clicked_, text_hovered_strayed_),
    m_selector(&selector)
  {
  }

  void Selector::Selector_Button::on_accept() {
    Text_Button::on_accept();
    m_selector->on_accept(get_text());
    m_selector->m_selected = false;
  }

  Selector::Selector_Slider::Selector_Slider(Selector &selector,
                                             const float &slider_radius_,
                                             const Color &line_color_,
                                             const Color &slider_color_,
                                             const std::pair<float, float> &bg_coordinates_,
                                             const Color &bg_color_)
    : Slider_Int(Range(0u, 1u),
                 Point2f(), Point2f(),
                 slider_radius_,
                 line_color_,
                 slider_color_),
    m_quad(Vertex2f_Color(Point2f(bg_coordinates_.first, 0.0f), bg_color_),
           Vertex2f_Color(Point2f(bg_coordinates_.first, 0.0f), bg_color_),
           Vertex2f_Color(Point2f(bg_coordinates_.second, 0.0f), bg_color_),
           Vertex2f_Color(Point2f(bg_coordinates_.second, 0.0f), bg_color_)),
    m_selector(&selector)
  {
  }

  void Selector::Selector_Slider::set_end_points(const Point2f &end_point_a_, const Point2f &end_point_b_) {
    Slider_Int::set_end_points(end_point_a_, end_point_b_);
    m_quad.a.position.y = end_point_a_.y;
    m_quad.b.position.y = end_point_b_.y;
    m_quad.c.position.y = end_point_b_.y;
    m_quad.d.position.y = end_point_a_.y;
  }

  void Selector::Selector_Slider::on_slide() {
    Slider_Int::on_slide();
    m_selector->decide_visible(get_value());
  }

  void Selector::Selector_Slider::render_impl() const {
    get_Video().render(m_quad);
    Slider_Int::render_impl();
  }

  Selector::Selector(const Point2f &upper_left_, const Point2f &lower_right_,
                     const Point2f &expanded_upper_left_, const Point2f &expanded_lower_right_,
                     const Color &bg_normal_, const Color &bg_clicked_, const Color &bg_hovered_strayed_,
                     const std::string &font_,
                     const Color &text_normal_, const Color &text_clicked_, const Color &text_hovered_strayed_,
                     const Color &slider_color_)
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif
    : m_expanded(expanded_upper_left_, expanded_lower_right_),
    m_option(0u),
    m_selected(false),
    m_normal_button(*this, upper_left_, lower_right_, bg_normal_, bg_clicked_, bg_hovered_strayed_, font_, text_normal_, text_clicked_, text_hovered_strayed_),
    m_selector_slider(*this,
                      0.5f * (expanded_lower_right_.x - lower_right_.x),
                      slider_color_,
                      slider_color_,
                      make_pair(lower_right_.x, expanded_lower_right_.x),
                      bg_normal_),
    m_bg_normal(bg_normal_),
    m_bg_clicked(bg_clicked_),
    m_bg_hovered_strayed(bg_hovered_strayed_),
    m_font(font_),
    m_text_normal(text_normal_),
    m_text_clicked(text_clicked_),
    m_text_hovered_strayed(text_hovered_strayed_)
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  {
  }

  Selector::~Selector() {
    clear();
  }

  const Selector::Options & Selector::get_options() const {
    return m_options;
  }

  void Selector::add_option(const std::string &option) {
    if(m_options.empty())
      m_option = 0u;
    m_options.push_back(option);

    add_selector_button(option);
  }

  void Selector::remove_option(const std::string &option) {
    Options::iterator it = std::find(m_options.begin(), m_options.end(), option);
    if(it != m_options.end())
      m_options.erase(it);

    build_selector_buttons();
  }

  void Selector::select_option(const std::string &option) {
    Options::iterator it = std::find(m_options.begin(), m_options.end(), option);
    if(it != m_options.end())
      m_option = it - m_options.begin();

    m_normal_button.set_text(m_options[m_option]);
  }

  void Selector::on_mouse_button(const Point2i &pos, const bool &down, const int &button) {
    if(!is_editable())
      return;

    if(!m_selected)
      m_normal_button.on_mouse_button(pos, down, button);
    else {
      const std::pair<Point2f, Point2f> v = visible_region();

      if((pos.x < v.first.x || pos.x > v.second.x || pos.y < v.first.y || pos.y > v.second.y) &&
         button == SDL_BUTTON_LEFT)
      {
        m_selected = false;

        set_busy(false);
        set_layer(0.0f);
      }
      else {
        const Point2i offset_pos(pos.x, int(pos.y + vertical_offset()));
        for(size_t i = view_start; i != view_end; ++i)
          m_selector_buttons[i]->on_mouse_button(offset_pos, down, button);

        if(view_hidden)
          m_selector_slider.on_mouse_button(pos, down, button);
      }
    }
  }

  void Selector::on_mouse_motion(const Point2i &pos) {
    if(!is_editable())
      return;

    if(!m_selected)
      m_normal_button.on_mouse_motion(pos);
    else {
      const Point2i offset_pos(pos.x, int(pos.y + vertical_offset()));
        for(size_t i = view_start; i != view_end; ++i)
          m_selector_buttons[i]->on_mouse_motion(offset_pos);

      if(view_hidden)
        m_selector_slider.on_mouse_motion(pos);
    }
  }

  void Selector::on_accept(const std::string &option) {
    select_option(option);

    set_busy(false);
    set_layer(0.0f);
  }

  void Selector::render_impl() const {
    if(!m_selected)
      m_normal_button.render_impl();
    else {
      Video &vr = get_Video();
      vr.push_world_stack();
      vr.translate_scene(Vector3f(0.0f, -vertical_offset(), 0.0f));

      for(size_t i = view_start; i != view_end; ++i)
        m_selector_buttons[i]->render_impl();

      vr.pop_world_stack();

      if(view_hidden)
        m_selector_slider.render_impl();
    }
  }

  float Selector::button_height() const {
    const Point2f &ul = m_normal_button.get_upper_left();
    const Point2f &lr = m_normal_button.get_lower_right();
    return lr.y - ul.y;
  }

  float Selector::vertical_offset() const {
    return view_offset * button_height();
  }

  void Selector::decide_visible(const size_t &centered) {
    const Point2f &nul = m_normal_button.get_upper_left();
    const Point2f &nlr = m_normal_button.get_lower_right();
    const Point2f &eul = m_expanded.get_upper_left();
    const Point2f &elr = m_expanded.get_lower_right();

    const size_t slots_above = int((nul.y - eul.y) / button_height());
    const size_t slots_below = int((elr.y - nlr.y) / button_height());
    size_t needed_above = centered;
    size_t needed_below = m_options.size() - centered - 1u;

    view_start = centered - min(slots_above, needed_above);
    view_end = centered + min(slots_below, needed_below) + 1u;
    view_offset = centered;

    // Shift up as needed and possible
    while(needed_above < slots_above && needed_below > slots_below) {
      ++needed_above;
      --needed_below;
      ++view_end;
      ++view_offset;
    }

    // Shift down as needed and possible
    while(needed_below < slots_below && needed_above > slots_above) {
      --needed_above;
      ++needed_below;
      --view_start;
      --view_offset;
    }

    /// Setup slider as needed

    view_hidden = 0u;
    if(needed_above > slots_above)
      view_hidden += needed_above - slots_above;
    if(needed_below > slots_below)
      view_hidden += needed_below - slots_below;

    if(view_hidden) {
      const std::pair<Point2f, Point2f> v = visible_region();
      const float r = m_selector_slider.get_slider_radius();
      const float hx = 0.5f * (m_normal_button.get_lower_right().x + m_expanded.get_lower_right().x);

      m_selector_slider.set_end_points(Point2f(hx, v.first.y + r), Point2f(hx, v.second.y - r));
      m_selector_slider.set_range(make_pair(int(view_offset - needed_above + slots_above),
                                            int(view_offset + needed_below - slots_below)));
      m_selector_slider.set_value(int(view_offset));
    }
  }

  std::pair<Point2f, Point2f> Selector::visible_region() const {
    const Point2f &ul = m_normal_button.get_upper_left();
    const Point2f &lr = m_normal_button.get_lower_right();
    const float bh = button_height();
    const float ex = view_hidden ? 2.0f * m_selector_slider.get_slider_radius() : 0.0f;
    return make_pair(Point2f(ul.x, ul.y - bh * (view_offset - view_start)),
                     Point2f(lr.x + ex, ul.y + bh * (view_end - view_offset)));
  }

  void Selector::add_selector_button(const std::string &option) {
    const Point2f &ul = m_normal_button.get_upper_left();
    const Point2f &lr = m_normal_button.get_lower_right();
    const float vertical_offset = m_selector_buttons.size() * (lr.y - ul.y);
    m_selector_buttons.push_back(new Selector_Button(*this, option,
                                                     Point2f(ul.x, ul.y + vertical_offset),
                                                     Point2f(lr.x, lr.y + vertical_offset),
                                                     m_bg_normal, m_bg_clicked, m_bg_hovered_strayed,
                                                     m_font,
                                                     m_text_normal, m_text_clicked, m_text_hovered_strayed));
  }

  void Selector::build_selector_buttons() {
    clear();

    for(Options::const_iterator it = m_options.begin(); it != m_options.end(); ++it)
      add_selector_button(*it);
  }

  void Selector::clear() {
    for(vector<Selector_Button *>::const_iterator it = m_selector_buttons.begin(); it != m_selector_buttons.end(); ++it)
      delete *it;
    m_selector_buttons.clear();
  }

  void Text_Box::on_key(const SDL_keysym &keysym, const bool &down) {
    if(down && m_edit_pos != -1) {
      Game &gr = get_Game();
      const bool mod_alt = gr.get_key_state(SDLK_LALT) || gr.get_key_state(SDLK_RALT);
      const bool mod_ctrl = gr.get_key_state(SDLK_LCTRL) || gr.get_key_state(SDLK_RCTRL);
      const bool mod_meta = gr.get_key_state(SDLK_LMETA) || gr.get_key_state(SDLK_RMETA);
      const bool mod_shift = gr.get_key_state(SDLK_LSHIFT) || gr.get_key_state(SDLK_RSHIFT);
      const bool mod_super = gr.get_key_state(SDLK_LSUPER) || gr.get_key_state(SDLK_RSUPER);
      const bool mod_none = !mod_alt && !mod_ctrl && !mod_meta && !mod_shift && !mod_super;
      const bool mod_ctrl_only = !mod_alt && mod_ctrl && !mod_meta && !mod_shift && !mod_super;
      const bool mod_shift_only = !mod_alt && !mod_ctrl && !mod_meta && mod_shift && !mod_super;

      switch(keysym.sym) {
        case SDLK_BACKSPACE:
          if(mod_none)
          {
            const string &t = get_text();

            if(m_edit_pos > 0) {
              string t0 = t.substr(0, m_edit_pos - 1);
              string t1 = t.substr(m_edit_pos, t.size() - m_edit_pos);

              m_text.set_text(t0 + t1);
              format();
              seek(m_edit_pos - 1);

              on_change();
            }
          }
          break;
        case SDLK_DELETE:
          if(mod_none)
          {
            const string &t = get_text();

            if(m_edit_pos < int(t.size())) {
              string t0 = t.substr(0, m_edit_pos);
              string t1 = t.substr(m_edit_pos + 1, t.size() - m_edit_pos - 1);

              m_text.set_text(t0 + t1);
              format();
              seek(m_edit_pos);

              on_change();
            }
          }
          break;
        case SDLK_HOME:
          if(mod_none || mod_ctrl_only)
          {
            if(mod_ctrl)
              seek_cursor(0);
            else if(m_cursor_index.x) {
              int cursor_pos = get_cursor_pos() - m_cursor_index.x;
              if(m_lines[m_cursor_index.y].endled)
                ++cursor_pos;
              
              seek_cursor(cursor_pos);
            }
          }
          break;
        case SDLK_END:
          if(mod_none || mod_ctrl_only)
          {
            int cursor_pos = mod_ctrl ?
                             get_max_cursor_seek() :
                             get_cursor_pos() - m_cursor_index.x + int(m_lines[m_cursor_index.y].unformatted.size());
            
            seek_cursor(cursor_pos);
          }
          break;
        case SDLK_LEFT:
          if(mod_none)
            seek(m_edit_pos - 1);
          break;
        case SDLK_RIGHT:
          if(mod_none)
            seek(m_edit_pos + 1);
          break;
        case SDLK_UP:
          if(mod_none)
            if(m_cursor_index.y) {
              int count = 0;
              for(int i = 0, iend = m_cursor_index.y - 1; i != iend; ++i)
                count += int(m_lines[i].unformatted.size());
              count += std::min(m_cursor_index.x, int(m_lines[m_cursor_index.y - 1].unformatted.size()));
              
              seek(count);
            }
          break;
        case SDLK_DOWN:
          if(mod_none)
            if(m_cursor_index.y + 1 < int(m_lines.size())) {
              int count = 0;
              for(int i = 0, iend = m_cursor_index.y + 1; i != iend; ++i)
                count += int(m_lines[i].unformatted.size());
              count += std::min(m_cursor_index.x, int(m_lines[m_cursor_index.y + 1].unformatted.size()));
              
              seek(count);
            }
          break;
        default:
          if(mod_none || mod_shift_only)
          {
            const char c = Gamestate_Base::to_char(keysym);
            const string &t = get_text();
            string t0 = t.substr(0, m_edit_pos);
            string t1 = t.substr(m_edit_pos, t.size() - m_edit_pos);
            string next = clean_string(t0 + c + t1);

            if(next.size() != t.size()) {
              m_text.set_text(next);
              format();
              seek(m_edit_pos + 1);

              on_change();
            }
          }
          break;
        }
      }
  }

  void Text_Box::on_mouse_button(const Point2i &pos, const bool &down, const int &button) {
    if(button != SDL_BUTTON_LEFT)
      return;

    m_cursor_pos.x = int(pos.x - get_upper_left().x);
    m_cursor_pos.y = int(pos.y - get_upper_left().y);

    const bool was_focused = m_edit_pos != -1;
    invalidate_edit_pos();

    Widget_Button::on_mouse_button(pos, down, button);

    if(m_edit_pos == -1) {
      if(was_focused)
        on_unfocus();
    }
    else {
      if(!was_focused)
        on_focus();
    }
  }

  void Text_Box::on_accept() {
    int j = 0, jend = int(m_lines.size());
    for(; j != jend && m_cursor_pos.y > m_lines[j].glyph_top; ++j);
    --j;

    /// BEGIN JUSTIFICATION FIX

    const Font &f = get_font();

    float x_pos;
    if(m_justify == ZENI_LEFT)
      x_pos = 0.0f;
    else if(m_justify == ZENI_RIGHT)
      x_pos = get_lower_right().x - get_upper_left().x;
    else
      x_pos = (get_lower_right().x - get_upper_left().x) / 2.0f;

    if(m_justify == ZENI_RIGHT)
      x_pos -= f.get_text_width(m_lines[j].formatted);
    else if(m_justify == ZENI_CENTER)
      x_pos -= f.get_text_width(m_lines[j].formatted) / 2.0f;

    /// END JUSTIFICATION FIX

    int i = 0, iend = int(m_lines[j].unformatted_glyph_sides.size());
    for(; i != iend && m_cursor_pos.x > x_pos + m_lines[j].unformatted_glyph_sides[i]; ++i);
    if(i) // Can be negative if using ZENI_CENTER or ZENI_RIGHT justification
      --i;

    /// HACK: Pleasentness Increase
    if(i + 1 != iend && m_cursor_pos.x > x_pos + (0.2f * m_lines[j].unformatted_glyph_sides[i] +
                                                  0.8f * m_lines[j].unformatted_glyph_sides[i + 1]))
      ++i;

    m_cursor_index.x = i;
    m_cursor_index.y = j;
    m_edit_pos = i;
    for(int k = 0; k < j; ++k)
      m_edit_pos += int(m_lines[k].unformatted.size());

#ifdef _DEBUG
    {
      const int size = int(get_text().size());
      int count = 0;
      for(int k = 0, kend = int(m_lines.size()); k != kend; ++k)
        count += int(m_lines[k].unformatted.size());

      assert(size == count);
    }

    seek(m_edit_pos);
#endif
  }

  void Text_Box::on_focus() {
  }

  void Text_Box::on_unfocus() {
  }

  void Text_Box::on_change() {
  }

  void Text_Box::render_impl() const {
    Video &vr = get_Video();

    m_bg.render_impl();

    const Font &f = get_font();
    const Color &c = m_text.get_color();

    float x_pos;
    if(m_justify == ZENI_LEFT)
      x_pos = get_upper_left().x;
    else if(m_justify == ZENI_RIGHT)
      x_pos = get_lower_right().x;
    else
      x_pos = (get_upper_left().x + get_lower_right().x) / 2.0f;

    const float &y_offset = get_upper_left().y;
    for(int i = 0, iend = int(m_lines.size()); i != iend; ++i)
      f.render_text(m_lines[i].formatted, Point2f(x_pos, y_offset + m_lines[i].glyph_top), c, m_justify);

    if(m_cursor_index.x != -1 && m_cursor_index.y != -1
      && !((get_Timer().get_time().get_ticks_since(m_last_seek) / SDL_DEFAULT_REPEAT_DELAY) & 1) // HACK: render every other second
       )
    {
      Point2f p0(x_pos + m_lines[m_cursor_index.y].unformatted_glyph_sides[m_cursor_index.x],
                 get_upper_left().y + m_lines[m_cursor_index.y].glyph_top);
      if(m_justify == ZENI_RIGHT)
        p0.x -= f.get_text_width(m_lines[m_cursor_index.y].formatted);
      else if(m_justify == ZENI_CENTER)
        p0.x -= f.get_text_width(m_lines[m_cursor_index.y].formatted) / 2.0f;

      const Point2f p1(p0.x, p0.y + f.get_text_height());
      const float epsilon = f.get_text_height() * ZENI_TEXT_CURSOR_WIDTH / 2.0f;

      const Vertex2f_Color v0(Point2f(p0.x - epsilon, p0.y), c);
      const Vertex2f_Color v1(Point2f(p1.x - epsilon, p1.y), c);
      const Vertex2f_Color v2(Point2f(p1.x + epsilon, p1.y), c);
      const Vertex2f_Color v3(Point2f(p0.x + epsilon, p0.y), c);

      const Quadrilateral<Vertex2f_Color> visible_cursor(v0, v1, v2, v3);
      vr.render(visible_cursor);
    }
  }

  void Text_Box::format() {
    m_lines.clear();
    m_lines.push_back(Line());

    const std::string t = get_text();
    const Font &f = get_font();
    const float mll = max_line_width();

    if(t.empty())
      return;

    list<Word> words;
    Word next_word;

    for(int i = 0, iend = int(t.size()); i != iend; ++i) {
      const Word::Type type = isspace(t[i]) ? Word::SPACE : Word::WORD;

      if(next_word.type) {
        if(next_word.type != type || t[i] == '\n') {
          words.push_back(next_word);
          next_word = Word(type);
        }
      }
      else
        next_word.type = type;

      next_word.unformatted += t[i];

      const float next_width = get_text_width(f, next_word.unformatted);
      next_word.unformatted_glyph_sides.push_back(next_width);
      if(type != Word::SPACE && next_width > mll)
        next_word.splittable = true;
    }
    if(next_word.type != Word::NONSENSE)
      words.push_back(next_word);

    for(list<Word>::iterator it = words.begin(); it != words.end(); ++it)
      append_word(*it);

    float glyph_top = 0.0f;
    for(vector<Line>::iterator it = m_lines.begin(); it != m_lines.end(); ++it) {
      it->formatted = untablinebreak(it->unformatted);
      if(it->fpsplit)
        it->formatted += "-";
      it->glyph_top = glyph_top;
      if(!it->unformatted.empty() && it->unformatted[0] == '\n')
        it->endled = true;

      glyph_top += f.get_text_height();
    }
  }

  void Text_Box::append_word(const Word &word) {
    const Font &f = get_font();
    const float mll = max_line_width();

    if(!word.unformatted.empty() && word.unformatted[0] == '\n')
      m_lines.push_back(Line());

    Line &l = *m_lines.rbegin();
    float next_sum = get_text_width(f, l.unformatted + word.unformatted);

    if(word.type != Word::SPACE && next_sum > mll && !word.fpsplit) {
      if(word.splittable) {
        int i = 0, iend = int(word.unformatted.size());
        for(; i != iend && get_text_width(f, l.unformatted + word.unformatted.substr(0, i) + "-") < mll; ++i);
        if(!l.unformatted.empty())
          --i;
        if(i > 0) {
          {
            Word first_word(word.type);
            first_word.unformatted = word.unformatted.substr(0, i);
            for(int j = 1, jend = int(first_word.unformatted.size()); j <= jend; ++j)
              first_word.unformatted_glyph_sides.push_back(get_text_width(f, first_word.unformatted.substr(0, j)));
            first_word.fpsplit = true;
            append_word(first_word);
          }

          {
            Word second_word(word.type);
            second_word.unformatted = word.unformatted.substr(i, word.unformatted.size() - i);
            for(int j = 1, jend = int(second_word.unformatted.size()); j <= jend; ++j)
              second_word.unformatted_glyph_sides.push_back(get_text_width(f, second_word.unformatted.substr(0, j)));
            second_word.splittable = get_text_width(f, second_word.unformatted) > mll;
            append_word(second_word);
          }
        }
        else {
          Word only_word(word);
          only_word.fpsplit = l.unformatted.empty();
          m_lines.push_back(Line());
          append_word(only_word);
          return;
        }
      }
      else {
        m_lines.push_back(Line());
        append_word(word);
      }
    }
    else {
      for(int i = 0, iend = int(word.unformatted.size()); i != iend; ++i) {
        l.unformatted += word.unformatted[i];
        l.unformatted_glyph_sides.push_back(get_text_width(f, l.unformatted));
      }

      l.fpsplit = word.fpsplit;
    }
  }

  void Text_Box::set_editable(const bool &editable_) {
    Widget::set_editable(editable_);
    format();
    invalidate_edit_pos();
  }

  const int & Text_Box::get_edit_pos() const {
    return m_edit_pos;
  }

  int Text_Box::get_cursor_pos() const {
    int count = m_cursor_index.x;
    for(int j = 0, jend = m_cursor_index.y; j < jend; ++j)
      count += int(m_lines[j].unformatted_glyph_sides.size());
    return count;
  }

  int Text_Box::get_max_seek() const {
    return int(get_text().size());
  }

  int Text_Box::get_max_cursor_seek() const {
    int count = -1;
    for(vector<Line>::const_iterator it = m_lines.begin(); it != m_lines.end(); ++it)
      count += int(it->unformatted_glyph_sides.size());
    return count;
  }

  void Text_Box::seek(const int &edit_pos) {
    if(!is_editable())
      return;

    const string t = get_text();

    if(edit_pos < 0 ||
      edit_pos > int(t.size()))
      return;

    m_edit_pos = edit_pos;
    int count = 0, j = 0, jend = int(m_lines.size()), i = -1, iend;
    for(; j != jend; ++j) {
      iend = int(m_lines[j].unformatted.size());

      if(count + iend < edit_pos) {
        count += iend;
        continue;
      }

      i = edit_pos - count;
      break;
    }

    m_cursor_index.x = i;
    m_cursor_index.y = j;

    m_last_seek = get_Timer().get_time();
  }

  void Text_Box::seek_cursor(const int &cursor_pos) {
    if(!is_editable() || cursor_pos < 0)
      return;

    int edit_count = 0, count = 0, j = 0, jend = int(m_lines.size()), i = -1, iend;
    for(; j != jend; ++j) {
      iend = int(m_lines[j].unformatted_glyph_sides.size());

      if(count + iend <= cursor_pos) {
        count += iend;
        edit_count += int(m_lines[j].unformatted.size());
        continue;
      }

      i = cursor_pos - count;
      edit_count += i;
      break;
    }

    if(i != -1) {
      m_edit_pos = edit_count;
      m_cursor_index.x = i;
      m_cursor_index.y = j;
    }

    m_last_seek = get_Timer().get_time();
  }
  
  void Text_Box::set_focus(const bool &value) {
    if(!is_editable())
      return;

    if(value) {
      const bool was_focused = m_edit_pos != -1;
      seek(get_max_seek());
      if(!was_focused)
        on_focus();
    }
    else {
      const bool was_focused = m_edit_pos != -1;
      invalidate_edit_pos();
      if(was_focused)
        on_unfocus();
    }
  }

  string Text_Box::clean_string(const string &unclean_string) const {
    string cleaned;
    for(string::const_iterator it = unclean_string.begin(); it != unclean_string.end(); ++it)
      if(*it >= 0x20 ||
         *it == '\n' || *it == '\t')
        cleaned += *it;
    return cleaned;
  }

  string Text_Box::untablinebreak(const string &tabbed_text) const {
    string untabbed_text;

    for(string::const_iterator it = tabbed_text.begin(); it != tabbed_text.end(); ++it) {
      if(*it != '\t') {
        if(*it > 0x1F)
          untabbed_text += *it;
      }
      else
        for(int i = m_tab_spaces; i; --i)
          untabbed_text += ' ';
    }

    return untabbed_text;
  }

  float Text_Box::get_text_width(const Font &font, const string &text) {
    const string untabbed_text = untablinebreak(text);

    if(font.get_text_width(" "))
      return font.get_text_width(untabbed_text);

    string fake_text;
    for(string::const_iterator it = untabbed_text.begin(); it != untabbed_text.end(); ++it)
      if(*it != ' ')
        fake_text += *it;
      else
        fake_text += '.';

    return font.get_text_width(fake_text);
  }

  float Text_Box::max_line_width() const {
    return get_lower_right().x - get_upper_left().x;
  }

  void Widget_Input_Repeater::on_key(const SDL_keysym &keysym, const bool &down) {
    if(!is_editable())
      return;

    m_keysym = keysym;
    m_down = down;

    m_last_repeated = get_Timer().get_time();
    m_active = true;
    m_delay_finished = false;

    m_widget->on_key(m_keysym, m_down);

    set_busy(m_widget->is_busy());
  }

  void Widget_Input_Repeater::on_mouse_button(const Point2i &pos, const bool &down, const int &button) {
    if(!is_editable())
      return;

    m_active = false;

    m_widget->on_mouse_button(pos, down, button);

    set_busy(m_widget->is_busy());
  }

  void Widget_Input_Repeater::on_mouse_motion(const Point2i &pos) {
    if(!is_editable())
      return;

    m_widget->on_mouse_motion(pos);

    set_busy(m_widget->is_busy());
  }

  void Widget_Input_Repeater::render_impl() const {
    m_widget->render();
  }

  static bool widget_layer_less(const Widget * const &lhs, const Widget * const &rhs) {
    return lhs->get_layer() < rhs->get_layer();
  }

  void Widgets::on_key(const SDL_keysym &keysym, const bool &down) {
    if(!is_editable())
      return;

    if(m_busy_one) {
      m_busy_one->on_key(keysym, down);

      if(!m_busy_one->is_busy()) {
        m_busy_one = 0;
        set_busy(false);
      }
    }
    else {
      std::sort(m_widgets.begin(), m_widgets.end(), &widget_layer_less);

      for(std::vector<Widget *>::iterator it = m_widgets.begin(); it != m_widgets.end(); ++it) {
        (*it)->on_key(keysym, down);

        if((*it)->is_busy()) {
          m_busy_one = *it;
          set_busy(true);
          break;
        }
      }
    }
  }

  void Widgets::on_mouse_button(const Point2i &pos, const bool &down, const int &button) {
    if(!is_editable())
      return;

    if(m_busy_one) {
      m_busy_one->on_mouse_button(pos, down, button);

      if(!m_busy_one->is_busy()) {
        m_busy_one = 0;
        set_busy(false);
      }
    }
    else {
      std::sort(m_widgets.begin(), m_widgets.end(), &widget_layer_less);

      for(std::vector<Widget *>::iterator it = m_widgets.begin(); it != m_widgets.end(); ++it) {
        (*it)->on_mouse_button(pos, down, button);

        if((*it)->is_busy()) {
          m_busy_one = *it;
          set_busy(true);
          break;
        }
      }
    }
  }
    
  void Widgets::on_mouse_motion(const Point2i &pos) {
    if(!is_editable())
      return;

    if(m_busy_one) {
      m_busy_one->on_mouse_motion(pos);

      if(!m_busy_one->is_busy()) {
        m_busy_one = 0;
        set_busy(false);
      }
    }
    else {
      std::sort(m_widgets.begin(), m_widgets.end(), &widget_layer_less);

      for(std::vector<Widget *>::iterator it = m_widgets.begin(); it != m_widgets.end(); ++it) {
        (*it)->on_mouse_motion(pos);

        if((*it)->is_busy()) {
          m_busy_one = *it;
          set_busy(true);
          break;
        }
      }
    }
  }

  void Widgets::render_impl() const {
    std::sort(m_widgets.begin(), m_widgets.end(), &widget_layer_less);

    for(std::vector<Widget *>::const_reverse_iterator it = m_widgets.rbegin(), iend = m_widgets.rend(); it != iend; ++it)
      (*it)->render();
  }

}
