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

#ifndef ZENI_SINGLETON_HXX
#define ZENI_SINGLETON_HXX

#include <Zeni/Singleton.h>

namespace Zeni {

  template <typename TYPE>
  bool Singleton<TYPE>::is_initialized() {
    return g_type != 0;
  }

  template <typename TYPE>
  TYPE & Singleton<TYPE>::get() {
    if(!g_type) {
      g_type = TYPE::create();

      post_reinit.fire();
    }

    return *g_type;
  }
  
  template <typename TYPE>
  void Singleton<TYPE>::destroy() {
    pre_uninit.fire();

    delete g_type;
    g_type = 0;
  }
    
  template <typename TYPE>
  void Singleton<TYPE>::completely_destroy() {
    destroy();
    clear();
  }
  
  template <typename TYPE>
  void Singleton<TYPE>::lend_pre_uninit(Event::Handler * const &handler) {
    pre_uninit.lend_Handler(handler);
  }

  template <typename TYPE>
  void Singleton<TYPE>::give_pre_uninit(Event::Handler * const &handler) {
    pre_uninit.give_Handler(handler);
  }

  template <typename TYPE>
  void Singleton<TYPE>::fax_pre_uninit(Event::Handler * const &handler) {
    pre_uninit.fax_Handler(handler);
  }

  template <typename TYPE>
  void Singleton<TYPE>::remove_pre_uninit(Event::Handler * const &handler) {
    pre_uninit.remove_Handler(handler);
  }
  
  template <typename TYPE>
  void Singleton<TYPE>::lend_post_reinit(Event::Handler * const &handler) {
    post_reinit.lend_Handler(handler);
  }
  
  template <typename TYPE>
  void Singleton<TYPE>::give_post_reinit(Event::Handler * const &handler) {
    post_reinit.give_Handler(handler);
  }
  
  template <typename TYPE>
  void Singleton<TYPE>::fax_post_reinit(Event::Handler * const &handler) {
    post_reinit.fax_Handler(handler);
  }
  
  template <typename TYPE>
  void Singleton<TYPE>::remove_post_reinit(Event::Handler * const &handler) {
    post_reinit.remove_Handler(handler);
  }
  
  template <typename TYPE>
  void Singleton<TYPE>::uninit() {
    pre_uninit.fire();
  }
  
  template <typename TYPE>
  void Singleton<TYPE>::reinit() {
    post_reinit.fire();
  }
  
  template <typename TYPE>
  void Singleton<TYPE>::Uninit::operator()() {
    TYPE::destroy();
  }
  
  template <typename TYPE>
  typename Singleton<TYPE>::Uninit * Singleton<TYPE>::Uninit::duplicate() const {
    return new Uninit;
  }

  template <typename TYPE>
  void Singleton<TYPE>::Reinit::operator()() {
    TYPE::get();
  }
    
  template <typename TYPE>
  typename Singleton<TYPE>::Reinit * Singleton<TYPE>::Reinit::duplicate() const {
    return new typename Singleton<TYPE>::Reinit;
  }
  
  template <typename TYPE>
  void Singleton<TYPE>::clear() {
    pre_uninit.clear();
    post_reinit.clear();
  }

}

#endif
