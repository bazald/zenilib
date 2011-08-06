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

#ifndef ZENI_NET_HXX
#define ZENI_NET_HXX

#include <Zeni/Net.h>

namespace Zeni {

  IPaddress Net::resolve_host(const String &host, const Uint16 &port) {
    IPaddress ip;
    SDLNet_ResolveHost(&ip, host.c_str(), port);
    return ip;
  }

  String Net::reverse_lookup(IPaddress ip) {
    return SDLNet_ResolveIP(&ip);
  }

}

#endif
