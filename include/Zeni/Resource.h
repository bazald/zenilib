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

#include <Zeni/Error.h>
#include <Zeni/Hash_Map.h>
#include <Zeni/Singleton.h>
#include <Zeni/String.h>

namespace Zeni {

  class Resource;

#ifdef _WINDOWS
  ZENI_EXT template class ZENI_DLL Singleton<Resource>;
#endif

  class ZENI_DLL Resource : public Singleton<Resource> {
    friend class Singleton<Resource>;

    static Resource * create();

    Resource() {}
    ~Resource() {}

    // Undefined
    Resource(const Resource &);
    Resource & operator=(const Resource &);

  public:
    inline unsigned long assign(); ///< Get a value, unique within this run of the program

  private:
    //Mutex m_mutex;
    static unsigned long m_current;
  };

  ZENI_DLL Resource & get_Resource(); ///< Get access to the singleton.

  struct ZENI_DLL Resource_Init_Failure : public Error {
    Resource_Init_Failure() : Error("Zeni Resource Failed to Initialize Correctly") {}
  };

}

#endif
