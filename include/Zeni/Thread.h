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
 * \class Zeni::Task
 *
 * \ingroup zenilib
 *
 * \brief A Task to be run in a Runonce_Computation
 *
 * One way of "simply" achieving parallelism.  Best used for background tasks.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Repeatable_Task
 *
 * \ingroup zenilib
 *
 * \brief A Task to be run repeatedly in a Repeatable_Computation
 *
 * One way of "simply" achieving parallelism.  Best used for background tasks.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Thread
 *
 * \ingroup zenilib
 *
 * \brief A Thread
 *
 * If you want to spawn a Thread, this way is as good as any.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Repeatable_Computation
 *
 * \ingroup zenilib
 *
 * \brief The preferred way of running a Task
 *
 * One way of "simply" achieving parallelism.  Best used for background tasks.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Runonce_Computation
 *
 * \ingroup zenilib
 *
 * \brief The preferred way of running a Repeatable_Task
 *
 * One way of "simply" achieving parallelism.  Best used for background tasks.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_THREAD_H
#define ZENI_THREAD_H

#include <Zeni/Mutex.h>

#include <set>

namespace Zeni {

  extern int run_task(void *task_ptr);
  extern int run_repeatable_task(void *task_ptr);

  class Task {
    Task(const Task &rhs);
    Task & operator=(const Task &rhs);
    
    friend class Repeatable_Task;
    friend int run_task(void *task_ptr);
    friend int run_repeatable_task(void *task_ptr);
    
  public:
    typedef std::set<const Task *> Depends;
    
    Task() : status(0), m_done(false) {}
    virtual ~Task() {}
    
    const Depends & dependencies() const {return m_depends;}
    void add_dependency(const Task *dependency) {m_depends.insert(dependency);}
    void remove_dependency(const Task *dependency) {Depends::iterator it = m_depends.find(dependency); if(it != m_depends.end()) m_depends.erase(it);}
    void clear_dependencies() {m_depends.clear();}
    
    bool done() const {Recursive_Mutex::Lock lock(m_done_mutex); return m_done;}
    void set_done(const bool &done_) {Recursive_Mutex::Lock lock(m_done_mutex); m_done = done_;}
    void finish() const {Recursive_Mutex::Lock lock(m_done_mutex); while(!m_done) m_done_condition.wait(lock);}
    
    Recursive_Mutex & done_mutex() const {return m_done_mutex;}
    Condition_Variable & done_condition() const {return m_done_condition;}
    const bool & done_variable() const {return m_done;}
    
    virtual int function() = 0;
    
    int run() {
      if(!m_done) {
        for(Depends::const_iterator it = m_depends.begin(); it != m_depends.end(); ++it)
          (*it)->finish();
        
        status = function();
        
        m_done = true;
        m_done_condition.broadcast();
      }
      
      return status;
    }
    
    int status;
    std::string msg;
    
  private:
    Depends m_depends;
    
    mutable Recursive_Mutex m_done_mutex;
    mutable Condition_Variable m_done_condition;
    bool m_done;
  };
  
#define GUARANTEED_FINISHED_BEGIN(TASK) {Recursive_Mutex::Lock GF_lock(TASK.done_mutex()); {Condition_Variable &GF_condition = TASK.done_condition(); const bool &GF_done = TASK.done_variable(); for(;;) {if(TASK.status) throw Error(TASK.msg); if(GF_done) break; GF_condition.wait(GF_lock);}} {
#define GUARANTEED_FINISHED_END() }}

  class Repeatable_Task : public Task {
    Repeatable_Task(const Repeatable_Task &rhs);
    Repeatable_Task & operator=(const Repeatable_Task &rhs);
    
    friend int run_repeatable_task(void *task_ptr);
    
  public:
    Repeatable_Task() : m_terminated(false) {}
    ~Repeatable_Task() {self_terminate();}
    
    void repeat() {Recursive_Mutex::Lock lock(m_done_mutex); while(!m_done) m_done_condition.wait(lock); if(!m_terminated) {m_done = false; m_continue_condition.signal();}}
    void terminate() {Recursive_Mutex::Lock lock(m_done_mutex); m_terminated = true; m_continue_condition.signal();}
    
    bool terminated() const {return m_terminated;}
    
  protected:
    void self_terminate() {m_terminated = true;}
  
  private:
    Condition_Variable m_continue_condition;
    
    bool m_terminated;
  };

  class Thread {
    Thread(const Thread &rhs);
    Thread & operator=(const Thread &rhs);
    
  public:
    Thread(int (SDLCALL *fn)(void *), void *data, int * const status = 0);
    Thread(Task &task);
    Thread(Repeatable_Task &task);
    ~Thread();
    
    unsigned int get_id();

  private:
    SDL_Thread *m_impl;
    int *m_status;
  };
  
  class Runonce_Computation {
    Runonce_Computation(const Runonce_Computation &rhs);
    Runonce_Computation & operator=(const Runonce_Computation &rhs);
    
  public:
    Runonce_Computation(Task &task) : m_thread(new Thread(run_task, &task)) {}
    ~Runonce_Computation() {delete m_thread;}
    
    void finish() const {delete m_thread; m_thread = 0;}
  
  private:
    mutable Thread *m_thread;
  };
  
  class Repeatable_Computation {
    Repeatable_Computation(const Repeatable_Computation &rhs);
    Repeatable_Computation & operator=(const Repeatable_Computation &rhs);
    
  public:
    Repeatable_Computation(Repeatable_Task &task, const bool &done = false) : m_task(task), m_thread(0) {m_task.set_done(done); m_thread = new Thread(run_repeatable_task, &task);}
    ~Repeatable_Computation() {m_task.terminate(); delete m_thread;}
    
    void repeat() {m_task.repeat();}
    void finish() const {m_task.finish();}
  
  private:
    Repeatable_Task &m_task;
    mutable Thread *m_thread;
  };

}

#endif
