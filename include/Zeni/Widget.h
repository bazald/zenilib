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

#include "Coordinate.h"
#include "Texture.h"
#include "Textures.h"
#include "Quadrilateral.h"
#include "Vertex2f.h"
#include "Video.h"

#include <set>

namespace Zeni {

  class Widget_Callback {
  public:
    virtual void operator()() {};

    virtual Widget_Callback * get_duplicate() const {return new Widget_Callback();}
  };

  class Widget {
  public:
    enum MOUSE_STATE {
      ZENI_MOUSE_NORMAL = 0,
      ZENI_MOUSE_HOVERING,
      ZENI_MOUSE_CLICKED,
      ZENI_MOUSE_UNCLICKED};

    Widget();
    Widget(const Point2f &upper_left, const Point2f &lower_right);
    virtual ~Widget();

    Widget(const Widget &rhs);
    Widget & operator=(const Widget &rhs);

    const Point2f & get_upper_left() const {return m_upper_left;}
    const Point2f & get_lower_right() const {return m_lower_right;}
    bool is_clicked_down() const {return m_clicked_down;}
    bool is_clicked_elsewhere() const {return m_clicked_elsewhere;}

    virtual Widget * get_duplicate() const = 0;

    void set_on_mouse_normal(Widget_Callback *callback);
    void set_on_mouse_hover(Widget_Callback *callback);
    void set_on_mouse_click(Widget_Callback *callback);
    void set_on_mouse_unclick(Widget_Callback *callback);
    
    virtual MOUSE_STATE mouse_move(const Point2f &mouse_pos);
    virtual MOUSE_STATE mouse_click(const Point2f &mouse_pos, const bool &down);

    virtual void render() const;

  protected:
    void copy_from(const Widget &widget);

  private:
    Point2f m_upper_left, m_lower_right;

    bool m_clicked_down;
    bool m_clicked_elsewhere;

    MOUSE_STATE prev_mouse_state;
    Widget_Callback *on_mouse_normal;
    Widget_Callback *on_mouse_hover;
    Widget_Callback *on_mouse_click;
    Widget_Callback *on_mouse_unclick;
  };

  class Widgets : public Widget {
  public:
    Widgets();
    virtual ~Widgets();

    Widgets(const Widgets &rhs);
    Widgets & operator=(const Widgets &rhs);

    virtual void add_Widget(Widget * widget);
    virtual void remove_Widget(Widget * widget);

    virtual Widget * get_duplicate() const;

    virtual MOUSE_STATE mouse_move(const Point2f &mouse_pos);
    virtual MOUSE_STATE mouse_click(const Point2f &mouse_pos, const bool &down);

    virtual void render() const;

  private:
    std::set<Widget *> m_widgets;
  };

  class Button : public Widget {
  public:
    Button();
    Button(const Point2f &upper_left, const Point2f &lower_right,
      const std::string &normal, const std::string &mouse_over, const std::string &click_down);
    ~Button();

    Button(const Button &rhs);
    Button & operator=(const Button &rhs);
    
    virtual Widget * get_duplicate() const;

    virtual MOUSE_STATE mouse_move(const Point2f &mouse_pos);
    virtual MOUSE_STATE mouse_click(const Point2f &mouse_pos, const bool &down);

    virtual void render() const;

  private:
    Sprite m_sprite;
    std::string m_sprite_name;

    Quadrilateral<Vertex2f_Texture> *m_quad;
  };

}
