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

#include <Zeni/Colors.h>
#include <Zeni/Database.hxx>

#include <iomanip>
#include <iostream>
#include <fstream>

namespace Zeni {

  /// Convert hex string "0x0a23e3df" to uint32
  static inline Uint32 axToU32( const char* str )
  {
    Uint32 u = 0;
    if( str != 0 && str[0] == '0' && tolower( str[1] ) == 'x' )
    {
      const char* baseStr = &str[2];
      size_t len = strnlen( baseStr, 8 );
      for( size_t i = 0; i < len; i++ )
      {
        switch( tolower( baseStr[i] ) )
        {
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
          u = u << 4;
          u += ( tolower( baseStr[i] ) - 'a' + 10 );
          break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          u = u << 4;
          u += ( baseStr[i] - '0' );
          break;
        default:
          //M_ASSERT( false );
          //LOG_W( "Util", ( "Unidentified character found in hexstring: %s\n", str ) );
          return u;
        }
      }
    }
    else
    {
      //LOG_W( "Util", ( "NULL or badly formatted string passed to axToU32: %s\n", str ) );
    }
    return u;
  }

  Colors::Colors()
    : Database<Color>("config/colors.xml", "Colors")
  {
    init();
  }

  Colors::~Colors() {
    uninit();
  }

  Colors & get_Colors() {
    static Colors e_color;
    return e_color;
  }

  Color * Colors::load(XML_Element_c &xml_element, const std::string &/*name*/, const std::string &/*filename*/) {
    if( xml_element["argb"].good() )
    {
      return new Color( axToU32( xml_element["argb"].to_string().c_str() ) );
    }
    else
    {
      const float alpha = xml_element["alpha"].to_float();
      const float red = xml_element["red"].to_float();
      const float green = xml_element["green"].to_float();
      const float blue = xml_element["blue"].to_float();

      return new Color(alpha, red, green, blue);
	}
  }

}
