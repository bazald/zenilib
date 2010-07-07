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

/**
 * \class Zeni::Error
 *
 * \ingroup zenilib
 *
 * \brief The Error Class
 *
 * A simple class for throwing an error message.
 *
 * \note This is also a base class for all error messages in zenilib.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Core
 *
 * \ingroup zenilib
 *
 * \brief The Core Singleton
 *
 * The Core singleton essentially initializes and uninitializes SDL.  
 * It is depended upon by most other classes in some way.
 *
 * \note You almost definitely don't need to interact directly with this class.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_CORE_H
#define ZENI_CORE_H

#include <SDL/SDL.h>

#include <cassert>
#include <string>
#include <vector>

namespace Zeni {

  struct Error {
    Error(const std::string &msg_) : msg(msg_) {
#ifdef _WINDOWS
      assert("Error! - Set a Breakpoint Here to Help Your Debugger Work Correctly" && false);
#endif
    }
    std::string msg;
  };

  class Core {
    // Get reference to only instance; Might throw Core_Init_Failure
    friend Core & get_Core(); ///< Get access to the singleton.

    Core();
    ~Core();

    // Undefined
    Core(const Core &);
    Core & operator=(const Core &);

  public:
    static const std::string & get_uniqname(); ///< Get the unique app identifier for the game, set int zenilib.xml
    const std::string & get_username(); ///< Get the logged-in user's username
    std::string get_appdata_path(); ///< Get the path that should be used for user-modifiable storage

    static bool create_directory(const std::string &directory_path); ///< Create a directory if it doesn't already exist; It is not considered an error if it already exists.
    static bool remove_directory(const std::string &directory_path); ///< Remove a directory if it exists; It is not considered an error if it does not exist.

    static bool file_exists(const std::string &file_path); ///< Test for the existence of a file
    static bool delete_file(const std::string &file_path); ///< Delete a file
    static bool copy_file(const std::string &from, const std::string &to); ///< Copy a file from one filepath to another

    bool is_screen_saver_enabled(); ///< Check to see if the screen saver is enabled
    void set_screen_saver(const bool &enabled); ///< Enable/Disable the screen saver

    size_t get_num_joysticks() const; ///< Get the number of joysticks attached to the system

    const std::string & get_joystick_name(const size_t &index) const; ///< Get the name of a given joystick
    int get_joystick_num_axes(const size_t &index) const; ///< Get the number of axes for a joystick
    int get_joystick_num_balls(const size_t &index) const; ///< Get the number of balls for a joystick
    int get_joystick_num_hats(const size_t &index) const; ///< Get the number of hats for a joystick
    int get_joystick_num_buttons(const size_t &index) const; ///< Get the number of buttons for a joystick

    void regenerate_joysticks(); ///< Reload all joysticks, flusing *all* SDL events and possibly changing 'which' values for joysticks

    // Can be called once only, and only before Core is initialized; May throw Core_Initialized
    static void preinit(const std::string &unique_app_identifier);

    static void assert_no_error(); ///< If there is an SDL error, print it and assert(false)
    static bool print_error(); ///< If there is an SDL error, print it and then clear it; returns true iff there was an error

  private:
    void init_joysticks();
    void quit_joysticks();

    static std::string & get_unique_app_identifier();

    std::string m_username;
    std::string m_appdata_path;

    std::vector<std::pair<SDL_Joystick *, std::string> > m_joystick;
  };

  Core & get_Core(); ///< Get access to the singleton.

  struct Core_Init_Failure : public Error {
    Core_Init_Failure() : Error("Zeni Core Failed to Initialize Correctly") {}
  };

  struct Core_Initialized : public Error {
    Core_Initialized() : Error("Zeni Core Already Initialized") {}
  };

  struct Joystick_Init_Failure : public Error {
    Joystick_Init_Failure() : Error("Zeni Joystick Failed to Initialize Correctly") {}
  };

}

#endif
