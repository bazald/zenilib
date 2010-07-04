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

/**
 * \class Zeni::Resource
 *
 * \ingroup zenilib
 *
 * \brief A Resource Assignement Singleton
 *
 * The Resource Singleton assigns identifiers unique over the runtime of your program.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_RESOURCE_H
#define ZENI_RESOURCE_H

#include <Zeni/Core.h>
#include <Zeni/Hash_Map.h>
#include <Zeni/Mutex.h>

#include <string>

namespace Zeni {

  class Resource {
    // Get reference to only instance;
    friend Resource & get_Resource(); ///< Get access to the singleton.

    Resource();

    // Undefined
    Resource(const Resource &);
    Resource & operator=(const Resource &);

  public:
    inline unsigned long assign(); ///< Get a value, unique within this run of the program

  private:
    Mutex m_mutex;
    unsigned long m_current;
  };

  Resource & get_Resource(); ///< Get access to the singleton.

  struct Resource_Init_Failure : public Error {
    Resource_Init_Failure() : Error("Zeni Resource Failed to Initialize Correctly") {}
  };

}

#endif
