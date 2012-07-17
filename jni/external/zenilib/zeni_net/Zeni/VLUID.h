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

#ifndef ZENI_VLUID_H
#define ZENI_VLUID_H

#include <Zeni/Error.h>
#include <Zeni/Serialization.h>

namespace Zeni {

  /*** VLUID class ***/

  struct ZENI_NET_DLL VLUID_Too_Large : public Zeni::Error {
    VLUID_Too_Large() : Error("VLUID too large!") {}
  };

  class ZENI_NET_DLL VLUID : public Serializable {
  public:
    int compare(const VLUID &rhs) const;
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
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    std::basic_string<unsigned char> m_uid;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  };

  typedef VLUID Nonce;
  
  /*** VLUID Based Datatypes ***/

  //template <typename TYPE>
  //class Net_Datum {
  //public:
  //  Net_Datum(const TYPE &rhs = TYPE()) : m_datum(rhs) {}
  //  Net_Datum(const VLUID &sauid, const TYPE &rhs = TYPE()) : m_sauid(sauid), m_datum(rhs) {}

  //  /**  This recv function assumes that
  //    *    -the size has already been read in
  //    *    -sauid has already been read in
  //    *    -sauid has been verified to correspond to this Net_Datum
  //    **/
  //  virtual std::istream & recv(std::istream &is) = 0;
  //  
  //  virtual std::ostream & send(std::ostream &os) const = 0;
  //  
  //  operator const TYPE & () const {return this->m_datum;}
  //  operator TYPE & () {return this->m_datum;}
  //  
  //protected:
  //  VLUID m_sauid; // server assigned unique identifier
  //  
  //  VLUID m_nonce; // so old packets don't supercede newer ones
  //  
  //  TYPE m_datum;
  //};

  //template <typename TYPE>
  //class Local_Datum : public Net_Datum<TYPE> {
  //public:
  //  Local_Datum(const TYPE &rhs = TYPE()) : Net_Datum<TYPE>(rhs) {}
  //  Local_Datum(const VLUID &sauid, const TYPE &rhs = TYPE()) : Net_Datum<TYPE>(sauid, rhs) {}

  //  virtual std::istream & recv(std::istream &is) {return is;}
  //  
  //  virtual std::ostream & send(std::ostream &os) const {
  //    std::ostringstream oss;
  //    oss << this->m_sauid << this->m_nonce << this->m_datum;
  //    const String &str = oss.str();
  //    Uint16 size = str.size();
  //    return serialize(os, size) << str;
  //  }
  //};

  //template <typename TYPE>
  //class Remote_Datum : public Net_Datum<TYPE> {
  //public:
  //  Remote_Datum(const TYPE &rhs = TYPE()) : Net_Datum<TYPE>(rhs) {}
  //  Remote_Datum(const VLUID &sauid, const TYPE &rhs = TYPE()) : Net_Datum<TYPE>(sauid, rhs) {}

  //  /**  This recv function assumes that
  //    *    -the size has already been read in
  //    *    -sauid has already been read in
  //    *    -sauid has been verified to correspond to this Net_Datum
  //    **/
  //  virtual std::istream & recv(std::istream &is) {
  //    VLUID nonce(is);
  //    
  //    if(this->m_nonce > nonce) {
  //      TYPE datum;
  //      is >> datum;
  //    }
  //    else
  //      is >> this->m_datum;
  //    
  //    return is;
  //  }
  //  
  //  virtual std::ostream & send(std::ostream &os) const {return os;}
  //};

}

#endif
