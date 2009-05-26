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

#ifndef ZENI_XML_HXX
#define ZENI_XML_HXX

#include <Zeni/XML.h>

#include <Zeni/Net_Primitives.h>
#include <iostream>

namespace Zeni {

  XML_Element::XML_Element(const TiXmlHandle &handle_)
    : m_handle(handle_)
  {
  }

  XML_Element XML_Element::operator[](const std::string &field) const {
    if(!m_handle.ToNode()) {
      std::cerr << "Bad XML_Element attempted to access field '" << field << "'\n";
      throw XML_Element_Ungood();
    }

    TiXmlNode * node = child(field);
    return XML_Element(node ? node->ToElement() : 0);
  }

  XML_Element XML_Element::first() const {
    if(!m_handle.ToNode()) {
      std::cerr << "Bad XML_Element attempted to access first field\n";
      throw XML_Element_Ungood();
    }

    TiXmlNode * node = first_child();
    return XML_Element(node ? node->ToElement() : 0);
  }

  XML_Element XML_Element::next() const {
    if(!m_handle.ToNode()) {
      std::cerr << "Bad XML_Element attempted to access next field\n";
      throw XML_Element_Ungood();
    }

    TiXmlNode * node = m_handle.ToNode();
    return XML_Element(node ? node->NextSiblingElement() : 0);
  }

  bool XML_Element::good() const {
    return m_handle.ToElement() != 0;
  }

  std::string XML_Element::value() const {
    if(!good()) {
      std::cerr << "Bad XML_Element attempted to access value\n";
      throw XML_Element_Ungood();
    }

    TiXmlElement * element = m_handle.ToElement();
    if(!element) {
      std::cerr << "XML_Element invalid\n";
      throw XML_Element_Ungood();
    }

    return element->Value();
  }

  bool XML_Element::to_bool() const {
    return to_int() != 0;
  }

  int XML_Element::to_int() const {
    return atoi(to_string().c_str());
  }

  float XML_Element::to_float() const {
    return float(atof(to_string().c_str()));
  }

  double XML_Element::to_double() const {
    return atof(to_string().c_str());
  }

  std::string XML_Element::to_string() const {
    if(!good()) {
      std::cerr << "Bad XML_Element attempted to_string()\n";
      throw XML_Element_Ungood();
    }

    TiXmlElement *element = m_handle.ToElement();
    if(!element) {
      std::cerr << "XML_Element invalid\n";
      throw XML_Element_Ungood();
    }

    const char * text = element->GetText();
    if(!text) {
      std::cerr << "XML_Element attempted to_string() on non-leaf node\n";
      throw XML_Element_Nonleaf();
    }

    return text;
  }

  void XML_Element::create_child(const std::string &field) {
    if(!m_handle.ToNode()) {
      std::cerr << "Bad XML_Element attempted to create_child(...)\n";
      throw XML_Element_Ungood();
    }

    TiXmlElement * element = new TiXmlElement(field.c_str());
    if(!m_handle.ToNode()->LinkEndChild(element)) {
      std::cerr << "XML_Element failed to create a child\n";
      throw XML_Create_Child_Failure();
    }
  }

  void XML_Element::remove_child(const XML_Element &child) {
    if(!good() || !child.good()) {
      std::cerr << "Bad XML_Element attempted to remove_child(...)\n";
      throw XML_Element_Ungood();
    }

    if(!m_handle.ToNode()->RemoveChild(child.m_handle.ToNode())) {
      std::cerr << "XML_Element failed to remove a child\n";
      throw XML_Remove_Child_Failure();
    }
  }

  void XML_Element::set_bool(const bool &b) {
    set_string(itoa(b));
  }

  void XML_Element::set_int(const int &i) {
    set_string(itoa(i));
  }

  void XML_Element::set_float(const float &f) {
    set_string(ftoa(f));
  }

  void XML_Element::set_double(const double &d) {
    set_string(dtoa(d));
  }

  void XML_Element::set_string(const std::string &s) {
    if(!good()) {
      std::cerr << "Bad XML_Element attempted set_string(...)\n";
      throw XML_Element_Ungood();
    }

    TiXmlNode * const node = first_child();

    if(!node)
      m_handle.ToNode()->LinkEndChild(new TiXmlText(s.c_str()));
    else if(node->ToText()) {
      TiXmlText replacement(s.c_str());
      m_handle.ToNode()->ReplaceChild(node, replacement);
    }
    else {
      std::cerr << "XML_Element attempted set_string(...) on non-leaf node\n";
      throw XML_Element_Nonleaf();
    }
  }

  TiXmlNode * XML_Element::child(const std::string &field) const {
    TiXmlNode * node = m_handle.ToNode();
    return node ? node->FirstChild(field.c_str()) : 0;
  }

  TiXmlNode * XML_Element::first_child() const {
    TiXmlNode * node = m_handle.ToNode();
    return node ? node->FirstChild() : 0;
  }

  XML_Document::XML_Document()
  : m_root(0)
  {
  }

  XML_Document::XML_Document(const std::string &filename)
  : m_root(0)
  {
    load(filename);
  }

  XML_Document::~XML_Document() {
    delete m_root;
  }

  bool XML_Document::good() const {
    return m_root != 0;
  }

  void XML_Document::load(const std::string &filename) {
    delete m_root;
    m_root = 0;

    if(!m_xml_file.LoadFile(filename.c_str())) {
      std::cerr << "Failed to load XML_Document '" << filename << "'\n"
                << m_xml_file.ErrorDesc() << std::endl;
      throw XML_Load_Failure();
    }

    TiXmlHandle root = &m_xml_file;
    m_root = new XML_Element(root);
  }

  void XML_Document::save() {
    if(!good()) {
      std::cerr << "Bad XML_Document attempted to save\n";
      throw XML_Document_Ungood();
    }

    if(!m_xml_file.SaveFile()) {
      std::cerr << "Failed to save XML_Document" << std::endl
                << m_xml_file.ErrorDesc() << std::endl;
      throw XML_Save_Failure();
    }
  }

  void XML_Document::save(const std::string &filename) {
    if(!good()) {
      std::cerr << "Bad XML_Document attempted to save to '" << filename << "'\n";
      throw XML_Document_Ungood();
    }

    if(!m_xml_file.SaveFile(filename.c_str())) {
      std::cerr << "Failed to save XML_Document '" << filename << "'\n"
                << m_xml_file.ErrorDesc() << std::endl;
      throw XML_Save_Failure();
    }
  }

  XML_Element XML_Document::operator[](const std::string &field) const {
    if(!good()) {
      std::cerr << "Bad XML_Document attempted to access field '" << field << "'\n";
      throw XML_Document_Ungood();
    }

    return (*m_root)[field];
  }

  XML_Element XML_Document::first() const {
    if(!good()) {
      std::cerr << "Bad XML_Document attempted to access first field\n";
      throw XML_Document_Ungood();
    }

    return m_root->first();
  }

  void XML_Document::create_root(const std::string &field) {
    delete m_root;
    m_root = 0;

    m_xml_file = TiXmlDocument();

    m_xml_file.LinkEndChild(new TiXmlDeclaration("1.0", "", ""));
    m_xml_file.LinkEndChild(new TiXmlElement(field.c_str()));

    TiXmlHandle root = &m_xml_file;
    m_root = new XML_Element(root);
  }

}

#endif
