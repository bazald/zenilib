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

#ifndef ZENI_H
#define ZENI_H

#ifdef _MSC_VER
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#ifndef _WINDOWS
#define _WINDOWS
#endif
#if defined(_WIN64) && !defined(X64)
#define X64
#endif
#endif

#ifndef ZENI_DLL
#define ZENI_DLL __declspec(dllimport)
#endif
#ifndef ZENI_EXT
#define ZENI_EXT extern
#endif

#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <Zeni/Android.h>
#include <Zeni/Camera.h>
#include <Zeni/Chronometer.h>
#include <Zeni/Collision.h>
#include <Zeni/Color.h>
#include <Zeni/Colors.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Database.h>
#include <Zeni/File_Ops.h>
#include <Zeni/Hash_Map.h>
#include <Zeni/Matrix4f.h>
#include <Zeni/Quaternion.h>
#include <Zeni/Quit_Event.h>
#include <Zeni/Random.h>
#include <Zeni/Resource.h>
#include <Zeni/Serialization.h>
#include <Zeni/String.h>
#include <Zeni/Timer_HQ.h>
#include <Zeni/Vector3f.h>
#include <Zeni/XML.h>

#include <Zeni/Camera.hxx>
#include <Zeni/Collision.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Coordinate.hxx>
#include <Zeni/Matrix4f.hxx>
#include <Zeni/Quaternion.hxx>
#include <Zeni/Resource.hxx>
#include <Zeni/Timer_HQ.hxx>
#include <Zeni/Vector2f.hxx>
#include <Zeni/Vector3f.hxx>

#endif
