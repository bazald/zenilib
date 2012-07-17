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

  class ZENI_GRAPHICS_DLL Fonts;

#ifdef _WINDOWS
  ZENI_GRAPHICS_EXT template class ZENI_GRAPHICS_DLL Singleton<Fonts>;
  ZENI_GRAPHICS_EXT template class ZENI_GRAPHICS_DLL Database<Font>;
#endif

  class ZENI_GRAPHICS_DLL Fonts : public Singleton<Fonts>, public Database<Font> {
    friend class Singleton<Fonts>;

    static Fonts * create();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    static class Lose : public Event::Handler {
      void operator()() {
        get().lose_resources();
      }

      Lose * duplicate() const {
        return new Lose;
      }

    public:
      Lose() {}

      // Undefined
      Lose(const Lose &);
      Lose operator=(const Lose &);
    } g_lose;

    static class Unlose : public Event::Handler {
      void operator()() {
        get().unlose_resources();
      }

      Unlose * duplicate() const {
        return new Unlose;
      }

    public:
      Unlose() {}

      // Undefined
      Unlose(const Unlose &);
      Unlose operator=(const Unlose &);
    } g_unlose;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

    Fonts();
    ~Fonts();

    // Undefined
    Fonts(const Fonts &);
    Fonts & operator=(const Fonts &);

    virtual Font * load(XML_Element_c &xml_element, const String &name, const String &filename);
  };

  ZENI_GRAPHICS_DLL Fonts & get_Fonts(); ///< Get access to the singleton.

}

#endif
