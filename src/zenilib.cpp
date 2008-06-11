/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
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

#include "zenilib.h"

#include "Zeni/Camera.hpp"
#include "Zeni/Collision.hpp"
#include "Zeni/Color.hpp"
#include "Zeni/Colors.hpp"
#include "Zeni/Coordinate.hpp"
#include "Zeni/Core.hpp"
#include "Zeni/EZ2D.hpp"
#include "Zeni/Font.hpp"
#include "Zeni/Fonts.hpp"
#include "Zeni/Game.hpp"
#include "Zeni/Gamestate.hpp"
#include "Zeni/Light.hpp"
#include "Zeni/main.hpp"
#include "Zeni/Material.hpp"
#include "Zeni/Matrix4f.hpp"
#include "Zeni/Model.hpp"
#include "Zeni/Mutex.hpp"
#include "Zeni/Net.hpp"
#include "Zeni/Net_Primitives.hpp"
#include "Zeni/Quaternion.hpp"
#include "Zeni/Render_Wrapper.hpp"
#include "Zeni/Resource.hpp"
#include "Zeni/Sound.hpp"
#include "Zeni/Sounds.hpp"
#include "Zeni/Texture.hpp"
#include "Zeni/Textures.hpp"
#include "Zeni/Thread.hpp"
#include "Zeni/Timer.hpp"
#include "Zeni/Vector3f.hpp"
#include "Zeni/Vertex2f.hpp"
#include "Zeni/Vertex3f.hpp"
#include "Zeni/Vertex_Buffer.hpp"
#include "Zeni/Video.hpp"
#include "Zeni/Video_DX9.hpp"
#include "Zeni/Video_GL.hpp"
#include "Zeni/Widget.hpp"
