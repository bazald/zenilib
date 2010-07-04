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

/**
 * \class Zeni::Widget
 *
 * \ingroup zenilib
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
 * \class Zeni::Widget_Text
 *
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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

  class Widget_Render_Function;

  class Widget {
    Widget(const Widget &);
    Widget & operator=(const Widget &);

  public:
    inline Widget();
    virtual ~Widget();

    inline const bool & is_busy() const;
    inline const bool & is_editable() const;
    inline const float & get_layer() const;
    inline void set_busy(const bool &busy_);
    virtual void set_editable(const bool &editable_);
    inline void set_layer(const float &layer_ = 0.0f);

    inline void on_event(const SDL_KeyboardEvent &event);
    inline void on_event(const SDL_MouseButtonEvent &event);
    inline void on_event(const SDL_MouseButtonEvent &event, const Projector2D &projector);
    inline void on_event(const SDL_MouseMotionEvent &event);
    inline void on_event(const SDL_MouseMotionEvent &event, const Projector2D &projector);

    virtual void on_key(const SDL_keysym & /*keysym*/, const bool & /*down*/) {}
    virtual void on_mouse_button(const Point2i &pos, const bool &down, const int &button) = 0;
    virtual void on_mouse_motion(const Point2i &pos) = 0;

    inline void render() const;

    inline const Widget_Render_Function * get_Renderer() const; ///< Get the current Widget_Render_Function
    inline void give_Renderer(Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget ownership
    inline void lend_Renderer(const Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget no ownership
    inline void fax_Renderer(const Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget a copy

    virtual void render_impl() const;

  private:
    float m_layer;
    bool m_busy;
    bool m_editable;

    Widget_Render_Function * m_renderer;
    bool delete_m_renderer;
  };

  class Widget_Render_Function {
  public:
    virtual ~Widget_Render_Function() {}

    virtual void render_to(const Widget &widget) = 0;

    virtual Widget_Render_Function * get_duplicate() const = 0;
  };

  template <typename T1, typename T2>
  class Widget_Renderer_Pair : public Widget_Render_Function {
  public:
    inline Widget_Renderer_Pair(const T1 * const &first_, const bool &delete_first_,
                                const T2 * const &second_, const bool &delete_second_);
    virtual ~Widget_Renderer_Pair();

    inline const T1 * const & first() const;
    inline const T2 * const & second() const;
    inline T1 * const & first();
    inline T2 * const & second();

    virtual void render_to(const Widget &widget);

    virtual Widget_Renderer_Pair<T1, T2> * get_duplicate() const;

  private:
    T1 * m_first;
    bool delete_m_first;

    T2 * m_second;
    bool delete_m_second;
  };

  template <typename T1, typename T2>
  Widget_Renderer_Pair<T1, T2> * make_Widget_Renderer_Pair(const T1 * const &first_, const bool &delete_first_,
                                                           const T2 * const &second_, const bool &delete_second_);

  class Widget_Renderer_Text : public Widget_Render_Function {
  public:
    inline Widget_Renderer_Text(const std::string &font_name_, const std::string &text_, const Color &color_);

    /// rect must be of type Widget_Rectangle
    virtual void render_to(const Widget &widget);

    virtual Widget_Renderer_Text * get_duplicate() const;

    std::string font_name;
    std::string text;
    Color color;
  };

  class Widget_Renderer_Color : public Widget_Render_Function {
  public:
    inline Widget_Renderer_Color(const Color &color_);

    /// rect must be of type Widget_Rectangle
    virtual void render_to(const Widget &widget);

    virtual Widget_Renderer_Color * get_duplicate() const;

    Color color;
  };

  class Widget_Renderer_Texture : public Widget_Render_Function {
  public:
    inline Widget_Renderer_Texture(const std::string &texture_);
    inline Widget_Renderer_Texture(const std::string &texture_,
                                   const Point2f &tex_coord_ul_,
                                   const Point2f &tex_coord_ll_,
                                   const Point2f &tex_coord_lr_,
                                   const Point2f &tex_coord_ur_);

    /// rect must be of type Widget_Rectangle
    virtual void render_to(const Widget &widget);

    virtual Widget_Renderer_Texture * get_duplicate() const;

    std::string texture;
    Point2f tex_coord_ul;
    Point2f tex_coord_ll;
    Point2f tex_coord_lr;
    Point2f tex_coord_ur;
  };

  class Widget_Renderer_Tricolor : public Widget_Renderer_Color {
  public:
    inline Widget_Renderer_Tricolor();
    inline Widget_Renderer_Tricolor(const Color &bg_normal_, const Color &bg_clicked_, const Color &bg_hovered_strayed_,
                                    const Color &text_normal_, const Color &text_clicked_, const Color &text_hovered_strayed_);

    /// rect must be of type Widget_Button and Widget_Renderer_Text
    virtual void render_to(const Widget &widget);

    virtual Widget_Renderer_Tricolor * get_duplicate() const;

    Color bg_normal;
    Color bg_clicked;
    Color bg_hovered_strayed;
    Color text_normal;
    Color text_clicked;
    Color text_hovered_strayed;
  };

  class Widget_Renderer_Check_Box : public Widget_Render_Function {
  public:
    inline Widget_Renderer_Check_Box();
    inline Widget_Renderer_Check_Box(const Color &border_color_, const Color &check_color_);

    /// rect must be of type Check_Box
    virtual void render_to(const Widget &widget);

    virtual Widget_Renderer_Check_Box * get_duplicate() const;

    Color border_color;
    Color check_color;
  };

  class Widget_Renderer_Slider : public Widget_Render_Function {
  public:
    inline Widget_Renderer_Slider();
    inline Widget_Renderer_Slider(const Color &line_color_, const Color &slider_color_);

    /// rect must be of type Slider
    virtual void render_to(const Widget &widget);

    virtual Widget_Renderer_Slider * get_duplicate() const;

    Color line_color;
    Color slider_color;
  };

  class Widget_Rectangle {
  public:
    inline Widget_Rectangle(const Point2f &upper_left_, const Point2f &lower_right_);
    virtual ~Widget_Rectangle() {}
 
    inline const Point2f & get_upper_left() const;
    inline Point2f get_lower_left() const;
    inline const Point2f & get_lower_right() const;
    inline Point2f get_upper_right() const;
 
    virtual void set_upper_left(const Point2f &upper_left_);
    virtual void set_lower_right(const Point2f &lower_right_);

    inline float get_height() const;
    inline float get_width() const;
    inline Point2f get_center() const;

    inline bool is_inside(const Point2i &pos) const;

  private:
    Point2f m_upper_left;
    Point2f m_lower_right;
  };

  class Widget_Button : public Widget, public Widget_Rectangle {
    Widget_Button(const Widget_Button &);
    Widget_Button & operator=(const Widget_Button &);

  public:
    enum State {NORMAL, CLICKED, HOVERED, STRAYED, UNACTIONABLE};

    inline Widget_Button(const Point2f &upper_left_, const Point2f &lower_right_);

    inline const State & get_State() const; ///< Get the State of the button

    virtual void on_mouse_button(const Point2i &pos, const bool &down, const int &button);
    virtual void on_mouse_motion(const Point2i &pos);

    /// Called when the cursor passes over the button
    virtual void on_hover() {}
    /// Called when the cursor leaves the button without clicking
    virtual void on_unhover() {}

    /// Called when the cursor downclicks the button
    virtual void on_click() {}
    /// Called when the cursor is dragged off the button after being clicked
    virtual void on_stray() {}
    /// Called when the cursor is dragged back onto the button without releasing the clicker
    virtual void on_unstray() {}

    /// Called when the cursor is released inside the button
    virtual void on_accept() {}
    /// Called when the cursor is released outside the button
    virtual void on_reject() {}

  private:
    State m_state;
  };

  class Text_Button : public Widget_Button, public Widget_Renderer_Text {
    Text_Button(const Text_Button &);
    Text_Button & operator=(const Text_Button &);

  public:
    inline Text_Button(const Point2f &upper_left_, const Point2f &lower_right_,
                       const std::string &font_name_, const std::string &text_);
  };

  class Check_Box : public Widget_Button {
    Check_Box(const Check_Box &);
    Check_Box & operator=(const Check_Box &);

  public:
    inline Check_Box(const Point2f &upper_left_, const Point2f &lower_right_,
                     const bool &checked_ = false, const bool &toggleable_ = true);

    inline const bool & is_checked() const;
    inline const void set_checked(const bool &checked_);
    inline const bool & is_toggling() const;

    virtual void on_accept();

    virtual void on_click();
    virtual void on_unstray();

    virtual void on_reject();
    virtual void on_stray();

  private:
    bool m_checked;
    bool m_toggling;
  };

  class Radio_Button;
  class Radio_Button_Set : public Widget {
    Radio_Button_Set(const Radio_Button_Set &);
    Radio_Button_Set & operator=(const Radio_Button_Set &);

    friend class Radio_Button;

  public:
    virtual void on_mouse_button(const Point2i &pos, const bool &down, const int &button);
    virtual void on_mouse_motion(const Point2i &pos);

    inline void accept(Radio_Button &radio_button);
    inline void clear();

    virtual void render_impl() const;

  private:
    inline void lend_Radio_Button(Radio_Button &radio_button);
    inline void unlend_Radio_Button(Radio_Button &radio_button);

    std::set<Radio_Button *> m_radio_buttons;
  };

  class Radio_Button : public Check_Box {
    Radio_Button(const Radio_Button &);
    Radio_Button & operator=(const Radio_Button &);

    friend class Radio_Button_Set;

  public:
    inline Radio_Button(Radio_Button_Set &radio_button_set_,
                        const Point2f &upper_left_, const Point2f &lower_right_,
                        const bool &checked_ = false, const bool &toggleable_ = true);
    inline ~Radio_Button();

    virtual void on_accept();

  private:
    Radio_Button_Set * m_radio_button_set;
  };

  class Slider : public Widget {
    Slider(const Slider &);
    Slider & operator=(const Slider &);

  public:
    Slider(const Point2f &end_point_a_, const Point2f &end_point_b_,
           const float &slider_radius_,
           const float &slider_position_ = ZENI_DEFAULT_SLIDER_POSITION);

    inline Point2f get_end_point_a() const;
    inline Point2f get_end_point_b() const;
    inline const float & get_slider_radius() const;
    inline const float & get_slider_position() const;

    inline void set_end_points(const Point2f &end_point_a_, const Point2f &end_point_b_);
    inline void set_slider_radius(const float &radius_);
    inline void set_slider_position(const float &slider_position_);

    virtual void on_mouse_button(const Zeni::Point2i &pos, const bool &down, const int &button);
    virtual void on_mouse_motion(const Zeni::Point2i &pos);

    virtual void on_slide();
    virtual void on_accept();

  protected:
    inline const Collision::Line_Segment & get_line_segment() const;

  private:
    Collision::Line_Segment m_line_segment;
    float m_slider_radius;

    float m_slider_position;
    float m_backup_position;
    bool m_down;
  };

  class Slider_Int : public Slider {
    Slider_Int(const Slider_Int &);
    Slider_Int & operator=(const Slider_Int &);

  public:
    typedef std::pair<int, int> Range;

    Slider_Int(const Range &range,
               const Point2f &end_point_a_, const Point2f &end_point_b_,
               const float &slider_radius_,
               const float &slider_position_ = ZENI_DEFAULT_SLIDER_POSITION);

    inline const Range & get_range() const;
    inline void set_range(const Range &range_);

    inline int get_value() const;
    inline void set_value(const int &value);

    virtual void on_mouse_button(const Zeni::Point2i &pos, const bool &down, const int &button);
    virtual void on_slide();

    inline const bool & is_mouse_wheel_inverted() const;
    inline void invert_mouse_wheel(const bool &invert);

  private:
    Range m_range;
    bool m_mouse_wheel_inverted;
  };

  class Selector : public Widget {
    Selector(const Selector &);
    Selector & operator=(const Selector &);

    class Normal_Button : public Text_Button {
      Normal_Button(const Normal_Button &);
      Normal_Button & operator=(const Normal_Button &);

    public:
      Normal_Button(Selector &selector,
                    const Point2f &upper_left_,
                    const Point2f &lower_right_);

      void on_accept();

    private:
      Selector * m_selector;
    };

    class Selector_Button : public Text_Button {
      Selector_Button(const Selector_Button &);
      Selector_Button & operator=(const Selector_Button &);

    public:
      Selector_Button(Selector &selector,
                      const std::string &option,
                      const Point2f &upper_left_,
                      const Point2f &lower_right_);

      void on_accept();

    private:
      Selector * m_selector;
    };

    class Selector_Slider : public Widget_Rectangle, public Slider_Int {
      Selector_Slider(const Selector_Slider &);
      Selector_Slider & operator=(const Selector_Slider &);

    public:
      Selector_Slider(Selector &selector,
                      const float &slider_radius_,
                      const std::pair<float, float> &bg_coordinates_);

      void set_end_points(const Point2f &end_point_a_, const Point2f &end_point_b_);

      void on_slide();

      void render_impl() const;

    private:
      Selector * m_selector;
    };

  public:
    typedef std::vector<std::string> Options;

    Selector(const Point2f &upper_left_, const Point2f &lower_right_,
             const Point2f &expanded_upper_left_, const Point2f &expanded_lower_right_,
             const std::string &font_);
    ~Selector();

    const Options & get_options() const;

    void add_option(const std::string &option);
    void remove_option(const std::string &option);

    std::string get_selected() const;
    void select_option(const std::string &option);

    virtual void on_mouse_button(const Point2i &pos, const bool &down, const int &button);
    virtual void on_mouse_motion(const Point2i &pos);

    virtual void on_accept(const std::string &option);

    virtual void render_impl() const;

    inline const Widget_Render_Function * get_Text_Button_Renderer() const; ///< Get the current Widget_Render_Function
    inline void give_Text_Button_Renderer(Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget ownership
    inline void lend_Text_Button_Renderer(const Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget no ownership
    inline void fax_Text_Button_Renderer(const Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget a copy

    inline const Widget_Render_Function * get_Slider_Renderer() const; ///< Get the current Widget_Render_Function
    inline void give_Slider_Renderer(Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget ownership
    inline void lend_Slider_Renderer(const Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget no ownership
    inline void fax_Slider_Renderer(const Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget a copy

    inline const Widget_Render_Function * get_Slider_BG_Renderer() const; ///< Get the current Widget_Render_Function
    inline void give_Slider_BG_Renderer(Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget ownership
    inline void lend_Slider_BG_Renderer(const Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget no ownership
    inline void fax_Slider_BG_Renderer(const Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget a copy

    inline const std::string & get_font() const; ///< Get the current font
    inline void set_font(const std::string &font_); ///< Set the current font

  private:
    const Widget_Render_Function * get_Renderer() const; ///< Disable
    void give_Renderer(Widget_Render_Function * const &); ///< Disable
    void lend_Renderer(const Widget_Render_Function * const &); ///< Disable
    void fax_Renderer(const Widget_Render_Function * const &); ///< Disable

    float button_height() const;
    float vertical_offset() const;

    void decide_visible(const size_t &centered);

    std::pair<Point2f, Point2f> visible_region() const;

    void add_selector_button(const std::string &option);

    void build_selector_buttons();

    void clear();

    Widget_Render_Function * m_button_renderer;
    bool delete_m_button_renderer;
    Widget_Render_Function * m_slider_renderer;
    bool delete_m_slider_renderer;
    Widget_Render_Function * m_slider_bg_renderer;
    bool delete_m_slider_bg_renderer;
    std::string m_font;

    Widget_Rectangle m_expanded;

    Options m_options;
    size_t m_option;

    bool m_selected;

    Normal_Button m_normal_button;
    std::vector<Selector_Button *> m_selector_buttons;
    Selector_Slider m_selector_slider;

    size_t view_start;
    size_t view_end;
    size_t view_offset;
    size_t view_hidden;
  };

  class Text_Box : public Widget_Button {
    Text_Box(const Text_Box &);
    Text_Box & operator=(const Text_Box &);

  public:
    Text_Box(const Point2f &upper_left_, const Point2f &lower_right_,
             const std::string &font_name_, const std::string &text_, const Color &text_color_,
             const bool &editable_ = ZENI_DEFAULT_TEXTBOX_EDITABLE,
             const JUSTIFY &justify_ = ZENI_DEFAULT_JUSTIFY,
             const int &tab_spaces_ = ZENI_DEFAULT_TAB_SPACES);
    ~Text_Box();

    virtual void on_key(const SDL_keysym &keysym, const bool &down);

    virtual void on_mouse_button(const Point2i &pos, const bool &down, const int &button);
    virtual void on_accept();

    virtual void on_focus();
    virtual void on_unfocus();
    virtual void on_change();

    inline const std::string & get_font_name() const;
    inline const Font & get_font() const;
    inline const std::string & get_text() const;
    inline const Color & get_text_color() const;
    inline const JUSTIFY & get_justify() const;
    inline int get_num_lines() const;
    inline int get_max_lines() const;

    virtual void set_editable(const bool &editable_);
    virtual void set_upper_left(const Point2f &upper_left_);
    virtual void set_lower_right(const Point2f &lower_right_);
    inline void set_font_name(const std::string &font_name_);
    inline void set_text(const std::string &text_);
    inline void set_text_color(const Color &text_color_);
    inline void set_justify(const JUSTIFY &justify_);
    inline void erase_lines(const int &begin, const int &end);

    const int & get_edit_pos() const;
    int get_cursor_pos() const;
    int get_max_seek() const;
    int get_max_cursor_seek() const;

    void seek(const int &edit_pos);
    void seek_cursor(const int &cursor_pos);
    void set_focus(const bool &value);

    virtual void render_impl() const;

    inline const Widget_Render_Function * get_BG_Renderer() const; ///< Get the current Widget_Render_Function
    inline void give_BG_Renderer(Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget ownership
    inline void lend_BG_Renderer(const Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget no ownership
    inline void fax_BG_Renderer(const Widget_Render_Function * const &renderer); ///< Set the current Widget_Render_Function, giving the Widget a copy

  private:
    const Widget_Render_Function * get_Renderer() const; ///< Disable
    void give_Renderer(Widget_Render_Function * const &); ///< Disable
    void lend_Renderer(const Widget_Render_Function * const &); ///< Disable
    void fax_Renderer(const Widget_Render_Function * const &); ///< Disable

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

    Widget_Render_Function * m_bg_renderer;
    bool delete_m_bg_renderer;
    Widget_Renderer_Text m_text;

    std::string clean_string(const std::string &unclean_string) const;
    std::string untablinebreak(const std::string &tabbed_text) const;
    float get_text_width(const Font &font, const std::string &text); 
    float max_line_width() const;

    inline void invalidate_edit_pos();

    std::vector<Line> m_lines;

    int m_edit_pos;
    Time m_last_seek;

    JUSTIFY m_justify;
    int m_tab_spaces;

    Point2i m_cursor_pos;
    Point2i m_cursor_index;

  public:
    static void reformat_all(); ///< Reformat all Text_Box instances

  private:
    static std::set<Text_Box *> & get_text_boxes();
  };

  class Widget_Input_Repeater : public Widget {
    Widget_Input_Repeater(const Widget_Input_Repeater &);
    Widget_Input_Repeater & operator=(const Widget_Input_Repeater &);

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
    virtual void on_mouse_button(const Point2i &pos, const bool &down, const int &button);
    /// on_mouse_motion input is simply passed through
    virtual void on_mouse_motion(const Point2i &pos);

    /// Call this function in your perform_logic function to get this Widget to actually do its job
    inline void perform_logic();

    // render is simply passed through
    virtual void render_impl() const;

  private:
    const Widget_Render_Function * get_Renderer() const; ///< Disable
    void give_Renderer(Widget_Render_Function * const &); ///< Disable
    void lend_Renderer(const Widget_Render_Function * const &); ///< Disable
    void fax_Renderer(const Widget_Render_Function * const &); ///< Disable

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
    Widgets(const Widgets &);
    Widgets & operator=(const Widgets &);

  public:
    inline Widgets();

    inline void lend_Widget(Widget &widget);
    inline void unlend_Widget(Widget &widget);

    virtual void on_key(const SDL_keysym &keysym, const bool &down);

    virtual void on_mouse_button(const Point2i &pos, const bool &down, const int &button);
    virtual void on_mouse_motion(const Point2i &pos);

    virtual void render_impl() const;

  private:
    const Widget_Render_Function * get_Renderer() const; ///< Disable
    void give_Renderer(Widget_Render_Function * const &); ///< Disable
    void lend_Renderer(const Widget_Render_Function * const &); ///< Disable
    void fax_Renderer(const Widget_Render_Function * const &); ///< Disable

    mutable std::vector<Widget *> m_widgets;
    Widget * m_busy_one;
  };

  class Widget_Renderer_Wrong_Type : public Error {
  public:
    Widget_Renderer_Wrong_Type() : Error("Widget_Renderer_Function received an unexpected type") {}
  };

}

#include <Zeni/Global_Undef.h>

#endif
