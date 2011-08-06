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

#ifndef ZENI_MUTEX_HXX
#define ZENI_MUTEX_HXX

#include <Zeni/Mutex.h>
#include <cassert>

namespace Zeni {

  Mutex::Lock::Lock(Mutex &mutex)
    : m_mutex(mutex)
  {
    m_mutex.lock();
  }
  
  Mutex::Lock::~Lock() {
    m_mutex.unlock();
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
