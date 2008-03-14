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

#ifndef ZENI_MUTEX_HXX
#define ZENI_MUTEX_HXX

#include <Zeni/Mutex.h>

namespace Zeni {
  
  void Mutex::lock() {
    if(SDL_mutexP(m_impl))
      throw Mutex_Lock_Failure();
  }
  
  void Mutex::unlock() {
    if(SDL_mutexV(m_impl))
      throw Mutex_Unlock_Failure();
  }
  
  void Semaphore::down() {
    if(SDL_SemWait(m_impl))
      throw Semaphore_Down_Failure();
  }
  
  void Semaphore::try_down() {
    if(SDL_SemTryWait(m_impl))
      throw Semaphore_Try_Down_Failure();
  }
  
  void Semaphore::down_timeout(const unsigned int &ms) {
    if(SDL_SemWaitTimeout(m_impl, ms))
      throw Semaphore_Down_Timeout_Failure();
  }
  
  void Semaphore::up() {
    if(SDL_SemPost(m_impl))
      throw Semaphore_Up_Failure();
  }
  
  unsigned int Semaphore::count() const {
    return SDL_SemValue(m_impl);
  }
  
  void Condition_Variable::signal() {
    if(SDL_CondSignal(m_impl))
      throw CV_Signal_Failure();
  }
  
  void Condition_Variable::broadcast() {
    if(SDL_CondBroadcast(m_impl))
      throw CV_Broadcast_Failure();
  }
  
  void Condition_Variable::wait(Mutex::Lock &mutex_lock) {
    if(SDL_CondWait(m_impl, mutex_lock.m_mutex.m_impl))
      throw CV_Wait_Failure();
  }
  
  void Condition_Variable::wait_timeout(Mutex::Lock &mutex_lock, const unsigned int &ms) {
    if(SDL_CondWaitTimeout(m_impl, mutex_lock.m_mutex.m_impl, ms))
      throw CV_Wait_Timeout_Failure();
  }

}

#endif
