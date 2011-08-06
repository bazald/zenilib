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

#include <Zeni/Error.h>
#include <Zeni/Hash_Map.h>
#include <Zeni/XML.h>
/* \cond */
#include <list>
#include <set>
/* \endcond */

namespace Zeni {

  template <class TYPE>
  class Database {
    struct Lookup {
      struct Handle {
        Handle();
        Handle(const String &filename_);
        Handle(TYPE * const &ptr_, const String &filename_, const bool &lent_, const bool &keep_);

        bool operator==(const Handle &rhs) const;

        TYPE * ptr;
        String filename;
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

    typedef std::list<String> Filenames;
    typedef Unordered_Map<String, Lookup *> Lookups; // (id, filename)
    typedef Unordered_Map<unsigned long, TYPE *> Entries; // (datum, lent)

    // Undefined
    Database(const Database &);
    Database & operator=(const Database &);

  public:
    Database(const String &filename, const String &xml_identifier);
    virtual ~Database();

    unsigned long get_id(const String &name) const; ///< Get an id by name, possibly throwing an Error
    unsigned long find(const String &name) const; ///< Get an id by name, without throwing an Error
    bool find(const unsigned long &id) const; ///< Check to see that an id is valid
    
    TYPE & operator[](const String &name) const; ///< Get a TYPE by name
    TYPE & operator[](const unsigned long &id) const; ///< Get a TYPE by id

    // Loaders
    unsigned long give(const String &name, TYPE * const &type, const bool &keep, const String &filename = ""); ///< Add an entry (which it will later delete)
    unsigned long lend(const String &name, TYPE * const &type, const bool &keep); ///< Add an entry (which it will NEVER delete)
    void clear(const String &name, const String &filename = ""); ///< Clear an entry

    // Initialization Functions
    void clear(); ///< Permanently clear all resources.
    void load_file(const String &filename); ///< Load all resources from a given file, giving them highest priority
    void unload_file(const String &filename); ///< Unload all resources from a given file, reloading lower priority resources
    void reload(); ///< lose_resources + init

    const bool & lost_resources(); ///< Check to see if resources have been lost
    void lose_resources(); ///< Wipe losable resources and prepare to reload them when they are next needed
    void unlose_resources(); ///< If resources have been lost, then reload them

  protected:
    void init();
    void uninit();
    bool give_priority(const String &name, const bool &lent, const bool &keep, const String &filename = ""); ///< If 'lent', 'keep', and 'filename' match, give priority over other 'name' entries

  private:
    virtual void on_load() {}
    virtual void on_clear() {}
    virtual void on_lose() {}

    virtual TYPE * load(XML_Element_c &xml_element, const String &name, const String &filename) = 0;

    String m_xml_identifier;

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    Filenames m_filenames;
    Lookups m_lookups;
    Entries m_entries;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

    bool m_lost;
  };

  struct ZENI_DLL Database_File_Not_Loaded : public Error {
    Database_File_Not_Loaded(const String &identifier) : Error("Zeni File '" + identifier + "' Not Loaded") {}
  };

  struct ZENI_DLL Database_Entry_Not_Found : public Error {
    Database_Entry_Not_Found(const String &identifier) : Error("Zeni Database Entry '" + identifier + "' Not Found") {}
  };

  struct ZENI_DLL Database_Load_Entry_Failed : public Error {
    Database_Load_Entry_Failed(const String &identifier) : Error("Zeni Database Entry '" + identifier + "' Could Not Be Loaded") {}
  };

  struct ZENI_DLL Null_Database_Entry_Set : public Error {
    Null_Database_Entry_Set() : Error("Null Entry Added to Zeni Database") {}
  };

}

#endif
