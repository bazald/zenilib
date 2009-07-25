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
* \class Zeni::Title_State
*
* \ingroup Zenilib
*
* \brief Provide a simple title screen for games
*
* \author bazald
*
* Contact: bazald@zenipex.com
*/

#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include <Zeni/Widget_Gamestate.h>

namespace Zeni {

  template <typename GAMESTATE>
  class Title_State : public Widget_Gamestate {
    class Title : public Text_Box {
    public:
      Title(const std::string &title_)
        : Text_Box(Point2f(000.0f, 50.0f), Point2f(800.0f, 250.0f),
                   get_Colors()["title_bg"], "title", title_, get_Colors()["title_text"])
      {
      }
    } m_title;

    class Play_Button : public Text_Button_3C {
    public:
      Play_Button()
        : Text_Button_3C(Point2f(200.0f, 250.0f), Point2f(600.0f, 320.0f),
                         "system_36_600", "Play")
      {
      }

      void on_accept() {
        get_Game().push_state(new GAMESTATE());
      }
    } m_play_button;

    class Configure_Video_Button : public Text_Button_3C {
    public:
      Configure_Video_Button()
        : Text_Button_3C(Point2f(200.0f, 350.0f), Point2f(600.0f, 420.0f),
                         "system_36_600", "Configure Video")
      {
      }

      void on_accept() {
        get_Game().push_state(new Configurator_Video());
      }
    } m_configure_video_button;

    class Quit_Button : public Text_Button_3C {
    public:
      Quit_Button()
        : Text_Button_3C(Point2f(200.0f, 450.0f), Point2f(600.0f, 520.0f),
                         "system_36_600", "Quit")
      {
      }

      void on_accept() {
        throw Quit_Event();
      }
    } m_quit_button;

  public:
    Title_State(const std::string &title_)
      : Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f))),
      m_title(title_)
    {
      m_widgets.lend_Widget(m_title);
      m_widgets.lend_Widget(m_play_button);
      m_widgets.lend_Widget(m_configure_video_button);
      m_widgets.lend_Widget(m_quit_button);

      m_title.set_justify(ZENI_CENTER);

      get_Video().set_clear_color(get_Colors()["title_bg"]);
    }

    ~Title_State() {
      get_Video().set_clear_color(Color(1.0f, 0.0f, 0.0f, 0.0f));
    }

    void perform_logic() {
      get_Video().set_clear_color(get_Colors()["title_bg"]);
    }
  };

}

#endif
