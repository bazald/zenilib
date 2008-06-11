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

#include <Zeni/Mutex.hxx>

namespace Zeni {

  Mutex::Mutex()
    : m_impl(0)
#ifndef NDEBUG
    , self_lock(1)
    , locking_thread(0)
#endif
  {
#ifdef NDEBUG
    // Ensure Core is initialized
    Core::get_reference();
#endif
    
    m_impl = SDL_CreateMutex();
  }
  
  Mutex::~Mutex() {
    SDL_DestroyMutex(m_impl);
  }
  
  void Mutex::lock() {
#ifndef NDEBUG
    const unsigned int current_thread = SDL_ThreadID();
    
    {
      Semaphore::Down down(self_lock);
      assert(locking_thread != current_thread);
    }
#endif
    
    if(SDL_mutexP(m_impl))
      throw Mutex_Lock_Failure();
    
#ifndef NDEBUG
    locking_thread = current_thread;
#endif
  }

  Mutex::Lock::Lock(Mutex &mutex)
    : m_mutex(mutex)
  {
    m_mutex.lock();
  }
  
  Mutex::Lock::~Lock() {
    m_mutex.unlock();
  }

  Recursive_Mutex::Recursive_Mutex()
    : m_impl(0)
    , self_lock(1)
    , locking_thread(0)
    , count(0)
  {
    m_impl = SDL_CreateMutex();
  }
  
  Recursive_Mutex::~Recursive_Mutex() {
    SDL_DestroyMutex(m_impl);
  }
  
  void Recursive_Mutex::lock() {
    const unsigned int current_thread = SDL_ThreadID();
    
    {
      Semaphore::Down down(self_lock);
      
      if(locking_thread == current_thread) {
        ++count;
        return;
      }
    }
    
    if(SDL_mutexP(m_impl))
      throw Mutex_Lock_Failure();
      
    locking_thread = current_thread;
    count = 1u;
  }
  
  void Recursive_Mutex::unlock() {
    Semaphore::Down down(self_lock);
    
    assert(locking_thread == SDL_ThreadID() && count);
    
    if(--count)
      return;
    
    locking_thread = 0;

    if(SDL_mutexV(m_impl))
      throw Mutex_Unlock_Failure();
  }

  Recursive_Mutex::Lock::Lock(Recursive_Mutex &mutex)
    : m_mutex(mutex)
  {
    m_mutex.lock();
  }
  
  Recursive_Mutex::Lock::~Lock() {
    m_mutex.unlock();
  }

  Semaphore::Semaphore(const unsigned int &count)
    : m_impl(0)
  {
    // Ensure Core is initialized
    Core::get_reference();
    
    m_impl = SDL_CreateSemaphore(count);
  }
  
  Semaphore::~Semaphore() {
    SDL_DestroySemaphore(m_impl);
  }

  Semaphore::Down::Down(Semaphore &semaphore) : m_semaphore(semaphore) {
    m_semaphore.down();
  }
  
  Semaphore::Down::~Down() {
    m_semaphore.up();
  }

  Condition_Variable::Condition_Variable()
    : m_impl(0)
  {
    // Ensure Core is initialized
    Core::get_reference();
    
    m_impl = SDL_CreateCond();
  }
  
  Condition_Variable::~Condition_Variable() {
    SDL_DestroyCond(m_impl);
  }
  
  void Condition_Variable::wait(Recursive_Mutex::Lock &mutex_lock) {
    Recursive_Mutex &mutex = mutex_lock.m_mutex;
    unsigned int &locking_thread = mutex.locking_thread;
    unsigned int &count = mutex.count;
    
    const unsigned int current_thread = SDL_ThreadID();
    const unsigned int current_count = count;
    
    assert(locking_thread == current_thread && count);
    
    {
      Semaphore::Down down(mutex.self_lock);
      locking_thread = 0;
      count = 0;
    }
    
    const int result = SDL_CondWait(m_impl, mutex_lock.m_mutex.m_impl);
    
    {
      Semaphore::Down down(mutex.self_lock);
      locking_thread = current_thread;
      count = current_count;
    }
    
    if(result)
      throw CV_Wait_Failure();
  }
  
  void Condition_Variable::wait_timeout(Recursive_Mutex::Lock &mutex_lock, const unsigned int &ms) {
    Recursive_Mutex &mutex = mutex_lock.m_mutex;
    unsigned int &locking_thread = mutex.locking_thread;
    unsigned int &count = mutex.count;
    
    const unsigned int current_thread = SDL_ThreadID();
    const unsigned int current_count = count;
    
    assert(locking_thread == current_thread && count);
    
    {
      Semaphore::Down down(mutex.self_lock);
      locking_thread = 0;
      count = 0;
    }
    
    const int result = SDL_CondWaitTimeout(m_impl, mutex_lock.m_mutex.m_impl, ms);
    
    {
      Semaphore::Down down(mutex.self_lock);
      locking_thread = current_thread;
      count = current_count;
    }
    
    if(result)
      throw CV_Wait_Timeout_Failure();
  }
  
}
