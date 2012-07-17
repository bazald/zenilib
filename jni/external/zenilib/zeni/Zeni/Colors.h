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
#include <Zeni/Database.h>
#include <Zeni/Singleton.h>

namespace Zeni {

  class ZENI_DLL Colors;

#ifdef _WINDOWS
  ZENI_EXT template class ZENI_DLL Singleton<Colors>;
  ZENI_EXT template class ZENI_DLL Database<Color>;
#endif

  class ZENI_DLL Colors : public Singleton<Colors>, public Database<Color> {
    friend class Singleton<Colors>;

    static Colors * create();

    Colors();
    ~Colors();

    // Undefined
    Colors(const Colors &);
    Colors & operator=(const Colors &);

    virtual Color * load(XML_Element_c &xml_element, const String &name, const String &filename);
  };

  ZENI_DLL Colors & get_Colors(); ///< Get access to the singleton.

}

#endif
