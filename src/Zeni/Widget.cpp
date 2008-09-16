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

using std::string;
using std::vector;
using std::list;

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

  void Check_Box::on_accept() {
    m_checked = !m_checked;
  }
  
  void Check_Box::render() const {
    Video &vr = Video::get_reference();

    Vertex2f_Color ul(get_upper_left(), m_border_color);
    Vertex2f_Color ll(get_lower_left(), m_border_color);
    Vertex2f_Color lr(get_lower_right(), m_border_color);
    Vertex2f_Color ur(get_upper_right(), m_border_color);

    Line_Segment<Vertex2f_Color> line_seg(ul, ll);
    vr.render(line_seg);

    line_seg.set_vertex(0, lr);
    vr.render(line_seg);

    line_seg.set_vertex(1, ur);
    vr.render(line_seg);

    line_seg.set_vertex(0, ul);
    vr.render(line_seg);

    if(m_checked) {
      ul.set_color(m_check_color);
      ll.set_color(m_check_color);
      lr.set_color(m_check_color);
      ur.set_color(m_check_color);

      line_seg.set_vertex(0, ul);
      line_seg.set_vertex(1, lr);
      vr.render(line_seg);

      line_seg.set_vertex(0, ll);
      line_seg.set_vertex(1, ur);
      vr.render(line_seg);
    }
  }

  void Radio_Button::on_accept() {
    if(is_checked())
      return;

    Check_Box::on_accept();

    m_radio_button_set->accept(*this);
  }

  void Radio_Button_Set::on_mouse_button(const Point2i &pos, const bool &down) {
    for(std::set<Radio_Button *>::iterator it = m_radio_buttons.begin(); it != m_radio_buttons.end(); ++it)
      (*it)->on_mouse_button(pos, down);
  }
    
  void Radio_Button_Set::on_mouse_motion(const Point2i &pos) {
    for(std::set<Radio_Button *>::iterator it = m_radio_buttons.begin(); it != m_radio_buttons.end(); ++it)
      (*it)->on_mouse_motion(pos);
  }

  void Radio_Button_Set::render() const {
    for(std::set<Radio_Button *>::const_iterator it = m_radio_buttons.begin(); it != m_radio_buttons.end(); ++it)
      (*it)->render();
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

  void Slider::on_mouse_button(const Zeni::Point2i &pos, const bool &down) {
    if(down) {
      const Point3f mouse_pos(float(pos.x), float(pos.y), 0.0f);

      const std::pair<float, float> test = m_line_segment.nearest_point(mouse_pos);
      if(test.first < m_slider_radius) {
        m_down = true;
        m_slider_position = test.second;
        regenerate_slider_r();
      }
      else
        m_down = false;
    }
    else
      m_down = false;
  }

  void Slider::on_mouse_motion(const Zeni::Point2i &pos) {
    if(m_down) {
      const Point3f mouse_pos(float(pos.x), float(pos.y), 0.0f);

      const std::pair<float, float> test = m_line_segment.nearest_point(mouse_pos);
      if(test.first < m_slider_radius) {
        m_slider_position = test.second;
        regenerate_slider_r();
      }
    }
  }

  void Slider::render() const {
    Video &vr = Video::get_reference();

    vr.render(m_line_segment_r);
    vr.render(m_slider_r);
  }

  Text_Box::Text_Box(const Point2f &upper_left_, const Point2f &lower_right_,
                     const Color &bg_color_,
                     const std::string &font_name_, const std::string &text_, const Color &text_color_,
                     const bool &editable_, const JUSTIFY &justify_, const int &tab_spaces_)
  : Widget_Button(upper_left_, lower_right_),
    m_bg(upper_left_, lower_right_, bg_color_),
    m_text(font_name_, clean_string(text_), text_color_),
    m_editable(editable_),
    m_edit_pos(-1),
    m_last_seek(0),
    m_justify(justify_),
    m_tab_spaces(tab_spaces_),
    m_cursor_index(-1, -1)
  {
    format();
  }

  void Text_Box::on_key(const SDL_keysym &keysym, const bool &down) {
    if(down && m_edit_pos != -1)
      switch(keysym.sym) {
        case SDLK_BACKSPACE:
          {
            const string &t = get_text();

            if(m_edit_pos > 0) {
              string t0 = t.substr(0, m_edit_pos - 1);
              string t1 = t.substr(m_edit_pos, t.size() - m_edit_pos);

              m_text.set_text(t0 + t1);
              format();
              seek(m_edit_pos - 1);
            }
          }
          break;
        case SDLK_DELETE:
          {
            const string &t = get_text();

            if(m_edit_pos < int(t.size())) {
              string t0 = t.substr(0, m_edit_pos);
              string t1 = t.substr(m_edit_pos + 1, t.size() - m_edit_pos - 1);

              m_text.set_text(t0 + t1);
              format();
              seek(m_edit_pos);
            }
          }
          break;
        case SDLK_HOME:
          if(m_cursor_index.x) {
            int cursor_pos = get_cursor_pos() - m_cursor_index.x;
            if(m_lines[m_cursor_index.y].endled)
              ++cursor_pos;
            
            seek_cursor(cursor_pos);
          }
          break;
        case SDLK_END:
          {
            int cursor_pos = get_cursor_pos() - m_cursor_index.x + int(m_lines[m_cursor_index.y].unformatted.size());
            
            seek_cursor(cursor_pos);
          }
          break;
        case SDLK_LEFT:
          seek(m_edit_pos - 1);
          break;
        case SDLK_RIGHT:
          seek(m_edit_pos + 1);
          break;
        case SDLK_UP:
          if(m_cursor_index.y) {
            int count = 0;
            for(int i = 0, iend = m_cursor_index.y - 1; i != iend; ++i)
              count += int(m_lines[i].unformatted.size());
            count += std::min(m_cursor_index.x, int(m_lines[m_cursor_index.y - 1].unformatted.size()));
            
            seek(count);
          }
          break;
        case SDLK_DOWN:
          if(m_cursor_index.y + 1 < int(m_lines.size())) {
            int count = 0;
            for(int i = 0, iend = m_cursor_index.y + 1; i != iend; ++i)
              count += int(m_lines[i].unformatted.size());
            count += std::min(m_cursor_index.x, int(m_lines[m_cursor_index.y + 1].unformatted.size()));
            
            seek(count);
          }
          break;
        default:
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
            }
          }
          break;
      }
  }

  void Text_Box::on_mouse_button(const Point2i &pos, const bool &down) {
    m_cursor_pos.x = int(pos.x - get_upper_left().x);
    m_cursor_pos.y = int(pos.y - get_upper_left().y);

    invalidate_edit_pos();

    Widget_Button::on_mouse_button(pos, down);
  }

  void Text_Box::on_accept() {
    if(!m_editable)
      return;

    int j = 0, jend = int(m_lines.size());
    for(; j != jend && m_cursor_pos.y > m_lines[j].glyph_top; ++j);
    --j;

    int i = 0, iend = int(m_lines[j].unformatted_glyph_sides.size());
    for(; i != iend && m_cursor_pos.x > m_lines[j].unformatted_glyph_sides[i]; ++i);
    --i;

    /// HACK: Pleasentness Increase
    if(i + 1 != iend && m_cursor_pos.x > (0.2f * m_lines[j].unformatted_glyph_sides[i] +
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

  void Text_Box::render() const {
    Video &vr = Video::get_reference();

    m_bg.render();

    const Font &f = get_font();
    const Color &c = m_text.get_color();

    float x_pos;
    if(m_justify == ZENI_LEFT)
      x_pos = get_upper_left().x;
    else if(m_justify == ZENI_RIGHT)
      x_pos = get_lower_right().x;
    else
      x_pos = (get_upper_left().x + get_lower_right().x) / 2;

    const float &y_offset = get_upper_left().y;
    for(int i = 0, iend = int(m_lines.size()); i != iend; ++i)
      f.render_text(m_lines[i].formatted, Point2f(x_pos, y_offset + m_lines[i].glyph_top), c, m_justify);

    if(m_cursor_index.x != -1 && m_cursor_index.y != -1
      && !((Timer::get_reference().get_time().get_ticks_since(m_last_seek) / SDL_DEFAULT_REPEAT_DELAY) & 1) // render every other second
       )
    {
      const Point2f p0(get_upper_left().x + m_lines[m_cursor_index.y].unformatted_glyph_sides[m_cursor_index.x],
                       get_upper_left().y + m_lines[m_cursor_index.y].glyph_top);
      const Point2f p1(p0.x,
                       p0.y + f.get_text_height());

      Vertex2f_Color v0(p0, c);
      Vertex2f_Color v1(p1, c);

      const Line_Segment<Vertex2f_Color> visible_cursor(v0, v1);
      vr.render(visible_cursor);
    }
  }

  void Text_Box::format() {
    m_lines.clear();
    m_lines.push_back(Line());

    const std::string t = get_text();
    const Font &f = get_font();
    const int &mll = max_line_width();

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

      const int next_width = get_text_width(f, next_word.unformatted);
      next_word.unformatted_glyph_sides.push_back(next_width);
      if(type != Word::SPACE && next_width > mll)
        next_word.splittable = true;
    }
    if(next_word.type != Word::NONSENSE)
      words.push_back(next_word);

    for(list<Word>::iterator it = words.begin(); it != words.end(); ++it)
      append_word(*it);

    int glyph_top = 0;
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
    const int &mll = max_line_width();

    if(!word.unformatted.empty() && word.unformatted[0] == '\n')
      m_lines.push_back(Line());

    Line &l = *m_lines.rbegin();
    int next_sum = get_text_width(f, l.unformatted + word.unformatted);

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
    if(!m_editable)
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

    m_last_seek = Timer::get_reference().get_time();
  }

  void Text_Box::seek_cursor(const int &cursor_pos) {
    if(!m_editable || cursor_pos < 0)
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

    m_last_seek = Timer::get_reference().get_time();
  }
  
  void Text_Box::set_focus(const bool &value) {
    if(value)
      seek(get_max_seek());
    else
      invalidate_edit_pos();
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

  int Text_Box::get_text_width(const Font &font, const string &text) {
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

  int Text_Box::max_line_width() const {
    return int(get_lower_right().x - get_upper_left().x);
  }

  void Widget_Input_Repeater::on_key(const SDL_keysym &keysym, const bool &down) {
    m_keysym = keysym;
    m_down = down;

    m_last_repeated = Timer::get_reference().get_time();
    m_active = true;
    m_delay_finished = false;

    m_widget->on_key(m_keysym, m_down);
  }

  void Widget_Input_Repeater::on_mouse_button(const Point2i &pos, const bool &down) {
    m_active = false;

    m_widget->on_mouse_button(pos, down);
  }

  void Widget_Input_Repeater::on_mouse_motion(const Point2i &pos) {
    m_widget->on_mouse_motion(pos);
  }

  void Widget_Input_Repeater::render() const {
  }

  void Widgets::on_key(const SDL_keysym &keysym, const bool &down) {
    for(std::set<Widget *>::iterator it = m_widgets.begin(); it != m_widgets.end(); ++it)
      (*it)->on_key(keysym, down);
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
