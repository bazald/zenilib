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

/**
 * \class Zeni::Widget
 *
 * \ingroup Zenilib
 *
 * \brief The Widget base class
 *
 * The basic interface for all Widgets is outlined in this class.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Widget_Positioned
 *
 * \ingroup Zenilib
 *
 * \brief Widget with a clear Rectangular bound
 *
 * Usually Widget_Rectangle (inheriting from this) is a fine choice.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Widget_Text
 *
 * \ingroup Zenilib
 *
 * \brief Widget Text rendering
 *
 * Any Widget using Text should incorporate this object.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Widget_Rectangle
 *
 * \ingroup Zenilib
 *
 * \brief Rectangle positioning
 *
 * Any Widget occupying a rectangle should inherit from this class and 
 * offer its interface.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Widget_Rectangle_Color
 *
 * \ingroup Zenilib
 *
 * \brief A Colored Rectangle Widget
 *
 * Any Widget using a Colored Rectangle should incorporate this object.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Widget_Rectangle_Texture
 *
 * \ingroup Zenilib
 *
 * \brief A Textured Rectangle Widget
 *
 * Any Widget using a Textured Rectangle should incorporate this object.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Widget_Button
 *
 * \ingroup Zenilib
 *
 * \brief A Button Widget
 *
 * Any simple click-button should inherit from this class.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Text_Button
 *
 * \ingroup Zenilib
 *
 * \brief A Text Button Widget
 *
 * Any simple click-button with text overlaid should inherit from this class.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Check_Box
 *
 * \ingroup Zenilib
 *
 * \brief A Check Box
 *
 * Basically a button that can be toggled on and off.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Radio_Button
 *
 * \ingroup Zenilib
 *
 * \brief A Radio Button
 *
 * Basically a Check_Box that turns off other Check_Boxes.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Radio_Button_Set
 *
 * \ingroup Zenilib
 *
 * \brief A Set of Radio Buttons
 *
 * A set of all Radio_Buttons that are tied together.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Slider
 *
 * \ingroup Zenilib
 *
 * \brief A Slider
 *
 * A line segment that has a slider overlaid, so a floating point 
 * value from [0.0f, 1.0f] can be extracted.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Text_Box
 *
 * \ingroup Zenilib
 *
 * \brief A Text_Box
 *
 * An (optionally) editable text box.  It can behave as a simple text 
 * editor.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Widget_Input_Repeater
 *
 * \ingroup Zenilib
 *
 * \brief An Input Repeater Class
 *
 * This will repeat keypresses for anything looking for text-editor-like 
 * keyboard behavior.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Widgets
 *
 * \ingroup Zenilib
 *
 * \brief A Widget Set
 *
 * This allows you to batch send events and render commands to many 
 * Widgets at once.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifdef ZENI_INLINES
#include <Zeni/Widget.hxx>
#endif

#ifndef ZENI_WIDGET_H
#define ZENI_WIDGET_H

#include <Zeni/Collision.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Font.h>
#include <Zeni/Line_Segment.h>
#include <Zeni/Projector.h>
#include <Zeni/Texture.h>
#include <Zeni/Timer.h>
#include <Zeni/Quadrilateral.h>
#include <Zeni/Vertex2f.h>
#include <Zeni/Video.h>

#include <string>
#include <vector>
#include <set>

#include <Zeni/Global.h>

namespace Zeni {

  class Widget {
    Widget(const Widget &);
    Widget & operator=(const Widget &);

  public:
    Widget() {}
    virtual ~Widget() {}

    inline void on_event(const SDL_KeyboardEvent &event);
    inline void on_event(const SDL_MouseButtonEvent &event);
    inline void on_event(const SDL_MouseButtonEvent &event, const Projector2D &projector);
    inline void on_event(const SDL_MouseMotionEvent &event);
    inline void on_event(const SDL_MouseMotionEvent &event, const Projector2D &projector);

    virtual void on_key(const SDL_keysym & /*keysym*/, const bool & /*down*/) {}
    virtual void on_mouse_button(const Point2i &pos, const bool &down) = 0;
    virtual void on_mouse_motion(const Point2i &pos) = 0;

    virtual void render() const = 0;
  };

  class Widget_Positioned {
  public:
    virtual ~Widget_Positioned() {}

    virtual const Point2f & get_upper_left() const = 0;
    inline Point2f get_lower_left() const;
    virtual const Point2f & get_lower_right() const = 0;
    inline Point2f get_upper_right() const;
    inline Point2f get_center() const;
  };

  class Widget_Text {
  public:
    inline Widget_Text(const std::string &font_name_, const std::string &text_, const Color &color_);

    inline const std::string & get_font_name() const;
    inline const std::string & get_text() const;
    inline const Color & get_color() const;

    inline void set_font_name(const std::string &font_name_);
    inline void set_text(const std::string &text_);
    inline void set_color(const Color &color_);

    inline void render(const Point2f &center) const;

  private:
    std::string m_font_name;
    std::string m_text;
    Color m_color;
  };

  class Widget_Rectangle : public Widget_Positioned {
  public:
    inline Widget_Rectangle(const Point2f &upper_left_, const Point2f &lower_right_);
    
    virtual const Point2f & get_upper_left() const;
    virtual const Point2f & get_lower_right() const;

    virtual void render() const {}

  private:
    Point2f m_upper_left;
    Point2f m_lower_right;
  };

  class Widget_Rectangle_Color : public Widget_Rectangle {
  public:
    inline Widget_Rectangle_Color(const Point2f &upper_left_, const Point2f &lower_right_,
                                  const Color &color_);
    inline ~Widget_Rectangle_Color();
    
    inline Widget_Rectangle_Color(const Widget_Rectangle_Color &rhs);
    inline Widget_Rectangle_Color & operator=(const Widget_Rectangle_Color &rhs);

    inline const Color & get_color() const;
    inline void set_color(const Color &color_);

    virtual void render() const;

  private:
    inline void generate_quadrilateral();

    Color m_color;
    Quadrilateral<Vertex2f_Color> * m_quad;
  };

  class Widget_Rectangle_Texture : public Widget_Rectangle {
  public:
    inline Widget_Rectangle_Texture(const Point2f &upper_left_, const Point2f &lower_right_,
                                    const std::string &texture_name_);
    inline ~Widget_Rectangle_Texture();

    inline Widget_Rectangle_Texture(const Widget_Rectangle_Texture &rhs);
    inline Widget_Rectangle_Texture & operator=(const Widget_Rectangle_Texture &rhs);

    inline const std::string get_texture_name() const;
    inline void set_texture_name(const std::string &texture_name_);

    virtual void render() const;

  private:
    inline void generate_quadrilateral();

    std::string m_texture_name;
    Quadrilateral<Vertex2f_Texture> * m_quad;
  };

  class Widget_Button : public Widget, public Widget_Positioned {
  public:
    inline Widget_Button(const Point2f &upper_left_, const Point2f &lower_right_);

    virtual const Point2f & get_upper_left() const;
    virtual const Point2f & get_lower_right() const;

    virtual void on_mouse_button(const Point2i &pos, const bool &down);
    virtual void on_mouse_motion(const Point2i &pos);

    // Called when the cursor passes over the button
    virtual void on_hover() {}
    // Called when the cursor leaves the button without clicking
    virtual void on_unhover() {}

    // Called when the cursor downclicks the button
    virtual void on_click() {}
    // Called when the cursor is dragged off the button after being clicked
    virtual void on_stray() {}
    // Called when the cursor is dragged back onto the button without releasing the clicker
    virtual void on_unstray() {}

    // Called when the cursor is released inside the button
    virtual void on_accept() {}
    // Called when the cursor is released outside the button
    virtual void on_reject() {}

  private:
    inline bool is_inside(const Point2i &pos) const;

    Point2f m_upper_left;
    Point2f m_lower_right;

    bool m_clicked_inside;
    bool m_clicked_outside;
    bool m_transient;
  };

  class Text_Button : public Widget_Button {
  public:
    inline Text_Button(const Point2f &upper_left_, const Point2f &lower_right_,
                       const Color &bg_color_,
                       const std::string &font_name_, const std::string &text_, const Color &text_color_);

    virtual void render() const;

  protected:
    Widget_Rectangle_Color m_bg;
    Widget_Text m_text;
  };

  class Check_Box : public Widget_Button {
  public:
    inline Check_Box(const Point2f &upper_left_, const Point2f &lower_right_,
                     const Color &border_color_, const Color &check_color_,
                     const bool &checked_ = false, const bool &toggleable_ = true);

    inline const Color & get_border_color() const;
    inline const Color & get_check_color() const;
    inline const bool & is_checked() const;

    inline void set_border_color(const Color &border_color_);
    inline void set_check_color(const Color &check_color_);
    inline const void set_checked(const bool &checked_);
    
    virtual void on_accept();

    virtual void render() const;

  protected:
    Color m_border_color;
    Color m_check_color;
    bool m_checked;
    bool m_toggleable;
  };

  class Radio_Button;
  class Radio_Button_Set : public Widget {
    friend class Radio_Button;

  public:
    virtual void on_mouse_button(const Point2i &pos, const bool &down);
    virtual void on_mouse_motion(const Point2i &pos);

    inline void accept(Radio_Button &radio_button);
    inline void clear();

    virtual void render() const;

  private:
    inline void lend_Radio_Button(Radio_Button &radio_button);
    inline void unlend_Radio_Button(Radio_Button &radio_button);

    std::set<Radio_Button *> m_radio_buttons;
  };

  class Radio_Button : public Check_Box {
    friend class Radio_Button_Set;

  public:
    inline Radio_Button(Radio_Button_Set &radio_button_set_,
                        const Point2f &upper_left_, const Point2f &lower_right_,
                        const Color &border_color_, const Color &check_color_,
                        const bool &checked_ = false, const bool &toggleable_ = true);
    inline ~Radio_Button();

    virtual void on_accept();

  private:
    Radio_Button_Set * m_radio_button_set;
  };
  
  class Slider : public Widget {
  public:
    Slider(const Point2f &end_point_a_, const Point2f &end_point_b_,
           const float &slider_radius_,
           const Color &line_color_,
           const Color &slider_color_,
           const float &slider_position_ = ZENI_DEFAULT_SLIDER_POSITION);

    inline Point2f get_end_point_a() const;
    inline Point2f get_end_point_b() const;
    inline const float & get_slider_radius() const;
    inline const Color & get_line_color() const;
    inline const Color & get_slider_color() const;
    inline const float & get_slider_position() const;

    inline void set_line_color(const Color &line_color_);
    inline void set_slider_color(const Color &slider_color_);
    inline void set_slider_position(const float &slider_position_);

    virtual void on_mouse_button(const Zeni::Point2i &pos, const bool &down);

    virtual void on_mouse_motion(const Zeni::Point2i &pos);

    virtual void render() const;

  private:
    inline void regenerate_slider_r();

    Zeni_Collision::Line_Segment m_line_segment;
    Zeni::Color m_line_color;
    Zeni::Line_Segment<Vertex2f_Color> m_line_segment_r;

    float m_slider_radius;
    Zeni::Color m_slider_color;
    Zeni::Line_Segment<Vertex2f_Color> m_slider_r;

    float m_slider_position;
    bool m_down;
  };
  
  class Text_Box : public Widget_Button {
  public:
    Text_Box(const Point2f &upper_left_, const Point2f &lower_right_,
             const Color &bg_color_,
             const std::string &font_name_, const std::string &text_, const Color &text_color_,
             const bool &editable_ = ZENI_DEFAULT_TEXTBOX_EDITABLE,
             const JUSTIFY &justify_ = ZENI_DEFAULT_JUSTIFY,
             const int &tab_spaces_ = ZENI_DEFAULT_TAB_SPACES);

    virtual void on_key(const SDL_keysym &keysym, const bool &down);

    virtual void on_mouse_button(const Point2i &pos, const bool &down);
    virtual void on_accept();

    inline const Color & get_bg_color() const;
    inline const std::string & get_font_name() const;
    inline const Font & get_font() const;
    inline const std::string & get_text() const;
    inline const Color & get_text_color() const;
    inline const bool & is_editable() const;
    inline const JUSTIFY & get_justify() const;
    inline int get_num_lines() const;
    inline int get_max_lines() const;

    inline void set_bg_color(const Color &bg_color_);
    inline void set_font_name(const std::string &font_name_);
    inline void set_text(const std::string &text_);
    inline void set_text_color(const Color &text_color_);
    inline void set_editable(const bool &editable_);
    inline void set_justify(const JUSTIFY &justify_);
    inline void erase_lines(const int &before_index, const int &after_and_including_index);

    const int & get_edit_pos() const;
    int get_cursor_pos() const;
    int get_max_seek() const;
    int get_max_cursor_seek() const;

    void seek(const int &edit_pos);
    void seek_cursor(const int &cursor_pos);
    void set_focus(const bool &value);

    virtual void render() const;

  private:
    struct Word {
      enum Type {NONSENSE = 0x0, WORD = 0x1, SPACE = 0x2};

      Word(const Type &type_ = NONSENSE) : unformatted_glyph_sides(1, 0), type(type_), splittable(false), fpsplit(false) {}

      std::string unformatted;
      std::vector<float> unformatted_glyph_sides;
      Type type;
      bool splittable;
      bool fpsplit; // indicates it has been split already and a '-' should be appended
    };

    struct Line : public Word {
      Line() : glyph_top(0), endled(false) {}

      std::string formatted;
      float glyph_top;
      bool endled;
    };

    void format();
    void append_word(const Word &word);
    
    Widget_Rectangle_Color m_bg;
    Widget_Text m_text;

    std::string clean_string(const std::string &unclean_string) const;
    std::string untablinebreak(const std::string &tabbed_text) const;
    float get_text_width(const Font &font, const std::string &text); 
    float max_line_width() const;

    inline void invalidate_edit_pos();

    std::vector<Line> m_lines;

    bool m_editable;
    int m_edit_pos;
    Time m_last_seek;

    JUSTIFY m_justify;
    int m_tab_spaces;

    Point2i m_cursor_pos;
    Point2i m_cursor_index;
  };

  class Widget_Input_Repeater : public Widget {
  public:
    /// Pass in the Widget to have input "repeated" to
    inline Widget_Input_Repeater(Widget &widget_,
                                 const int &repeat_delay_ = SDL_DEFAULT_REPEAT_DELAY,
                                 const int &repeat_interval_ = SDL_DEFAULT_REPEAT_INTERVAL);

    inline Widget * const & get_widget() const;
    inline const int & get_repeat_delay() const;
    inline const int & get_repeat_interval() const ;

    inline void set_widget(Widget &widget_);
    inline void set_repeat_delay(const int &repeat_delay_ = SDL_DEFAULT_REPEAT_DELAY);
    inline void set_repeat_interval(const int &repeat_interval_ = SDL_DEFAULT_REPEAT_INTERVAL);

    /// on_key input is repeated
    virtual void on_key(const SDL_keysym &keysym, const bool &down);
    
    /// on_mouse_button deactivates the repeater (if active) and then passes input to the Widget
    virtual void on_mouse_button(const Point2i &pos, const bool &down);
    /// on_mouse_motion input is simply passed through
    virtual void on_mouse_motion(const Point2i &pos);

    /// Call this function in your perform_logic function to get this Widget to actually do its job
    inline void perform_logic();

    // render is simply passed through
    virtual void render() const;

  private:
    Widget * m_widget;
    int m_repeat_delay;
    int m_repeat_interval;
    
    Time m_last_repeated;
    bool m_active;
    bool m_delay_finished;

    SDL_keysym m_keysym;
    bool m_down;
  };

  class Widgets : public Widget {
  public:
    inline void lend_Widget(Widget &widget);
    inline void unlend_Widget(Widget &widget);

    virtual void on_key(const SDL_keysym &keysym, const bool &down);

    virtual void on_mouse_button(const Point2i &pos, const bool &down);
    virtual void on_mouse_motion(const Point2i &pos);

    virtual void render() const;

  private:
    std::set<Widget *> m_widgets;
  };

}

#include <Zeni/Global_Undef.h>

#endif
