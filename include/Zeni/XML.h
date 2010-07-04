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

#include <tinyxml.h>
#include <string>

namespace Zeni {

  class XML_Element_c {
  public:
    inline XML_Element_c(const TiXmlHandle &handle);

    /** Tree Traversal **/

    inline XML_Element_c operator[](const std::string &field) const; ///< Get a named subelement

    inline XML_Element_c first() const; ///< Get the first subelement
    inline XML_Element_c next() const; ///< Get the next subelement

    /** Basic Element Functionality **/

    inline bool good() const; ///< Test to see if the Element is valid
    inline std::string value() const; ///< Get the value of the element (For nodes, this is the name)

    /** Text Accessors **/

    inline bool to_bool() const; ///< Get the contained string as a boolean
    inline int to_int() const; ///< Get the contained string as an integer
    inline float to_float() const; ///< Get the contained string as a floating point number
    inline double to_double() const; ///< Get the contained string as a double precision floating point number
    inline std::string to_string() const; ///< Get the contained string

  protected:
    inline TiXmlNode * child(const std::string &field) const;
    inline TiXmlNode * first_child() const;
    inline TiXmlNode * child_element(const std::string &field) const;
    inline TiXmlNode * first_child_element() const;

    TiXmlHandle m_handle;
  };

  class XML_Element : public XML_Element_c {
  public:
    inline XML_Element(const TiXmlHandle &handle);

    /** Tree Traversal **/

    inline XML_Element operator[](const std::string &field); ///< Get a named subelement

    inline XML_Element first(); ///< Get the first subelement
    inline XML_Element next(); ///< Get the next subelement

    /** Tree Modifiers **/

    inline void create_child(const std::string &field); ///< Create a child node
    inline void remove_child(const XML_Element &child); ///< Remove a child node, which will be rendered invalid

    /** Text Modifiers **/

    inline void set_bool(const bool &b); ///< Set the contained string as a boolean
    inline void set_int(const int &i); ///< Set the contained string as an integer
    inline void set_float(const float &f); ///< Set the contained string as a floating point number
    inline void set_double(const double &d); ///< Set the contained string as a double precision floating point number
    inline void set_string(const std::string &s); ///< Set the contained string
  };

  class XML_Document {
    XML_Document(const XML_Document &);
    XML_Document & operator=(const XML_Document &);

  public:
    inline XML_Document();
    inline XML_Document(const std::string &filename);
    inline XML_Document(const std::string &filename, const std::string &backup);
    inline ~XML_Document();

    /** Load and Save **/

    inline bool good() const;
    inline void load(const std::string &filename);
    inline void save();
    inline void save(const std::string &filename);

    inline bool try_load(const std::string &filename);
    inline bool try_save();
    inline bool try_save(const std::string &filename);

    /** Tree Traversal **/

    inline XML_Element_c operator[](const std::string &field) const; ///< Get a named subelement
    inline XML_Element operator[](const std::string &field); ///< Get a named subelement

    inline XML_Element_c first() const; ///< Get the first subelement
    inline XML_Element first(); ///< Get the first subelement

    /** Manipulation Functions **/

    inline void create_root(const std::string &field); ///< (Re)Initializes the document with a given root field

  private:
    TiXmlDocument m_xml_file;
    XML_Element * m_root;
  };

  struct XML_Load_Failure : public Error {
    XML_Load_Failure() : Error("XML_Document could not be loaded.") {}
  };

  struct XML_Save_Failure : public Error {
    XML_Save_Failure() : Error("XML_Document could not be saved.") {}
  };

  struct XML_Document_Ungood : public Error {
    XML_Document_Ungood() : Error("XML_Document not good!") {}
  };

  struct XML_Element_Ungood : public Error {
    XML_Element_Ungood() : Error("XML_Element not good!") {}
  };

  struct XML_Element_Nonleaf : public Error {
    XML_Element_Nonleaf() : Error("XML_Element attempted leaf-node operation on non-leaf node!") {}
  };

  struct XML_Create_Child_Failure : public Error {
    XML_Create_Child_Failure() : Error("XML_Element failed to create a child node!") {}
  };

  struct XML_Remove_Child_Failure : public Error {
    XML_Remove_Child_Failure() : Error("XML_Element failed to remove a child node!") {}
  };

}

#endif
