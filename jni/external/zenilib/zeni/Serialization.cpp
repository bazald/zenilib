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

#include <zeni.h>

#include <Zeni/Define.h>
#include <sstream>
#include <iomanip>

/*** IPaddress functions ***/

bool operator==(const IPaddress &lhs, const IPaddress &rhs) {
  const IPaddress lhs_l = {SDLNet_Read32(const_cast<Uint32 *>(&lhs.host)), SDLNet_Read16(const_cast<Uint16 *>(&lhs.port))};
  const IPaddress rhs_l = {SDLNet_Read32(const_cast<Uint32 *>(&rhs.host)), SDLNet_Read16(const_cast<Uint16 *>(&rhs.port))};
  return lhs_l.host == rhs_l.host && lhs_l.port == rhs_l.port;
}

bool operator!=(const IPaddress &lhs, const IPaddress &rhs) {
  const IPaddress lhs_l = {SDLNet_Read32(const_cast<Uint32 *>(&lhs.host)), SDLNet_Read16(const_cast<Uint16 *>(&lhs.port))};
  const IPaddress rhs_l = {SDLNet_Read32(const_cast<Uint32 *>(&rhs.host)), SDLNet_Read16(const_cast<Uint16 *>(&rhs.port))};
  return lhs_l.host != rhs_l.host || lhs_l.port != rhs_l.port;
}

bool operator<(const IPaddress &lhs, const IPaddress &rhs) {
  const IPaddress lhs_l = {SDLNet_Read32(const_cast<Uint32 *>(&lhs.host)), SDLNet_Read16(const_cast<Uint16 *>(&lhs.port))};
  const IPaddress rhs_l = {SDLNet_Read32(const_cast<Uint32 *>(&rhs.host)), SDLNet_Read16(const_cast<Uint16 *>(&rhs.port))};
  return lhs_l.host < rhs_l.host || (lhs_l.host == rhs_l.host && lhs_l.port <  rhs_l.port);
}

bool operator<=(const IPaddress &lhs, const IPaddress &rhs) {
  const IPaddress lhs_l = {SDLNet_Read32(const_cast<Uint32 *>(&lhs.host)), SDLNet_Read16(const_cast<Uint16 *>(&lhs.port))};
  const IPaddress rhs_l = {SDLNet_Read32(const_cast<Uint32 *>(&rhs.host)), SDLNet_Read16(const_cast<Uint16 *>(&rhs.port))};
  return lhs_l.host < rhs_l.host || (lhs_l.host == rhs_l.host && lhs_l.port <= rhs_l.port);
}

bool operator>(const IPaddress &lhs, const IPaddress &rhs) {
  const IPaddress lhs_l = {SDLNet_Read32(const_cast<Uint32 *>(&lhs.host)), SDLNet_Read16(const_cast<Uint16 *>(&lhs.port))};
  const IPaddress rhs_l = {SDLNet_Read32(const_cast<Uint32 *>(&rhs.host)), SDLNet_Read16(const_cast<Uint16 *>(&rhs.port))};
  return lhs_l.host > rhs_l.host || (lhs_l.host == rhs_l.host && lhs_l.port >  rhs_l.port);
}

bool operator>=(const IPaddress &lhs, const IPaddress &rhs) {
  const IPaddress lhs_l = {SDLNet_Read32(const_cast<Uint32 *>(&lhs.host)), SDLNet_Read16(const_cast<Uint16 *>(&lhs.port))};
  const IPaddress rhs_l = {SDLNet_Read32(const_cast<Uint32 *>(&rhs.host)), SDLNet_Read16(const_cast<Uint16 *>(&rhs.port))};
  return lhs_l.host > rhs_l.host || (lhs_l.host == rhs_l.host && lhs_l.port >= rhs_l.port);
}

namespace Zeni {
  /*** Simple Helper Functions ***/

  String ustoa(const unsigned short &number) {
    char buf[ZENI_SPRINTF_BUFFER_SIZE];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%hu", number);
    return buf;
  }

  String stoa(const short &number) {
    char buf[ZENI_SPRINTF_BUFFER_SIZE];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%hd", number);
    return buf;
  }

  String uitoa(const unsigned int &number) {
    char buf[ZENI_SPRINTF_BUFFER_SIZE];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%u", number);
    return buf;
  }

  String itoa(const int &number) {
    char buf[ZENI_SPRINTF_BUFFER_SIZE];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%d", number);
    return buf;
  }

  String ultoa(const unsigned long &number) {
    char buf[ZENI_SPRINTF_BUFFER_SIZE];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%lu", number);
    return buf;
  }

  String ltoa(const long &number) {
    char buf[ZENI_SPRINTF_BUFFER_SIZE];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%ld", number);
    return buf;
  }

#ifdef _WINDOWS
  String ulltoa(const unsigned long long &number) {
    char buf[ZENI_SPRINTF_BUFFER_SIZE];
    sprintf_s(buf, "%llu", number);
    return buf;
  }

  String lltoa(const long long &number) {
    char buf[ZENI_SPRINTF_BUFFER_SIZE];
    sprintf_s(buf, "%lld", number);
    return buf;
  }
#else
  String ulltoa(const unsigned long &number) {
    char buf[ZENI_SPRINTF_BUFFER_SIZE];
    sprintf(buf, "%lu", number);
    return buf;
  }

  String lltoa(const long &number) {
    char buf[ZENI_SPRINTF_BUFFER_SIZE];
    sprintf(buf, "%ld", number);
    return buf;
  }
#endif

  String ftoa(const float &number) {
    char buf[ZENI_SPRINTF_BUFFER_SIZE];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%f", number);
    return buf;
  }

  String ftoa(const float &number, const unsigned int &precision) {
    std::ostringstream oss;
    oss << std::setprecision(int(precision)) << number;
    return String(oss.str());
  }

  String dtoa(const double &number) {
    char buf[ZENI_SPRINTF_BUFFER_SIZE];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%f", number);
    return buf;
  }

  String dtoa(const double &number, const unsigned int &precision) {
    std::ostringstream oss;
    oss << std::setprecision(int(precision)) << number;
    return String(oss.str());
  }

  String ldtoa(const long double &number) {
    char buf[ZENI_SPRINTF_BUFFER_SIZE];
  #ifdef _WINDOWS
    sprintf_s
  #else
    sprintf
  #endif
      (buf, "%Lf", number);
    return buf;
  }

  String iptoa(const IPaddress &address) {
    Uint32 host = SDLNet_Read32(const_cast<Uint32 *>(&address.host));
    Uint16 port = SDLNet_Read16(const_cast<Uint16 *>(&address.port));

    return uitoa(host >> 24 & 0xFF) + '.' +
           uitoa(host >> 16 & 0xFF) + '.' +
           uitoa(host >> 8 & 0xFF) + '.' +
           uitoa(host & 0xFF) + ':' + uitoa(port);
  }

  size_t grab_bytes(std::istream &is, char * const &store, const size_t &num_bytes) {
    char c;
    size_t i = 0u;
    for(char * store_ptr = store; i != num_bytes; ++i, ++store_ptr)
      if(is.get(c))
        *store_ptr = c;
      else
        break;
    return i;
  }

  size_t grab_bytes(std::istream &is, String &store, const size_t &num_bytes) {
    store.resize(num_bytes);
    const size_t rv = grab_bytes(is, const_cast<char * const>(store.c_str()), num_bytes);
    store.resize(rv);
    return rv;
  }
  
  std::ostream & Serializable::serialize(std::ostream &os) const {
    return Zeni::serialize(os, m_size);
  }
  
  std::istream & Serializable::unserialize(std::istream &is) {
    if(!Zeni::unserialize(is, m_size))
      m_size = 0;
    
    return is;
  }

  /*** Stand-Alone serialization/unserialization functions ***/
  
  std::ostream & serialize(std::ostream &os, const Sint32 &value) {
    char buf[sizeof(Sint32)];
    void *bp = buf;

    SDLNet_Write32(reinterpret_cast<const Uint32 &>(value), bp);

    return os.write(buf, sizeof(Sint32));
  }
  
  std::ostream & serialize(std::ostream &os, const Uint32 &value) {
    char buf[sizeof(Uint32)];
    void *bp = buf;
    
    SDLNet_Write32(value, bp);
    
    return os.write(buf, sizeof(Uint32));
  }
  
  std::ostream & serialize(std::ostream &os, const Sint16 &value) {
    char buf[sizeof(Sint16)];
    void *bp = buf;
    
    SDLNet_Write16(reinterpret_cast<const Uint16 &>(value), bp);
    
    return os.write(buf, sizeof(Sint16));
  }
  
  std::ostream & serialize(std::ostream &os, const Uint16 &value) {
    char buf[sizeof(Uint16)];
    void *bp = buf;
    
    SDLNet_Write16(value, bp);
    
    return os.write(buf, sizeof(Uint16));
  }
  
  std::ostream & serialize(std::ostream &os, const Sint8 &value) {
    return os.write(reinterpret_cast<const char *>(&value), 1);
  }
  
  std::ostream & serialize(std::ostream &os, const char &value) {
    return os.write(&value, 1);
  }
  
  std::ostream & serialize(std::ostream &os, const unsigned char &value) {
    return os.write(reinterpret_cast<const char *>(&value), 1);
  }
  
  std::ostream & serialize(std::ostream &os, const float &value) {
    return os.write(reinterpret_cast<const char * const>(&value), sizeof(float));
  }
  
  std::ostream & serialize(std::ostream &os, const double &value) {
    return os.write(reinterpret_cast<const char * const>(&value), sizeof(double));
  }
  
  //std::ostream & serialize(std::ostream &os, const bool &value) {
  //  const char c(value ? 1 : 0);
  //  return os.put(c);
  //}

  std::ostream & serialize(std::ostream &os, const IPaddress &address) {
    const char *buf = reinterpret_cast<const char *>(&address);
    
    return os.write(buf, sizeof(IPaddress));
  }

  std::ostream & serialize(std::ostream &os, const String &string) {
    const Uint16 sz = Uint16(string.size());
    return serialize(os, sz).write(string.c_str(), sz);
  }
  
  std::istream & unserialize(std::istream &is, Sint32 &value) {
    char buf[sizeof(Sint32)];
    void *bp = buf;
    
    if(is.read(buf, sizeof(Sint32))) {
      const Uint32 s_value = SDLNet_Read32(bp);
      value = reinterpret_cast<const Sint32 &>(s_value);
    }
    
    return is;
  }
  
  std::istream & unserialize(std::istream &is, Uint32 &value) {
    char buf[sizeof(Uint32)];
    void *bp = buf;
    
    if(is.read(buf, sizeof(Uint32)))
      value = SDLNet_Read32(bp);
    
    return is;
  }
  
  std::istream & unserialize(std::istream &is, Sint16 &value) {
    char buf[sizeof(Sint16)];
    void *bp = buf;
    
    if(is.read(buf, sizeof(Sint16))) {
      const Uint16 s_value = SDLNet_Read16(bp);
      value = reinterpret_cast<const Sint16 &>(s_value);
    }
    
    return is;
  }
  
  std::istream & unserialize(std::istream &is, Uint16 &value) {
    char buf[sizeof(Uint16)];
    void *bp = buf;
    
    if(is.read(buf, sizeof(Uint16)))
      value = SDLNet_Read16(bp);
    
    return is;
  }
  
  std::istream & unserialize(std::istream &is, Sint8 &value) {
    is.read(reinterpret_cast<char *>(&value), 1);
    return is;
  }
  
  std::istream & unserialize(std::istream &is, char &value) {
    is.read(&value, 1);
    return is;
  }
  
  std::istream & unserialize(std::istream &is, unsigned char &value) {
    is.read(reinterpret_cast<char *>(&value), 1);
    return is;
  }
  
  std::istream & unserialize(std::istream &is, float &value) {
    return is.read(reinterpret_cast<char * const>(&value), sizeof(float));
  }
  
  std::istream & unserialize(std::istream &is, double &value) {
    return is.read(reinterpret_cast<char * const>(&value), sizeof(double));
  }
  
  //std::istream & unserialize(std::istream &is, bool &value) {
  //  char c;
  //  
  //  if(is.get(c))
  //    value = c ? true : false;
  //  
  //  return is;
  //}

  std::istream & unserialize(std::istream &is, IPaddress &address) {
    char buf[sizeof(IPaddress)];
    
    if(is.read(buf, sizeof(IPaddress)))
      memcpy(&address, buf, sizeof(IPaddress));
    
    return is;
  }

  std::istream & unserialize(std::istream &is, String &string) {
    Uint16 sz;
    unserialize(is, sz);

    if(is) {
      string.resize(sz);
      is.read(const_cast<char *>(string.c_str()), sz);
    }

    return is;
  }

}

#include <Zeni/Undefine.h>
