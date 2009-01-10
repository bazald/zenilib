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

namespace Zeni {

  //child = parent->FirstChild(); child; child = child->NextSibling()

  XML_Element::XML_Element(const TiXmlHandle &handle_)
    : m_handle(handle_)
  {
  }

  XML_Element XML_Element::operator[](const std::string &field) const {
    return XML_Element(m_handle.FirstChildElement(field.c_str()));
  }

  XML_Element XML_Element::first() const {
    return XML_Element(m_handle.FirstChildElement());
  }

  XML_Element XML_Element::next() const {
    TiXmlNode * node = m_handle.Node();
    if(!node)
      throw Bad_XML_Access();

    return XML_Element(node->NextSiblingElement());
  }

  std::string XML_Element::value() const {
    TiXmlElement * element = m_handle.Element();
    if(!element)
      throw Bad_XML_Access();

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
    TiXmlElement *element = m_handle.Element();
    if(!element)
      throw Bad_XML_Access();

    const char * text = element->GetText();
    if(!text)
      throw Bad_XML_Access();

    return text;
  }

  XML_Reader::XML_Reader(const std::string &filename)
  : m_xml_file(filename.c_str()),
  m_root(0)
  {
    if(!m_xml_file.LoadFile())
      throw Bad_XML();

    TiXmlHandle root = &m_xml_file;
    m_root = new XML_Element(root);
  }

  XML_Reader::~XML_Reader() {
    delete m_root;
  }

  XML_Element XML_Reader::operator[](const std::string &field) const {
    return (*m_root)[field];
  }

  XML_Element XML_Reader::first() const {
    return m_root->first();
  }

}

#endif
