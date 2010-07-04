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

/**
 * \class Zeni::VLUID
 *
 * \ingroup zenilib
 *
 * \brief Variable Length Unique IDentifier
 *
 * When the length of an ID has to be able to grow until the end of time, 
 * this is the way to go.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_NET_PRIMITIVES_H
#define ZENI_NET_PRIMITIVES_H

#include "Coordinate.h"
#include "Core.h"
#include "Vector3f.h"
#include "Quaternion.h"

#include "Vector3f.hxx"
#include "Quaternion.hxx"

#ifdef _MACOSX
#include <SDL_net/SDL_net.h>
#else
#include <SDL/SDL_net.h>
#endif

#include <string>
#include <sstream>
  
/*** IPaddress functions ***/

bool operator==(const IPaddress &lhs, const IPaddress &rhs);
bool operator!=(const IPaddress &lhs, const IPaddress &rhs);
bool operator<(const IPaddress &lhs, const IPaddress &rhs);
bool operator<=(const IPaddress &lhs, const IPaddress &rhs);
bool operator>(const IPaddress &lhs, const IPaddress &rhs);
bool operator>=(const IPaddress &lhs, const IPaddress &rhs);

namespace Zeni {
  /*** Simple Helper Functions ***/

  std::string ustoa(const unsigned short &number);
  std::string stoa(const short &number);
  std::string uitoa(const unsigned int &number);
  std::string itoa(const int &number);
  std::string ultoa(const unsigned long &number);
  std::string ltoa(const long &number);
#ifdef _WINDOWS
  std::string ulltoa(const unsigned long long &number);
  std::string lltoa(const long long &number);
#else
  std::string ulltoa(const unsigned long &number);
  std::string lltoa(const long &number);
#endif
  std::string ftoa(const float &number);
  std::string ftoa(const float &number, const unsigned int &precision);
  std::string dtoa(const double &number);
  std::string dtoa(const double &number, const unsigned int &precision);
  std::string ldtoa(const long double &number);
  std::string iptoa(const IPaddress &address);

  size_t grab_bytes(std::istream &is, char * const &store, const size_t &num_bytes);
  size_t grab_bytes(std::istream &is, std::string &store, const size_t &num_bytes);
  
  class Serializable {
  public:
    Serializable() : m_size(0) {}
    virtual ~Serializable() {}
    
    virtual Uint16 size() const {return sizeof(m_size) + m_size;}
    
    virtual std::ostream & serialize(std::ostream &os) const = 0;
    virtual std::istream & unserialize(std::istream &is) = 0;
    
  protected:
    Uint16 m_size;
  };

  /*** VLUID class ***/

  struct VLUID_Too_Large : public Zeni::Error {
    VLUID_Too_Large() : Error("VLUID too large!") {}
  };

  class VLUID : public Serializable {
  public:
    char compare(const VLUID &rhs) const;
    bool operator==(const VLUID &rhs) const {return !compare(rhs);}
    bool operator!=(const VLUID &rhs) const {return compare(rhs) != 0;}
    bool operator<(const VLUID &rhs) const {return compare(rhs) < 0;}
    bool operator<=(const VLUID &rhs) const {return compare(rhs) < 1;}
    bool operator>(const VLUID &rhs) const {return compare(rhs) > 0;}
    bool operator>=(const VLUID &rhs) const {return compare(rhs) > -1;}
    
    VLUID & operator++(); // prefix
    VLUID operator++(int); // postfix
    
    Uint16 size() const {return sizeof(Serializable::m_size) + m_size;}
    
    const unsigned char & operator[](const size_t &index) const {return reinterpret_cast<const unsigned char &>(m_uid[index]);}
    
    virtual std::ostream & serialize(std::ostream &os) const;
    virtual std::istream & unserialize(std::istream &is);
    
  private:
    std::string m_uid;
  };

  typedef VLUID Nonce;

  /*** Stand-Alone serialization/unserialization functions ***/
  
  inline std::ostream & serialize(std::ostream &os, const Serializable &value) {return value.serialize(os);}

  std::ostream & serialize(std::ostream &os, const Sint32 &value);
  std::ostream & serialize(std::ostream &os, const Uint32 &value);
  std::ostream & serialize(std::ostream &os, const Sint16 &value);
  std::ostream & serialize(std::ostream &os, const Uint16 &value);
  std::ostream & serialize(std::ostream &os, const Sint8 &value);
  std::ostream & serialize(std::ostream &os, const char &value);
  std::ostream & serialize(std::ostream &os, const unsigned char &value);
  std::ostream & serialize(std::ostream &os, const float &value);
  std::ostream & serialize(std::ostream &os, const double &value);
  std::ostream & serialize(std::ostream &os, const bool &value);
  std::ostream & serialize(std::ostream &os, const Point2i &value);
  std::ostream & serialize(std::ostream &os, const Point2f &value);
  std::ostream & serialize(std::ostream &os, const Point3i &value);
  std::ostream & serialize(std::ostream &os, const Point3f &value);
  std::ostream & serialize(std::ostream &os, const Vector3f &value);
  std::ostream & serialize(std::ostream &os, const Quaternion &value);
  std::ostream & serialize(std::ostream &os, const IPaddress &address);
  
  inline std::istream & unserialize(std::istream &is, Serializable &value) {return value.unserialize(is);}

  std::istream & unserialize(std::istream &is, Sint32 &value);
  std::istream & unserialize(std::istream &is, Uint32 &value);
  std::istream & unserialize(std::istream &is, Sint16 &value);
  std::istream & unserialize(std::istream &is, Uint16 &value);
  std::istream & unserialize(std::istream &is, Sint8 &value);
  std::istream & unserialize(std::istream &is, char &value);
  std::istream & unserialize(std::istream &is, unsigned char &value);
  std::istream & unserialize(std::istream &is, float &value);
  std::istream & unserialize(std::istream &is, double &value);
  std::istream & unserialize(std::istream &is, bool &value);
  std::istream & unserialize(std::istream &is, Point2i &value);
  std::istream & unserialize(std::istream &is, Point2f &value);
  std::istream & unserialize(std::istream &is, Point3i &value);
  std::istream & unserialize(std::istream &is, Point3f &value);
  std::istream & unserialize(std::istream &is, Vector3f &value);
  std::istream & unserialize(std::istream &is, Quaternion &value);
  std::istream & unserialize(std::istream &is, IPaddress &address);

  template <typename TYPE>
  std::ostream & serialize(std::ostream &os, const std::vector<TYPE> &v) {
    Zeni::serialize(os, static_cast<Uint16>(v.size()));
    for(typename std::vector<TYPE>::const_iterator it = v.begin(); it != v.end(); ++it)
      if(!serialize(os, *it))
        break;
    return os;
  }
    
  template <typename TYPE>
  std::istream & unserialize(std::istream &is, std::vector<TYPE> &v) {
    Uint16 size;
    if(Zeni::unserialize(is, size)) {
      v.resize(size, TYPE());
      for(typename std::vector<TYPE>::iterator it = v.begin(); it != v.end(); ++it)
        if(!unserialize(is, *it))
          break;
    }
    return is;
  }
  
  /*** VLUID Based Datatypes ***/

  template <typename TYPE>
  class Net_Datum {
  public:
    Net_Datum(const TYPE &rhs = TYPE()) : m_datum(rhs) {}
    Net_Datum(const VLUID &sauid, const TYPE &rhs = TYPE()) : m_sauid(sauid), m_datum(rhs) {}

    /**  This recv function assumes that
      *    -the size has already been read in
      *    -sauid has already been read in
      *    -sauid has been verified to correspond to this Net_Datum
      **/
    virtual std::istream & recv(std::istream &is) = 0;
    
    virtual std::ostream & send(std::ostream &os) const = 0;
    
    operator const TYPE & () const {return this->m_datum;}
    operator TYPE & () {return this->m_datum;}
    
  protected:
    VLUID m_sauid; // server assigned unique identifier
    
    VLUID m_nonce; // so old packets don't supercede newer ones
    
    TYPE m_datum;
  };

  template <typename TYPE>
  class Local_Datum : public Net_Datum<TYPE> {
  public:
    Local_Datum(const TYPE &rhs = TYPE()) : Net_Datum<TYPE>(rhs) {}
    Local_Datum(const VLUID &sauid, const TYPE &rhs = TYPE()) : Net_Datum<TYPE>(sauid, rhs) {}

    virtual std::istream & recv(std::istream &is) {return is;}
    
    virtual std::ostream & send(std::ostream &os) const {
      std::ostringstream oss;
      oss << this->m_sauid << this->m_nonce << this->m_datum;
      const std::string &str = oss.str();
      Uint16 size = str.size();
      return serialize(os, size) << str;
    }
  };

  template <typename TYPE>
  class Remote_Datum : public Net_Datum<TYPE> {
  public:
    Remote_Datum(const TYPE &rhs = TYPE()) : Net_Datum<TYPE>(rhs) {}
    Remote_Datum(const VLUID &sauid, const TYPE &rhs = TYPE()) : Net_Datum<TYPE>(sauid, rhs) {}

    /**  This recv function assumes that
      *    -the size has already been read in
      *    -sauid has already been read in
      *    -sauid has been verified to correspond to this Net_Datum
      **/
    virtual std::istream & recv(std::istream &is) {
      VLUID nonce(is);
      
      if(this->m_nonce > nonce) {
        TYPE datum;
        is >> datum;
      }
      else
        is >> this->m_datum;
      
      return is;
    }
    
    virtual std::ostream & send(std::ostream &os) const {return os;}
  };
}

#endif
