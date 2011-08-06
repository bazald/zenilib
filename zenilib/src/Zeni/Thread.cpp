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

#include <Zeni/Define.h>

namespace Zeni {

  int run_task(void *task_ptr) {
    Task &task = *reinterpret_cast<Task *>(task_ptr);
    Recursive_Mutex::Lock lock(task.m_done_mutex);
    
    try {
      task.status = task.run();
      task.m_done = true;
    }
    catch(Quit_Event &) {
      task.status = NO_ERROR_STATUS;
      task.msg = "Task completed normally.";
      task.m_done = true;
    }
    catch(Error &error) {
      task.status = ZENI_ERROR_STATUS;
      task.msg = error.msg;
      task.m_done = true;
    }
    catch(std::exception &error) {
      task.status = STD_ERROR_STATUS;
      task.msg = error.what();
      task.m_done = true;
    }
    catch(...) {
      task.status = OTHER_ERROR_STATUS;
      task.msg = "Unknown Error (Not of Type 'Zeni::Error')";
      task.m_done = true;
    }
    
    task.m_done = true;
    task.m_done_condition.broadcast();
    
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
      catch(Quit_Event &) {
        task.status = NO_ERROR_STATUS;
        task.msg = "Repeatable Task completed normally.";
        task.m_done = task.m_terminated = true;
        return task.status;
      }
      catch(Error &error) {
        task.status = ZENI_ERROR_STATUS;
        task.msg = error.msg;
        task.m_done = task.m_terminated = true;
        return task.status;
      }
      catch(std::exception &error) {
        task.status = STD_ERROR_STATUS;
        task.msg = error.what();
        task.m_done = true;
      }
      catch(...) {
        task.status = OTHER_ERROR_STATUS;
        task.msg = "Unknown Error (Not of Type 'Zeni::Error')";
        task.m_done = task.m_terminated = true;
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
    : m_impl(0),
    m_status(status),
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif
    m_uninit(*this)
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  {
    // Ensure Core is initialized
    Core &cr = get_Core();
    
    m_impl = SDL_CreateThread(fn, data);
    
    cr.lend_pre_uninit(&m_uninit);
  }
  
  Thread::Thread(Task &task)
    : m_impl(0),
    m_status(&task.status),
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif
    m_uninit(*this)
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  {
    // Ensure Core is initialized
    Core &cr = get_Core();
    
    m_impl = SDL_CreateThread(run_task, &task);
    
    cr.lend_pre_uninit(&m_uninit);
  }
  
  Thread::Thread(Repeatable_Task &task)
    : m_impl(0),
    m_status(&task.status),
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif
    m_uninit(*this)
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  {
    // Ensure Core is initialized
    Core &cr = get_Core();

    m_impl = SDL_CreateThread(run_repeatable_task, &task);

    cr.lend_pre_uninit(&m_uninit);
  }
  
  Thread::~Thread() {
    if(m_impl) {
      SDL_WaitThread(m_impl, m_status);

      get_Core().remove_pre_uninit(&m_uninit);
    }
  }
  
  unsigned int Thread::get_id() {
    return SDL_GetThreadID(m_impl);
  }
  
}

#include <Zeni/Undefine.h>
