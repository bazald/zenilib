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

#include <Zeni/Mutex.hxx>

namespace Zeni {

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
