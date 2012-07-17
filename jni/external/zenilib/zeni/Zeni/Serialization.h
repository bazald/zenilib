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

/**
 * \class Zeni::Serializable
 *
 * \ingroup zenilib
 *
 * \brief A base class for objects that can be sent over the network
 *
 * Any class deriving from Serializable should know how to send themselves 
 * over a network socket, and how to receive themselves from a network 
 * socket.  This is good for primitives, but for complicated interactions 
 * where only part of a class should be sent or received at a time, it is 
 * not the way to go.  Go higher level in those cases.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_SERIALIZATION_H
#define ZENI_SERIALIZATION_H

#include <Zeni/String.h>

#include <SDL/SDL_net.h>

#include <iostream>
#include <list>
#include <set>
#include <vector>

/*** IPaddress functions ***/

ZENI_DLL bool operator==(const IPaddress &lhs, const IPaddress &rhs);
ZENI_DLL bool operator!=(const IPaddress &lhs, const IPaddress &rhs);
ZENI_DLL bool operator<(const IPaddress &lhs, const IPaddress &rhs);
ZENI_DLL bool operator<=(const IPaddress &lhs, const IPaddress &rhs);
ZENI_DLL bool operator>(const IPaddress &lhs, const IPaddress &rhs);
ZENI_DLL bool operator>=(const IPaddress &lhs, const IPaddress &rhs);

namespace Zeni {
  /*** Simple Helper Functions ***/

  ZENI_DLL String ustoa(const unsigned short &number);
  ZENI_DLL String stoa(const short &number);
  ZENI_DLL String uitoa(const unsigned int &number);
  ZENI_DLL String itoa(const int &number);
  ZENI_DLL String ultoa(const unsigned long &number);
  ZENI_DLL String ltoa(const long &number);
#ifdef _WINDOWS
  ZENI_DLL String ulltoa(const unsigned long long &number);
  ZENI_DLL String lltoa(const long long &number);
#else
  ZENI_DLL String ulltoa(const unsigned long &number);
  ZENI_DLL String lltoa(const long &number);
#endif
  ZENI_DLL String ftoa(const float &number);
  ZENI_DLL String ftoa(const float &number, const unsigned int &precision);
  ZENI_DLL String dtoa(const double &number);
  ZENI_DLL String dtoa(const double &number, const unsigned int &precision);
  ZENI_DLL String ldtoa(const long double &number);
  ZENI_DLL String iptoa(const IPaddress &address);

  ZENI_DLL size_t grab_bytes(std::istream &is, char * const &store, const size_t &num_bytes);
  ZENI_DLL size_t grab_bytes(std::istream &is, String &store, const size_t &num_bytes);

  class ZENI_DLL Serializable {
  public:
    Serializable() : m_size(0) {}
    virtual ~Serializable() {}
    
    virtual Uint16 size() const {return sizeof(m_size) + m_size;}
    
    virtual std::ostream & serialize(std::ostream &os) const = 0;
    virtual std::istream & unserialize(std::istream &is) = 0;
    
  protected:
    Uint16 m_size;
  };

  /*** Stand-Alone serialization/unserialization functions ***/
  
  inline std::ostream & serialize(std::ostream &os, const Serializable &value) {return value.serialize(os);}

  ZENI_DLL std::ostream & serialize(std::ostream &os, const Sint32 &value);
  ZENI_DLL std::ostream & serialize(std::ostream &os, const Uint32 &value);
  ZENI_DLL std::ostream & serialize(std::ostream &os, const Sint16 &value);
  ZENI_DLL std::ostream & serialize(std::ostream &os, const Uint16 &value);
  ZENI_DLL std::ostream & serialize(std::ostream &os, const Sint8 &value);
  ZENI_DLL std::ostream & serialize(std::ostream &os, const char &value);
  ZENI_DLL std::ostream & serialize(std::ostream &os, const unsigned char &value);
  ZENI_DLL std::ostream & serialize(std::ostream &os, const float &value);
  ZENI_DLL std::ostream & serialize(std::ostream &os, const double &value);
  //ZENI_DLL std::ostream & serialize(std::ostream &os, const bool &value);
  ZENI_DLL std::ostream & serialize(std::ostream &os, const IPaddress &address);
  ZENI_DLL std::ostream & serialize(std::ostream &os, const String &string);
  
  inline std::istream & unserialize(std::istream &is, Serializable &value) {return value.unserialize(is);}

  ZENI_DLL std::istream & unserialize(std::istream &is, Sint32 &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, Uint32 &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, Sint16 &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, Uint16 &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, Sint8 &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, char &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, unsigned char &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, float &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, double &value);
  //ZENI_DLL std::istream & unserialize(std::istream &is, bool &value);
  ZENI_DLL std::istream & unserialize(std::istream &is, IPaddress &address);
  ZENI_DLL std::istream & unserialize(std::istream &is, String &string);

  template <typename TYPE>
  std::ostream & serialize(std::ostream &os, const std::list<TYPE> &list_) {
    Zeni::serialize(os, static_cast<Uint16>(list_.size()));
    for(typename std::list<TYPE>::const_iterator it = list_.begin(); it != list_.end(); ++it)
      if(!serialize(os, *it))
        break;
    return os;
  }
    
  template <typename TYPE>
  std::istream & unserialize(std::istream &is, std::list<TYPE> &list_) {
    list_.clear();

    Uint16 size;
    if(Zeni::unserialize(is, size)) {
      TYPE el;
      for(Uint16 i = 0u; i != size; ++i) {
        if(!unserialize(is, el))
          break;
        list_.push_back(el);
      }
    }
    return is;
  }

  template <typename TYPE>
  std::ostream & serialize(std::ostream &os, const std::set<TYPE> &list_) {
    Zeni::serialize(os, static_cast<Uint16>(list_.size()));
    for(typename std::set<TYPE>::const_iterator it = list_.begin(); it != list_.end(); ++it)
      if(!serialize(os, *it))
        break;
    return os;
  }
    
  template <typename TYPE>
  std::istream & unserialize(std::istream &is, std::set<TYPE> &list_) {
    list_.clear();

    Uint16 size;
    if(Zeni::unserialize(is, size)) {
      TYPE el;
      for(Uint16 i = 0u; i != size; ++i) {
        if(!unserialize(is, el))
          break;
        list_.insert(el);
      }
    }
    return is;
  }

  template <typename TYPE>
  std::ostream & serialize(std::ostream &os, const std::vector<TYPE> &list_) {
    if(Zeni::serialize(os, static_cast<Uint16>(list_.size())))
      for(typename std::vector<TYPE>::const_iterator it = list_.begin(); it != list_.end(); ++it)
        if(!serialize(os, *it))
          break;
    return os;
  }
    
  template <typename TYPE>
  std::istream & unserialize(std::istream &is, std::vector<TYPE> &list_) {
    list_.clear();

    Uint16 size;
    if(Zeni::unserialize(is, size)) {
      TYPE el;
      list_.reserve(size);
      for(Uint16 i = 0u; i != size; ++i) {
        if(!unserialize(is, el))
          break;
        list_.push_back(el);
      }
    }
    return is;
  }

}

#endif
