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

#ifdef ZENI_DEFINE_H
#undef ZENI_DEFINE_H

// Camera.h
#undef ZENI_DEFAULT_CAMERA_POSITION
#undef ZENI_DEFAULT_CAMERA_ORIENTATION
#undef ZENI_DEFAULT_CAMERA_NEAR_CLIP
#undef ZENI_DEFAULT_CAMERA_FAR_CLIP
#undef ZENI_DEFAULT_CAMERA_FOV
#undef ZENI_DEFAULT_CAMERA_TUNNEL_VISION
#undef ZENI_DEFAULT_FORWARD_VECTOR
#undef ZENI_DEFAULT_UP_VECTOR
#undef ZENI_DEFAULT_LEFT_VECTOR

// Fog.h
#undef ZENI_DEFAULT_FOG_COLOR
#undef ZENI_DEFAULT_FOG_DENSITY
#undef ZENI_DEFAULT_FOG_TYPE
#undef ZENI_DEFAULT_FOG_START
#undef ZENI_DEFAULT_FOG_END

// Material.h
#undef ZENI_DEFAULT_MATERIAL_DIFFUSE
#undef ZENI_DEFAULT_MATERIAL_SPECULAR
#undef ZENI_DEFAULT_MATERIAL_EMISSIVE
#undef ZENI_DEFAULT_MATERIAL_POWER

// Net.h
#undef ZENI_DEFAULT_CHUNK_SIZE
#undef ZENI_DEFAULT_CHUNK_SETS

// Sound_Source.h
#undef ZENI_DEFAULT_PITCH
#undef ZENI_DEFAULT_GAIN
#undef ZENI_DEFAULT_NEAR_CLAMP
#undef ZENI_DEFAULT_FAR_CLAMP
#undef ZENI_DEFAULT_ROLLOFF
#undef ZENI_DEFAULT_SOUND_PRIORITY
#undef ZENI_DEFAULT_MUSIC_PRIORITY

// Widget.h
#undef ZENI_DEFAULT_SLIDER_POSITION
#undef ZENI_DEFAULT_TAB_SPACES
#undef ZENI_DEFAULT_JUSTIFY
#undef ZENI_DEFAULT_TEXTBOX_EDITABLE

// Widget.cpp
#undef ZENI_TEXT_CURSOR_WIDTH

// Video.hxx
#undef ZENI_2D_NEAR
#undef ZENI_2D_FAR

// Video_DX9.hxx
#undef ZENI_STANDARD_DPI

// Collision.cpp
#undef ZENI_COLLISION_EPSILON

// Configurator_Video.cpp
#undef ZENI_REVERT_TIMEOUT

// Font.cpp
#undef MINIMUM_VIRTUAL_SCREEN_HEIGHT
#undef MAXIMUM_VIRTUAL_SCREEN_HEIGHT

// Game.cpp
#undef NASTY_MIN_RATE
#undef NASTY_MAX_RATE
#undef NASTY_RATE_CUTOFF
#undef NASTY_ZERO_STEP_FREQUENCY

// Gamestate.cpp
#undef ZENI_DEFAULT_II_JOYBALL_MIN
#undef ZENI_DEFAULT_II_JOYBALL_MAX
#undef ZENI_DEFAULT_II_JOYSTICK_MIN
#undef ZENI_DEFAULT_II_JOYSTICK_MAX
#undef ZENI_DEFAULT_II_MOUSE_MIN
#undef ZENI_DEFAULT_II_MOUSE_MAX

// Material.cpp
#undef ZENI_DIFFUSE_TO_SPECULAR

// Net_Primitives.cpp
#undef ZENI_SPRINTF_BUFFER_SIZE

// Texture.cpp
#undef ZENI_MAX_TEXTURE_WIDTH
#undef ZENI_MAX_TEXTURE_HEIGHT

// Thread.cpp
#undef NO_ERROR_STATUS
#undef ZENI_ERROR_STATUS
#undef STD_ERROR_STATUS
#undef OTHER_ERROR_STATUS

// Vertex_Buffer.cpp
#undef CLOSENESS_THRESHOLD_SQUARED
#undef ALIKENESS_THRESHOLD
#undef CLOSENESS_THRESHOLD

// Video.cpp
#undef MINIMUM_SCREEN_WIDTH
#undef MINIMUM_SCREEN_HEIGHT

// Nasty Conditions
#undef NASTY_SOUND_SOURCE_CAP

// Miscellaney
#undef END_OF_TIME
#undef INFINTESSIMAL
#undef INFINTESSIMAL_SQUARED

#endif
