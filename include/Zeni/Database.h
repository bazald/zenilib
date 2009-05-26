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

/**
 * \class Zeni::Database
 *
 * \ingroup Zenilib
 *
 * \brief A Texture Database Singleton
 *
 * \note Database will be reloaded automatically if settings are changed with a call to set_texturing_mode.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifdef ZENI_INLINES
#include <Zeni/Database.hxx>
#endif

#ifndef ZENI_DATABASE_H
#define ZENI_DATABASE_H

#include <Zeni/Core.h>
#include <Zeni/Hash_Map.h>
#include <Zeni/XML.h>
#include <set>

namespace Zeni {

  template <class TYPE>
  class Database {
    // Undefined
    Database(const Database &);
    Database & operator=(const Database &);

  public:
    Database(const std::string &filename, const std::string &xml_identifier);
    ~Database();

    unsigned long get_id(const std::string &name) const; ///< Get an id by name.
    
    TYPE & operator[](const std::string &name) const; ///< Get a TYPE by name
    TYPE & operator[](const unsigned long &id) const; ///< Get a TYPE by id

    // Loaders
    unsigned long give(const std::string &name, TYPE * const &type); ///< Add an entry (which it will later delete)
    unsigned long lend(const std::string &name, TYPE * const &type); ///< Add an entry (which it will NEVER delete)
    void clear(const std::string &name); ///< Clear an entry

    // Initialization Functions
    void reload(const std::string &filename); ///< lose_resources + init
    void reload(); ///< lose_resources + init

    void lose_resources(); ///< Wipe losable resources and prepare to reload them when they are next needed

  protected:
    void init();
    void uninit();

  private:
    virtual void pre_init() {}
    virtual void post_init() {}
    virtual void pre_uninit() {}
    virtual void post_uninit() {}
    virtual void pre_lose() {}
    virtual void post_lose() {}

    virtual TYPE * load(XML_Element &xml_element) = 0;
    virtual bool keep(const TYPE &type); ///< Default behavior is to keep none. lose_resources then behaves identically to uninit.

    std::string m_filename;
    std::string m_xml_identifier;

    stdext::hash_map<std::string, unsigned long> m_lookup;
    stdext::hash_map<unsigned long, TYPE *> m_database;
    std::set<TYPE *> m_lent;
  };

  struct Database_Entry_Not_Found : public Error {
    Database_Entry_Not_Found(const std::string &identifier) : Error("Zeni Database Entry '" + identifier + "' Not Found") {}
  };

  struct Database_Load_Entry_Failed : public Error {
    Database_Load_Entry_Failed(const std::string &identifier) : Error("Zeni Database Entry '" + identifier + "' Could Not Be Loaded") {}
  };

  struct Null_Database_Entry_Set : public Error {
    Null_Database_Entry_Set() : Error("Null Entry Added to Zeni Database") {}
  };

}

#endif
