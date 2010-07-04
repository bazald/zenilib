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
 * \class Zeni::Database
 *
 * \ingroup zenilib
 *
 * \brief A Texture Database Singleton
 *
 * \note Database will be reloaded automatically if settings are changed with a call to set_texturing_mode.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_DATABASE_H
#define ZENI_DATABASE_H

#include <Zeni/Core.h>
#include <Zeni/Hash_Map.h>
#include <Zeni/XML.h>
#include <list>
#include <set>

namespace Zeni {

  template <class TYPE>
  class Database {
    struct Lookup {
      struct Handle {
        Handle();
        Handle(const std::string &filename_);
        Handle(TYPE * const &ptr_, const std::string &filename_, const bool &lent_, const bool &keep_);

        bool operator==(const Handle &rhs) const;

        TYPE * ptr;
        std::string filename;
        bool lent;
        bool keep;
      };

      typedef std::list<Handle> Handles;

    private:
      // Undefined
      Lookup(const Lookup &);
      Lookup & operator=(const Lookup &);

    public:
      Lookup();
      Lookup(const unsigned long &id_, const Handle &handle_);

      unsigned long id;
      Handles handles;
    };

    typedef std::list<std::string> Filenames;
    typedef Unordered_Map<std::string, Lookup *> Lookups; // (id, filename)
    typedef Unordered_Map<unsigned long, TYPE *> Entries; // (datum, lent)

    // Undefined
    Database(const Database &);
    Database & operator=(const Database &);

  public:
    Database(const std::string &filename, const std::string &xml_identifier);
    virtual ~Database();

    unsigned long get_id(const std::string &name) const; ///< Get an id by name, possibly throwing an Error
    unsigned long find(const std::string &name) const; ///< Get an id by name, without throwing an Error
    bool find(const unsigned long &id) const; ///< Check to see that an id is valid
    
    TYPE & operator[](const std::string &name) const; ///< Get a TYPE by name
    TYPE & operator[](const unsigned long &id) const; ///< Get a TYPE by id

    // Loaders
    unsigned long give(const std::string &name, TYPE * const &type, const bool &keep, const std::string &filename = ""); ///< Add an entry (which it will later delete)
    unsigned long lend(const std::string &name, TYPE * const &type, const bool &keep); ///< Add an entry (which it will NEVER delete)
    void clear(const std::string &name, const std::string &filename = ""); ///< Clear an entry

    // Initialization Functions
    void clear(); ///< Permanently clear all resources.
    void load_file(const std::string &filename); ///< Load all resources from a given file, giving them highest priority
    void unload_file(const std::string &filename); ///< Unload all resources from a given file, reloading lower priority resources
    void reload(); ///< lose_resources + init

    const bool & lost_resources(); ///< Check to see if resources have been lost
    void lose_resources(); ///< Wipe losable resources and prepare to reload them when they are next needed
    void unlose_resources(); ///< If resources have been lost, then reload them

  protected:
    void init();
    void uninit();
    bool give_priority(const std::string &name, const bool &lent, const bool &keep, const std::string &filename = ""); ///< If 'lent', 'keep', and 'filename' match, give priority over other 'name' entries

  private:
    virtual void on_load() {}
    virtual void on_clear() {}
    virtual void on_lose() {}

    virtual TYPE * load(XML_Element_c &xml_element, const std::string &name, const std::string &filename) = 0;

    std::string m_xml_identifier;

    Filenames m_filenames;
    Lookups m_lookups;
    Entries m_entries;
    bool m_lost;
  };

  struct Database_File_Not_Loaded : public Error {
    Database_File_Not_Loaded(const std::string &identifier) : Error("Zeni File '" + identifier + "' Not Loaded") {}
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
