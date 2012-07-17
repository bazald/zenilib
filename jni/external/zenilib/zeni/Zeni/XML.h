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
 * \class Zeni::XML_Element
 *
 * \ingroup zenilib
 *
 * \brief A node in an XML tree, possibly containing useful data.
 *
 * \note There is no functionality (currently) to cleanly exit an iteration through XML_Elements.  The only way out is to catch the Bad_XML_Access when thrown.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::XML_Document
 *
 * \ingroup zenilib
 *
 * \brief A class to open an XML file and manage the root node.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_XML_H
#define ZENI_XML_H

#include <Zeni/Error.h>

#include <tinyxml.h>

namespace Zeni {

  class ZENI_DLL XML_Element_c {
  public:
    XML_Element_c(const TiXmlHandle &handle);

    /** Tree Traversal **/

    XML_Element_c operator[](const String &field) const; ///< Get a named subelement

    XML_Element_c first() const; ///< Get the first subelement
    XML_Element_c next() const; ///< Get the next subelement

    /** Basic Element Functionality **/

    bool good() const; ///< Test to see if the Element is valid
    String value() const; ///< Get the value of the element (For nodes, this is the name)

    /** Text Accessors **/

    bool to_bool() const; ///< Get the contained string as a boolean
    int to_int() const; ///< Get the contained string as an integer
    float to_float() const; ///< Get the contained string as a floating point number
    double to_double() const; ///< Get the contained string as a double precision floating point number
    String to_string() const; ///< Get the contained string

  protected:
    TiXmlNode * child(const String &field) const;
    TiXmlNode * first_child() const;
    TiXmlNode * child_element(const String &field) const;
    TiXmlNode * first_child_element() const;

    TiXmlHandle m_handle;
  };

  class ZENI_DLL XML_Element : public XML_Element_c {
  public:
    XML_Element(const TiXmlHandle &handle);

    /** Tree Traversal **/

    XML_Element operator[](const String &field); ///< Get a named subelement

    XML_Element first(); ///< Get the first subelement
    XML_Element next(); ///< Get the next subelement

    /** Tree Modifiers **/

    void create_child(const String &field); ///< Create a child node
    void remove_child(const XML_Element &child); ///< Remove a child node, which will be rendered invalid

    /** Text Modifiers **/

    void set_bool(const bool &b); ///< Set the contained string as a boolean
    void set_int(const int &i); ///< Set the contained string as an integer
    void set_float(const float &f); ///< Set the contained string as a floating point number
    void set_double(const double &d); ///< Set the contained string as a double precision floating point number
    void set_string(const String &s); ///< Set the contained string
  };

  class ZENI_DLL XML_Document {
    XML_Document(const XML_Document &);
    XML_Document & operator=(const XML_Document &);

  public:
    XML_Document();
    XML_Document(const String &filename);
    XML_Document(const String &filename, const String &backup);
    ~XML_Document();

    /** Load and Save **/

    bool good() const;
    void load_mem(const String &data);
    void load(const String &filename);
    void save();
    void save(const String &filename);

    bool try_load_mem(const String &data);
    bool try_load(const String &filename);
    bool try_save();
    bool try_save(const String &filename);

    /** Tree Traversal **/

    XML_Element_c operator[](const String &field) const; ///< Get a named subelement
    XML_Element operator[](const String &field); ///< Get a named subelement

    XML_Element_c first() const; ///< Get the first subelement
    XML_Element first(); ///< Get the first subelement

    /** Manipulation Functions **/

    void create_root(const String &field); ///< (Re)Initializes the document with a given root field

  private:
    TiXmlDocument m_xml_file;
    XML_Element * m_root;
  };

  struct ZENI_DLL XML_Load_Failure : public Error {
    XML_Load_Failure() : Error("XML_Document could not be loaded.") {}
  };

  struct ZENI_DLL XML_Save_Failure : public Error {
    XML_Save_Failure() : Error("XML_Document could not be saved.") {}
  };

  struct ZENI_DLL XML_Document_Ungood : public Error {
    XML_Document_Ungood() : Error("XML_Document not good!") {}
  };

  struct ZENI_DLL XML_Element_Ungood : public Error {
    XML_Element_Ungood() : Error("XML_Element not good!") {}
  };

  struct ZENI_DLL XML_Element_Nonleaf : public Error {
    XML_Element_Nonleaf() : Error("XML_Element attempted leaf-node operation on non-leaf node!") {}
  };

  struct ZENI_DLL XML_Create_Child_Failure : public Error {
    XML_Create_Child_Failure() : Error("XML_Element failed to create a child node!") {}
  };

  struct ZENI_DLL XML_Remove_Child_Failure : public Error {
    XML_Remove_Child_Failure() : Error("XML_Element failed to remove a child node!") {}
  };

}

#endif
