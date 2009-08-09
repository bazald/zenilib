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

  template <typename PLAY_STATE, typename INSTRUCTIONS_STATE>
  class Title_State : public Widget_Gamestate {
    Title_State(const Title_State<PLAY_STATE, INSTRUCTIONS_STATE> &);
    Title_State<PLAY_STATE, INSTRUCTIONS_STATE> & operator=(const Title_State<PLAY_STATE, INSTRUCTIONS_STATE> &);

    class Title : public Text_Box {
      Title(const Title &);
      Title & operator=(const Title &);

    public:
      Title(const std::string &title_)
        : Text_Box(Point2f(000.0f, 50.0f), Point2f(800.0f, 250.0f),
                   get_Colors()["title_bg"], "title", title_, get_Colors()["title_text"])
      {
      }
    } m_title;

    class Play_Button : public Text_Button_3C {
      Play_Button(const Play_Button &);
      Play_Button & operator=(const Play_Button &);

    public:
      Play_Button()
        : Text_Button_3C(Point2f(200.0f, 250.0f), Point2f(600.0f, 310.0f),
                         "system_36_600", "Play")
      {
      }

      void on_accept() {
        Text_Button_3C::on_accept();
        get_Game().push_state(new PLAY_STATE());
      }
    } m_play_button;

    class Instructions_Button : public Text_Button_3C {
      Instructions_Button(const Instructions_Button &);
      Instructions_Button & operator=(const Instructions_Button &);

    public:
      Instructions_Button()
        : Text_Button_3C(Point2f(200.0f, 330.0f), Point2f(600.0f, 390.0f),
                         "system_36_600", "Instructions")
      {
      }

      void on_accept() {
        Text_Button_3C::on_accept();
        get_Game().push_state(new INSTRUCTIONS_STATE());
      }
    } m_instructions_button;

    class Configure_Video_Button : public Text_Button_3C {
      Configure_Video_Button(const Configure_Video_Button &);
      Configure_Video_Button & operator=(const Configure_Video_Button &);

    public:
      Configure_Video_Button()
        : Text_Button_3C(Point2f(200.0f, 410.0f), Point2f(600.0f, 470.0f),
                         "system_36_600", "Configure Video")
      {
      }

      void on_accept() {
        Text_Button_3C::on_accept();
        get_Game().push_state(new Configurator_Video());
      }
    } m_configure_video_button;

    class Quit_Button : public Text_Button_3C {
      Quit_Button(const Quit_Button &);
      Quit_Button & operator=(const Quit_Button &);

    public:
      Quit_Button()
        : Text_Button_3C(Point2f(200.0f, 490.0f), Point2f(600.0f, 550.0f),
                         "system_36_600", "Quit")
      {
      }

      void on_accept() {
        Text_Button_3C::on_accept();
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
      m_widgets.lend_Widget(m_instructions_button);
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
