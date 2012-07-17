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

#include <Zeni/Database.h>
#include <Zeni/Error.h>
#include <Zeni/Sound.h>

namespace Zeni {

  class ZENI_AUDIO_DLL Sounds;

#ifdef _WINDOWS
  ZENI_AUDIO_EXT template class ZENI_AUDIO_DLL Singleton<Sounds>;
  ZENI_AUDIO_EXT template class ZENI_AUDIO_DLL Database<Sound_Buffer>;
#endif

  class ZENI_AUDIO_DLL Sounds : public Singleton<Sounds>, public Database<Sound_Buffer> {
    friend class Singleton<Sounds>;

    static Sounds * create();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    static Uninit g_uninit;
    static Reinit g_reinit;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

    Sounds();
    ~Sounds();

    // Undefined
    Sounds(const Sounds &);
    Sounds & operator=(const Sounds &);

    virtual Sound_Buffer * load(XML_Element_c &xml_element, const String &name, const String &filename);
  };

  ZENI_AUDIO_DLL Sounds & get_Sounds(); ///< Get access to the singleton.

}

#endif
