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
 * \class Zeni::Window
 *
 * \ingroup zenilib
 *
 * \brief The Window Management Singleton
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_WINDOW_H
#define ZENI_WINDOW_H

#include <Zeni/Core.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Singleton.h>

#include <SDL/SDL.h>

#ifdef ANDROID
#include <EGL/egl.h>
#endif

namespace Zeni {

  class ZENI_GRAPHICS_DLL Window;
  class XML_Document;

#ifdef _WINDOWS
  ZENI_GRAPHICS_EXT template class ZENI_GRAPHICS_DLL Singleton<Window>;
#endif

  class ZENI_GRAPHICS_DLL Window : public Singleton<Window> {
    friend class Singleton<Window>;

    static Window * create();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    static Uninit g_uninit;
    static Reinit g_reinit;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

  protected:
    Window();
    ~Window();

  private:
    // Undefined
    Window(const Window &);
    Window & operator=(const Window &);

  public:
    // Accessors
    inline static const bool & is_enabled(); ///< Determine whether the use of rendering is desired
    inline static const Point2i & get_size(); /// Get the size of the window
    inline static const int & get_width(); ///< Get the width of the window
    inline static const int & get_height(); ///< Get the height of the window
#ifndef ANDROID
    inline static const bool & is_full_screen(); ///< Determine whether the window is windowed or full screen
    inline static const bool & is_frame_visible(); ///< Determine whether the frame is visible (windowed only)
    inline static const bool & is_resizable(); ///< Determine whether the frame is resizable (windowed only)
    inline const std::vector<Point2i> & get_resolutions() const; ///< Get available full screen resolutions

    // Window Decorations
    inline const String & get_title() const; ///< Get the window title
    inline const String & get_taskmsg() const; ///< Get the taskbar message
    static void set_tt(const String &title, const String &taskmsg); ///< Set the window title and taskbar message
    static void set_title(const String &title); ///< Set the window title
    static void set_taskmsg(const String &taskmsg); ///< Set the taskbar message
    static bool set_icon(const String &filename); ///< Set the window icon

    // Mouse Functions
    bool is_mouse_grabbed() const; ///< Find out if the (primary) Window has grabbed the mouse
    bool is_mouse_hidden() const; ///< Find out if the mouse cursor is hidden
    void mouse_grab(const bool &grab); ///< Tell the (primary) Window to grab/ungrab the mouse
    void mouse_hide(const bool &hide); ///< Hide/Unhide the mouse
#endif

    // Enable/Disable use of the Rendering Device
    static void set_enabled(const bool &enabled); ///< Enable/Disable the use of rendering; This will not close the rendering window once it is open

#ifdef ANDROID
    inline EGLDisplay & get_display();
    inline EGLSurface & get_surface();
    inline EGLContext & get_context();
  private:
    EGLDisplay m_display;
    EGLSurface m_surface;
    EGLContext m_context;
#else
    // Call before any other Window functions; May throw Window_Initialized
    static void preinit_resolution(const Point2i &resolution = Point2i(800, 600)); ///< Set the rendering window to have a given resolution
    static void preinit_full_screen(const bool &full_screen = false); ///< Set the rendering window to be full screen at launch
    static void preinit_show_frame(const bool &show_frame_ = true); ///< Show a frame around the rendering window when in windowed mode
    static void preinit_resizable(const bool &resizable_ = true); ///< Allow the frame to be dynamically resized when in windows mode
    static void preinit_from_xml(const XML_Element_c &video); ///< Set rendering options from an XML data structure

    // Re/uninitialize the Rendering Device
    static void set_failsafe_defaults(); ///< Set failsafe default options

    static const bool & get_opengl_flag();
    static void set_opengl_flag(const bool &on = true);

#if SDL_VERSION_ATLEAST(1,3,0)
    inline SDL_Window * get_window(); ///< Get the (primary) window for your application
    virtual void alert_window_destroyed(); ///< Tell Window that its SDL_Window has been destroyed
#endif
    void alert_window_resized(const Point2i &resolution); ///< Tell Window that it has been resized

  protected:
#if !SDL_VERSION_ATLEAST(1,3,0)
    inline SDL_Surface * get_display_surface();
#endif

  private:
    // Set title and taskmsg texts
    void set_tt();
    // Set icon
    bool set_icon();

#if SDL_VERSION_ATLEAST(1,3,0)
    SDL_Window *m_window;
#else
    SDL_Surface *m_display_surface;
#endif
    SDL_Surface *m_icon_surface;

    static bool g_screen_full;
    static bool g_screen_show_frame;
    static bool g_screen_resizable;
    static bool g_opengl_flag;

    static String & get_m_title();
    static String & get_m_taskmsg();
    static String & get_m_icon();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::vector<Point2i> m_modes;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
#endif

    static bool g_enabled;

    static Point2i g_screen_size;
  };

  ZENI_GRAPHICS_DLL Window & get_Window(); ///< Get access to the singleton.

  struct ZENI_GRAPHICS_DLL Window_Init_Failure : public Error {
    Window_Init_Failure() : Error("Zeni Window Failed to Initialize Correctly") {}
  };

}

#endif
