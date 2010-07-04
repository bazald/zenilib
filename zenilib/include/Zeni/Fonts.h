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
 * \class Zeni::Fonts
 *
 * \ingroup zenilib
 *
 * \brief A Font database read in from a file.
 *
 * One can retrieve fonts by name from this singleton class.
 *
 * \note It is important to note that Fonts can potentially take up a lot of memory.  While it is still desirable to name fonts by use rather than actual font name, it may be necessary to reload different font databases to keep memory usage down.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_FONTS_H
#define ZENI_FONTS_H

#include <Zeni/Database.h>
#include <Zeni/Font.h>

#include <vector>

namespace Zeni {

  class Fonts : public Database<Font> {
    // Get reference to only instance;
    friend Fonts & get_Fonts(); ///< Get access to the singleton.

    Fonts();
    ~Fonts();

    // Undefined
    Fonts(const Fonts &);
    Fonts & operator=(const Fonts &);

    virtual Font * load(XML_Element_c &xml_element, const std::string &name, const std::string &filename);
  };

  Fonts & get_Fonts(); ///< Get access to the singleton.

}

#endif
