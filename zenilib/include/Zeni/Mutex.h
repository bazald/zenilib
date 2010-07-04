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

/**
 * \class Zeni::Semaphore
 *
 * \ingroup zenilib
 *
 * \brief A simple Semaphore class
 *
 * Just one way of offering mutual exclusion and/or waiting conditions.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Mutex
 *
 * \ingroup zenilib
 *
 * \brief A simple Mutex class
 *
 * Just one way of offering mutual exclusion.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Recursive_Mutex
 *
 * \ingroup zenilib
 *
 * \brief A more sophisticated Mutex class
 *
 * A slower Mutex allowing repeated locks, useful for some tasks but otherwise to be avoided.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Condition_Variable
 *
 * \ingroup zenilib
 *
 * \brief A simple Condition Variable class
 *
 * Just one way of offering waiting conditions.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_MUTEX_H
#define ZENI_MUTEX_H

#include <Zeni/Core.h>

#include <SDL/SDL_mutex.h>
//#ifdef _WINDOWS
//#include <Windows.h>
//#else
//#include <pthread.h>
//#endif

namespace Zeni {

  class Mutex {
    friend class Condition_Variable;
    
    Mutex(const Mutex &rhs);
    Mutex & operator=(const Mutex &rhs);
    
  public:
    inline Mutex();
    inline ~Mutex();
    
    inline void lock();
    inline void unlock();

  private:
    SDL_mutex *m_impl;
//#ifdef _WINDOWS
//    CRITICAL_SECTION m_impl;
//#else
//    pthread_mutex_t m_impl;
//#endif
  
  public:
    class Lock {
      friend class Condition_Variable;
      
      Lock(const Lock &rhs);
      Lock & operator=(const Lock &rhs);
      
    public:
      inline Lock(Mutex &mutex);
      inline ~Lock();
      
    private:
      Mutex &m_mutex;
    };
  };

  class Recursive_Mutex {
    friend class Condition_Variable;
    
    Recursive_Mutex(const Recursive_Mutex &rhs);
    Recursive_Mutex & operator=(const Recursive_Mutex &rhs);
    
  public:
    Recursive_Mutex();
    
    void lock();
    void unlock();

  private:
    Mutex m_impl;
    
    Mutex self_lock;
    unsigned int locking_thread;
    unsigned int count;
  
  public:
    class Lock {
      friend class Condition_Variable;
      
      Lock(const Lock &rhs);
      Lock & operator=(const Lock &rhs);
      
    public:
      Lock(Recursive_Mutex &mutex);
      ~Lock();
      
    private:
      Recursive_Mutex &m_mutex;
    };
  };

  class Condition_Variable {
    Condition_Variable(const Condition_Variable &rhs);
    Condition_Variable & operator=(const Condition_Variable &rhs);
    
  public:
    inline Condition_Variable();
    inline ~Condition_Variable();
    
    inline void signal();
    inline void broadcast();
    
    inline void wait(Mutex::Lock &mutex_lock);
    inline void wait_timeout(Mutex::Lock &mutex_lock, const unsigned int &ms);
    void wait(Recursive_Mutex::Lock &mutex_lock);
    void wait_timeout(Recursive_Mutex::Lock &mutex_lock, const unsigned int &ms);

  private:
    SDL_cond *m_impl;
//#ifdef _WINDOWS
//  CONDITION_VARIABLE m_impl;
//#else
//  pthread_cond_t m_impl;
//#endif
  };

  class Semaphore {
    Semaphore(const Semaphore &rhs);
    Semaphore & operator=(const Semaphore &rhs);
    
  public:
    inline Semaphore(const unsigned int &count = 1);
    
    inline void down();
    inline void down_timeout(const unsigned int &ms);
    
    inline void up();
    
    inline unsigned int count() const;

  private:
    mutable Mutex m_mutex;
    Condition_Variable m_positive_count_cond;
    unsigned int m_count;
  
  public:
    class Down {
      Down(const Down &rhs);
      Down & operator=(const Down &rhs);
      
    public:
      inline Down(Semaphore &semaphore);
      inline ~Down();
      
    private:
      Semaphore &m_semaphore;
    };
  };

  struct Mutex_Init_Failure : public Error {
    Mutex_Init_Failure() : Error("Zeni Mutex Init Failure") {}
  };

  struct Mutex_Destroy_Failure : public Error {
    Mutex_Destroy_Failure() : Error("Zeni Mutex Destroy Failure") {}
  };

  struct Mutex_Lock_Failure : public Error {
    Mutex_Lock_Failure() : Error("Zeni Mutex Lock Failure") {}
  };

  struct Mutex_Unlock_Failure : public Error {
    Mutex_Unlock_Failure() : Error("Zeni Mutex Unlock Failure") {}
  };

  struct Semaphore_Down_Failure : public Error {
    Semaphore_Down_Failure() : Error("Zeni Semaphore Down Failure") {}
  };

  struct Semaphore_Try_Down_Failure : public Error {
    Semaphore_Try_Down_Failure() : Error("Zeni Semaphore Try Down Failure") {}
  };

  struct Semaphore_Down_Timeout_Failure : public Error {
    Semaphore_Down_Timeout_Failure() : Error("Zeni Semaphore Down Timeout Failure") {}
  };

  struct Semaphore_Up_Failure : public Error {
    Semaphore_Up_Failure() : Error("Zeni Semaphore Up Failure") {}
  };

  struct CV_Init_Failure : public Error {
    CV_Init_Failure() : Error("Zeni Condition Variable Init Failure") {}
  };

  struct CV_Destroy_Failure : public Error {
    CV_Destroy_Failure() : Error("Zeni Condition Variable Destroy Failure") {}
  };

  struct CV_Signal_Failure : public Error {
    CV_Signal_Failure() : Error("Zeni Condition Variable Signal Failure") {}
  };

  struct CV_Broadcast_Failure : public Error {
    CV_Broadcast_Failure() : Error("Zeni Condition Variable Broadcast Failure") {}
  };

  struct CV_Wait_Failure : public Error {
    CV_Wait_Failure() : Error("Zeni Condition Variable Wait Failure") {}
  };

  struct CV_Wait_Timeout_Failure : public Error {
    CV_Wait_Timeout_Failure() : Error("Zeni Condition Variable Wait Timeout Failure") {}
  };

}

#endif
