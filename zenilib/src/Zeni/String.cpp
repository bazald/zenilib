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

#include <cctype>
#include <cstring>
#include <iostream>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace Zeni {

  String::String()
    : m_size(0),
    m_cap(8),
    m_c_str(new char [8])
  {
    m_c_str[0] = '\0';
  }

  String::String(const char * const &c_str_)
    : m_size(std::strlen(c_str_)),
    m_cap(m_size + 1u),
    m_c_str(new char [m_cap])
  {
    std::memcpy(m_c_str, c_str_, m_cap);
  }

  String::String(const String &rhs)
    : m_size(rhs.m_size),
    m_cap(m_size + 1u),
    m_c_str(new char [m_cap])
  {
    std::memcpy(m_c_str, rhs.m_c_str, m_cap);
  }

  String::~String() {
    delete [] m_c_str;
  }

  void String::resize(const size_t &size) {
    const size_t req_cap = size + 1;

    if(req_cap > m_cap) {
      char * const new_str = new char [req_cap];

      std::memcpy(new_str, m_c_str, m_cap);
      delete [] m_c_str;

      m_size = size;
      m_cap = req_cap;
      m_c_str = new_str;
    }
    else {
      if(2u * req_cap < m_size) {
        char * const new_str = new char [req_cap];

        std::memcpy(new_str, m_c_str, size);
        delete [] m_c_str;

        m_cap = req_cap;
        m_c_str = new_str;
      }

      m_size = size;
    }

    m_c_str[m_size] = '\0';
  }

  void String::clear() {
    String temp;
    swap(temp);
  }

  String & String::operator=(const char &c) {
    String temp;
    temp += c;

    swap(temp);

    return *this;
  }

  String & String::operator=(const char * const &rhs) {
    String temp(rhs);

    swap(temp);

    return *this;
  }

  String & String::operator=(const String &rhs) {
    String temp(rhs);

    swap(temp);

    return *this;
  }

  String & String::operator+=(const char &c) {
    if(m_size + 1u == m_cap) {
      const size_t new_cap = 2u * m_cap;
      char * const new_str = new char [new_cap];

      std::memcpy(new_str, m_c_str, m_size);
      delete [] m_c_str;

      m_cap = new_cap;
      m_c_str = new_str;
    }

    m_c_str[m_size] = c;
    m_c_str[++m_size] = '\0';

    return *this;
  }

  String & String::operator+=(const char * const &str) {
    String temp(*this);

    for(const char * ci = str; *ci; ++ci)
      temp += *ci;

    swap(temp);

    return *this;
  }

  String & String::operator+=(const String &str) {
    (*this) += str.c_str();

    return *this;
  }

  String String::operator+(const char &c) {
    String temp(*this);
    temp += c;
    return temp;
  }

  String String::operator+(const char * const &str) {
    String temp(*this);
    temp += str;
    return temp;
  }

  String String::operator+(const String &str) {
    String temp(*this);
    temp += str;
    return temp;
  }
    
  bool String::operator==(const char * const &str) const {
    return std::strcmp(m_c_str, str) == 0;
  }

  bool String::operator==(const String &str) const {
    return std::strcmp(m_c_str, str.m_c_str) == 0;
  }

  bool String::operator!=(const char * const &str) const {
    return std::strcmp(m_c_str, str) != 0;
  }

  bool String::operator!=(const String &str) const {
    return std::strcmp(m_c_str, str.m_c_str) != 0;
  }

  bool String::operator<(const char * const &str) const {
    return std::strcmp(m_c_str, str) < 0;
  }

  bool String::operator<(const String &str) const {
    return std::strcmp(m_c_str, str.m_c_str) < 0;
  }

  bool String::operator>(const char * const &str) const {
    return std::strcmp(m_c_str, str) > 0;
  }

  bool String::operator>(const String &str) const {
    return std::strcmp(m_c_str, str.m_c_str) > 0;
  }

  bool String::operator<=(const char * const &str) const {
    return std::strcmp(m_c_str, str) <= 0;
  }

  bool String::operator<=(const String &str) const {
    return std::strcmp(m_c_str, str.m_c_str) <= 0;
  }

  bool String::operator>=(const char * const &str) const {
    return std::strcmp(m_c_str, str) >= 0;
  }

  bool String::operator>=(const String &str) const {
    return std::strcmp(m_c_str, str.m_c_str) >= 0;
  }

  char String::operator[](const size_t &index) const {
    return m_c_str[index];
  }

  char & String::operator[](const size_t &index) {
    return m_c_str[index];
  }

  char String::operator[](const int &index) const {
    return m_c_str[index];
  }

  char & String::operator[](const int &index) {
    return m_c_str[index];
  }

  bool String::empty() const {
    return m_size == 0u;
  }

  size_t String::size() const {
    return m_size;
  }

  const char * const String::c_str() const {
    return m_c_str;
  }

  void String::swap(String &rhs) {
    std::swap(m_size, rhs.m_size);
    std::swap(m_cap, rhs.m_cap);
    std::swap(m_c_str, rhs.m_c_str);
  }

  size_t String::Hash::operator()(const String &str) const {
    size_t val = 42u;
    for(const char * ci = str.c_str(); *ci; ++ci)
      val = ((val << 5) | (val >> (8u * sizeof(size_t) - 5))) + *ci;
    return val;
  }

  bool String::Hash::operator()(const String &lhs, const String &rhs) const {
    return lhs != rhs;
  }

  String::const_iterator String::begin() const {
    return m_c_str;
  }

  String::iterator String::begin() {
    return m_c_str;
  }

  String::const_iterator String::end() const {
    return m_c_str + m_size;
  }

  String::iterator String::end() {
    return m_c_str + m_size;
  }

  size_t String::find(const String &str, const size_t &pos) const {
    const char * ci = std::strstr(m_c_str + pos, str.m_c_str);
    return ci ? ci - m_c_str : size_t(-1);
  }

  size_t String::find(const char *str, const size_t &pos) const {
    const char * ci = std::strstr(m_c_str + pos, str);
    return ci ? ci - m_c_str : size_t(-1);
  }

  size_t String::find(const char &c, const size_t &pos) const {
    const char * ci = std::strchr(m_c_str + pos, c);
    return ci ? ci - m_c_str : size_t(-1);
  }

  String String::substr(size_t pos, size_t n) const {
    String temp(m_c_str + pos);

    if(n != size_t(-1))
      temp.resize(n);

    return temp;
  }

}

Zeni::String operator+(const char &c, const Zeni::String &str) {
  Zeni::String temp;
  temp += c;
  temp += str;
  return temp;
}

Zeni::String operator+(const char * const &strl, const Zeni::String &strr) {
  Zeni::String temp(strl);
  temp += strr;
  return temp;
}

bool operator==(const char * const &strl, const Zeni::String &strr) {
  return strr == strl;
}

bool operator!=(const char * const &strl, const Zeni::String &strr) {
  return strr != strl;
}

bool operator<(const char * const &strl, const Zeni::String &strr) {
  return strr > strl;
}

bool operator>(const char * const &strl, const Zeni::String &strr) {
  return strr < strl;
}

bool operator<=(const char * const &strl, const Zeni::String &strr) {
  return strr >= strl;
}

bool operator>=(const char * const &strl, const Zeni::String &strr) {
  return strr <= strl;
}

std::istream & operator>>(std::istream &is, Zeni::String &str) {
  Zeni::String temp;

  if(is >> std::ws)
    while(!std::isspace(is.peek()) && is)
      temp += (char)is.get();

  str.swap(temp);

  return is;
}

std::ostream & operator<<(std::ostream &os, const Zeni::String &str) {
  return os << str.c_str();
}
