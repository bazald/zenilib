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

#include <zeni_core.h>

#include <SDL/SDL_thread.h>

namespace Zeni {

  Mutex::Mutex()
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif
    : m_uninit(*this),
    m_reinit(*this)
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  {
    // Ensure Core is initialized
    Core &cr = get_Core();

    m_impl = SDL_CreateMutex();
//#ifdef _WINDOWS
//    InitializeCriticalSection(&m_impl);
//#else
//    if(pthread_mutex_init(&m_impl, 0))
//      throw Mutex_Init_Failure();
//#endif

    cr.lend_pre_uninit(&m_uninit);
    cr.lend_post_reinit(&m_reinit);
  }
  
  Mutex::~Mutex() {
    if(m_impl)
      SDL_DestroyMutex(m_impl);

//#ifdef _WINDOWS
//    DeleteCriticalSection(&m_impl);
//#else
//    if(pthread_mutex_destroy(&m_impl)) {
//      pthread_mutex_destroy(&self_lock);
//      throw Mutex_Destroy_Failure();
//    }
//#endif

    get_Core().remove_pre_uninit(&m_uninit);
    get_Core().remove_post_reinit(&m_reinit);
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

  Recursive_Mutex::Recursive_Mutex()
    : locking_thread(0)
    , count(0)
  {
  }
  
  void Recursive_Mutex::lock() {
    const unsigned int current_thread = SDL_ThreadID();

    {
      Mutex::Lock lock(self_lock);
      
      if(locking_thread == current_thread) {
        ++count;
        return;
      }
    }

    m_impl.lock();

    {
      Mutex::Lock lock(self_lock);

      locking_thread = current_thread;
      count = 1u;
    }
  }
  
  void Recursive_Mutex::unlock() {
    Mutex::Lock lock(self_lock);
    
    assert(locking_thread == SDL_ThreadID() && count);
    
    if(--count)
      return;
    
    locking_thread = 0;

    m_impl.unlock();
  }

  Recursive_Mutex::Lock::Lock(Recursive_Mutex &mutex)
    : m_mutex(mutex)
  {
    m_mutex.lock();
  }
  
  Recursive_Mutex::Lock::~Lock() {
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
  
  void Condition_Variable::wait(Recursive_Mutex::Lock &mutex_lock) {
    Recursive_Mutex &mutex = mutex_lock.m_mutex;
    unsigned int &locking_thread = mutex.locking_thread;
    unsigned int &count = mutex.count;
    
    const unsigned int current_thread = SDL_ThreadID();
    unsigned int current_count;

    {
      Mutex::Lock lock(mutex.self_lock);
      assert(locking_thread == current_thread && count);
      current_count = count;
      count = 0u;
    }

    const int result = SDL_CondWait(m_impl, mutex_lock.m_mutex.m_impl.m_impl);
//#ifdef _WINDOWS
//    const bool result = SleepConditionVariableCS(&m_impl, &mutex_lock.m_mutex.m_impl.m_impl, INFINITE) == 0;
//#else
//    const bool result = pthread_cond_wait(&m_impl, &mutex_lock.m_mutex.m_impl.m_impl) != 0;
//#endif
    if(result)
      throw CV_Wait_Failure();

    {
      Mutex::Lock lock(mutex.self_lock);
      locking_thread = current_thread;
      count = current_count;
    }
  }
  
  void Condition_Variable::wait_timeout(Recursive_Mutex::Lock &mutex_lock, const unsigned int &ms) {
    Recursive_Mutex &mutex = mutex_lock.m_mutex;
    unsigned int &locking_thread = mutex.locking_thread;
    unsigned int &count = mutex.count;
    
    const unsigned int current_thread = SDL_ThreadID();
    unsigned int current_count;
    
    assert(locking_thread == current_thread && count);

    {
      Mutex::Lock lock(mutex.self_lock);
      assert(locking_thread == current_thread && count);
      current_count = count;
      count = 0u;
    }

    const int result = SDL_CondWaitTimeout(m_impl, mutex_lock.m_mutex.m_impl.m_impl, ms);
//#ifdef _WINDOWS
//    const bool result = SleepConditionVariableCS(&m_impl, &mutex_lock.m_mutex.m_impl.m_impl, ms) == 0;
//#else
//    const struct timespec ts = {ms / 1000u, 1000u * (ms % 1000u)};
//    const bool result = pthread_cond_timedwait(&m_impl, &mutex_lock.m_mutex.m_impl.m_impl, &ts) != 0;
//#endif
    if(result)
      throw CV_Wait_Timeout_Failure();
    
    {
      Mutex::Lock lock(mutex.self_lock);
      locking_thread = current_thread;
      count = current_count;
    }
    
    if(result)
      throw CV_Wait_Timeout_Failure();
  }
  
}
