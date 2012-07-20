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

/**
* \class Zeni::Configurator_Video
*
* \ingroup zenilib
*
* \brief Configure zenilib.xml
*
* \author bazald
*
* Contact: bazald@zenipex.com
*/

#if !defined(ANDROID) && !defined(CONFIGURATOR_VIDEO_H)
#define CONFIGURATOR_VIDEO_H

#include <Zeni/Chronometer.h>
#include <Zeni/Widget_Gamestate.h>
#include <Zeni/XML.h>

namespace Zeni {

  class ZENI_REST_DLL Configurator_Video : public Widget_Gamestate {
    Configurator_Video(const Configurator_Video &);
    Configurator_Video & operator=(const Configurator_Video &);

  public:
    class ZENI_REST_DLL Check_State : public Zeni::Widget_Gamestate {
      Check_State(const Check_State &);
      Check_State operator=(const Check_State &);

      enum Code {CODE_IGNORE, CODE_SAVE, CODE_REVERT};

      class ZENI_REST_DLL Accept_Button : public Zeni::Text_Button {
        Accept_Button(const Accept_Button &);
        Accept_Button operator=(const Accept_Button &);

      public:
        Accept_Button(Check_State &check_video, const std::pair<Point2f, Point2f> &virtual_screen);

        void on_accept();

      private:
        Check_State &m_check_video;
      };

    public:
      Check_State();

    private:
      void on_pop();

      void on_key(const SDL_KeyboardEvent &event);
      void on_video_resize(const SDL_ResizeEvent &event);

      void perform_logic();

      Accept_Button m_accept_button;
      int m_seconds_remaining;
      Text_Box m_text;

      Time m_start_time;

      Code m_code;
    };

  private:
    class ZENI_REST_DLL Check_Box_Element : public Check_Box {
      Check_Box_Element(const Check_Box_Element &);
      Check_Box_Element & operator=(const Check_Box_Element &);

    public:
      Check_Box_Element(const XML_Element &element,
                                         const bool &checked,
                                         const Point2f &upper_left,
                                         const float &height);

      void on_accept();

      void render_impl() const;

    private:
      XML_Element m_element;
    };

    class ZENI_REST_DLL Slider_Element : public Slider_Int {
      Slider_Element(const Slider_Element &);
      Slider_Element & operator=(const Slider_Element &);

    public:
      Slider_Element(const XML_Element &element,
                                      const int &value,
                                      const std::pair<int, int> &range,
                                      const Point2f &upper_left,
                                      const Point2f &lower_right);

      void on_accept();

      void render_impl() const;

    private:
      XML_Element m_element;
      Point2f m_text_coord;
    };

    class ZENI_REST_DLL Text_Element : public Text_Box {
      Text_Element(const Text_Element &);
      Text_Element & operator=(const Text_Element &);

    public:
      Text_Element(const XML_Element &element,
                                    const String &text,
                                    const Point2f &upper_left,
                                    const Point2f &lower_right);

      void on_change();

      void render_impl() const;

    private:
      XML_Element m_element;
    };

    class ZENI_REST_DLL Selector_Element : public Selector {
      Selector_Element(const Selector_Element &);
      Selector_Element & operator=(const Selector_Element &);

    public:
      Selector_Element(const XML_Element &element,
                                        const Point2f &upper_left,
                                        const Point2f &lower_right,
                                        const Point2f &expanded_upper_left,
                                        const Point2f &expanded_lower_right);

      void add_entry(const String &option, const String &save_as);

      void on_accept(const String &option);

    private:
      XML_Element m_element;
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
      std::map<String, String> m_save_as;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
    };

    class ZENI_REST_DLL Resolution_Element : public Selector {
      Resolution_Element(const Resolution_Element &);
      Resolution_Element & operator=(const Resolution_Element &);

    public:
      Resolution_Element(const XML_Element &element,
                                          const Point2f &upper_left,
                                          const Point2f &lower_right,
                                          const Point2f &expanded_upper_left,
                                          const Point2f &expanded_lower_right);

      void on_accept(const String &option);

    private:
      XML_Element m_element;
    };

    class ZENI_REST_DLL Custom_Resolution_Box : public Check_Box {
      Custom_Resolution_Box(const Custom_Resolution_Box &);
      Custom_Resolution_Box & operator=(const Custom_Resolution_Box &);

    public:
      Custom_Resolution_Box(Configurator_Video &configurator,
                                             const bool &checked,
                                             const Point2f &upper_left,
                                             const float &height);

      void apply();

      void on_accept();

      void render_impl() const;

    private:
      Configurator_Video &m_configurator;
    };

    class ZENI_REST_DLL Apply_Button : public Text_Button {
      Apply_Button(const Apply_Button &);
      Apply_Button & operator=(const Apply_Button &);

    public:
      Apply_Button(XML_Document &file,
                                    const Point2f &upper_left,
                                    const Point2f &lower_right);

      void on_accept();

    private:
      XML_Document * m_file;
    };

    class ZENI_REST_DLL Cancel_Button : public Text_Button {
      Cancel_Button(const Cancel_Button &);
      Cancel_Button & operator=(const Cancel_Button &);

    public:
      Cancel_Button(const Point2f &upper_left,
                                     const Point2f &lower_right);

      void on_accept();
    };

  public:
    Configurator_Video();
    ~Configurator_Video();

  private:
    void on_push();

    void on_key(const SDL_KeyboardEvent &event);

    void render();

    XML_Document m_file;
    XML_Element m_zenilib;

  public:
    Slider_Element anisotropy;
    Check_Box_Element bilinear_filtering;
    Check_Box_Element mipmapping;

    Selector_Element api;
    Check_Box_Element full_screen;
    Slider_Element multisampling;

    Resolution_Element resolution;
    Custom_Resolution_Box custom_resolution;
    Text_Element custom_width;
    Text_Element custom_height;

    Check_Box_Element vertical_sync;

    Apply_Button save;
    Cancel_Button cancel;

  private:
    String m_prev_title;
  };

}

#endif
