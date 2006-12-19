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

/**
 * \class Zeni::Net
 *
 * \ingroup Zenilib
 *
 * \brief The Net Singleton
 *
 * The Net Singleton is responsible for setting up IP sockets.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::TCP_Socket
 *
 * \ingroup Zenilib
 *
 * \brief A TCP Socket for sending and receiving data
 *
 * This class not only provides a TCP socket but prevents receive calls from blocking as well.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::TCP_Listener
 *
 * \ingroup Zenilib
 *
 * \brief A Socket for accepting new TCP connections (in the form of TCP_Sockets)
 *
 * Not much more to it.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::UDP_Socket
 *
 * \ingroup Zenilib
 *
 * \brief A UDP Socket for sending and receiving data
 *
 * UDP Sockets are little more than a lock on a given port on a computer.  A program
 * that might need a dozen TCP Sockets can get by with one UDP_Socket.  Note that
 * if you opt to use UDP_Sockets, you'll get faster data transmission at the expense 
 * of reliability.  If all the data being transmitted has to get there, you're better
 * off using TCP_Sockets, even if the order of arrival is unimportant.  Code carefully.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_NET_H
#define ZENI_NET_H

#include "Core.h"

#include <SDL/SDL_net.h>
#include <string>

struct SDL_Thread;

namespace Zeni {

  class Net {
    Net();
    ~Net();

    // Undefined
    Net(const Net &);
    Net & operator=(const Net &);

  public:
    // Get reference to only instance; Might throw Sound_Init_Failure
    static Net & get_reference();

    /// Default port 0 indicates a pure lookup with no intention of connecting.
    inline IPaddress resolve_host(const std::string &host, const unsigned short &port = 0);
    /// If you want to find a URL associated with an IP address
    inline std::string reverse_lookup(IPaddress ip);
  };

  class TCP_Socket {
  public:
    TCP_Socket(IPaddress ip); ///< For outgoing connections
    TCP_Socket(TCPsocket sock); ///< For incoming connections
    ~TCP_Socket();

    IPaddress peer_address() const;

    void send(const std::string &data); ///< Send data
    std::string receive(const int &num_bytes); ///< Receive up to num_bytes

  private:
    TCPsocket sock;
    SDLNet_SocketSet sockset;
  };

  class TCP_Listener {
  public:
    TCP_Listener(const unsigned short &port);
    ~TCP_Listener();

    TCP_Socket accept(); ///< Will cause an error to be thrown if there exists nothing to listen to; Expect to catch it

  private:
    TCPsocket sock;
  };

  class UDP_Socket {
  public:
    UDP_Socket(const unsigned short &port);
    ~UDP_Socket();

    void send(const std::string &data, IPaddress ip); ///< Send data to an IPaddress
    IPaddress receive(std::string &data); ///< Receive data of up to data.size() from the returned IPaddress; Will error if data.size() is too low
    
  private:
    UDPsocket sock;
  };

  struct Net_Init_Failure : public Error {
    Net_Init_Failure() : Error("Zeni Net Failed to Initialize Correctly") {}
  };

  struct TCP_Socket_Init_Failure : public Error {
    TCP_Socket_Init_Failure() : Error("Zeni TCP Socket Failed to Initialize Correctly") {}
  };

  struct UDP_Socket_Init_Failure : public Error {
    UDP_Socket_Init_Failure() : Error("Zeni UDP Socket Failed to Initialize Correctly") {}
  };

  struct Socket_Closed : public Error {
    Socket_Closed() : Error("Zeni Socket Unexpectedly Closed") {}
  };

}

#ifdef ZENI_INLINES
#include "Net.hxx"
#endif

#endif
