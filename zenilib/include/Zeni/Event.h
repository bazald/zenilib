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

#ifndef ZENI_EVENT_H
#define ZENI_EVENT_H

#include <Zeni/Hash_Map.h>

namespace Zeni {

  class ZENI_DLL Event {
  public:
    class ZENI_DLL Handler {
    public:
      virtual ~Handler() {}

      virtual void operator()() = 0;

      virtual Handler * duplicate() const = 0;
    };

    typedef Unordered_Map<Handler *, bool> Handlers;

    ~Event() {
      clear();
    }

    void lend_Handler(Handler * const &handler) {
      m_handlers[handler] = false;
    }

    void give_Handler(Handler * const &handler) {
      m_handlers[handler] = true;
    }

    void fax_Handler(Handler * const &handler) {
      give_Handler(handler->duplicate());
    }

    void remove_Handler(Handler * const &handler) {
      Handlers::iterator ht = m_handlers.find(handler);

      if(ht != m_handlers.end()) {
        if(ht->second)
          delete ht->first;

        m_handlers.erase(ht);
      }
    }

    void fire() {
      const Handlers m_copy = m_handlers;
      for(Handlers::const_iterator ht = m_copy.begin(), hend = m_copy.end(); ht != hend; ++ht)
        (*ht->first)();
    }

    void clear() {
      for(Handlers::const_iterator ht = m_handlers.begin(), hend = m_handlers.end(); ht != hend; ++ht)
        if(ht->second)
          delete ht->first;

      m_handlers.clear();
    }

  private:
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    Handlers m_handlers;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  };

}

#endif
