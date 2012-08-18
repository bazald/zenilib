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
 
 /** \defgroup zenilib
*
* \brief Game Development Framework
*
* This library acts as as frontend to SDL (Simple Directmedia Layer), 
* OpenGL, and Direct3D 9.  It also uses GLEW, libpng, FreeType2, lib3ds, 
* OpenAL, libogg, libvorbis, and TinyXml.
* 
* For more information, please visit zenilib.com.
*
* Featurelist:
* Cameras
* Fonts
* Gamestates and a Game Loop
* Dynamic Lighting and Materials
* Model Loading and Rendering
* Sound Loading and Playing
* Texturing
* Timers
* ...and more!
*
* All built-in rendering functionality is supported in both OpenGL and 
* Direct3D 9.
*/

#ifndef ZENILIB_H
#define ZENILIB_H

#include <zeni.h>
#include <zeni_audio.h>
#include <zeni_core.h>
#include <zeni_graphics.h>
#ifndef TEMP_DISABLE
#include <zeni_net.h>
#endif
#include <zeni_rest.h>

#endif
