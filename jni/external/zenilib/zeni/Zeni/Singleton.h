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

#ifndef ZENI_SINGLETON_H
#define ZENI_SINGLETON_H

#ifdef _WINDOWS
#pragma warning (disable : 4231)
#pragma warning( disable : 4661 )
#endif

#include <Zeni/Event.h>

namespace Zeni {

  template <typename TYPE>
  class Singleton {
    // Undefined
    Singleton(const Singleton &);
    Singleton & operator=(const Singleton &);

  protected:
    Singleton() {}

    virtual ~Singleton() {};

  public:
    static bool is_initialized();

    static TYPE & get();

    static void destroy();
    static void completely_destroy();

    void lend_pre_uninit(Event::Handler * const &handler);
    void give_pre_uninit(Event::Handler * const &handler);
    void fax_pre_uninit(Event::Handler * const &handler);
    static void remove_pre_uninit(Event::Handler * const &handler);

    void lend_post_reinit(Event::Handler * const &handler);
    void give_post_reinit(Event::Handler * const &handler);
    void fax_post_reinit(Event::Handler * const &handler);
    static void remove_post_reinit(Event::Handler * const &handler);

    class Uninit : public Event::Handler {
      void operator()();

      Uninit * duplicate() const;
    };

    class Reinit : public Event::Handler {
      void operator()();

      Reinit * duplicate() const;
    };

  private:
    virtual void uninit();
    virtual void reinit();

    static void clear();

    static TYPE * g_type;

    static Event pre_uninit;
    static Event post_reinit;
  };

  template <typename TYPE>
  TYPE * Singleton<TYPE>::g_type = 0;

  template <typename TYPE>
  Event Singleton<TYPE>::pre_uninit;

  template <typename TYPE>
  Event Singleton<TYPE>::post_reinit;

}

#endif
