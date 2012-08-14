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
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef char value_type;
    typedef value_type * pointer;
    typedef value_type & reference;

    static const size_type npos = size_type(-1);

    class ZENI_DLL iterator {
      friend class String;

    public:
      typedef ptrdiff_t difference_type;
      typedef std::random_access_iterator_tag iterator_category;
      typedef char value_type;
      typedef value_type * pointer;
      typedef value_type & reference;

      iterator();
      ~iterator();

      iterator(const iterator &rhs);
      iterator & operator=(const iterator &rhs);

      bool operator==(const iterator &rhs) const;
      bool operator!=(const iterator &rhs) const;

      value_type operator*() const;
      value_type & operator*();
      value_type operator->() const;
      value_type & operator->();
      
      iterator operator++();
      iterator operator--();
      iterator operator++(int);
      iterator operator--(int);

      iterator operator+(const difference_type &n) const;
      iterator operator-(const difference_type &n) const;
      difference_type operator-(const iterator &rhs) const;

      bool operator<(const iterator &rhs) const;
      bool operator>(const iterator &rhs) const;
      bool operator<=(const iterator &rhs) const;
      bool operator>=(const iterator &rhs) const;

      iterator operator+=(const difference_type &n);
      iterator operator-=(const difference_type &n);
      
      value_type operator[](const difference_type &n) const;
      value_type & operator[](const difference_type &n);

    private:
      iterator(const std::string::iterator &iter);
      iterator & operator=(const std::string::iterator &iter);

      void * m_impl;
    };
    class ZENI_DLL const_iterator {
      friend class String;

    public:
      typedef ptrdiff_t difference_type;
      typedef std::random_access_iterator_tag iterator_category;
      typedef const char value_type;
      typedef value_type * pointer;
      typedef value_type & reference;

      const_iterator();
      ~const_iterator();

      const_iterator(const const_iterator &rhs);
      const_iterator & operator=(const const_iterator &rhs);

      const_iterator(const String::iterator &rhs);
      const_iterator & operator=(const String::iterator &rhs);

      bool operator==(const const_iterator &rhs) const;
      bool operator!=(const const_iterator &rhs) const;

      value_type operator*() const;
      value_type operator->() const;

      const_iterator operator++();
      const_iterator operator--();
      const_iterator operator++(int);
      const_iterator operator--(int);

      const_iterator operator+(const difference_type &n) const;
      const_iterator operator-(const difference_type &n) const;
      difference_type operator-(const const_iterator &rhs) const;

      bool operator<(const const_iterator &rhs) const;
      bool operator>(const const_iterator &rhs) const;
      bool operator<=(const const_iterator &rhs) const;
      bool operator>=(const const_iterator &rhs) const;

      const_iterator operator+=(const difference_type &n);
      const_iterator operator-=(const difference_type &n);
      
      value_type operator[](const difference_type &n) const;

    private:
      const_iterator(const std::string::const_iterator &iter);
      const_iterator & operator=(const std::string::const_iterator &iter);

      void * m_impl;
    };
    class ZENI_DLL reverse_iterator {
      friend class String;

    public:
      typedef ptrdiff_t difference_type;
      typedef std::random_access_iterator_tag iterator_category;
      typedef char value_type;
      typedef value_type * pointer;
      typedef value_type & reference;

      reverse_iterator();
      ~reverse_iterator();

      reverse_iterator(const reverse_iterator &rhs);
      reverse_iterator & operator=(const reverse_iterator &rhs);

      bool operator==(const reverse_iterator &rhs) const;
      bool operator!=(const reverse_iterator &rhs) const;

      value_type operator*() const;
      value_type & operator*();
      value_type operator->() const;
      value_type & operator->();

      reverse_iterator operator++();
      reverse_iterator operator--();
      reverse_iterator operator++(int);
      reverse_iterator operator--(int);

      reverse_iterator operator+(const difference_type &n) const;
      reverse_iterator operator-(const difference_type &n) const;
      difference_type operator-(const reverse_iterator &rhs) const;

      bool operator<(const reverse_iterator &rhs) const;
      bool operator>(const reverse_iterator &rhs) const;
      bool operator<=(const reverse_iterator &rhs) const;
      bool operator>=(const reverse_iterator &rhs) const;

      reverse_iterator operator+=(const difference_type &n);
      reverse_iterator operator-=(const difference_type &n);

      value_type operator[](const difference_type &n) const;
      value_type & operator[](const difference_type &n);

    private:
      reverse_iterator(const std::string::reverse_iterator &iter);
      reverse_iterator & operator=(const std::string::reverse_iterator &iter);

      void * m_impl;
    };
    class ZENI_DLL const_reverse_iterator {
      friend class String;

    public:
      typedef ptrdiff_t difference_type;
      typedef std::random_access_iterator_tag iterator_category;
      typedef const char value_type;
      typedef value_type * pointer;
      typedef value_type & reference;

      const_reverse_iterator();
      ~const_reverse_iterator();

      const_reverse_iterator(const const_reverse_iterator &rhs);
      const_reverse_iterator & operator=(const const_reverse_iterator &rhs);

      const_reverse_iterator(const String::reverse_iterator &rhs);
      const_reverse_iterator & operator=(const String::reverse_iterator &rhs);
      
      bool operator==(const const_reverse_iterator &rhs) const;
      bool operator!=(const const_reverse_iterator &rhs) const;
      
      value_type operator*() const;
      value_type operator->() const;
      
      const_reverse_iterator operator++();
      const_reverse_iterator operator--();
      const_reverse_iterator operator++(int);
      const_reverse_iterator operator--(int);

      const_reverse_iterator operator+(const difference_type &n) const;
      const_reverse_iterator operator-(const difference_type &n) const;
      difference_type operator-(const const_reverse_iterator &rhs) const;

      bool operator<(const const_reverse_iterator &rhs) const;
      bool operator>(const const_reverse_iterator &rhs) const;
      bool operator<=(const const_reverse_iterator &rhs) const;
      bool operator>=(const const_reverse_iterator &rhs) const;

      const_reverse_iterator operator+=(const difference_type &n);
      const_reverse_iterator operator-=(const difference_type &n);

      value_type operator[](const difference_type &n) const;

    private:
      const_reverse_iterator(const std::string::const_reverse_iterator &iter);
      const_reverse_iterator & operator=(const std::string::const_reverse_iterator &iter);

      void * m_impl;
    };

    String();
    String(const String &str);
    String(const String &str, size_t pos, size_t n = npos);
    String(const char * s, size_t n);
    String(const char * s);
    String(size_t n, char c);
    //template<class InputIterator> String(InputIterator begin, InputIterator end) : std::string(begin, end) {}

    ~String();

    String & operator=(const String &str);
    String & operator=(const char *s);
    String & operator=(char c);

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;

    reverse_iterator rend();
    const_reverse_iterator rend() const;

    size_t size() const;

    size_t length() const;

    size_t max_size() const;

    void resize(size_t n, char c);
    void resize(size_t n);

    size_t capacity() const;

    void reserve(size_t res_arg = 0);

    void clear();

    bool empty() const;

    value_type operator[](const unsigned int &pos) const;
    value_type & operator[](const unsigned int &pos);

    value_type at(size_t pos) const;
    value_type & at(size_t pos);

    String & operator+=(const String & str);
    String & operator+=(const char *s);
    String & operator+=(char c);
    
    String & append(const String &str);
    String & append(const String &str, size_t pos, size_t n);
    String & append(const char *s, size_t n);
    String & append(const char *s);
    String & append(size_t n, char c);
    //template <class InputIterator>
    //String & append(InputIterator first, InputIterator last) {
    //  append(first, last);
    //  return *this;
    //}

    void push_back(char c);
    
    String & assign(const String &str);
    String & assign(const String &str, size_t pos, size_t n);
    String & assign(const char *s, size_t n);
    String & assign(const char *s);
    String & assign(size_t n, char c);
    //template <class InputIterator>
    //String & assign(InputIterator first, InputIterator last) {
    //  assign(first, last);
    //  return *this;
    //}
    
    String & insert(size_t pos1, const String &str);
    String & insert(size_t pos1, const String &str, size_t pos2, size_t n);
    String & insert(size_t pos1, const char *s, size_t n);
    String & insert(size_t pos1, const char *s);
    String & insert(size_t pos1, size_t n, char c);
    iterator insert(iterator p, char c);
    void insert(iterator p, size_t n, char c);
    //template<class InputIterator>
    //void insert(iterator p, InputIterator first, InputIterator last) {
    //  insert(p, first, last);
    //}

    String & erase(size_t pos = 0, size_t n = npos);
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);

    String & replace(size_t pos1, size_t n1, const String &str);
    String & replace(iterator i1, iterator i2, const String &str);
    String & replace(size_t pos1, size_t n1, const String &str, size_t pos2, size_t n2);
    String & replace(size_t pos1, size_t n1, const char *s, size_t n2);
    String & replace(iterator i1, iterator i2, const char *s, size_t n2);
    String & replace(size_t pos1, size_t n1, const char *s);
    String & replace(iterator i1, iterator i2, const char *s);
    String & replace(size_t pos1, size_t n1, size_t n2, char c);
    String & replace(iterator i1, iterator i2, size_t n2, char c);
    //template<class InputIterator>
    //String & replace(iterator i1, iterator i2, InputIterator j1, InputIterator j2) {
    //  replace(i1, i2, j1, j2);
    //  return *this;
    //}

    void swap(String &str);

    const char * c_str() const;

    const char * data() const;

    size_t copy(char *s, size_t n, size_t pos = 0) const;

    size_t find(const String &str, size_t pos = 0) const;
    size_t find(const char *s, size_t pos, size_t n) const;
    size_t find(const char *s, size_t pos = 0) const;
    size_t find(char c, size_t pos = 0) const;

    size_t rfind(const String &str, size_t pos = npos) const;
    size_t rfind(const char *s, size_t pos, size_t n) const;
    size_t rfind(const char *s, size_t pos = npos) const;
    size_t rfind(char c, size_t pos = npos) const;

    size_t find_first_of(const String &str, size_t pos = 0) const;
    size_t find_first_of(const char *s, size_t pos, size_t n) const;
    size_t find_first_of(const char *s, size_t pos = 0) const;
    size_t find_first_of(char c, size_t pos = 0) const;

    size_t find_last_of(const String &str, size_t pos = npos) const;
    size_t find_last_of(const char *s, size_t pos, size_t n) const;
    size_t find_last_of(const char *s, size_t pos = npos) const;
    size_t find_last_of(char c, size_t pos = npos) const;

    size_t find_first_not_of(const String &str, size_t pos = 0) const;
    size_t find_first_not_of(const char *s, size_t pos, size_t n) const;
    size_t find_first_not_of(const char *s, size_t pos = 0) const;
    size_t find_first_not_of(char c, size_t pos = 0) const;

    size_t find_last_not_of(const String &str, size_t pos = npos) const;
    size_t find_last_not_of(const char *s, size_t pos, size_t n) const;
    size_t find_last_not_of(const char *s, size_t pos = npos) const;
    size_t find_last_not_of(char c, size_t pos = npos) const;

    String substr(size_t pos = 0, size_t n = npos) const;

    int compare(const String &str) const;
    int compare(const char *s) const;
    int compare(size_t pos1, size_t n1, const String &str) const;
    int compare(size_t pos1, size_t n1, const char *s) const;
    int compare(size_t pos1, size_t n1, const String &str, size_t pos2, size_t n2) const;
    int compare(size_t pos1, size_t n1, const char *s, size_t n2) const;

    bool operator==(const Zeni::String &rhs) const;
    bool operator==(const char *rhs) const;
    bool operator!=(const Zeni::String &rhs) const;
    bool operator!=(const char *rhs) const;
    bool operator<(const Zeni::String &rhs) const;
    bool operator<(const char *rhs) const;
    bool operator>(const Zeni::String &rhs) const;
    bool operator>(const char *rhs) const;
    bool operator<=(const Zeni::String &rhs) const;
    bool operator<=(const char *rhs) const;
    bool operator>=(const Zeni::String &rhs) const;
    bool operator>=(const char *rhs) const;

    struct ZENI_DLL Hash {
      static const size_t bucket_size = 4;
      static const size_t min_buckets = 8;

      size_t operator()(const String &str) const;

      bool operator()(const String &lhs, const String &rhs) const;
    };

    explicit inline String(const std::string &rhs) : m_impl(0) {
      String lhs(rhs.data(), rhs.size());
      swap(lhs);
    }

    inline String & operator=(const std::string &rhs) {
      String lhs(rhs);
      swap(lhs);
      return *this;
    }

    inline std::string std_str() const {
      std::string rv(data(), size());
      return rv;
    }

  private:
    void * m_impl;
  };

}

ZENI_DLL Zeni::String::iterator operator+(const Zeni::String::iterator::difference_type &lhs, const Zeni::String::iterator &rhs);
ZENI_DLL Zeni::String::const_iterator operator+(const Zeni::String::const_iterator::difference_type &lhs, const Zeni::String::const_iterator &rhs);
ZENI_DLL Zeni::String::reverse_iterator operator+(const Zeni::String::reverse_iterator::difference_type &lhs, const Zeni::String::reverse_iterator &rhs);
ZENI_DLL Zeni::String::const_reverse_iterator operator+(const Zeni::String::const_reverse_iterator::difference_type &lhs, const Zeni::String::const_reverse_iterator &rhs);

ZENI_DLL Zeni::String operator+(const Zeni::String &lhs, const Zeni::String &rhs);
ZENI_DLL Zeni::String operator+(const Zeni::String &lhs, const char *rhs);
ZENI_DLL Zeni::String operator+(const Zeni::String &lhs, char rhs);
ZENI_DLL Zeni::String operator+(const char *lhs, const Zeni::String &rhs);
ZENI_DLL Zeni::String operator+(char lhs, const Zeni::String &rhs);

ZENI_DLL bool operator==(const char *lhs, const Zeni::String &rhs);
ZENI_DLL bool operator!=(const char *lhs, const Zeni::String &rhs);
ZENI_DLL bool operator<(const char *lhs, const Zeni::String &rhs);
ZENI_DLL bool operator>(const char *lhs, const Zeni::String &rhs);
ZENI_DLL bool operator<=(const char *lhs, const Zeni::String &rhs);
ZENI_DLL bool operator>=(const char *lhs, const Zeni::String &rhs);

ZENI_DLL void swap(Zeni::String &lhs, Zeni::String &rhs);

ZENI_DLL std::istream & operator>>(std::istream &is, const Zeni::String &str);
ZENI_DLL std::ostream & operator<<(std::ostream &os, const Zeni::String &str);

#endif
