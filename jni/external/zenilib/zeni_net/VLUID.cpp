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

#include <zeni_net.h>

#include <iostream>

namespace Zeni {
  /*** VLUID Class ***/

  int VLUID::compare(const VLUID &rhs) const {
    if(m_size > rhs.m_size)
      return 1;
    else if(rhs.m_size > m_size)
      return -1;
    
    Sint32 index = m_size - 1;
    for(; index > -1; --index) {
      const unsigned char &c = m_uid[size_t(index)];
      const unsigned char &rhs_c = rhs.m_uid[size_t(index)];
      
      if(c > rhs_c)
        return 1;
      else if(rhs_c > c)
        return -1;
    }
    
    return 0;
  }

  VLUID & VLUID::operator++() { //prefix
    Uint16 index = 0;
    for(; index < m_size; ++index)
      if(++m_uid[index])
        break;
    
    if(index == m_size) {
      if(++m_size)
        m_uid += static_cast<unsigned char>(0);
      else
        m_uid.clear();
    }

    return *this;
  }

  VLUID VLUID::operator++(int) { //postfix
    VLUID prev = *this;
    ++*this;
    return prev;
  }
  
  std::ostream & VLUID::serialize(std::ostream &os) const {
    Serializable::serialize(os);
    
    return os.write(reinterpret_cast<const char *>(m_uid.c_str()), m_size);
  }
  
  std::istream & VLUID::unserialize(std::istream &is) {
    Serializable::unserialize(is);
    
    if(is) {
      m_uid.resize(m_size);
      
      if(!is.read(const_cast<char *>(reinterpret_cast<const char *>(m_uid.c_str())), m_size))
        m_uid.clear();
    }
    else
      m_uid.clear();
    
    return is;
  }

}

#include <Zeni/Undefine.h>
