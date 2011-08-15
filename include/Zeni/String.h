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

#ifndef ZENI_STRING_H
#define ZENI_STRING_H

#include <string>
#include <cctype>
#include <cstring>
#include <iosfwd>

namespace Zeni {

  class ZENI_DLL String {
  public:
    String();
    String(const char * const &c_str_);
    String(const String &rhs);
    ~String();

    explicit inline String(const std::string &rhs)
      : m_c_str(0)
    {
      String lhs;
      lhs.resize(rhs.size());
      memcpy(lhs.m_c_str, rhs.c_str(), m_size);

      std::swap(m_size, lhs.m_size);
      std::swap(m_cap, lhs.m_cap);
      std::swap(m_c_str, lhs.m_c_str);
    }

    inline String & operator=(const std::string &rhs) {
      String lhs;
      lhs.resize(rhs.size());
      memcpy(lhs.m_c_str, rhs.c_str(), m_size);

      std::swap(m_size, lhs.m_size);
      std::swap(m_cap, lhs.m_cap);
      std::swap(m_c_str, lhs.m_c_str);

      return *this;
    }

    void resize(const size_t &size);
    void clear();
    
    String & operator=(const char &c);
    String & operator=(const char * const &rhs);
    String & operator=(const String &rhs);

    String & operator+=(const char &c);
    String & operator+=(const char * const &str);
    String & operator+=(const String &str);

    String operator+(const char &c);
    String operator+(const char * const &str);
    String operator+(const String &str);
    
    bool operator==(const char * const &str) const;
    bool operator==(const String &str) const;
    bool operator!=(const char * const &str) const;
    bool operator!=(const String &str) const;
    bool operator<(const char * const &str) const;
    bool operator<(const String &str) const;
    bool operator>(const char * const &str) const;
    bool operator>(const String &str) const;
    bool operator<=(const char * const &str) const;
    bool operator<=(const String &str) const;
    bool operator>=(const char * const &str) const;
    bool operator>=(const String &str) const;

    char operator[](const size_t &index) const;
    char & operator[](const size_t &index);
    char operator[](const int &index) const;
    char & operator[](const int &index);

    bool empty() const;
    size_t size() const;

    inline std::string cpp_str() const {
      std::string rv;
      rv.resize(m_size);
      memcpy(const_cast<char *>(rv.c_str()), m_c_str, m_size);
      return rv;
    }

    const char * const c_str() const;

    inline operator std::string() const {
      return cpp_str();
    }

    inline operator const char * const () const {
      return c_str();
    }

    void swap(String &rhs);

    struct ZENI_DLL Hash {
      enum {bucket_size = 4, min_buckets = 8};

      size_t operator()(const String &str) const;

      bool operator()(const String &lhs, const String &rhs) const;
    };

    typedef char * iterator;
    typedef const char * const_iterator;
    
    const_iterator begin() const;
    iterator begin();
    const_iterator end() const;
    iterator end();

    size_t find(const String &str, const size_t &pos = 0) const;
    //size_t find(const char *str, const size_t& pos, const size_t &n) const;
    size_t find(const char *str, const size_t &pos = 0) const;
    size_t find(const char &c, const size_t &pos = 0) const;

    String substr(size_t pos = 0, size_t n = size_t(-1)) const;

  private:
    size_t m_size;
    size_t m_cap;
    char * m_c_str;
  };
}

ZENI_DLL Zeni::String operator+(const char &c, const Zeni::String &str);
ZENI_DLL Zeni::String operator+(const char * const &strl, const Zeni::String &strr);

ZENI_DLL bool operator==(const char * const &strl, const Zeni::String &strr);
ZENI_DLL bool operator!=(const char * const &strl, const Zeni::String &strr);
ZENI_DLL bool operator<(const char * const &strl, const Zeni::String &strr);
ZENI_DLL bool operator>(const char * const &strl, const Zeni::String &strr);
ZENI_DLL bool operator<=(const char * const &strl, const Zeni::String &strr);
ZENI_DLL bool operator>=(const char * const &strl, const Zeni::String &strr);

ZENI_DLL std::istream & operator>>(std::istream &is, Zeni::String &str);
ZENI_DLL std::ostream & operator<<(std::ostream &os, const Zeni::String &str);

#endif
