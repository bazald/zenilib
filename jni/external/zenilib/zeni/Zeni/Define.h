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

#ifndef ZENI_DEFINE_H
#define ZENI_DEFINE_H

// Camera.h
#define ZENI_DEFAULT_CAMERA_POSITION      (Point3f(0.0f, 0.0f, 0.0f))
#define ZENI_DEFAULT_CAMERA_ORIENTATION   Quaternion()
#define ZENI_DEFAULT_CAMERA_NEAR_CLIP     (10.0f)
#define ZENI_DEFAULT_CAMERA_FAR_CLIP      (1000.0f)
#define ZENI_DEFAULT_CAMERA_FOV           (Global::pi / 2)
#define ZENI_DEFAULT_CAMERA_TUNNEL_VISION (1.0f)
#define ZENI_DEFAULT_FORWARD_VECTOR       (Vector3f(1.0f, 0.0f, 0.0f))
#define ZENI_DEFAULT_UP_VECTOR            (Vector3f(0.0f, 0.0f, 1.0f))
#define ZENI_DEFAULT_LEFT_VECTOR          (Vector3f(0.0f, 1.0f, 0.0f) /* UP % FORWARD */)

// Fog.h
#define ZENI_DEFAULT_FOG_COLOR   (Color(1.0f, 1.0f, 1.0f, 1.0f))
#define ZENI_DEFAULT_FOG_DENSITY (0.01f)
#define ZENI_DEFAULT_FOG_TYPE    (FOG_EXP)
#define ZENI_DEFAULT_FOG_START   (10.0f)
#define ZENI_DEFAULT_FOG_END     (1000.0f)

// Material.h
#define ZENI_DEFAULT_MATERIAL_DIFFUSE  (Color(1.0f, 1.0f, 1.0f, 1.0f))
#define ZENI_DEFAULT_MATERIAL_SPECULAR (Color(1.0f, 0.2f, 0.2f, 0.2f))
#define ZENI_DEFAULT_MATERIAL_EMISSIVE (Color(1.0f, 0.0f, 0.0f, 0.0f))
#define ZENI_DEFAULT_MATERIAL_POWER    (1.0f)

// Net.h
#define ZENI_DEFAULT_CHUNK_SIZE (64u)
#define ZENI_DEFAULT_CHUNK_SETS (64u)

// Sound_Source.h
#define ZENI_DEFAULT_PITCH              (1.0f)
#define ZENI_DEFAULT_GAIN               (1.0f)
#define ZENI_DEFAULT_REFERENCE_DISTANCE (10.0f)
#define ZENI_DEFAULT_MAX_SOUND_DISTANCE (1000.0f)
#define ZENI_DEFAULT_ROLLOFF            (1.0f)
#define ZENI_DEFAULT_SOUND_PRIORITY     (1024)
#define ZENI_DEFAULT_MUSIC_PRIORITY     (4096)

// Widget.h
#define ZENI_DEFAULT_SLIDER_POSITION  (0.5f)
#define ZENI_DEFAULT_TAB_SPACES       (5)
#define ZENI_DEFAULT_JUSTIFY          (ZENI_LEFT)
#define ZENI_DEFAULT_TEXTBOX_EDITABLE (false)

// Widget.cpp
#define ZENI_TEXT_CURSOR_WIDTH (0.05f)

// Video.hxx
#define ZENI_2D_NEAR (-1.0f)
#define ZENI_2D_FAR  (1.0f)

// Video_DX9.hxx
#define ZENI_STANDARD_DPI (96.0f)

// Collision.cpp
#define ZENI_COLLISION_EPSILON (0.0001f)

// Configurator_Video.cpp
#define ZENI_REVERT_TIMEOUT 15

// Font.cpp
#define MINIMUM_VIRTUAL_SCREEN_HEIGHT (240.0f)
#define MAXIMUM_VIRTUAL_SCREEN_HEIGHT (9600.0f)

// Game.cpp
#define NASTY_MIN_RATE (0.5f)
#define NASTY_MAX_RATE (2.0f)
#define NASTY_RATE_CUTOFF (10)
#define NASTY_ZERO_STEP_FREQUENCY (10)

// Gamestate.cpp
#define ZENI_DEFAULT_II_JOYBALL_MIN (1)
#define ZENI_DEFAULT_II_JOYBALL_MAX (100)
#define ZENI_DEFAULT_II_JOYSTICK_MIN (0.1f)
#define ZENI_DEFAULT_II_JOYSTICK_MAX (1.0f)
#define ZENI_DEFAULT_II_MOUSE_MIN (1)
#define ZENI_DEFAULT_II_MOUSE_MAX (100)

// Material.cpp
#define ZENI_DIFFUSE_TO_SPECULAR(d) (Color(d.a, 0.5f * d.r + 0.5f, 0.5f * d.g + 0.5f, 0.5f * d.b + 0.5f))

// Net_Primitives.cpp
#define ZENI_SPRINTF_BUFFER_SIZE (64)

// Texture.cpp
#define ZENI_MAX_TEXTURE_WIDTH (2048)
#define ZENI_MAX_TEXTURE_HEIGHT (2048)

// Thread.cpp
#define NO_ERROR_STATUS    (1)
#define ZENI_ERROR_STATUS  (0x32202)
#define STD_ERROR_STATUS   (0x253322)
#define OTHER_ERROR_STATUS (-1)

// Vertex_Buffer.cpp
#define CLOSENESS_THRESHOLD_SQUARED (0.00001f)
#define ALIKENESS_THRESHOLD         (0.95f)
#define CLOSENESS_THRESHOLD         (0.001f)

// Video.cpp
#define MINIMUM_SCREEN_WIDTH  (320)
#define MINIMUM_SCREEN_HEIGHT (200)

// Nasty Conditions
#define NASTY_SOUND_SOURCE_CAP (24lu)

// Miscellaney
#define END_OF_TIME (42)
#define INFINTESSIMAL(x) (fabs(x) < 0.001f)
#define INFINTESSIMAL_SQUARED(x) ((x) < 0.0001f)

#endif
