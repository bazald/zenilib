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
 * \class Zeni::Sounds
 *
 * \ingroup zenilib
 *
 * \brief A Sound_Buffer Database Singleton
 *
 * The Sounds Singleton stores Sound_Buffers to be played from Sound_Sources.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_SOUNDS_H
#define ZENI_SOUNDS_H

#include <Zeni/Core.h>
#include <Zeni/Database.h>
#include <Zeni/Sound.h>

#include <string>

namespace Zeni {

  class Sounds : public Database<Sound_Buffer> {
    // Get reference to only instance;
    friend Sounds & get_Sounds(); ///< Get access to the singleton.

    Sounds();
    ~Sounds();

    // Undefined
    Sounds(const Sounds &);
    Sounds & operator=(const Sounds &);

    virtual Sound_Buffer * load(XML_Element_c &xml_element, const std::string &name, const std::string &filename);
  };

  Sounds & get_Sounds(); ///< Get access to the singleton.

}

#endif
