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

#ifndef ZENI_HASH_MAP_H
#define ZENI_HASH_MAP_H

#include <string>

#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 2)) && defined(__GXX_EXPERIMENTAL_CXX0X__)

#include <unordered_map>

namespace Zeni {

  template <typename Key, typename Ty>
  class Unordered_Map : public std::unordered_map<Key, Ty> {
  };

}

#elif defined(__GNUC__)

#include <ext/hash_map>

namespace __gnu_cxx {
  template<> struct hash<std::string> {
    size_t operator()(const std::string& s) const {
      const std::collate<char>& c = std::use_facet<std::collate<char> >(std::locale::classic());
      return c.hash(s.c_str(), s.c_str() + s.size());
    }
  };
}

namespace Zeni {

  template <typename Key, typename Ty>
  class Unordered_Map : public __gnu_cxx::hash_map<Key, Ty> {
  };

}

#else

#include <hash_map>

namespace Zeni {

  template <typename Key, typename Ty>
  class Unordered_Map : public stdext::hash_map<Key, Ty> {
  };

}

#endif
#endif
