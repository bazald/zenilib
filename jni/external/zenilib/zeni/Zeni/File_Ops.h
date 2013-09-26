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

#ifndef ZENI_FILE_OPS_H
#define ZENI_FILE_OPS_H

#include <Zeni/Error.h>
#include <Zeni/Singleton.h>

#if defined(ANDROID)
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

namespace Zeni {

  class ZENI_DLL File_Ops;

#ifdef _WINDOWS
  ZENI_EXT template class ZENI_DLL Singleton<File_Ops>;
#endif

  class ZENI_DLL File_Ops : public Singleton<File_Ops> {
    friend class Singleton<File_Ops>;

    static File_Ops * create();

    File_Ops();
    ~File_Ops();

    // Undefined
    File_Ops(const File_Ops &);
    File_Ops & operator=(const File_Ops &);

  public:
#ifdef ANDROID
    static void init_AAssetManager(AAssetManager * const &asset_manager); ///< Initialize the AAssetManager
    static AAssetManager * const get_AAssetManager(); ///< Get the AAssetManager
#endif
    static FILE * get_asset_FILE(const String &filename, off_t * const &start = 0, off_t * const &length = 0); ///< Get a FILE * from an Asset
    static String & load_asset(String &memory, const String &filename); ///< Load a file into memory
    static const String & get_uniqname(); ///< Get the unique app identifier for the game, set in zenilib.xml
    const String & get_username(); ///< Get the logged-in user's username
    String get_appdata_path(); ///< Get the path that should be used for user-modifiable storage

    static bool create_directory(const String &directory_path); ///< Create a directory if it doesn't already exist; It is not considered an error if it already exists.
    static bool remove_directory(const String &directory_path); ///< Remove a directory if it exists; It is not considered an error if it does not exist.

    static bool file_exists(const String &file_path); ///< Test for the existence of a file
    static bool delete_file(const String &file_path); ///< Delete a file
    static bool copy_file(const String &from, const String &to); ///< Copy a file from one filepath to another

    // Can be called once only, and only before File_Ops is initialized; May throw File_Ops_Initialized
    static void preinit(const String &unique_app_identifier);

  private:
    static String & get_unique_app_identifier();

#ifdef ANDROID
    static AAssetManager * m_asset_manager;
#endif

    String m_username;
    String m_appdata_path;
  };

  ZENI_DLL File_Ops & get_File_Ops(); ///< Get access to the singleton.

  struct ZENI_DLL File_Ops_Init_Failure : public Error {
    File_Ops_Init_Failure() : Error("Zeni File_Ops Failed to Initialize Correctly") {}
  };

#ifdef ANDROID
  struct ZENI_DLL File_Ops_No_AAssetManager : public Error {
    File_Ops_No_AAssetManager() : Error("Zeni File_Ops No AAssetManager") {}
  };
#endif

  struct ZENI_DLL File_Ops_Asset_Load_Failure : public Error {
    File_Ops_Asset_Load_Failure() : Error("Zeni File_Ops Failed to Load Asset") {}
  };

  struct ZENI_DLL File_Ops_Initialized : public Error {
    File_Ops_Initialized() : Error("Zeni File_Ops Already Initialized") {}
  };

}

#endif
