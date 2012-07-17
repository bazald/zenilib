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

#ifndef ZENILIB_AUDIO_H
#define ZENILIB_AUDIO_H

#ifndef ZENI_AUDIO_DLL
#define ZENI_AUDIO_DLL __declspec(dllimport)
#endif
#ifndef ZENI_AUDIO_EXT
#define ZENI_AUDIO_EXT extern
#endif

#include <zeni.h>

#include <Zeni/Sound.h>
#include <Zeni/Sound_Buffer.h>
#include <Zeni/Sound_Renderer.h>
#include <Zeni/Sound_Renderer_AL.h>
#include <Zeni/Sound_Renderer_SLES.h>
#include <Zeni/Sound_Source.h>
#include <Zeni/Sound_Source_Pool.h>
#include <Zeni/Sounds.h>

#include <Zeni/Sound.hxx>
#include <Zeni/Sound_Source.hxx>

#endif
