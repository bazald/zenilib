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

#ifndef ZENI_GAME_HXX
#define ZENI_GAME_HXX

#include <Zeni/Console_State.h>

// HXXed below
#include <Zeni/Gamestate.h>

#include <Zeni/Game.h>

namespace Zeni {

  Gamestate Game::get_top() {
    if(m_states.empty())
      throw Zero_Gamestate();

    return m_states.top();
  }

  size_t Game::size() const {
    return m_states.size();
  }

  size_t Game::get_fps() const {
    return fps;
  }

}

#include <Zeni/Gamestate.hxx>

#endif
