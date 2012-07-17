/* This file is part of the Zenipex Library (zenilib).
 * Copyright (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * zenilib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * zenilib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with zenilib.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef POPUP_STATE_H
#define POPUP_STATE_H

#include <Zeni/Colors.h>
#include <Zeni/Configurator_Video.h>
#include <Zeni/Chronometer.hxx>
#include <Zeni/Sound.hxx>
#include <Zeni/Sound_Renderer_AL.h>
#include <Zeni/Sound_Source_Pool.h>
#include <Zeni/Widget_Gamestate.h>
#include <Zeni/Quit_Event.h>

namespace Zeni {

  class ZENI_REST_DLL Popup_State : public Widget_Gamestate {
    Popup_State(const Popup_State &);
    Popup_State operator=(const Popup_State &);

  public:
    Popup_State()
      : Widget_Gamestate(std::make_pair(Point2f(), Point2f(800.0f, 600.0f))),
      m_gamestate(get_Game().get_top())
    {
    }

  protected:
    void on_push() {
      if(m_gamestate.is_pausable()) {
        Chronometer<Time_HQ>::pause_all();
        Chronometer<Time>::pause_all();
        get_Sound_Source_Pool().pause_all();
        get_Core().set_screen_saver(true);
      }

      Widget_Gamestate::on_push();
    }

    void on_pop() {
      Widget_Gamestate::on_pop();

      if(m_gamestate.is_pausable()) {
        Chronometer<Time_HQ>::unpause_all();
        Chronometer<Time>::unpause_all();
        get_Sound_Source_Pool().unpause_all();
        get_Core().set_screen_saver(false);
      }
    }

#ifndef ANDROID
    void on_key(const SDL_KeyboardEvent &event) {
      if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
        get_Game().pop_state();
      else
        m_widgets.on_event(event);
    }
#endif

    void perform_logic() {
      Widget_Gamestate::perform_logic();

      if(!m_gamestate.is_pausable())
        m_gamestate.perform_logic();
    }

    void prerender() {
      Widget_Gamestate::prerender();

      m_gamestate.prerender();
    }

    void render() {
      m_gamestate.render();

      Video &vr = get_Video();
      const bool ztest = vr.is_ztest_enabled();

      if(ztest)
        vr.set_ztest(false);

      vr.set_2d(std::make_pair(Point2f(), Point2f(800.0f, 600.0f)));

      Color color = get_Colors()["title_bg"];
      color.a = 0.5f;
      Quadrilateral<Vertex2f_Color> quad(Vertex2f_Color(Point2f(), color),
                                         Vertex2f_Color(Point2f(0.0f, 600.0f), color),
                                         Vertex2f_Color(Point2f(800.0f, 600.0f), color),
                                         Vertex2f_Color(Point2f(800.0f, 0.0f), color));

      vr.render(quad);

      vr.set_2d(get_virtual_window(), fix_aspect_ratio());

      m_widgets.render();

      if(ztest)
        vr.set_ztest(true);
    }

    Gamestate m_gamestate;
  };

  class ZENI_REST_DLL Popup_Menu_State : public Popup_State {
    Popup_Menu_State(const Popup_Menu_State &);
    Popup_Menu_State operator=(const Popup_Menu_State &);

    class ZENI_REST_DLL Continue_Button : public Text_Button {
      Continue_Button(const Continue_Button &);
      Continue_Button operator=(const Continue_Button &);

    public:
      Continue_Button(const Gamestate &gamestate)
        : Text_Button(Point2f(200.0f, 150.0f), Point2f(600.0f, 210.0f),
                      "system_36_800x600", "Continue Game"),
        m_gamestate(gamestate)
      {
      }

      void on_accept() {
        get_Game().pop_state();
      }

    private:
      Gamestate m_gamestate;
    } continue_button;

    class ZENI_REST_DLL Menu_Button : public Text_Button {
      Menu_Button(const Menu_Button &);
      Menu_Button operator=(const Menu_Button &);

    public:
      Menu_Button()
        : Text_Button(Point2f(200.0f, 230.0f), Point2f(600.0f, 290.0f),
                      "system_36_800x600", "Back to Menu")
      {
      }

      void on_accept() {
        get_Game().pop_state();
        get_Game().pop_state();
      }
    } menu_button;

  public:
#ifndef ANDROID
    class ZENI_REST_DLL Configure_Video_Button : public Text_Button {
      Configure_Video_Button(const Configure_Video_Button &);
      Configure_Video_Button & operator=(const Configure_Video_Button &);

    public:
      Configure_Video_Button(const Point2f &upper_left, const Point2f &lower_right)
        : Text_Button(upper_left, lower_right, "system_36_800x600", "Configure Video")
      {
      }

      void on_accept() {
        get_Game().push_state(new Configurator_Video());
      }
    } configure_video_button;
#endif

    class ZENI_REST_DLL Sound_Check_Box : public Check_Box {
      Sound_Check_Box(const Sound_Check_Box &);
      Sound_Check_Box operator=(const Sound_Check_Box &);

    public:
      Sound_Check_Box(const Point2f &upper_left, const Point2f &lower_right)
        : Check_Box(upper_left, lower_right,
                    !get_Sound().is_listener_muted())
      {
        if(dynamic_cast<Sound_Renderer_NULL *>(&get_Sound().get_Renderer())) {
          set_checked(false);
          set_editable(false);
        }
      }

      void on_accept() {
        Check_Box::on_accept();
        get_Sound().set_listener_muted(!is_checked());
      }

      void render_impl() const {
        Colors &cr = get_Colors();
        Video &vr = get_Video();
        Font &font = get_Fonts()["system_36_800x600"];

        const Point2f &ul = get_upper_left();
        const Point2f &lr = get_lower_right();
        Color bgc = cr["title_bg"];
        bgc.a = 0.5f;

        const float tw = font.get_text_width("Sound");
        const float tl = 0.2f * tw;
        const float tr = 1.4f * tw;
        const Quadrilateral<Vertex2f_Color> bg(Vertex2f_Color(ul, bgc),
                                               Vertex2f_Color(Point2f(ul.x, lr.y), bgc),
                                               Vertex2f_Color(Point2f(lr.x + tr, lr.y), bgc),
                                               Vertex2f_Color(Point2f(lr.x + tr, ul.y), bgc));
        vr.render(bg);

        Check_Box::render_impl();

        const Color fgc = cr["default_button_bg_normal"];
        font.render_text("Sound",
                         Point2f(get_lower_right().x + 15.0f,
                                 0.5f * (get_lower_right().y + get_upper_left().y - font.get_text_height())),
                         fgc);

        if(dynamic_cast<Sound_Renderer_NULL *>(&get_Sound().get_Renderer())) {
          const Line_Segment<Vertex2f_Color> ns(Vertex2f_Color(Point2f(get_lower_right().x + tl, 0.5f * (get_lower_right().y + get_upper_left().y)), fgc),
                                                Vertex2f_Color(Point2f(get_lower_right().x + tr, 0.5f * (get_lower_right().y + get_upper_left().y)), fgc));
          vr.render(ns);
        }
      }
    } sound_check_box;

    class ZENI_REST_DLL Quit_Button : public Text_Button {
      Quit_Button(const Quit_Button &);
      Quit_Button & operator=(const Quit_Button &);

    public:
      Quit_Button(const Point2f &upper_left, const Point2f &lower_right)
        : Text_Button(upper_left, lower_right, "system_36_800x600", "Quit")
      {
      }

      void on_accept() {
        throw Quit_Event();
      }
    } quit_button;

    Popup_Menu_State()
      : continue_button(m_gamestate),
#ifndef ANDROID
      configure_video_button(Point2f(200.0f, 310.0f), Point2f(600.0f, 370.0f)),
#endif
      sound_check_box(Point2f(200.0f, 390.0f), Point2f(260.0f, 450.0f)),
      quit_button(Point2f(400.0f, 390.0f), Point2f(600.0f, 450.0f))
    {
      m_widgets.lend_Widget(continue_button);
#ifndef ANDROID
      m_widgets.lend_Widget(configure_video_button);
#endif
      m_widgets.lend_Widget(sound_check_box);
      m_widgets.lend_Widget(menu_button);
      m_widgets.lend_Widget(quit_button);
    }

    void on_push() {
      Popup_State::on_push();

      if(get_Game().size() == 1u)
        m_widgets.unlend_Widget(menu_button);
    }

    void on_pop() {
      if(get_Game().size() == 0u)
        m_widgets.lend_Widget(menu_button);

      Popup_State::on_pop();
    }
  };

  class ZENI_REST_DLL Popup_Pause_State : public Popup_State {
    Popup_Pause_State(const Popup_Pause_State &);
    Popup_Pause_State operator=(const Popup_Pause_State &);

  public:
    Popup_Pause_State()
    {
    }

#ifndef ANDROID
    void on_active(const SDL_ActiveEvent &event) {
      if(event.gain && (event.state & SDL_APPINPUTFOCUS))
        get_Game().pop_state();

      Gamestate_Base::on_active(event);
    }
#endif

    void perform_logic() {
#ifndef ANDROID
      if(m_gamestate.is_pausable())
        SDL_Delay(5);
#endif
    }

    void render() {
      if(m_gamestate.is_pausable()) {
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
      else
        m_gamestate.render();
    }
  };

}

#endif
