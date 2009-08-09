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
* \class Zeni::Configurator_Video
*
* \ingroup Zenilib
*
* \brief Configure zenilib.xml
*
* \author bazald
*
* Contact: bazald@zenipex.com
*/

#ifndef CONFIGURATOR_VIDEO_H
#define CONFIGURATOR_VIDEO_H

#include <Zeni/Widget_Gamestate.h>
#include <Zeni/XML.h>

namespace Zeni {

  class Configurator_Video : public Widget_Gamestate {
    Configurator_Video(const Configurator_Video &);
    Configurator_Video & operator=(const Configurator_Video &);

    class Check_Box_Element : public Check_Box {
      Check_Box_Element(const Check_Box_Element &);
      Check_Box_Element & operator=(const Check_Box_Element &);

    public:
      Check_Box_Element(const XML_Element &element,
                        const Point2f &upper_left,
                        const float &height);

      void on_accept();

      void render_impl() const;

    private:
      XML_Element m_element;
    };

    class Slider_Element : public Slider_Int {
      Slider_Element(const Slider_Element &);
      Slider_Element & operator=(const Slider_Element &);

    public:
      Slider_Element(const XML_Element &element,
                     const std::pair<int, int> &range,
                     const Point2f &upper_left,
                     const Point2f &lower_right);

      void on_accept();

      void render_impl() const;

    private:
      XML_Element m_element;
      Point2f m_text_coord;
    };

    class Text_Element : public Text_Box {
      Text_Element(const Text_Element &);
      Text_Element & operator=(const Text_Element &);

    public:
      Text_Element(const XML_Element &element,
                   const Point2f &upper_left,
                   const Point2f &lower_right);

      void on_change();

      void render_impl() const;

    private:
      XML_Element m_element;
    };

    class Selector_Element : public Selector {
      Selector_Element(const Selector_Element &);
      Selector_Element & operator=(const Selector_Element &);

    public:
      Selector_Element(const XML_Element &element,
                       const Point2f &upper_left,
                       const Point2f &lower_right,
                       const Point2f &expanded_upper_left,
                       const Point2f &expanded_lower_right);

      void add_entry(const std::string &option, const std::string &save_as);

      void on_accept(const std::string &option);

    private:
      XML_Element m_element;
      std::map<std::string, std::string> m_save_as;
    };

    class Save_Button : public Text_Button_3C {
      Save_Button(const Save_Button &);
      Save_Button & operator=(const Save_Button &);

    public:
      Save_Button(XML_Document &file,
                  const Point2f &upper_left,
                  const Point2f &lower_right);

      void on_accept();

    private:
      XML_Document * m_file;
    };

    class Cancel_Button : public Text_Button_3C {
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
    void render();

    XML_Document m_file;
    XML_Element m_zenilib;

    Slider_Element m_anisotropy;
    Check_Box_Element m_bilinear_filtering;
    Check_Box_Element m_mipmapping;

    Selector_Element m_api;
    Check_Box_Element m_full_screen;
    Slider_Element m_multisampling;
    Text_Element m_resolution_width;
    Text_Element m_resolution_height;
    Check_Box_Element m_vertical_sync;

    Save_Button m_save;
    Cancel_Button m_cancel;

    std::string m_prev_title;
  };

}

#endif
