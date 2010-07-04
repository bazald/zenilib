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

#ifndef ZENI_MUTEX_HXX
#define ZENI_MUTEX_HXX

#include <Zeni/Mutex.h>
#include <cassert>

namespace Zeni {

  Mutex::Mutex() {
    // Ensure Core is initialized
    get_Core();

    m_impl = SDL_CreateMutex();
//#ifdef _WINDOWS
//    InitializeCriticalSection(&m_impl);
//#else
//    if(pthread_mutex_init(&m_impl, 0))
//      throw Mutex_Init_Failure();
//#endif
  }
  
  Mutex::~Mutex() {
    SDL_DestroyMutex(m_impl);
//#ifdef _WINDOWS
//    DeleteCriticalSection(&m_impl);
//#else
//    if(pthread_mutex_destroy(&m_impl)) {
//      pthread_mutex_destroy(&self_lock);
//      throw Mutex_Destroy_Failure();
//    }
//#endif
  }
  
  void Mutex::lock() {
    if(SDL_mutexP(m_impl))
      throw Mutex_Lock_Failure();
//#ifdef _WINDOWS
//    EnterCriticalSection(&m_impl);
//#else
//    if(pthread_mutex_lock(&m_impl))
//      throw Mutex_Lock_Failure();
//#endif
  }
  
  void Mutex::unlock() {
    if(SDL_mutexV(m_impl))
      throw Mutex_Unlock_Failure();
//#ifdef _WINDOWS
//    LeaveCriticalSection(&m_impl);
//#else
//    if(pthread_mutex_unlock(&m_impl))
//      throw Mutex_Unlock_Failure();
//#endif
  }

  Mutex::Lock::Lock(Mutex &mutex)
    : m_mutex(mutex)
  {
    m_mutex.lock();
  }
  
  Mutex::Lock::~Lock() {
    m_mutex.unlock();
  }

  Condition_Variable::Condition_Variable() {
    // Ensure Core is initialized
    get_Core();

    m_impl = SDL_CreateCond();
//#ifdef _WINDOWS
//    InitializeConditionVariable(&m_impl);
//#else
//    if(pthread_cond_init(&m_impl, 0))
//      throw CV_Init_Failure();
//#endif
  }
  
  Condition_Variable::~Condition_Variable() {
    SDL_DestroyCond(m_impl);
//#ifndef _WINDOWS
//    if(pthread_cond_destroy(&m_impl))
//      throw CV_Destroy_Failure();
//#endif
  }
  
  void Condition_Variable::signal() {
    if(SDL_CondSignal(m_impl))
      throw CV_Signal_Failure();
//#ifdef _WINDOWS
//    WakeConditionVariable(&m_impl);
//#else
//    if(pthread_cond_signal(&m_impl))
//      throw CV_Signal_Failure();
//#endif
  }
  
  void Condition_Variable::broadcast() {
    if(SDL_CondBroadcast(m_impl))
      throw CV_Signal_Failure();
//#ifdef _WINDOWS
//    WakeAllConditionVariable(&m_impl);
//#else
//    if(pthread_cond_broadcast(&m_impl))
//      throw CV_Broadcast_Failure();
//#endif
  }
  
  void Condition_Variable::wait(Mutex::Lock &mutex_lock) {
    const int result = SDL_CondWait(m_impl, mutex_lock.m_mutex.m_impl);
//#ifdef _WINDOWS
//    const bool result = SleepConditionVariableCS(&m_impl, &mutex_lock.m_mutex.m_impl, INFINITE) == 0;
//#else
//    const bool result = pthread_cond_wait(&m_impl, &mutex_lock.m_mutex.m_impl) != 0;
//#endif
    if(result)
      throw CV_Wait_Failure();
  }
  
  void Condition_Variable::wait_timeout(Mutex::Lock &mutex_lock, const unsigned int &ms) {
    const int result = SDL_CondWaitTimeout(m_impl, mutex_lock.m_mutex.m_impl, ms);
//#ifdef _WINDOWS
//    const bool result = SleepConditionVariableCS(&m_impl, &mutex_lock.m_mutex.m_impl, ms) == 0;
//#else
//    const struct timespec ts = {ms / 1000u, 1000u * (ms % 1000u)};
//    const bool result = pthread_cond_timedwait(&m_impl, &mutex_lock.m_mutex.m_impl, &ts) != 0;
//#endif
    if(result)
      throw CV_Wait_Timeout_Failure();
  }

  Semaphore::Semaphore(const unsigned int &count)
    : m_count(count)
  {
  }

  void Semaphore::down() {
    Mutex::Lock lock(m_mutex);

    while(m_count < 1)
      m_positive_count_cond.wait(lock);
  }

  void Semaphore::down_timeout(const unsigned int &ms) {
    Mutex::Lock lock(m_mutex);

    while(m_count < 1)
      m_positive_count_cond.wait_timeout(lock, ms);
  }

  void Semaphore::up() {
    Mutex::Lock lock(m_mutex);

    ++m_count;

    m_positive_count_cond.signal();
  }

  unsigned int Semaphore::count() const {
    Mutex::Lock lock(m_mutex);

    return m_count;
  }

  Semaphore::Down::Down(Semaphore &semaphore) : m_semaphore(semaphore) {
    m_semaphore.down();
  }

  Semaphore::Down::~Down() {
    m_semaphore.up();
  }

}

#endif
