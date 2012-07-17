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

#ifndef ZENILIB_REST_H
#define ZENILIB_REST_H

#ifndef ZENI_REST_DLL
#define ZENI_REST_DLL __declspec(dllimport)
#endif
#ifndef ZENI_REST_EXT
#define ZENI_REST_EXT extern
#endif

#include <zeni_core.h>
#include <zeni_audio.h>
#include <zeni_graphics.h>

#include <Zeni/Configurator_Video.h>
#include <Zeni/Console_State.h>
#include <Zeni/Game.h>
#include <Zeni/Gamestate.h>
#include <Zeni/Gamestate_II.h>
#include <Zeni/Logo.h>
#include <Zeni/Popup_State.h>
#include <Zeni/Title_State.h>
#include <Zeni/Widget.h>
#include <Zeni/Widget_Gamestate.h>

#include <Zeni/Game.hxx>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Gamestate_II.hxx>
#include <Zeni/Widget.hxx>

ZENI_REST_DLL int zenilib_main(int argc, char **argv);

#endif
