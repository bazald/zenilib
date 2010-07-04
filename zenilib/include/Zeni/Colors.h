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
 * \class Zeni::Colors
 *
 * \ingroup zenilib
 *
 * \brief A Color database read in from a file.
 *
 * One can retrieve colors by name from this singleton class.
 *
 * \note It is more useful to create names based on intended use of a color than it is to create names based on the color itself.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_COLORS_H
#define ZENI_COLORS_H

#include <Zeni/Color.h>
#include <Zeni/Core.h>
#include <Zeni/Database.h>

namespace Zeni {

  class Colors : public Database<Color> {
    // Get reference to only instance;
    friend Colors & get_Colors(); ///< Get access to the singleton.

    Colors();
    ~Colors();

    // Undefined
    Colors(const Colors &);
    Colors & operator=(const Colors &);

    virtual Color * load(XML_Element_c &xml_element, const std::string &name, const std::string &filename);
  };

  Colors & get_Colors(); ///< Get access to the singleton.

}

#endif
