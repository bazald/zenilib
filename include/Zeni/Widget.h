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

#ifndef ZENI_WIDGET_H
#define ZENI_WIDGET_H

#include <Zeni/Coordinate.h>
#include <Zeni/Texture.h>
#include <Zeni/Quadrilateral.h>
#include <Zeni/Vertex2f.h>
#include <Zeni/Video.h>

#include <set>

namespace Zeni {

  class Widget {
  public:
    virtual ~Widget() {}

    inline void on_mouse_event(const SDL_MouseButtonEvent &event);
    inline void on_mouse_event(const SDL_MouseMotionEvent &event);

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

  class Widgets : public Widget {
  public:
    inline void add_Widget(Widget * const &widget);
    inline void remove_Widget(Widget * const &widget);

    virtual void on_mouse_button(const Point2i &pos, const bool &down);
    virtual void on_mouse_motion(const Point2i &pos);

    virtual void render() const;

  private:
    std::set<Widget *> m_widgets;
  };

}

#ifdef ZENI_INLINES
#include <Zeni/Widget.hxx>
#endif

#endif
