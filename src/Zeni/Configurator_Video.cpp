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

#include <Zeni/Configurator_Video.h>

#include <Zeni/Colors.h>
#include <Zeni/Fonts.h>
#include <Zeni/Game.h>
#include <Zeni/Net_Primitives.h>
#include <Zeni/Video.hxx>
#include <Zeni/Widget.hxx>

#include <Zeni/Global.h>

namespace Zeni {

  Configurator_Video::Check_State::Accept_Button::Accept_Button(Check_State &check_video, const std::pair<Point2f, Point2f> &virtual_screen)
    : Text_Button(Point2f(virtual_screen.second.x - 100.0f, virtual_screen.second.y - 42.0f),
                          Point2f(virtual_screen.second),
                          "system_36_x600",
                          "Accept"),
    m_check_video(check_video)
  {
  }

  void Configurator_Video::Check_State::Accept_Button::on_accept() {
    m_check_video.m_accept = true;
    get_Game().pop_state();
  }

  Configurator_Video::Check_State::Check_State(const bool &failsafe)
    : Widget_Gamestate(std::make_pair(Point2f(), Point2f(600.0f * get_Video().get_screen_width() / get_Video().get_screen_height(), 600.0f))),
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif
    m_accept_button(*this, get_virtual_window()),
#ifdef _WINDOWS
#pragma warning( pop )
#endif
    m_text(Point2f(), get_virtual_window().second, "system_36_x600",
           "Click 'Accept' to save current rendering options.\n"
           "Hit 'Escape' to reject current rendering options.\n"
           "Current rendering options will be rejected in "
#define xstr(s) str(s)
#define str(s) #s
           xstr(ZENI_REVERT_TIMEOUT)
#undef str
#undef xstr
           " seconds.",
           get_Colors()["default_button_bg_normal"], false),
    m_accept(false),
    m_failsafe(failsafe)
  {
    m_widgets.lend_Widget(m_accept_button);
    m_widgets.lend_Widget(m_text);

    m_text.give_BG_Renderer(new Widget_Renderer_Color(get_Colors()["default_button_text_normal"]));
  }

  void Configurator_Video::Check_State::on_pop() {
    Widget_Gamestate::on_pop();

    if(m_accept)
      Video::save();
    else if(m_failsafe) {
      Video::destroy();

      Video::set_failsafe_defaults();

      get_Video();
      get_Textures().unlose_resources();
      get_Fonts().unlose_resources();
      Text_Box::reformat_all();
    }
    else
      Video::reinit();
  }

  void Configurator_Video::Check_State::on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE) {
      if(event.state == SDL_PRESSED)
        get_Game().pop_state();
    }
    else
      Widget_Gamestate::on_key(event);
  }

  void Configurator_Video::Check_State::on_video_resize(const SDL_ResizeEvent &event) {
    Gamestate_Base::on_video_resize(event);

    m_accept = true;

    get_Game().pop_state();
  }

  void Configurator_Video::Check_State::perform_logic() {
    Widget_Gamestate::perform_logic();

    const Time current_time;
    const float seconds_remaining = ZENI_REVERT_TIMEOUT - current_time.get_seconds_since(m_start_time);

    if(seconds_remaining < 0.0f) {
      get_Game().pop_state();
      return;
    }

    m_text.set_text(
      "Click 'Accept' to save current rendering options\n"
      "Hit 'Escape' to reject current rendering options\n"
      "Current rendering options will be rejected in " +
      itoa(int(seconds_remaining + 0.999f)) +
      " seconds.");
  }

  Configurator_Video::Check_Box_Element::Check_Box_Element(const XML_Element &element,
                                                           const bool &checked,
                                                           const Point2f &upper_left,
                                                           const float &height)
    : Check_Box(upper_left,
                Point2f(upper_left.x + height, upper_left.y + height),
                checked),
    m_element(element)
  {
    m_element.set_bool(checked);
  }

  void Configurator_Video::Check_Box_Element::on_accept() {
    Check_Box::on_accept();
    m_element.set_bool(is_checked());
  }

  void Configurator_Video::Check_Box_Element::render_impl() const {
    Check_Box::render_impl();

    Font &font = get_Fonts()["system_36_500x600"];

    font.render_text(" " + m_element.value(),
                     Point2f(get_lower_right().x,
                             0.5f * (get_lower_right().y + get_upper_left().y - font.get_text_height())),
                     get_Colors()["default_button_bg_normal"]);
  }

  Configurator_Video::Slider_Element::Slider_Element(const XML_Element &element,
                                                     const int &value,
                                                     const std::pair<int, int> &range,
                                                     const Point2f &upper_left,
                                                     const Point2f &lower_right)
    : Slider_Int(range,
                 Point2f(0.9f * upper_left.x + 0.1f * lower_right.x, 0.5f * (upper_left.y + lower_right.y)),
                 Point2f(0.1f * upper_left.x + 0.9f * lower_right.x, 0.5f * (upper_left.y + lower_right.y)),
                 0.2f * (lower_right.x - upper_left.x)),
    m_element(element),
    m_text_coord(lower_right.x,
                 0.5f * (lower_right.y + upper_left.y))
  {
    set_value(value);
    m_element.set_int(value);
  }

  void Configurator_Video::Slider_Element::on_accept() {
    Slider_Int::on_accept();
    m_element.set_int(get_value());
  }

  void Configurator_Video::Slider_Element::render_impl() const {
    Slider::render_impl();

    Font &font = get_Fonts()["system_36_500x600"];

    font.render_text(" " + m_element.value() + " (" + itoa(get_value()) + ")",
                     Point2f(m_text_coord.x, m_text_coord.y - 0.5f * font.get_text_height()),
                     get_Colors()["default_button_bg_normal"]);
  }

  Configurator_Video::Text_Element::Text_Element(const XML_Element &element,
                                                 const std::string &text,
                                                 const Point2f &upper_left,
                                                 const Point2f &lower_right)
    : Text_Box(upper_left,
               Point2f(lower_right.x /*- get_Fonts()["system_36_500x600"].get_text_width(" " + element.value())*/, lower_right.y),
               "system_36_500x600",
               text,
               get_Colors()["default_button_text_normal"],
               true,
               ZENI_CENTER),
    m_element(element)
  {
    m_element.set_string(text);
  }

  void Configurator_Video::Text_Element::on_change() {
    Text_Box::on_change();
    m_element.set_string(get_text());
  }

  void Configurator_Video::Text_Element::render_impl() const {
    Text_Box::render_impl();

    //Font &font = get_Fonts()["system_36_500x600"];

    //font.render_text(" " + m_element.value(),
    //                 Point2f(get_lower_right().x,
    //                         0.5f * (get_lower_right().y + get_upper_left().y - font.get_text_height())),
    //                 get_Colors()["default_button_bg_normal"]);
  }

  Configurator_Video::Selector_Element::Selector_Element(const XML_Element &element,
                                                         const Point2f &upper_left,
                                                         const Point2f &lower_right,
                                                         const Point2f &expanded_upper_left,
                                                         const Point2f &expanded_lower_right)
    : Selector(upper_left, lower_right, expanded_upper_left, expanded_lower_right, "system_36_500x600"),
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

  Configurator_Video::Resolution_Element::Resolution_Element(const XML_Element &element,
                                                             const Point2f &upper_left,
                                                             const Point2f &lower_right,
                                                             const Point2f &expanded_upper_left,
                                                             const Point2f &expanded_lower_right)
    : Selector(upper_left, lower_right, expanded_upper_left, expanded_lower_right, "system_36_500x600"),
    m_element(element)
  {
    const std::vector<Point2i> &resolutions = get_Video().get_resolutions();

    for(std::vector<Point2i>::const_iterator it = resolutions.begin(); it != resolutions.end(); ++it)
      add_option(itoa(it->x) + "x" + itoa(it->y));

    // This will be the fall back if the other options do not exist
    select_option(itoa(resolutions.begin()->x) + 'x' + itoa(resolutions.begin()->y));

    // Prefer 800x600
    select_option("800x600");

    // Try the set the current resolution as the selection option
    select_option(itoa(get_Video().get_screen_width()) + 'x' + itoa(get_Video().get_screen_height()));
  }

  void Configurator_Video::Resolution_Element::on_accept(const std::string &option) {
    Selector::on_accept(option);

    const size_t x = option.find('x');

    m_element["Width"].set_string(option.substr(0, x));
    m_element["Height"].set_string(option.substr(x + 1));
  }

  Configurator_Video::Custom_Resolution_Box::Custom_Resolution_Box(Configurator_Video &configurator,
                                                                   const bool &checked,
                                                                   const Point2f &upper_left,
                                                                   const float &height)
    : Check_Box(upper_left,
                Point2f(upper_left.x + height, upper_left.y + height),
                checked),
    m_configurator(configurator)
  {
  }

  void Configurator_Video::Custom_Resolution_Box::apply() {
    if(is_checked()) {
      m_configurator.m_widgets.unlend_Widget(m_configurator.resolution);
      m_configurator.m_widgets.lend_Widget(m_configurator.custom_width);
      m_configurator.m_widgets.lend_Widget(m_configurator.custom_height);

      m_configurator.custom_width.on_change();
      m_configurator.custom_height.on_change();
    }
    else {
      m_configurator.m_widgets.unlend_Widget(m_configurator.custom_width);
      m_configurator.m_widgets.unlend_Widget(m_configurator.custom_height);
      m_configurator.m_widgets.lend_Widget(m_configurator.resolution);

      m_configurator.resolution.on_accept(m_configurator.resolution.get_selected());
    }
  }

  void Configurator_Video::Custom_Resolution_Box::on_accept() {
    Check_Box::on_accept();
    apply();
  }

  void Configurator_Video::Custom_Resolution_Box::render_impl() const {
    Check_Box::render_impl();

    Font &font = get_Fonts()["system_36_500x600"];

    font.render_text(" Custom",
                     Point2f(get_lower_right().x,
                             0.5f * (get_lower_right().y + get_upper_left().y - font.get_text_height())),
                     get_Colors()["default_button_bg_normal"]);
  }

  Configurator_Video::Apply_Button::Apply_Button(XML_Document &file,
                                                 const Point2f &upper_left,
                                                 const Point2f &lower_right)
    : Text_Button(upper_left, lower_right, "system_36_500x600", "Apply"),
    m_file(&file)
  {
  }

  void Configurator_Video::Apply_Button::on_accept() {
    //Core &cr = get_Core();
    //const std::string appdata_path = cr.get_appdata_path();

    //if(cr.create_directory(appdata_path) &&
    //   cr.create_directory(appdata_path + "config/"))
    //{
    //  m_file->try_save(appdata_path + "config/zenilib.xml");
    //}

    //m_file->try_save("config/zenilib.xml");

    XML_Element_c zenilib = (*m_file)["Zenilib"];
    XML_Element_c textures = zenilib["Textures"];
    XML_Element_c video = zenilib["Video"];
    const std::string api = video["API"].to_string();

    Video::preinit_video_mode(
#ifndef DISABLE_DX9
                              api == "DX9" ? Video_Base::ZENI_VIDEO_DX9 :
#endif
#ifndef DISABLE_GL
                              api == "OpenGL" ? Video_Base::ZENI_VIDEO_GL :
#endif
                              Video_Base::ZENI_VIDEO_ANY);
    Video::preinit_screen_resolution(Point2i(video["Resolution"]["Width"].to_int(),
                                             video["Resolution"]["Height"].to_int()));
    Video::preinit_full_screen(video["Full_Screen"].to_bool());
    Video::preinit_multisampling(video["Multisampling"].to_int());
    Video::preinit_vertical_sync(video["Vertical_Sync"].to_bool());

    Video::destroy();

    Textures::set_texturing_mode(textures["Anisotropy"].to_int(),
                                 textures["Bilinear_Filtering"].to_bool(),
                                 textures["Mipmapping"].to_bool());

    get_Textures().unlose_resources();
    get_Fonts().unlose_resources();
    Text_Box::reformat_all();

    get_Game().pop_state();
    get_Game().push_state(new Check_State(false));
  }

  Configurator_Video::Cancel_Button::Cancel_Button(const Point2f &upper_left,
              const Point2f &lower_right)
    : Text_Button(upper_left, lower_right, "system_36_500x600", "Cancel")
  {
  }

  void Configurator_Video::Cancel_Button::on_accept() {
    get_Game().pop_state();
  }

  Configurator_Video::Configurator_Video()
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif
    : Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(500.0f, 600.0f))),
    m_file(get_Core().get_appdata_path() + "config/zenilib.xml", "config/zenilib.xml"),
    m_zenilib(m_file["Zenilib"]),

    anisotropy(m_zenilib["Textures"]["Anisotropy"], Textures::get_anisotropic_filtering(), std::make_pair(0, get_Video().get_maximum_anisotropy()), Point2f(52.0f, 10.0f + 2 * 42.0f), Point2f(52.0f + 100.0f, 10.0f + 2 * 42.0f + 36.0f)),
    bilinear_filtering(m_zenilib["Textures"]["Bilinear_Filtering"], Textures::get_bilinear_filtering(), Point2f(52.0f, 10.0f + 3 * 42.0f), 36.0f),
    mipmapping(m_zenilib["Textures"]["Mipmapping"], Textures::get_mipmapping(), Point2f(52.0f, 10.0f + 4 * 42.0f), 36.0f),

    api(m_zenilib["Video"]["API"], Point2f(52.0f, 10.0f + 6 * 42.0f), Point2f(375.0f, 10.0f + 6 * 42.0f + 36.0f), Point2f(10.0f, 0.0f), Point2f(395.0f, 600.0f)),
    full_screen(m_zenilib["Video"]["Full_Screen"], Video::is_fullscreen(), Point2f(52.0f, 10.0f + 7 * 42.0f), 36.0f),
    multisampling(m_zenilib["Video"]["Multisampling"], Video::get_multisampling(), std::make_pair(0, 16), Point2f(52.0f, 10.0f + 8 * 42.0f), Point2f(52.0f + 100.0f, 10.0f + 8 * 42.0f + 36.0f)),

    resolution(m_zenilib["Video"]["Resolution"], Point2f(52.0f, 10.0f + 9 * 42.0f), Point2f(52.0f + 200.0f, 10.0f + 9 * 42.0f + 36.0f), Point2f(52.0f + 20.0f, 0.0f), Point2f(52.0f + 220.0f, 600.0f)),
    custom_resolution(*this, true, Point2f(294.0, 10.0f + 9 * 42.0f), 36.0f),
    custom_width(m_zenilib["Video"]["Resolution"]["Width"], itoa(Video::get_screen_width()), Point2f(52.0f, 10.0f + 9 * 42.0f), Point2f(52.0f + 80.0f, 10.0f + 9 * 42.0f + 36.0f)),
    custom_height(m_zenilib["Video"]["Resolution"]["Height"], itoa(Video::get_screen_height()), Point2f(52.0f + 120.0f, 10.0f + 9 * 42.0f), Point2f(52.0f + 200.0f, 10.0f + 9 * 42.0f + 36.0f)),

    vertical_sync(m_zenilib["Video"]["Vertical_Sync"], Video::get_vertical_sync(), Point2f(52.0f, 10.0f + 10 * 42.0f), 36.0f),

    save(m_file, Point2f(10.0f, 590.0f - 42.0f - 36.0f), Point2f(10.0f + 200.0f, 590.0f - 42.0f)),
    cancel(Point2f(10.0f, 590.0f - 36.0f), Point2f(10.0f + 200.0f, 590.0f)),
    m_prev_title(get_Video().get_title())
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  {
    /** Build m_widgets **/

#if !defined(DISABLE_DX9) && !defined(DISABLE_GL)
    api.add_entry("Direct3D 9", "DX9");
    api.add_entry("OpenGL", "OpenGL");
    api.select_option(Video::get_video_mode() == Video_Base::ZENI_VIDEO_DX9 ? "DX9" : "OpenGL");
#elif !defined(DISABLE_DX9)
    api.add_entry("Direct3D 9", "DX9");
    api.select_option("DX9");
    api.set_editable(false);
#elif !defined(DISABLE_GL)
    api.add_entry("OpenGL", "OpenGL");
    api.select_option("OpenGL");
    api.set_editable(false);
#else
    m_api.set_editable(false);
#endif

    m_widgets.lend_Widget(anisotropy);
    m_widgets.lend_Widget(bilinear_filtering);
    m_widgets.lend_Widget(mipmapping);

    m_widgets.lend_Widget(api);
    m_widgets.lend_Widget(full_screen);
    m_widgets.lend_Widget(multisampling);

    m_widgets.lend_Widget(custom_resolution);
    {
      const std::string selected = resolution.get_selected();
      const size_t x = selected.find('x');
      if(selected.substr(0, x) == custom_width.get_text() &&
         selected.substr(x + 1) == custom_height.get_text())
      {
        custom_resolution.set_checked(false);
      }
      else {
        custom_resolution.set_checked(true);
      }
      custom_resolution.apply();
    }

    m_widgets.lend_Widget(vertical_sync);

    m_widgets.lend_Widget(save);
    m_widgets.lend_Widget(cancel);

    /** Set Title **/

    get_Video().set_title("zenilib Configurator");
  }

  Configurator_Video::~Configurator_Video() {
    get_Video().set_title(m_prev_title);
  }

  void Configurator_Video::on_push() {
    Widget_Gamestate::on_push();

    get_Video().set_clear_color(get_Colors()["configurator_bg"]);
  }

  void Configurator_Video::on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE) {
      if(event.state == SDL_PRESSED)
        get_Game().pop_state();
    }
    else
      Widget_Gamestate::on_key(event);
  }

  void Configurator_Video::render() {
    get_Video().set_2d(get_virtual_window(), fix_aspect_ratio());

    Font &font = get_Fonts()["system_36_500x600"];
    const Color color = get_Colors()["system_font"];

    font.render_text("zenilib Renderer Configuration:", Point2f(10.0f, 10.0f), color);
    font.render_text("Textures:", Point2f(10.0f, 10.0f + 1 * 42.0f), color);
    font.render_text("Video:", Point2f(10.0f, 10.0f + 5 * 42.0f), color);
    font.render_text("x", Point2f(52.0f + 100.0f, 10.0f + 9 * 42.0f), color, ZENI_CENTER);

    m_widgets.render();
  }

}
