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

#ifndef ZENI_HASH_MAP_H
#define ZENI_HASH_MAP_H

#include <Zeni/String.h>

#ifndef __has_include
#define __has_include(x) 0
#endif

#if __has_include("unordered_map")

#include <unordered_map>

namespace Zeni {

  template <typename Key, typename Ty>
  class Unordered_Map : public std::unordered_map<Key, Ty> {
  };

  template <typename Ty>
  class Unordered_Map<String, Ty> : public std::unordered_map<String, Ty, String::Hash> {
  };

}

#elif (__GNUC__ > 3)

#include <tr1/unordered_map>

namespace Zeni {
    
  template <typename Key, typename Ty>
  class Unordered_Map : public std::tr1::unordered_map<Key, Ty> {
  };
    
  template <typename Ty>
  class Unordered_Map<String, Ty> : public std::tr1::unordered_map<String, Ty, String::Hash> {
  };
    
}

#elif defined(__GNUC__)

#include <ext/hash_map>

namespace Zeni {

  template <typename Key, typename Ty>
  class Unordered_Map : public __gnu_cxx::hash_map<Key, Ty> {
  };

  template <typename Ty>
  class Unordered_Map<String, Ty> : public __gnu_cxx::hash_map<String, Ty, String::Hash> {
  };

}

#else

#include <hash_map>

namespace Zeni {

  template <typename Key, typename Ty>
  class Unordered_Map : public stdext::hash_map<Key, Ty> {
  };

  template <typename Ty>
  class Unordered_Map<String, Ty> : public stdext::hash_map<String, Ty, String::Hash> {
  };

}

#endif
#endif
