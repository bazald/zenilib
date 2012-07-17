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
 * \struct Zeni::Quit_Event
 *
 * \ingroup zenilib
 *
 * \brief A class to signal that the program is quitting.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_QUIT_EVENT_H
#define ZENI_QUIT_EVENT_H

#include <stdexcept>

namespace Zeni {

  struct ZENI_DLL Quit_Event : public std::exception {
    Quit_Event() {fired = true;}

    static void fire() {throw Quit_Event();}
    static bool has_fired() {return fired;}

  private:
    static bool fired;
  };

}

#endif
