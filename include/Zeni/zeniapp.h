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

/** \defgroup Zenilib Zenipex Library
*
* \brief 3D Game Development Framework
*
* This library acts as as frontend to SDL (Simple Directmedia Layer), 
* OpenGL, and Direct3D 9.  It also uses SDL_image, SDL_mixer, SDL_ttf, 
* SDL_gfx, and lib3ds.  It has successfully been compiled using Microsoft 
* Visual Studio 2005 (Express and Professional) and mingw32 in Windows, 
* and using g++ in Linux.
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
*
* All built-in rendering functionality is supported in both OpenGL and 
* Direct3D 9.
*/

/**
* \class Zeni::Gamestate_One
*
* \ingroup Zenilib
*
* \brief Initial Gamestate
*
* This class is close to having the simplest definition possible for a 
* Gamestate in Zenilib.  It is to be used similarly to a main function, 
* in that it should exist only as a launching pad to more meaningful 
* Gamestates.
*
* \note It is not necessary to change the definition of this class to use it for its intended purpose.
*
* \author bazald
*
* Contact: bazald@zenipex.com
*/

#ifndef ZENIAPP_H
#define ZENIAPP_H

#include <Zeni/Game.h>
#include <Zeni/Video.h>

#include <string>
#include <iosfwd>

namespace Zeni {

  class Gamestate_One : public Gamestate_Base {
  public:
    Gamestate_One(const std::vector<std::string> * const args = 0); ///< Your first constructor
    virtual ~Gamestate_One(); ///< Your last destructor, excluding statics, globals, ...

    virtual void perform_logic(); ///< Your game logic, on a frame by frame basis; Should probably be used to instantiate other game states, using some sort of static/global counter
  };

}

#endif
