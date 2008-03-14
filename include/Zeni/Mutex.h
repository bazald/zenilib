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

#ifndef ZENI_MUTEX_H
#define ZENI_MUTEX_H

namespace Zeni {

  class Mutex {
    friend class Condition_Variable;
    
    Mutex(const Mutex &rhs);
    Mutex & operator=(const Mutex &rhs);
    
  public:
    Mutex();
    ~Mutex();
    
    inline void lock();
    
    inline void unlock();

  private:
    SDL_mutex *m_impl;
  
  public:
    class Lock {
      friend class Condition_Variable;
      
      Lock(const Lock &rhs);
      Lock & operator=(const Lock &rhs);
      
    public:
      Lock(Mutex &mutex);
      ~Lock();
      
    private:
      Mutex &m_mutex;
    };
  };

  class Semaphore {
    Semaphore(const Semaphore &rhs);
    Semaphore & operator=(const Semaphore &rhs);
    
  public:
    Semaphore(const unsigned int &count = 1);
    ~Semaphore();
    
    inline void down();
    inline void try_down();
    inline void down_timeout(const unsigned int &ms);
    
    inline void up();
    
    inline unsigned int count() const;

  private:
    SDL_sem *m_impl;
  
  public:
    class Down {
      Down(const Down &rhs);
      Down & operator=(const Down &rhs);
      
    public:
      Down(Semaphore &semaphore);
      ~Down();
      
    private:
      Semaphore &m_semaphore;
    };
  };

  class Condition_Variable {
    Condition_Variable(const Condition_Variable &rhs);
    Condition_Variable & operator=(const Condition_Variable &rhs);
    
  public:
    Condition_Variable();
    ~Condition_Variable();
    
    inline void signal();
    inline void broadcast();
    
    inline void wait(Mutex::Lock &mutex_lock);
    inline void wait_timeout(Mutex::Lock &mutex_lock, const unsigned int &ms);

  private:
    SDL_cond *m_impl;
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

#ifdef ZENI_INLINES
#include <Zeni/Mutex.hxx>
#endif

#endif
