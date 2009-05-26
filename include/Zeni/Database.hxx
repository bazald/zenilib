/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
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

#ifndef ZENI_DATABASE_HXX
#define ZENI_DATABASE_HXX

// HXXed below
#include <Zeni/Resource.h>
#include <Zeni/XML.h>

#include <Zeni/Database.h>

#include <iostream>

namespace Zeni {

  template <class TYPE>
  Database<TYPE>::Database(const std::string &filename, const std::string &xml_identifier)
    : m_filename(filename),
    m_xml_identifier(xml_identifier)
  {
  }

  template <class TYPE>
  Database<TYPE>::~Database() {
  }
  
  template <class TYPE>
  unsigned long Database<TYPE>::give(const std::string &name, TYPE * const &type) {
    try {
      clear(name);
    }
    catch(Database_Entry_Not_Found &)
    {
    }

    if(!type)
      throw Null_Database_Entry_Set();

    unsigned long id = get_Resource().assign();
    m_lookup[name] = id;
    m_database[id] = type;

    return id;
  }
  
  template <class TYPE>
  unsigned long Database<TYPE>::lend(const std::string &name, TYPE * const &type) {
    const unsigned long retval = give_type(name, type);
    m_lent.insert(type);
    return retval;
  }

  template <class TYPE>
  void Database<TYPE>::clear(const std::string &name) {
    stdext::hash_map<std::string, unsigned long>::iterator it = m_lookup.find(name);

    if(it == m_lookup.end())
      throw Database_Entry_Not_Found(name);

    typename stdext::hash_map<unsigned long, TYPE *>::iterator jt = m_database.find(it->second);

    assert(jt != m_database.end());

    typename std::set<TYPE *>::iterator kt = m_lent.find(jt->second);

    if(kt == m_lent.end())
      delete jt->second;
    else
      m_lent.erase(kt);

    m_database.erase(it->second);
    m_lookup.erase(it);
  }

  template <class TYPE>
  unsigned long Database<TYPE>::get_id(const std::string &name) const {
    stdext::hash_map<std::string, unsigned long>::const_iterator it = m_lookup.find(name);

    if(it == m_lookup.end() || !it->second)
      throw Database_Entry_Not_Found(name);

    return it->second;
  }

  template <class TYPE>
  TYPE & Database<TYPE>::operator[](const unsigned long &id) const {
    typename stdext::hash_map<unsigned long, TYPE *>::const_iterator it = m_database.find(id);

    if(it == m_database.end() || !it->second) {
      char buf[64];
#ifdef _WINDOWS
      sprintf_s
#else
      sprintf
#endif
        (buf, "ID = %u", static_cast<unsigned int>(id));
      throw Database_Entry_Not_Found(buf);
    }

    return *it->second;
  }

  template <class TYPE>
  TYPE & Database<TYPE>::operator[](const std::string &name) const {
    return (*this)[get_id(name)];
  }

  template <class TYPE>
  void Database<TYPE>::reload(const std::string &filename) {
    reload();
  }

  template <class TYPE>
  void Database<TYPE>::reload() {
    lose_resources();
    init();
  }

  template <class TYPE>
  void Database<TYPE>::init() {
    pre_init();

    m_lookup.clear();
    m_database.clear();

    XML_Document types_xml(m_filename.c_str());
    XML_Element types = types_xml[m_xml_identifier];
    std::string name;

    try {
      for(XML_Element it = types.first(); it.good(); it = it.next()) {
        const std::string name = it.value();

        TYPE * const type = load(it);
        if(!type)
          throw Database_Load_Entry_Failed(name);

        const unsigned long id = get_Resource().assign();
        m_lookup[name] = id;
        m_database[id] = type;
      }
    }
    catch(Database_Load_Entry_Failed &)
    {
      std::cerr << "Error loading '" << m_xml_identifier << "' entry '" << name << "'\n";
      throw;
    }

    post_init();
  }

  template <class TYPE>
  void Database<TYPE>::uninit() {
    pre_uninit();

    for(typename stdext::hash_map<unsigned long, TYPE *>::iterator it = m_database.begin();
        it != m_database.end();
        ++it)
    {
      typename std::set<TYPE *>::iterator jt = m_lent.find(it->second);

      if(jt == m_lent.end())
        delete it->second;
      else
        m_lent.erase(jt);
    }

    assert(m_lent.empty());
    m_database.clear();
    m_lookup.clear();

    post_uninit();
  }

  template <class TYPE>
  void Database<TYPE>::lose_resources() {
    pre_lose();

    typename stdext::hash_map<std::string, unsigned long> type_lookup;
    typename stdext::hash_map<unsigned long, TYPE *> types;

    type_lookup.swap(m_lookup);
    types.swap(m_database);

    for(typename stdext::hash_map<std::string, unsigned long>::iterator it = type_lookup.begin();
        it != type_lookup.end();
        ++it) {
      typename stdext::hash_map<unsigned long, TYPE *>::iterator jt = types.find(it->second);
      typename std::set<TYPE *>::iterator kt = m_lent.find(jt->second);

      if(keep(*jt->second))
        give(it->first, jt->second);
      else if(kt == m_lent.end())
        delete jt->second;
      else
        m_lent.erase(kt);
    }

    post_lose();
  }

  template <class TYPE>
  bool Database<TYPE>::keep(const TYPE &) {
    return false;
  }

}

#include <Zeni/Resource.hxx>
#include <Zeni/XML.hxx>

#endif
