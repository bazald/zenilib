/* This file is part of the Zenipex Library.
* Copyleft (C) 2010 Mitchell Keith Bloch a.k.a. bazald
*
* The Zenipex Library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU General Public License as 
* published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
*
* The Zenipex Library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with the Zenipex Library; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
* 02110-1301 USA.
*
* As a special exception, you may use this file as part of a free software
* library without restriction.  Specifically, if other files instantiate
* templates or use macros or inline functions from this file, or you compile
* this file and link it with other files to produce an executable, this
* file does not by itself cause the resulting executable to be covered by
* the GNU General Public License.  This exception does not however
* invalidate any other reasons why the executable file might be covered by
* the GNU General Public License.
*/

#ifndef ZENILIB_H
#define ZENILIB_H

#include <Zeni/Camera.h>
#include <Zeni/Chronometer.h>
#include <Zeni/Collision.h>
#include <Zeni/Color.h>
#include <Zeni/Colors.h>
#include <Zeni/Configurator_Video.h>
#include <Zeni/Console_State.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Core.h>
#include <Zeni/Database.h>
#include <Zeni/EZ2D.h>
#include <Zeni/Fog.h>
#include <Zeni/Font.h>
#include <Zeni/Fonts.h>
#include <Zeni/Game.h>
#include <Zeni/Gamestate.h>
#include <Zeni/Gamestate_One.h>
#include <Zeni/Hash_Map.h>
#include <Zeni/Image.h>
#include <Zeni/Light.h>
#include <Zeni/Line_Segment.h>
#include <Zeni/Logo.h>
#include <Zeni/Material.h>
#include <Zeni/Matrix4f.h>
#include <Zeni/Model.h>
#include <Zeni/Mutex.h>
#include <Zeni/Net.h>
#include <Zeni/Popup_State.h>
#include <Zeni/Projector.h>
#include <Zeni/Quadrilateral.h>
#include <Zeni/Quaternion.h>
#include <Zeni/Random.h>
#include <Zeni/Renderable.h>
#include <Zeni/Resource.h>
#include <Zeni/Shader.h>
#include <Zeni/Sound.h>
#include <Zeni/Sound_AL.h>
#include <Zeni/Sound_Buffer.h>
#include <Zeni/Sound_NULL.h>
#include <Zeni/Sound_Source.h>
#include <Zeni/Sound_Source_Pool.h>
#include <Zeni/Sounds.h>
#include <Zeni/Texture.h>
#include <Zeni/Textures.h>
#include <Zeni/Thread.h>
#include <Zeni/Timer.h>
#include <Zeni/Title_State.h>
#include <Zeni/Triangle.h>
#include <Zeni/Vector3f.h>
#include <Zeni/Vertex2f.h>
#include <Zeni/Vertex3f.h>
#include <Zeni/Vertex_Buffer.h>
#include <Zeni/Video.h>
#include <Zeni/Video_DX9.h>
#include <Zeni/Video_GL.h>
#include <Zeni/Widget.h>
#include <Zeni/Widget_Gamestate.h>
#include <Zeni/XML.h>

#include <Zeni/Camera.hxx>
#include <Zeni/Chronometer.hxx>
#include <Zeni/Collision.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Coordinate.hxx>
#include <Zeni/Database.hxx>
#include <Zeni/Font.hxx>
#include <Zeni/Game.hxx>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Image.hxx>
#include <Zeni/Light.hxx>
#include <Zeni/Line_Segment.hxx>
#include <Zeni/Material.hxx>
#include <Zeni/Matrix4f.hxx>
#include <Zeni/Model.hxx>
#include <Zeni/Mutex.hxx>
#include <Zeni/Net.hxx>
#include <Zeni/Projector.hxx>
#include <Zeni/Quadrilateral.hxx>
#include <Zeni/Quaternion.hxx>
#include <Zeni/Renderable.hxx>
#include <Zeni/Resource.hxx>
#include <Zeni/Shader.hxx>
#include <Zeni/Sound.hxx>
#include <Zeni/Sound_AL.hxx>
#include <Zeni/Sound_Buffer.hxx>
#include <Zeni/Sound_NULL.hxx>
#include <Zeni/Sound_Source.hxx>
#include <Zeni/Texture.hxx>
#include <Zeni/Textures.hxx>
#include <Zeni/Timer.hxx>
#include <Zeni/Triangle.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Vertex2f.hxx>
#include <Zeni/Vertex3f.hxx>
#include <Zeni/Vertex_Buffer.hxx>
#include <Zeni/Video.hxx>
#include <Zeni/Video_DX9.hxx>
#include <Zeni/Video_GL.hxx>
#include <Zeni/Widget.hxx>
#include <Zeni/XML.hxx>

#endif
