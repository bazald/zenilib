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

#include <Zeni/Configurator_Video.h>

#include <Zeni/Colors.h>
#include <Zeni/Fonts.h>
#include <Zeni/Game.h>
#include <Zeni/Net_Primitives.h>
#include <Zeni/Video.hxx>
#include <Zeni/Widget.hxx>

using namespace std;

namespace Zeni {

  Configurator_Video::Check_Box_Element::Check_Box_Element(const XML_Element &element,
                    const Point2f &upper_left,
                    const float &height)
    : Check_Box(upper_left,
                Point2f(upper_left.x + height, upper_left.y + height),
                get_Colors()["default_button_bg_normal"],
                get_Colors()["default_button_bg_normal"]),
    m_element(element)
  {
    set_checked(m_element.to_bool());
  }

  void Configurator_Video::Check_Box_Element::on_accept() {
    Check_Box::on_accept();
    m_element.set_bool(is_checked());
  }

  void Configurator_Video::Check_Box_Element::render_impl() const {
    Check_Box::render_impl();

    Font &font = get_Fonts()["system_36_600"];

    font.render_text(" " + m_element.value(),
                     Point2f(get_lower_right().x,
                             0.5f * (get_lower_right().y + get_upper_left().y - font.get_text_height())),
                     get_Colors()["default_button_bg_normal"]);
  }

  Configurator_Video::Slider_Element::Slider_Element(const XML_Element &element,
                 const std::pair<int, int> &range,
                 const Point2f &upper_left,
                 const Point2f &lower_right)
    : Slider_Int(range,
                 Point2f(0.9f * upper_left.x + 0.1f * lower_right.x, 0.5f * (upper_left.y + lower_right.y)),
                 Point2f(0.1f * upper_left.x + 0.9f * lower_right.x, 0.5f * (upper_left.y + lower_right.y)),
                 0.2f * (lower_right.x - upper_left.x),
                 get_Colors()["default_button_bg_normal"],
                 get_Colors()["default_button_bg_normal"]),
    m_element(element),
    m_text_coord(lower_right.x,
                 0.5f * (lower_right.y + upper_left.y))
  {
    set_value(m_element.to_int());
  }

  void Configurator_Video::Slider_Element::on_accept() {
    Slider_Int::on_accept();
    m_element.set_int(get_value());
  }

  void Configurator_Video::Slider_Element::render_impl() const {
    Slider::render_impl();

    Font &font = get_Fonts()["system_36_600"];

    font.render_text(" " + m_element.value() + " (" + itoa(get_value()) + ")",
                     Point2f(m_text_coord.x, m_text_coord.y - 0.5f * font.get_text_height()),
                     get_Colors()["default_button_bg_normal"]);
  }

  Configurator_Video::Text_Element::Text_Element(const XML_Element &element,
               const Point2f &upper_left,
               const Point2f &lower_right)
    : Text_Box(upper_left,
               Point2f(lower_right.x - get_Fonts()["system_36_600"].get_text_width(" " + element.value()), lower_right.y),
               get_Colors()["default_button_bg_normal"],
               "system_36_600",
               element.to_string(),
               get_Colors()["default_button_text_normal"],
               true,
               ZENI_CENTER),
    m_element(element)
  {
  }

  void Configurator_Video::Text_Element::on_change() {
    Text_Box::on_change();
    m_element.set_string(get_text());
  }

  void Configurator_Video::Text_Element::render_impl() const {
    Text_Box::render_impl();

    Font &font = get_Fonts()["system_36_600"];

    font.render_text(" " + m_element.value(),
                     Point2f(get_lower_right().x,
                             0.5f * (get_lower_right().y + get_upper_left().y - font.get_text_height())),
                     get_Colors()["default_button_bg_normal"]);
  }

  Configurator_Video::Selector_Element::Selector_Element(const XML_Element &element,
                                                         const Point2f &upper_left,
                                                         const Point2f &lower_right,
                                                         const Point2f &expanded_upper_left,
                                                         const Point2f &expanded_lower_right)
    : Selector(upper_left, lower_right, expanded_upper_left, expanded_lower_right,
               get_Colors()["default_button_bg_normal"], get_Colors()["default_button_bg_clicked"], get_Colors()["default_button_bg_hovered_strayed"],
               "system_36_600",
               get_Colors()["default_button_text_normal"], get_Colors()["default_button_text_clicked"], get_Colors()["default_button_text_hovered_strayed"],
               get_Colors()["default_button_bg_normal"]),
    m_element(element)
  {
  }

  void Configurator_Video::Selector_Element::add_entry(const std::string &option, const std::string &save_as) {
    add_option(option);
    m_save_as[option] = save_as;
  }

  void Configurator_Video::Selector_Element::on_accept(const std::string &option) {
    Selector::on_accept(option);
    m_element.set_string(m_save_as[option]);
  }

  Configurator_Video::Save_Button::Save_Button(XML_Document &file,
              const Point2f &upper_left,
              const Point2f &lower_right)
    : Text_Button_3C(upper_left, lower_right, "system_36_600", "Save"),
    m_file(&file)
  {
  }

  void Configurator_Video::Save_Button::on_accept() {
    Text_Button::on_accept();

    Core &cr = get_Core();
    const std::string appdata_path = cr.get_appdata_path();

    if(cr.create_directory(appdata_path) &&
       cr.create_directory(appdata_path + "config/"))
    {
      m_file->try_save(appdata_path + "config/zenilib.xml");
    }

    m_file->try_save("config/zenilib.xml");

    get_Game().pop_state();
  }

  Configurator_Video::Cancel_Button::Cancel_Button(const Point2f &upper_left,
              const Point2f &lower_right)
    : Text_Button_3C(upper_left, lower_right, "system_36_600", "Cancel")
  {
  }

  void Configurator_Video::Cancel_Button::on_accept() {
    Text_Button::on_accept();
    get_Game().pop_state();
  }

  Configurator_Video::Configurator_Video()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(500.0f, 600.0f))),
    m_file(get_Core().get_appdata_path() + "config/zenilib.xml", "config/zenilib.xml"),
    m_zenilib(m_file["Zenilib"]),

    m_anisotropy(m_zenilib["Textures"]["Anisotropy"], make_pair(0, 16), Point2f(52.0f, 10.0f + 2 * 42.0f), Point2f(52.0f + 100.0f, 10.0f + 2 * 42.0f + 36.0f)),
    m_bilinear_filtering(m_zenilib["Textures"]["Bilinear_Filtering"], Point2f(52.0f, 10.0f + 3 * 42.0f), 36.0f),
    m_mipmapping(m_zenilib["Textures"]["Mipmapping"], Point2f(52.0f, 10.0f + 4 * 42.0f), 36.0f),

    m_api(m_zenilib["Video"]["API"], Point2f(52.0f, 10.0f + 6 * 42.0f), Point2f(375.0f, 10.0f + 6 * 42.0f + 36.0f), Point2f(10.0f, 0.0f), Point2f(395.0f, 600.0f)),
    m_full_screen(m_zenilib["Video"]["Full_Screen"], Point2f(52.0f, 10.0f + 7 * 42.0f), 36.0f),
    m_multisampling(m_zenilib["Video"]["Multisampling"], make_pair(0, 16), Point2f(52.0f, 10.0f + 8 * 42.0f), Point2f(52.0f + 100.0f, 10.0f + 8 * 42.0f + 36.0f)),
    m_resolution_width(m_zenilib["Video"]["Resolution"]["Width"], Point2f(52.0f, 10.0f + 9 * 42.0f), Point2f(52.0f + 200.0f, 10.0f + 9 * 42.0f + 36.0f)),
    m_resolution_height(m_zenilib["Video"]["Resolution"]["Height"], Point2f(294.0f, 10.0f + 9 * 42.0f), Point2f(294.0f + 200.0f, 10.0f + 9 * 42.0f + 36.0f)),
    m_vertical_sync(m_zenilib["Video"]["Vertical_Sync"], Point2f(52.0f, 10.0f + 10 * 42.0f), 36.0f),

    m_save(m_file, Point2f(10.0f, 590.0f - 42.0f - 36.0f), Point2f(10.0f + 200.0f, 590.0f - 42.0f)),
    m_cancel(Point2f(10.0f, 590.0f - 36.0f), Point2f(10.0f + 200.0f, 590.0f)),
    m_prev_title(get_Video().get_title())
  {
    /** Build m_widgets **/

#ifdef _WINDOWS
    m_api.add_entry("Direct3D 9", "DX9");
#endif
    m_api.add_entry("OpenGL", "OpenGL");
#ifdef _WINDOWS
    m_api.select_option(m_zenilib["Video"]["API"].to_string());
#else
    m_api.select_option("OpenGL");
    m_api.set_editable(false);
#endif

    m_widgets.lend_Widget(m_anisotropy);
    m_widgets.lend_Widget(m_bilinear_filtering);
    m_widgets.lend_Widget(m_mipmapping);

    m_widgets.lend_Widget(m_api);
    m_widgets.lend_Widget(m_full_screen);
    m_widgets.lend_Widget(m_multisampling);
    m_widgets.lend_Widget(m_resolution_width);
    m_widgets.lend_Widget(m_resolution_height);
    m_widgets.lend_Widget(m_vertical_sync);

    m_widgets.lend_Widget(m_save);
    m_widgets.lend_Widget(m_cancel);

    /** Set Title **/

    get_Video().set_title("Zenilib Configurator");
  }

  Configurator_Video::~Configurator_Video() {
    get_Video().set_title(m_prev_title);
  }

  void Configurator_Video::render() {
    get_Video().set_2d_view(get_virtual_window(), get_crop_window());

    Font &font = get_Fonts()["system_36_600"];
    const Color color = get_Colors()["system_font"];

    font.render_text("Zenilib Renderer Configuration:", Point2f(10.0f, 10.0f), color);
    font.render_text("Textures:", Point2f(10.0f, 10.0f + 1 * 42.0f), color);
    font.render_text("Video:", Point2f(10.0f, 10.0f + 5 * 42.0f), color);

    m_widgets.render();
  }

}
