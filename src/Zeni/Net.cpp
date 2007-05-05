/* This file is part of the Zenipex Library.
* Copyleft (C) 2006 Mitchell Keith Bloch a.k.a. bazald
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

#include <Zeni/Net.hxx>

#include <SDL/SDL.h>

using std::string;

namespace Zeni {

  Net::Net() {
    // Ensure Core is initialized
    Core::get_reference();

    if(SDLNet_Init())
      throw Net_Init_Failure();
  }

  Net::~Net() {
    SDLNet_Quit();
  }

  Net & Net::get_reference() {
    static Net e_net;
    return e_net;
  }

  TCP_Socket::TCP_Socket(IPaddress ip)
    : sock(0),
      sockset(0)
  {
    Net::get_reference();

    if(!ip.host)
      throw TCP_Socket_Init_Failure();

    sock = SDLNet_TCP_Open(&ip);
    sockset = SDLNet_AllocSocketSet(1);
    
    if(!sock ||
       !sockset ||
       SDLNet_TCP_AddSocket(sockset, sock) == -1) {
      SDLNet_TCP_Close(sock);
      SDLNet_FreeSocketSet(sockset);
      throw TCP_Socket_Init_Failure();
    }
  }

  TCP_Socket::TCP_Socket(TCPsocket socket)
    : sock(socket),
      sockset(0)
  {
    if(!sock)
      throw TCP_Socket_Init_Failure();

    sockset = SDLNet_AllocSocketSet(1);

    if(!sockset ||
       SDLNet_TCP_AddSocket(sockset, sock) == -1) {
      SDLNet_TCP_Close(sock);
      throw TCP_Socket_Init_Failure();
    }
  }

  TCP_Socket::~TCP_Socket() {
    SDLNet_TCP_DelSocket(sockset, sock);
    SDLNet_FreeSocketSet(sockset);
    SDLNet_TCP_Close(sock);
  }

  IPaddress TCP_Socket::peer_address() const {
    return *SDLNet_TCP_GetPeerAddress(sock);
  }

  void TCP_Socket::send(const string &data) {
    int data_len = int(data.size());
    if(SDLNet_TCP_Send(sock, const_cast<char *>(data.c_str()), data_len) < data_len)
      throw Socket_Closed();
  }

  string TCP_Socket::receive(const int &num_bytes) {
    string data;
    data.resize(num_bytes);

    int retval = SDLNet_CheckSockets(sockset, 0);
    if(retval == -1)
      throw Socket_Closed();
    else if(retval) {
      retval = SDLNet_TCP_Recv(sock, const_cast<char *>(data.c_str()), num_bytes);
      if(retval <= 0)
        throw Socket_Closed();
    }

    return data.substr(0, retval);
  }

  TCP_Listener::TCP_Listener(const unsigned short &port)
    : sock(0)
  {
    Net::get_reference();

    IPaddress ip = {0, 0};
    SDLNet_Write16(port, &ip.port);

    sock = SDLNet_TCP_Open(&ip);
    if(!sock)
      throw TCP_Socket_Init_Failure();
  }

  TCP_Listener::~TCP_Listener() {
    SDLNet_TCP_Close(sock);
  }

  TCP_Socket TCP_Listener::accept() {
    return SDLNet_TCP_Accept(sock);
  }

  UDP_Socket::UDP_Socket(const unsigned short &port)
    : sock(0)
  {
    Net::get_reference();

    sock = SDLNet_UDP_Open(port);
    if(!sock)
      throw UDP_Socket_Init_Failure();
  }

  UDP_Socket::~UDP_Socket() {
    SDLNet_UDP_Close(sock);
  }

  void UDP_Socket::send(const std::string &data, IPaddress ip) {
    int data_len = int(data.size());

    UDPpacket packet =
    {
      -1,
      reinterpret_cast<Uint8 *>(const_cast<char *>(data.c_str())),
      data_len,
      data_len,
      0, // Will == -1 on error after UDP_Send, otherwise == # of bytes sent
      ip
    };

    if(!SDLNet_UDP_Send(sock, -1, &packet))
      throw Socket_Closed();
  }

  IPaddress UDP_Socket::receive(std::string &data) {
    IPaddress ipaddress = {0, 0};

    UDPpacket packet =
    {
      -1,
      reinterpret_cast<Uint8 *>(const_cast<char *>(data.c_str())),
      0,
      int(data.length()),
      0,
      ipaddress
    };

    int retval = SDLNet_UDP_Recv(sock, &packet);
    if(retval == -1)
      throw Socket_Closed();
    else if(!retval) {
      data.clear();
      packet.address.host = 0;
      packet.address.port = 0;
    }
    else
      data.resize(packet.len);

    return packet.address;
  }

}
