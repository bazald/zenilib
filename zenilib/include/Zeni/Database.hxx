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

#include <algorithm>
#include <iostream>

namespace Zeni {
  template <class TYPE>
  Database<TYPE>::Lookup::Handle::Handle()
    : ptr(0),
    lent(false)
  {
  }

  template <class TYPE>
  Database<TYPE>::Lookup::Handle::Handle(const std::string &filename_)
    : ptr(0),
    filename(filename_),
    lent(false)
  {
  }

  template <class TYPE>
  Database<TYPE>::Lookup::Handle::Handle(TYPE * const &ptr_,
                                         const std::string &filename_,
                                         const bool &lent_,
                                         const bool &keep_)
    : ptr(ptr_),
    filename(filename_),
    lent(lent_),
    keep(keep_)
  {
  }

  template <class TYPE>
  bool Database<TYPE>::Lookup::Handle::operator==(const Handle &rhs) const {
    return filename == rhs.filename;
  }

  template <class TYPE>
  Database<TYPE>::Lookup::Lookup()
    : id(0)
  {
  }

  template <class TYPE>
  Database<TYPE>::Lookup::Lookup(const unsigned long &id_, const Handle &handle_)
    : id(id_)
  {
    handles.push_front(handle_);
  }

  template <class TYPE>
  Database<TYPE>::Database(const std::string &filename, const std::string &xml_identifier)
    : m_xml_identifier(xml_identifier)
  {
    m_filenames.push_front(filename);
  }

  template <class TYPE>
  Database<TYPE>::~Database() {
  }
  
  template <class TYPE>
  unsigned long Database<TYPE>::give(const std::string &name, TYPE * const &type, const bool &keep, const std::string &filename) {
    if(!type)
      throw Null_Database_Entry_Set();

    Lookup *&lr = m_lookup[name];
    if(!lr)
      lr = new Lookup();

    if(!lr->id)
      lr->id = get_Resource().assign();

    Lookup::Handles::iterator it = std::find(lr->handles.begin(), lr->handles.end(), Lookup::Handle(filename));

    if(it != lr->handles.end()) {
      if(!it->lent)
        delete it->ptr;
      lr->handles.erase(it);
    }

    lr->handles.push_front(Lookup::Handle(type, filename, false, keep));

    m_database[lr->id] = type;

    return lr->id;
  }
  
  template <class TYPE>
  unsigned long Database<TYPE>::lend(const std::string &name, TYPE * const &type, const bool &keep) {
    if(!type)
      throw Null_Database_Entry_Set();

    Lookup *&lr = m_lookup[name];
    if(!lr)
      lr = new Lookup();

    if(!lr->id)
      lr->id = get_Resource().assign();

    Lookup::Handles::iterator it = std::find(lr->handles.begin(), lr->handles.end(), Lookup::Handle(filename));

    if(it != lr->handles.end()) {
      if(!it->lent)
        delete it->ptr;
      lr->handles.erase(it);
    }

    lr->handles.push_front(Lookup::Handle(type, filename, true, keep));

    m_database[lr->id] = type;

    return lr->id;
  }

  template <class TYPE>
  void Database<TYPE>::clear(const std::string &name, const std::string &filename) {
    Lookups::iterator it = m_lookup.find(name);

    if(it == m_lookup.end())
      throw Database_Entry_Not_Found("*::" + name);

    Lookup &lr = *it->second;
    Lookup::Handles::iterator jt = std::find(lr.handles.begin(), lr.handles.end(), Lookup::Handle(filename));

    if(jr == lr.handles.end())
      throw Database_Entry_Not_Found(filename + "::" + name);

    lr.handles.erase(jt);

    if(lr.handles.empty()) {
      m_database.erase(lr.id);
      m_lookup.erase(it);
    }
    else
      m_database[lr.id] = lr.handles.begin()->ptr;
  }

  template <class TYPE>
  unsigned long Database<TYPE>::get_id(const std::string &name) const {
    typename Lookups::const_iterator it = m_lookup.find(name);

    if(it == m_lookup.end() || !it->second->id)
      throw Database_Entry_Not_Found(name);

    return it->second->id;
  }

  template <class TYPE>
  TYPE & Database<TYPE>::operator[](const unsigned long &id) const {
    typename Entries::const_iterator it = m_database.find(id);

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
  void Database<TYPE>::clear() {
    uninit();
  }

  template <class TYPE>
  void Database<TYPE>::load(const std::string &filename) {
    XML_Document types_xml(filename.c_str());
    XML_Element_c types = types_xml[m_xml_identifier];
    std::string name;

    try {
      for(XML_Element_c it = types.first(); it.good(); it = it.next()) {
        const std::string name = it.value();

        TYPE * const type = load(it);
        if(!type)
          throw Database_Load_Entry_Failed(name);

        give(name, type, false, filename);
      }
    }
    catch(Database_Load_Entry_Failed &)
    {
      std::cerr << "Error loading '" << m_xml_identifier << "' entry '" << name << "'\n";
      throw;
    }

    on_load();
  }

  template <class TYPE>
  void Database<TYPE>::unload(const std::string &filename) {
    Filenames::iterator it = std::find(m_filenames.begin(), m_filenames.end(), filename);
    if(it == m_filenames.end())
      throw Database_File_Not_Loaded(filename);

    for(typename Lookups::iterator it = m_lookup.begin();
        it != m_lookup.end();)
    {
      for(typename Lookup::Handles::iterator jt = it->second->handles.begin();
          jt != it->second->handles.end();)
      {
        if(jt->filename != filename)
          ++jt;
        else {
          if(!jt->lent)
            delete jt->ptr;
          jt = it->second->handles.erase(jt);
        }
      }

      if(!it->second->handles.empty()) {
        m_database[it->second->id] = it->second->handles.begin()->ptr;
        ++it;
      }
      else {
        m_database.erase(it->second->id);
        it = m_lookup.erase(it);
      }
    }

    m_filenames.erase(filename);

    if(m_filenames.empty())
      on_clear();
  }

  template <class TYPE>
  void Database<TYPE>::reload() {
    lose_resources();
    init();
  }

  template <class TYPE>
  void Database<TYPE>::init() {
    for(Filenames::const_reverse_iterator it = m_filenames.rbegin();
       it != m_filenames.rend();
       ++it)
    {
      load(*it);
    }
  }

  template <class TYPE>
  void Database<TYPE>::uninit() {
    on_clear();

    m_database.clear();

    for(typename Lookups::iterator it = m_lookup.begin();
        it != m_lookup.end();
        ++it)
    {
      for(typename Lookup::Handles::iterator jt = it->second->handles.begin();
          jt != it->second->handles.end();
          ++jt)
      {
        if(!jt->lent)
          delete jt->ptr;
      }
    }

    m_lookup.clear();
  }

  template <class TYPE>
  void Database<TYPE>::lose_resources() {
    on_lose();

    m_database.clear();

    for(typename Lookups::iterator it = m_lookup.begin();
        it != m_lookup.end();)
    {
      for(typename Lookup::Handles::iterator jt = it->second->handles.begin();
          jt != it->second->handles.end();)
      {
        if(jt->keep)
          ++jt;
        else {
          if(!jt->lent)
            delete jt->ptr;
          jt = it->second->handles.erase(jt);
        }
      }

      if(!it->second->handles.empty()) {
        m_database[it->second->id] = it->second->handles.begin()->ptr;
        ++it;
      }
      else
        it = m_lookup.erase(it);
    }
  }

}

#include <Zeni/Resource.hxx>
#include <Zeni/XML.hxx>

#endif
