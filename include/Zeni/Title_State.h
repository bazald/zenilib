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

#include <Zeni/Sound_Source_Pool.h>
#include <Zeni/Widget_Gamestate.h>

namespace Zeni {

  class Popup_State : public Widget_Gamestate {
    Popup_State(const Popup_State &);
    Popup_State operator=(const Popup_State &);

  public:
    Popup_State(const Gamestate &gamestate, const bool &pause_game)
      : Widget_Gamestate(std::make_pair(Point2f(), Point2f(800.0f, 600.0f))),
      m_gamestate(gamestate),
      m_pause_game(pause_game)
    {
    }

  protected:
    void on_push() {
      if(m_pause_game) {
        Chronometer<Time_HQ>::pause_all();
        Chronometer<Time>::pause_all();
        get_Sound_Source_Pool().pause_all();
      }
    }

    void on_pop() {
      if(m_pause_game) {
        Chronometer<Time_HQ>::unpause_all();
        Chronometer<Time>::unpause_all();
        get_Sound_Source_Pool().unpause_all();
      }
    }

    void on_key(const SDL_KeyboardEvent &event) {
      if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED) {
        get_Game().pop_state();
        get_Game().push_state(m_gamestate);
      }
    }

    void perform_logic() {
      if(!m_pause_game)
        m_gamestate.perform_logic();
    }

    void render() {
      m_gamestate.render();

      Video &vr = get_Video();
      const bool ztest = vr.is_ztest_enabled();

      resize(vr.get_viewport());

      if(ztest)
        vr.set_ztest(false);

      vr.set_2d_view(get_virtual_window(), vr.get_viewport());

      Color color = get_Colors()["title_bg"];
      color.a = 0.5f;
      Quadrilateral<Vertex2f_Color> quad(Vertex2f_Color(Point2f(), color),
                                         Vertex2f_Color(Point2f(0.0f, 600.0f), color),
                                         Vertex2f_Color(Point2f(800.0f, 600.0f), color),
                                         Vertex2f_Color(Point2f(800.0f, 0.0f), color));

      vr.render(quad);

      vr.set_2d_view(get_virtual_window(), get_crop_window());

      m_widgets.render();

      if(ztest)
        vr.set_ztest(true);
    }

    Gamestate m_gamestate;

  private:
    bool m_pause_game;
  };

  class Popup_Menu_State : public Popup_State {
    Popup_Menu_State(const Popup_Menu_State &);
    Popup_Menu_State operator=(const Popup_Menu_State &);

    class Continue_Button : public Text_Button_3C {
      Continue_Button(const Continue_Button &);
      Continue_Button operator=(const Continue_Button &);

    public:
      Continue_Button(const Gamestate &gamestate)
        : Text_Button_3C(Point2f(200.0f, 150.0f), Point2f(600.0f, 210.0f),
                         "system_36_600", "Continue Game"),
        m_gamestate(gamestate)
      {
      }

      void on_accept() {
        get_Game().pop_state();
        get_Game().push_state(m_gamestate);
      }

    private:
      Gamestate m_gamestate;
    } m_continue_button;

    class Menu_Button : public Text_Button_3C {
      Menu_Button(const Menu_Button &);
      Menu_Button operator=(const Menu_Button &);

    public:
      Menu_Button()
        : Text_Button_3C(Point2f(200.0f, 230.0f), Point2f(600.0f, 290.0f),
                         "system_36_600", "Back to Menu")
      {
      }

      void on_accept() {
        get_Game().pop_state();
      }
    } m_menu_button;

  public:
    class Configure_Video_Button : public Text_Button_3C {
      Configure_Video_Button(const Configure_Video_Button &);
      Configure_Video_Button & operator=(const Configure_Video_Button &);

    public:
      Configure_Video_Button(const Point2f &upper_left, const Point2f &lower_right)
        : Text_Button_3C(upper_left, lower_right, "system_36_600", "Configure Video")
      {
      }

      void on_accept() {
        Text_Button_3C::on_accept();
        get_Game().push_state(new Configurator_Video());
      }
    } m_configure_video_button;

    class Sound_Check_Box : public Check_Box {
      Sound_Check_Box(const Sound_Check_Box &);
      Sound_Check_Box operator=(const Sound_Check_Box &);

    public:
      Sound_Check_Box(const Point2f &upper_left, const Point2f &lower_right)
        : Check_Box(upper_left, lower_right,
                    get_Colors()["default_button_bg_normal"], get_Colors()["default_button_bg_normal"],
                    !get_Sound_Source_Pool().is_muted())
      {
      }

      void on_accept() {
        Check_Box::on_accept();
        get_Sound_Source_Pool().mute(!is_checked());
      }

      void render_impl() const {
        const Point2f &ul = get_upper_left();
        const Color color = get_Colors()["title_bg"];
        const Quadrilateral<Vertex2f_Color> bg(Vertex2f_Color(ul, color),
                                               Vertex2f_Color(Point2f(ul.x, ul.y + 60.0f), color),
                                               Vertex2f_Color(Point2f(ul.x + 200.0f, ul.y + 60.0f), color),
                                               Vertex2f_Color(Point2f(ul.x + 200.0f, ul.y), color));
        get_Video().render(bg);

        Check_Box::render_impl();

        Font &font = get_Fonts()["system_36_600"];
        font.render_text(" Sound",
                         Point2f(get_lower_right().x,
                                 0.5f * (get_lower_right().y + get_upper_left().y - font.get_text_height())),
                         get_Colors()["default_button_bg_normal"]);
      }
    } m_sound_check_box;

    class Quit_Button : public Text_Button_3C {
      Quit_Button(const Quit_Button &);
      Quit_Button & operator=(const Quit_Button &);

    public:
      Quit_Button(const Point2f &upper_left, const Point2f &lower_right)
        : Text_Button_3C(upper_left, lower_right, "system_36_600", "Quit")
      {
      }

      void on_accept() {
        Text_Button_3C::on_accept();
        throw Quit_Event();
      }
    } m_quit_button;

    Popup_Menu_State(const Gamestate &gamestate, const bool &pause_game)
      : Popup_State(gamestate, pause_game),
      m_continue_button(gamestate),
      m_configure_video_button(Point2f(200.0f, 310.0f), Point2f(600.0f, 370.0f)),
      m_sound_check_box(Point2f(200.0f, 390.0f), Point2f(260.0f, 450.0f)),
      m_quit_button(Point2f(400.0f, 390.0f), Point2f(600.0f, 450.0f))
    {
      m_widgets.lend_Widget(m_continue_button);
      m_widgets.lend_Widget(m_configure_video_button);
      m_widgets.lend_Widget(m_sound_check_box);
      m_widgets.lend_Widget(m_menu_button);
      m_widgets.lend_Widget(m_quit_button);
    }
  };

  class Popup_Pause_State : public Popup_State {
    Popup_Pause_State(const Popup_Pause_State &);
    Popup_Pause_State operator=(const Popup_Pause_State &);

  public:
    Popup_Pause_State(const Gamestate &gamestate)
      : Popup_State(gamestate, true)
    {
    }

    void on_active(const SDL_ActiveEvent &event) {
      if(event.gain && (event.state & SDL_APPINPUTFOCUS)) {
        get_Game().pop_state();
        get_Game().push_state(m_gamestate);
      }
    }

    void render() {
      Popup_State::render();

      Video &vr = get_Video();
      Font &font = get_Fonts()["title"];
      const bool ztest = vr.is_ztest_enabled();

      if(ztest)
        vr.set_ztest(false);

      font.render_text("Paused", Point2f(400.0f, 300.0f - 0.5f * font.get_text_height()), get_Colors()["title_text"], ZENI_CENTER);

      if(ztest)
        vr.set_ztest(true);
    }
  };

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

    Popup_Menu_State::Configure_Video_Button m_configure_video_button;
    Popup_Menu_State::Sound_Check_Box m_sound_check_box;
    Popup_Menu_State::Quit_Button m_quit_button;

  public:
    Title_State(const std::string &title_)
      : Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f))),
      m_title(title_),
      m_configure_video_button(Point2f(200.0f, 410.0f), Point2f(600.0f, 470.0f)),
      m_sound_check_box(Point2f(200.0f, 490.0f), Point2f(260.0f, 550.0f)),
      m_quit_button(Point2f(400.0f, 490.0f), Point2f(600.0f, 550.0f))
    {
      m_widgets.lend_Widget(m_title);
      m_widgets.lend_Widget(m_play_button);
      m_widgets.lend_Widget(m_instructions_button);
      m_widgets.lend_Widget(m_configure_video_button);
      m_widgets.lend_Widget(m_sound_check_box);
      m_widgets.lend_Widget(m_quit_button);

      m_title.set_justify(ZENI_CENTER);

      get_Video().set_clear_color(get_Colors()["title_bg"]);
    }

    ~Title_State() {
      get_Video().set_clear_color(Color(1.0f, 0.0f, 0.0f, 0.0f));
    }

    void on_key(const SDL_KeyboardEvent &/*event*/)
    {
    }

    void perform_logic() {
      get_Video().set_clear_color(get_Colors()["title_bg"]);
    }

    void render() {
      m_sound_check_box.set_checked(!get_Sound_Source_Pool().is_muted());
      Widget_Gamestate::render();
    }

  };

}

#endif
