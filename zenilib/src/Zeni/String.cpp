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

  String::iterator::iterator() {}

  String::iterator::iterator(const iterator &rhs) : std::string::iterator(rhs) {}
  String::iterator::iterator(iterator &&rhs) : std::string::iterator(std::forward<std::string::iterator>(rhs)) {}
  String::iterator & String::iterator::operator=(const iterator &rhs) {
    this->std::string::iterator::operator=(rhs);
    return *this;
  }
  String::iterator & String::iterator::operator=(iterator &&rhs) {
    this->std::string::iterator::operator=(std::forward<std::string::iterator>(rhs));
    return *this;
  }
      
  bool String::iterator::operator==(const iterator &rhs) const {return static_cast<const std::string::iterator &>(*this) == static_cast<const std::string::iterator &>(rhs);}
  bool String::iterator::operator!=(const iterator &rhs) const {return static_cast<const std::string::iterator &>(*this) != static_cast<const std::string::iterator &>(rhs);}
      
  String::iterator::value_type String::iterator::operator*() const {return this->std::string::iterator::operator*();}
  String::iterator::value_type & String::iterator::operator*() {return this->std::string::iterator::operator*();}
  String::iterator::value_type String::iterator::operator->() const {return this->std::string::iterator::operator*();}
  String::iterator::value_type & String::iterator::operator->() {return this->std::string::iterator::operator*();}
      
  String::iterator String::iterator::operator++() {
    this->std::string::iterator::operator++();
    return *this;
  }
  String::iterator String::iterator::operator--() {
    this->std::string::iterator::operator--();
    return *this;
  }
  String::iterator String::iterator::operator++(int) {
    this->std::string::iterator::operator++(0);
    return *this;
  }
  String::iterator String::iterator::operator--(int) {
    this->std::string::iterator::operator--(0);
    return *this;
  }

  String::iterator String::iterator::operator+(const difference_type &n) const {return this->std::string::iterator::operator+(n);}
  String::iterator String::iterator::operator-(const difference_type &n) const {return this->std::string::iterator::operator-(n);}
  String::iterator::difference_type String::iterator::operator-(const iterator &rhs) const {return static_cast<const std::string::iterator &>(*this) - static_cast<const std::string::iterator &>(rhs);}

  bool String::iterator::operator<(const iterator &rhs) const {return static_cast<const std::string::iterator &>(*this) < static_cast<const std::string::iterator &>(rhs);}
  bool String::iterator::operator>(const iterator &rhs) const {return static_cast<const std::string::iterator &>(*this) > static_cast<const std::string::iterator &>(rhs);}
  bool String::iterator::operator<=(const iterator &rhs) const {return static_cast<const std::string::iterator &>(*this) <= static_cast<const std::string::iterator &>(rhs);}
  bool String::iterator::operator>=(const iterator &rhs) const {return static_cast<const std::string::iterator &>(*this) >= static_cast<const std::string::iterator &>(rhs);}

  String::iterator String::iterator::operator+=(const difference_type &n) {
    this->std::string::iterator::operator+=(n);
    return *this;
  }
  String::iterator String::iterator::operator-=(const difference_type &n) {
    this->std::string::iterator::operator-=(n);
    return *this;
  }
      
  String::iterator::value_type String::iterator::operator[](const difference_type &n) const {return this->std::string::iterator::operator[](n);}
  String::iterator::value_type & String::iterator::operator[](const difference_type &n) {return this->std::string::iterator::operator[](n);}

  String::iterator::iterator(const std::string::iterator &iter) : std::string::iterator(iter) {}
  String::iterator::iterator(std::string::iterator &&iter) : std::string::iterator(std::forward<std::string::iterator>(iter)) {}
  String::iterator & String::iterator::operator=(const std::string::iterator &iter) {
    std::string::iterator(*this) = iter;
    return *this;
  }
  String::iterator & String::iterator::operator=(std::string::iterator &&iter) {
    std::string::iterator(*this) = std::forward<std::string::iterator>(iter);
    return *this;
  }

  String::const_iterator::const_iterator() {}

  String::const_iterator::const_iterator(const const_iterator &rhs) : std::string::const_iterator(rhs) {}
  String::const_iterator::const_iterator(const_iterator &&rhs) : std::string::const_iterator(std::forward<std::string::const_iterator>(rhs)) {}
  String::const_iterator & String::const_iterator::operator=(const const_iterator &rhs) {
    this->std::string::const_iterator::operator=(rhs);
    return *this;
  }
  String::const_iterator & String::const_iterator::operator=(const_iterator &&rhs) {
    this->std::string::const_iterator::operator=(std::forward<std::string::const_iterator>(rhs));
    return *this;
  }

  String::const_iterator::const_iterator(const String::iterator &rhs) : std::string::const_iterator(rhs) {}
  String::const_iterator::const_iterator(String::iterator &&rhs) : std::string::const_iterator(std::forward<std::string::iterator>(rhs)) {}
  String::const_iterator & String::const_iterator::operator=(const String::iterator &rhs) {
    this->std::string::const_iterator::operator=(rhs);
    return *this;
  }
  String::const_iterator & String::const_iterator::operator=(String::iterator &&rhs) {
    this->std::string::const_iterator::operator=(std::forward<std::string::iterator>(rhs));
    return *this;
  }

  bool String::const_iterator::operator==(const const_iterator &rhs) const {return static_cast<const std::string::const_iterator &>(*this) == static_cast<const std::string::const_iterator &>(rhs);}
  bool String::const_iterator::operator!=(const const_iterator &rhs) const {return static_cast<const std::string::const_iterator &>(*this) != static_cast<const std::string::const_iterator &>(rhs);}
      
  String::const_iterator::value_type String::const_iterator::operator*() const {return this->std::string::const_iterator::operator*();}
  String::const_iterator::value_type String::const_iterator::operator->() const {return this->std::string::const_iterator::operator*();}
      
  String::const_iterator String::const_iterator::operator++() {
    this->std::string::const_iterator::operator++();
    return *this;
  }
  String::const_iterator String::const_iterator::operator--() {
    this->std::string::const_iterator::operator--();
    return *this;
  }
  String::const_iterator String::const_iterator::operator++(int) {
    this->std::string::const_iterator::operator++(0);
    return *this;
  }
  String::const_iterator String::const_iterator::operator--(int) {
    this->std::string::const_iterator::operator--(0);
    return *this;
  }

  String::const_iterator String::const_iterator::operator+(const difference_type &n) const {return this->std::string::const_iterator::operator+(n);}
  String::const_iterator String::const_iterator::operator-(const difference_type &n) const {return this->std::string::const_iterator::operator-(n);}
  String::const_iterator::difference_type String::const_iterator::operator-(const const_iterator &rhs) const {return static_cast<const std::string::const_iterator &>(*this) - static_cast<const std::string::const_iterator &>(rhs);}

  bool String::const_iterator::operator<(const const_iterator &rhs) const {return static_cast<const std::string::const_iterator &>(*this) < static_cast<const std::string::const_iterator &>(rhs);}
  bool String::const_iterator::operator>(const const_iterator &rhs) const {return static_cast<const std::string::const_iterator &>(*this) > static_cast<const std::string::const_iterator &>(rhs);}
  bool String::const_iterator::operator<=(const const_iterator &rhs) const {return static_cast<const std::string::const_iterator &>(*this) <= static_cast<const std::string::const_iterator &>(rhs);}
  bool String::const_iterator::operator>=(const const_iterator &rhs) const {return static_cast<const std::string::const_iterator &>(*this) >= static_cast<const std::string::const_iterator &>(rhs);}

  String::const_iterator String::const_iterator::operator+=(const difference_type &n) {
    this->std::string::const_iterator::operator+=(n);
    return *this;
  }
  String::const_iterator String::const_iterator::operator-=(const difference_type &n) {
    this->std::string::const_iterator::operator-=(n);
    return *this;
  }
      
  String::const_iterator::value_type String::const_iterator::operator[](const difference_type &n) const {return this->std::string::const_iterator::operator[](n);}

  String::const_iterator::const_iterator(const std::string::const_iterator &iter) : std::string::const_iterator(iter) {}
  String::const_iterator::const_iterator(std::string::iterator &&iter) : std::string::const_iterator(std::forward<std::string::const_iterator>(iter)) {}
  String::const_iterator & String::const_iterator::operator=(const std::string::const_iterator &iter) {
    std::string::const_iterator(*this) = iter;
    return *this;
  }
  String::const_iterator & String::const_iterator::operator=(std::string::const_iterator &&iter) {
    std::string::const_iterator(*this) = std::forward<std::string::const_iterator>(iter);
    return *this;
  }

  String::reverse_iterator::reverse_iterator() {}

  String::reverse_iterator::reverse_iterator(const reverse_iterator &rhs) : std::string::reverse_iterator(rhs) {}
  String::reverse_iterator::reverse_iterator(reverse_iterator &&rhs) : std::string::reverse_iterator(std::forward<std::string::reverse_iterator>(rhs)) {}
  String::reverse_iterator & String::reverse_iterator::operator=(const reverse_iterator &rhs) {
    this->std::string::reverse_iterator::operator=(rhs);
    return *this;
  }
  String::reverse_iterator & String::reverse_iterator::operator=(reverse_iterator &&rhs) {
    this->std::string::reverse_iterator::operator=(std::forward<std::string::reverse_iterator>(rhs));
    return *this;
  }
      
  bool String::reverse_iterator::operator==(const reverse_iterator &rhs) const {return static_cast<const std::string::reverse_iterator &>(*this) == static_cast<const std::string::reverse_iterator &>(rhs);}
  bool String::reverse_iterator::operator!=(const reverse_iterator &rhs) const {return static_cast<const std::string::reverse_iterator &>(*this) != static_cast<const std::string::reverse_iterator &>(rhs);}
      
  String::reverse_iterator::value_type String::reverse_iterator::operator*() const {return this->std::string::reverse_iterator::operator*();}
  String::reverse_iterator::value_type & String::reverse_iterator::operator*() {return this->std::string::reverse_iterator::operator*();}
  String::reverse_iterator::value_type String::reverse_iterator::operator->() const {return this->std::string::reverse_iterator::operator*();}
  String::reverse_iterator::value_type & String::reverse_iterator::operator->() {return this->std::string::reverse_iterator::operator*();}
      
  String::reverse_iterator String::reverse_iterator::operator++() {
    this->std::string::reverse_iterator::operator++();
    return *this;
  }
  String::reverse_iterator String::reverse_iterator::operator--() {
    this->std::string::reverse_iterator::operator--();
    return *this;
  }
  String::reverse_iterator String::reverse_iterator::operator++(int) {
    this->std::string::reverse_iterator::operator++(0);
    return *this;
  }
  String::reverse_iterator String::reverse_iterator::operator--(int) {
    this->std::string::reverse_iterator::operator--(0);
    return *this;
  }

  String::reverse_iterator String::reverse_iterator::operator+(const difference_type &n) const {return this->std::string::reverse_iterator::operator+(n);}
  String::reverse_iterator String::reverse_iterator::operator-(const difference_type &n) const {return this->std::string::reverse_iterator::operator-(n);}
  String::reverse_iterator::difference_type String::reverse_iterator::operator-(const reverse_iterator &rhs) const {return static_cast<const std::string::reverse_iterator &>(*this) - static_cast<const std::string::reverse_iterator &>(rhs);}

  bool String::reverse_iterator::operator<(const reverse_iterator &rhs) const {return static_cast<const std::string::reverse_iterator &>(*this) < static_cast<const std::string::reverse_iterator &>(rhs);}
  bool String::reverse_iterator::operator>(const reverse_iterator &rhs) const {return static_cast<const std::string::reverse_iterator &>(*this) > static_cast<const std::string::reverse_iterator &>(rhs);}
  bool String::reverse_iterator::operator<=(const reverse_iterator &rhs) const {return static_cast<const std::string::reverse_iterator &>(*this) <= static_cast<const std::string::reverse_iterator &>(rhs);}
  bool String::reverse_iterator::operator>=(const reverse_iterator &rhs) const {return static_cast<const std::string::reverse_iterator &>(*this) >= static_cast<const std::string::reverse_iterator &>(rhs);}

  String::reverse_iterator String::reverse_iterator::operator+=(const difference_type &n) {
    this->std::string::reverse_iterator::operator+=(n);
    return *this;
  }
  String::reverse_iterator String::reverse_iterator::operator-=(const difference_type &n) {
    this->std::string::reverse_iterator::operator-=(n);
    return *this;
  }
      
  String::reverse_iterator::value_type String::reverse_iterator::operator[](const difference_type &n) const {return this->std::string::reverse_iterator::operator[](n);}
  String::reverse_iterator::value_type & String::reverse_iterator::operator[](const difference_type &n) {return this->std::string::reverse_iterator::operator[](n);}

  String::reverse_iterator::reverse_iterator(const std::string::reverse_iterator &iter) : std::string::reverse_iterator(iter) {}
  String::reverse_iterator::reverse_iterator(std::string::reverse_iterator &&iter) : std::string::reverse_iterator(std::forward<std::string::reverse_iterator>(iter)) {}
  String::reverse_iterator & String::reverse_iterator::operator=(const std::string::reverse_iterator &iter) {
    std::string::reverse_iterator(*this) = iter;
    return *this;
  }
  String::reverse_iterator & String::reverse_iterator::operator=(std::string::reverse_iterator &&iter) {
    std::string::reverse_iterator(*this) = std::forward<std::string::reverse_iterator>(iter);
    return *this;
  }

  String::const_reverse_iterator::const_reverse_iterator() {}

  String::const_reverse_iterator::const_reverse_iterator(const const_reverse_iterator &rhs) : std::string::const_reverse_iterator(rhs) {}
  String::const_reverse_iterator::const_reverse_iterator(const_reverse_iterator &&rhs) : std::string::const_reverse_iterator(std::forward<std::string::const_reverse_iterator>(rhs)) {}
  String::const_reverse_iterator & String::const_reverse_iterator::operator=(const const_reverse_iterator &rhs) {
    this->std::string::const_reverse_iterator::operator=(rhs);
    return *this;
  }
  String::const_reverse_iterator & String::const_reverse_iterator::operator=(const_reverse_iterator &&rhs) {
    this->std::string::const_reverse_iterator::operator=(std::forward<std::string::const_reverse_iterator>(rhs));
    return *this;
  }

  String::const_reverse_iterator::const_reverse_iterator(const String::reverse_iterator &rhs) : std::string::const_reverse_iterator(rhs) {}
  String::const_reverse_iterator::const_reverse_iterator(String::reverse_iterator &&rhs) : std::string::const_reverse_iterator(std::forward<std::string::reverse_iterator>(rhs)) {}
  String::const_reverse_iterator & String::const_reverse_iterator::operator=(const String::reverse_iterator &rhs) {
    this->std::string::const_reverse_iterator::operator=(rhs);
    return *this;
  }
  String::const_reverse_iterator & String::const_reverse_iterator::operator=(String::reverse_iterator &&rhs) {
    this->std::string::const_reverse_iterator::operator=(std::forward<std::string::reverse_iterator>(rhs));
    return *this;
  }

  bool String::const_reverse_iterator::operator==(const const_reverse_iterator &rhs) const {return static_cast<const std::string::const_reverse_iterator &>(*this) == static_cast<const std::string::const_reverse_iterator &>(rhs);}
  bool String::const_reverse_iterator::operator!=(const const_reverse_iterator &rhs) const {return static_cast<const std::string::const_reverse_iterator &>(*this) != static_cast<const std::string::const_reverse_iterator &>(rhs);}

  String::const_reverse_iterator::value_type String::const_reverse_iterator::operator*() const {return this->std::string::const_reverse_iterator::operator*();}
  String::const_reverse_iterator::value_type String::const_reverse_iterator::operator->() const {return this->std::string::const_reverse_iterator::operator*();}

  String::const_reverse_iterator String::const_reverse_iterator::operator++() {
    this->std::string::const_reverse_iterator::operator++();
    return *this;
  }
  String::const_reverse_iterator String::const_reverse_iterator::operator--() {
    this->std::string::const_reverse_iterator::operator--();
    return *this;
  }
  String::const_reverse_iterator String::const_reverse_iterator::operator++(int) {
    this->std::string::const_reverse_iterator::operator++(0);
    return *this;
  }
  String::const_reverse_iterator String::const_reverse_iterator::operator--(int) {
    this->std::string::const_reverse_iterator::operator--(0);
    return *this;
  }

  String::const_reverse_iterator String::const_reverse_iterator::operator+(const difference_type &n) const {return this->std::string::const_reverse_iterator::operator+(n);}
  String::const_reverse_iterator String::const_reverse_iterator::operator-(const difference_type &n) const {return this->std::string::const_reverse_iterator::operator-(n);}
  String::const_reverse_iterator::difference_type String::const_reverse_iterator::operator-(const const_reverse_iterator &rhs) const {return static_cast<const std::string::const_reverse_iterator &>(*this) - static_cast<const std::string::const_reverse_iterator &>(rhs);}

  bool String::const_reverse_iterator::operator<(const const_reverse_iterator &rhs) const {return static_cast<const std::string::const_reverse_iterator &>(*this) < static_cast<const std::string::const_reverse_iterator &>(rhs);}
  bool String::const_reverse_iterator::operator>(const const_reverse_iterator &rhs) const {return static_cast<const std::string::const_reverse_iterator &>(*this) > static_cast<const std::string::const_reverse_iterator &>(rhs);}
  bool String::const_reverse_iterator::operator<=(const const_reverse_iterator &rhs) const {return static_cast<const std::string::const_reverse_iterator &>(*this) <= static_cast<const std::string::const_reverse_iterator &>(rhs);}
  bool String::const_reverse_iterator::operator>=(const const_reverse_iterator &rhs) const {return static_cast<const std::string::const_reverse_iterator &>(*this) >= static_cast<const std::string::const_reverse_iterator &>(rhs);}

  String::const_reverse_iterator String::const_reverse_iterator::operator+=(const difference_type &n) {
    this->std::string::const_reverse_iterator::operator+=(n);
    return *this;
  }
  String::const_reverse_iterator String::const_reverse_iterator::operator-=(const difference_type &n) {
    this->std::string::const_reverse_iterator::operator-=(n);
    return *this;
  }
      
  String::const_reverse_iterator::value_type String::const_reverse_iterator::operator[](const difference_type &n) const {return this->std::string::const_reverse_iterator::operator[](n);}

  String::const_reverse_iterator::const_reverse_iterator(const std::string::const_reverse_iterator &iter) : std::string::const_reverse_iterator(iter) {}
  String::const_reverse_iterator::const_reverse_iterator(std::string::const_reverse_iterator &&iter) : std::string::const_reverse_iterator(std::forward<std::string::const_reverse_iterator>(iter)) {}
  String::const_reverse_iterator & String::const_reverse_iterator::operator=(const std::string::const_reverse_iterator &iter) {
    std::string::const_reverse_iterator(*this) = iter;
    return *this;
  }
  String::const_reverse_iterator & String::const_reverse_iterator::operator=(std::string::const_reverse_iterator &&iter) {
    std::string::const_reverse_iterator(*this) = std::forward<std::string::const_reverse_iterator>(iter);
    return *this;
  }

  String::String() {}
  String::String(const String &str) : std::string(str) {}
  String::String(String &&str) : std::string(std::forward<String>(str)) {}
  String::String(const String &str, size_t pos, size_t n) : std::string(str, pos, n) {}
  String::String(const char * s, size_t n) : std::string(s, n) {}
  String::String(const char * s) : std::string(s) {}
  String::String(size_t n, char c) : std::string(n, c) {}

  String & String::operator=(const String &str) {
    std::string::operator=(str);
    return *this;
  }
  String & String::operator=(String &&str) {
    std::string::operator=(std::forward<String>(str));
    return *this;
  }
  String & String::operator=(const char *s) {
    std::string::operator=(s);
    return *this;
  }
  String & String::operator=(char c) {
    std::string::operator=(c);
    return *this;
  }

  String::iterator String::begin() {return std::string::begin();}
  String::const_iterator String::begin() const {return std::string::begin();}

  String::iterator String::end() {return std::string::end();}
  String::const_iterator String::end() const {return std::string::end();}

  String::reverse_iterator String::rbegin() {return std::string::rbegin();}
  String::const_reverse_iterator String::rbegin() const {return std::string::rbegin();}

  String::reverse_iterator String::rend() {return std::string::rend();}
  String::const_reverse_iterator String::rend() const {return std::string::rend();}

  size_t String::size() const {return std::string::size();}

  size_t String::length() const {return std::string::length();}

  size_t String::max_size() const {return std::string::max_size();}

  void String::resize(size_t n, char c) {std::string::resize(n, c);}
  void String::resize(size_t n) {std::string::resize(n);}

  size_t String::capacity() const {return std::string::capacity();}

  void String::reserve(size_t res_arg) {std::string::reserve(res_arg);}

  void String::clear() {std::string::clear();}

  bool String::empty() const {return std::string::empty();}

  String::value_type String::operator[](const unsigned int &pos) const {return static_cast<const std::string &>(*this)[pos];}
  String::value_type & String::operator[](const unsigned int &pos) {return static_cast<std::string &>(*this)[pos];}

  String::value_type String::at(size_t pos) const {return std::string::at(pos);}
  String::value_type & String::at(size_t pos) {return std::string::at(pos);}

  String & String::operator+=(const String & str) {
    std::string::operator+=(str);
    return *this;
  }
  String & String::operator+=(const char *s) {
    std::string::operator+=(s);
    return *this;
  }
  String & String::operator+=(char c) {
    std::string::operator+=(c);
    return *this;
  }
    
  String & String::append(const String &str) {
    std::string::append(str);
    return *this;
  }
  String & String::append(const String &str, size_t pos, size_t n) {
    std::string::append(str, pos, n);
    return *this;
  }
  String & String::append(const char *s, size_t n) {
    std::string::append(s, n);
    return *this;
  }
  String & String::append(const char *s) {
    std::string::append(s);
    return *this;
  }
  String & String::append(size_t n, char c) {
    std::string::append(n, c);
    return *this;
  }

  void String::push_back(char c) {std::string::push_back(c);}
    
  String & String::assign(const String &str) {
    std::string::assign(str);
    return *this;
  }
  String & String::assign(String &&str) {
    std::string::assign(std::forward<String>(str));
    return *this;
  }
  String & String::assign(const String &str, size_t pos, size_t n) {
    std::string::assign(str, pos, n);
    return *this;
  }
  String & String::assign(const char *s, size_t n) {
    std::string::assign(s, n);
    return *this;
  }
  String & String::assign(const char *s) {
    std::string::assign(s);
    return *this;
  }
  String & String::assign(size_t n, char c) {
    std::string::assign(n, c);
    return *this;
  }
    
  String & String::insert(size_t pos1, const String &str) {
    std::string::insert(pos1, str);
    return *this;
  }
  String & String::insert(size_t pos1, const String &str, size_t pos2, size_t n) {
    std::string::insert(pos1, str, pos2, n);
    return *this;
  }
  String & String::insert(size_t pos1, const char *s, size_t n) {
    std::string::insert(pos1, s, n);
    return *this;
  }
  String & String::insert(size_t pos1, const char *s) {
    std::string::insert(pos1, s);
    return *this;
  }
  String & String::insert(size_t pos1, size_t n, char c) {
    std::string::insert(pos1, n, c);
    return *this;
  }
  String::iterator String::insert(iterator p, char c) {
    return std::string::insert(p, c);
  }
  void String::insert(iterator p, size_t n, char c) {
    std::string::insert(p, n, c);
  }

  String & String::erase(size_t pos, size_t n) {
    std::string::erase(pos, n);
    return *this;
  }
  String::iterator String::erase(iterator position) {
    return std::string::erase(position);
  }
  String::iterator String::erase(iterator first, iterator last) {
    return std::string::erase(first, last);
  }

  String & String::replace(size_t pos1, size_t n1, const String &str) {
    std::string::replace(pos1, n1, str);
    return *this;
  }
  String & String::replace(iterator i1, iterator i2, const String &str) {
    std::string::replace(i1, i2, str);
    return *this;
  }
  String & String::replace(size_t pos1, size_t n1, const String &str, size_t pos2, size_t n2) {
    std::string::replace(pos1, n1, str, pos2, n2);
    return *this;
  }
  String & String::replace(size_t pos1, size_t n1, const char *s, size_t n2) {
    std::string::replace(pos1, n1, s, n2);
    return *this;
  }
  String & String::replace(iterator i1, iterator i2, const char *s, size_t n2) {
    std::string::replace(i1, i2, s, n2);
    return *this;
  }
  String & String::replace(size_t pos1, size_t n1, const char *s) {
    std::string::replace(pos1, n1, s);
    return *this;
  }
  String & String::replace(iterator i1, iterator i2, const char *s) {
    std::string::replace(i1, i2, s);
    return *this;
  }
  String & String::replace(size_t pos1, size_t n1, size_t n2, char c) {
    std::string::replace(pos1, n1, n2, c);
    return *this;
  }
  String & String::replace(iterator i1, iterator i2, size_t n2, char c) {
    std::string::replace(i1, i2, n2, c);
    return *this;
  }

  void String::swap(String &str) {std::string::swap(str);}

  const char * String::c_str() const {return std::string::c_str();}

  const char * String::data() const {return std::string::data();}

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif
  size_t String::copy(char *s, size_t n, size_t pos) const {return std::string::copy(s, n, pos);}
#ifdef _WINDOWS
#pragma warning( pop )
#endif

  size_t String::find(const String &str, size_t pos) const {return std::string::find(str, pos);}
  size_t String::find(const char *s, size_t pos, size_t n) const {return std::string::find(s, pos, n);}
  size_t String::find(const char *s, size_t pos) const {return std::string::find(s, pos);}
  size_t String::find(char c, size_t pos) const {return std::string::find(c, pos);}

  size_t String::rfind(const String &str, size_t pos) const {return std::string::rfind(str, pos);}
  size_t String::rfind(const char *s, size_t pos, size_t n) const {return std::string::rfind(s, pos, n);}
  size_t String::rfind(const char *s, size_t pos) const {return std::string::rfind(s, pos);}
  size_t String::rfind(char c, size_t pos) const {return std::string::rfind(c, pos);}

  size_t String::find_first_of(const String &str, size_t pos) const {return std::string::find_first_of(str, pos);}
  size_t String::find_first_of(const char *s, size_t pos, size_t n) const {return std::string::find_first_of(s, pos, n);}
  size_t String::find_first_of(const char *s, size_t pos) const {return std::string::find_first_of(s, pos);}
  size_t String::find_first_of(char c, size_t pos) const {return std::string::find_first_of(c, pos);}

  size_t String::find_last_of(const String &str, size_t pos) const {return std::string::find_last_of(str, pos);}
  size_t String::find_last_of(const char *s, size_t pos, size_t n) const {return std::string::find_last_of(s, pos, n);}
  size_t String::find_last_of(const char *s, size_t pos) const {return std::string::find_last_of(s, pos);}
  size_t String::find_last_of(char c, size_t pos) const {return std::string::find_last_of(c, pos);}

  size_t String::find_first_not_of(const String &str, size_t pos) const {return std::string::find_first_not_of(str, pos);}
  size_t String::find_first_not_of(const char *s, size_t pos, size_t n) const {return std::string::find_first_not_of(s, pos, n);}
  size_t String::find_first_not_of(const char *s, size_t pos) const {return std::string::find_first_not_of(s, pos);}
  size_t String::find_first_not_of(char c, size_t pos) const {return std::string::find_first_not_of(c, pos);}

  size_t String::find_last_not_of(const String &str, size_t pos) const {return std::string::find_last_not_of(str, pos);}
  size_t String::find_last_not_of(const char *s, size_t pos, size_t n) const {return std::string::find_last_not_of(s, pos, n);}
  size_t String::find_last_not_of(const char *s, size_t pos) const {return std::string::find_last_not_of(s, pos);}
  size_t String::find_last_not_of(char c, size_t pos) const {return std::string::find_last_not_of(c, pos);}

  String String::substr(size_t pos, size_t n) const {return String(std::string::substr(pos, n));}

  int String::compare(const String &str) const {return std::string::compare(str);}
  int String::compare(const char *s) const {return std::string::compare(s);}
  int String::compare(size_t pos1, size_t n1, const String &str) const {return std::string::compare(pos1, n1, str);}
  int String::compare(size_t pos1, size_t n1, const char *s) const {return std::string::compare(pos1, n1, s);}
  int String::compare(size_t pos1, size_t n1, const String &str, size_t pos2, size_t n2) const {return std::string::compare(pos1, n1, str, pos2, n2);}
  int String::compare(size_t pos1, size_t n1, const char *s, size_t n2) const {return std::string::compare(pos1, n1, s, n2);}

  bool String::operator==(const Zeni::String &rhs) const {return static_cast<const std::string &>(*this) == static_cast<const std::string &>(rhs);}
  bool String::operator==(const char *rhs) const {return static_cast<const std::string &>(*this) == rhs;}
  bool String::operator!=(const Zeni::String &rhs) const {return static_cast<const std::string &>(*this) != static_cast<const std::string &>(rhs);}
  bool String::operator!=(const char *rhs) const {return static_cast<const std::string &>(*this) != rhs;}
  bool String::operator<(const Zeni::String &rhs) const {return static_cast<const std::string &>(*this) < static_cast<const std::string &>(rhs);}
  bool String::operator<(const char *rhs) const {return static_cast<const std::string &>(*this) < rhs;}
  bool String::operator>(const Zeni::String &rhs) const {return static_cast<const std::string &>(*this) > static_cast<const std::string &>(rhs);}
  bool String::operator>(const char *rhs) const {return static_cast<const std::string &>(*this) > rhs;}
  bool String::operator<=(const Zeni::String &rhs) const {return static_cast<const std::string &>(*this) <= static_cast<const std::string &>(rhs);}
  bool String::operator<=(const char *rhs) const {return static_cast<const std::string &>(*this) <= rhs;}
  bool String::operator>=(const Zeni::String &rhs) const {return static_cast<const std::string &>(*this) >= static_cast<const std::string &>(rhs);}
  bool String::operator>=(const char *rhs) const {return static_cast<const std::string &>(*this) >= rhs;}

  size_t String::Hash::operator()(const String &str) const {
    size_t val = 42u;
    for(const_iterator ii = str.begin(), iend = str.end(); ii != iend; ++ii)
      val = ((val << 5) | (val >> (8u * sizeof(size_t) - 5))) + *ii;
    return val;
  }

  bool String::Hash::operator()(const String &lhs, const String &rhs) const {
    return lhs != rhs;
  }

}

ZENI_DLL Zeni::String::iterator operator+(const Zeni::String::iterator::difference_type &lhs, const Zeni::String::iterator &rhs) {return rhs + lhs;}
ZENI_DLL Zeni::String::const_iterator operator+(const Zeni::String::const_iterator::difference_type &lhs, const Zeni::String::const_iterator &rhs) {return rhs + lhs;}
ZENI_DLL Zeni::String::reverse_iterator operator+(const Zeni::String::reverse_iterator::difference_type &lhs, const Zeni::String::reverse_iterator &rhs) {return rhs + lhs;}
ZENI_DLL Zeni::String::const_reverse_iterator operator+(const Zeni::String::const_reverse_iterator::difference_type &lhs, const Zeni::String::const_reverse_iterator &rhs) {return rhs + lhs;}

ZENI_DLL Zeni::String operator+(const Zeni::String &lhs, const Zeni::String &rhs) {
  Zeni::String rv(lhs);
  return rv += rhs;
}
ZENI_DLL Zeni::String operator+(const Zeni::String &lhs, const char *rhs) {
  Zeni::String rv(lhs);
  return rv += rhs;
}
ZENI_DLL Zeni::String operator+(const Zeni::String &lhs, char rhs) {
  Zeni::String rv(lhs);
  return rv += rhs;
}
ZENI_DLL Zeni::String operator+(const char *lhs, const Zeni::String &rhs) {
  Zeni::String rv(lhs);
  return rv += rhs;
}
ZENI_DLL Zeni::String operator+(char lhs, const Zeni::String &rhs) {
  Zeni::String rv(1, lhs);
  return rv += rhs;
}

ZENI_DLL bool operator==(const char *lhs, const Zeni::String &rhs) {return rhs == lhs;}
ZENI_DLL bool operator!=(const char *lhs, const Zeni::String &rhs) {return rhs != lhs;}
ZENI_DLL bool operator<(const char *lhs, const Zeni::String &rhs) {return rhs >= lhs;}
ZENI_DLL bool operator>(const char *lhs, const Zeni::String &rhs) {return rhs <= lhs;}
ZENI_DLL bool operator<=(const char *lhs, const Zeni::String &rhs) {return rhs > lhs;}
ZENI_DLL bool operator>=(const char *lhs, const Zeni::String &rhs) {return rhs < lhs;}

ZENI_DLL void swap(Zeni::String &lhs, Zeni::String &rhs) {
  lhs.swap(rhs);
}
