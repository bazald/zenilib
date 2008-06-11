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

#include <Zeni/Thread.h>

#include <Zeni/Gamestate.h>

#include <Zeni/Mutex.hxx>

namespace Zeni {

  int run_task(void *task_ptr) {
    Task &task = *reinterpret_cast<Task *>(task_ptr);
    Recursive_Mutex::Lock lock(task.m_done_mutex);
    
    try {
      task.status = task.run();
      task.m_done = true;
    }
    catch(Quit_Event &nonerror) {
      task.status = 1;
      task.msg = nonerror.msg;
    }
    catch(Error &error) {
      task.status = 0x32202;
      task.msg = error.msg;
    }
    catch(...) {
      task.status = -1;
      task.msg = "Unknown Error (Not of Type 'Zeni::Error')";
    }
    
    return task.status;
  }
  
  int run_repeatable_task(void *task_ptr) {
    Repeatable_Task &task = *reinterpret_cast<Repeatable_Task *>(task_ptr);
    Recursive_Mutex::Lock lock(task.m_done_mutex);
    
    while(!task.status && !task.m_terminated) {
      if(task.m_done) {
        task.m_continue_condition.wait(lock);
        continue;
      }
      
      try {
        task.status = task.run();
      }
      catch(Quit_Event &nonerror) {
        task.m_done = task.m_terminated = true;
        task.status = 0;
        task.msg = nonerror.msg;
        return task.status;
      }
      catch(Error &error) {
        task.m_done = task.m_terminated = true;
        task.status = 0x32202;
        task.msg = error.msg;
        return task.status;
      }
      catch(...) {
        task.m_done = task.m_terminated = true;
        task.status = -1;
        task.msg = "Unknown Error (Not of Type 'Zeni::Error')";
        return task.status;
      }
      
      if(task.status || task.m_terminated)
        break;
      
      task.m_done = true;
      task.m_done_condition.broadcast();
    }
    
    task.m_done = task.m_terminated = true;
    task.m_done_condition.broadcast();
    
    return task.status;
  }

  Thread::Thread(int (SDLCALL *fn)(void *), void *data, int * const status)
    : m_impl(0), m_status(status)
  {
    // Ensure Core is initialized
    Core::get_reference();
    
    m_impl = SDL_CreateThread(fn, data);
  }
  
  Thread::Thread(Task &task)
    : m_impl(0), m_status(&task.status)
  {
    // Ensure Core is initialized
    Core::get_reference();
    
    m_impl = SDL_CreateThread(run_task, &task);
  }
  
  Thread::Thread(Repeatable_Task &task)
    : m_impl(0), m_status(&task.status)
  {
    // Ensure Core is initialized
    Core::get_reference();
    
    m_impl = SDL_CreateThread(run_repeatable_task, &task);
  }
  
  Thread::~Thread() {
    SDL_WaitThread(m_impl, m_status);
  }
  
  unsigned int Thread::get_id() {
    return SDL_GetThreadID(m_impl);
  }
  
}
