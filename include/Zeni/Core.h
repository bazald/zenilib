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
 * \class Zeni::Error
 *
 * \ingroup Zenilib
 *
 * \brief The Error Class
 *
 * A simple class for throwing an error message.
 *
 * \note This is also a base class for all error messages in Zenilib.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Core
 *
 * \ingroup Zenilib
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

#include <string>
#include <vector>

namespace Zeni {

  struct Error {
    Error(const std::string &msg_) : msg(msg_) {}
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
    int get_num_joysticks() const; //< Get the number of joysticks attached to the system

    const std::string & get_joystick_name(const int &index) const; //< Get the name of a given joystick
    int get_joystick_num_axes(const int &index) const; //< Get the number of axes for a joystick
    int get_joystick_num_balls(const int &index) const; //< Get the number of balls for a joystick
    int get_joystick_num_hats(const int &index) const; //< Get the number of hats for a joystick
    int get_joystick_num_buttons(const int &index) const; //< Get the number of buttons for a joystick

    void regenerate_joysticks(); //< Reload all joysticks, flusing *all* SDL events and possibly changing 'which' values for joysticks

  private:
    void init_joysticks();
    void quit_joysticks();

    std::vector<std::pair<SDL_Joystick *, std::string> > joystick;
  };

  Core & get_Core(); ///< Get access to the singleton.

  struct Core_Init_Failure : public Error {
    Core_Init_Failure() : Error("Zeni Core Failed to Initialize Correctly") {}
  };

  struct Joystick_Init_Failure : public Error {
    Joystick_Init_Failure() : Error("Zeni Joystick Failed to Initialize Correctly") {}
  };

}

#endif
