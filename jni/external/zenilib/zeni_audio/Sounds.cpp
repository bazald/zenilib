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

#include <zeni_audio.h>

#include <iostream>
#include <fstream>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Database.hxx>
#include <Zeni/Singleton.hxx>

namespace Zeni {

  template class Singleton<Sounds>;
  template class Database<Sound_Buffer>;

  Sounds * Sounds::create() {
    return new Sounds;
  }

  Singleton<Sounds>::Uninit Sounds::g_uninit;
  Singleton<Sounds>::Reinit Sounds::g_reinit;

  Sounds::Sounds()
    : Database<Sound_Buffer>("config/sounds.xml", "Sounds")
  {
    Sound::remove_post_reinit(&g_reinit);

    Sound &sr = get_Sound();

    init();

    sr.lend_pre_uninit(&g_uninit);
    sr.lend_post_reinit(&g_reinit);
  }

  Sounds::~Sounds() {
    Sound::remove_pre_uninit(&g_uninit);

    Database<Sound_Buffer>::uninit();
  }

  Sounds & get_Sounds() {
    return Sounds::get();
  }

  Sound_Buffer * Sounds::load(XML_Element_c &xml_element, const String &/*name*/, const String &/*filename*/) {
    const String filepath = xml_element["filepath"].to_string();

    return new Sound_Buffer(filepath);
  }

}
