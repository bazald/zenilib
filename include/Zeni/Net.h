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

/**
 * \class Zeni::Split_UDP_Socket
 *
 * \ingroup Zenilib
 *
 * \brief A higher level UDP_Socket
 *
 * It works almost identically to the UDP_Socket as far as users should be concerned.
 * On the other hand, it works around UDP packet size limitations very well (and adds 
 * some overhead in the process).  If you need to use this, your design is probably 
 * flawed, but it does its job as needed.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_NET_H
#define ZENI_NET_H

#include "Net_Primitives.h"

#include <Zeni/Core.h>

#include <SDL/SDL_net.h>
#include <cassert>
#include <string>
#include <list>

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
    TCP_Socket(const TCP_Socket &);
    TCP_Socket & operator=(const TCP_Socket &);
    
  public:
    TCP_Socket(IPaddress ip); ///< For outgoing connections
    TCP_Socket(TCPsocket sock); ///< For incoming connections
    ~TCP_Socket();

    IPaddress peer_address() const;
    int check_socket();
    
    /// Send data
    void send(const void * const &data, const int &num_bytes); 
    void send(const std::string &data);

    /// Receive up to num_bytes
    int receive(void * const &data, const int &num_bytes);
    int receive(std::string &data, const int &num_bytes);

  private:
    TCPsocket sock;
    SDLNet_SocketSet sockset;
  };

  class TCP_Listener {
    TCP_Listener(const TCP_Listener &);
    TCP_Listener & operator=(const TCP_Listener &);
    
  public:
    TCP_Listener(const unsigned short &port);
    ~TCP_Listener();

    TCPsocket accept(); ///< Will cause an error to be thrown if there exists nothing to listen to; Expect to catch it

  private:
    TCPsocket sock;
  };

  class UDP_Socket {
    UDP_Socket(const UDP_Socket &);
    UDP_Socket & operator=(const UDP_Socket &);
    
  public:
    UDP_Socket(const unsigned short &port);
    virtual ~UDP_Socket();
    
    IPaddress peer_address() const; ///< Apparently only works if the port was explicitly specified

    /// Send data to an IPaddress
    virtual void send(const IPaddress &ip, const void * const &data, const int &num_bytes);
    virtual void send(const IPaddress &ip, const std::string &data);
    
    /// Receive data of up to data.size() from the returned IPaddress; Will error if num_bytes/data.size() is too low
    virtual int receive(IPaddress &ip, const void * const &data, const int &num_bytes);
    virtual int receive(IPaddress &ip, std::string &data);
    
  private:
    UDPsocket sock;
  };
  
  class Split_UDP_Socket : public UDP_Socket {
    Split_UDP_Socket(const Split_UDP_Socket &);
    Split_UDP_Socket & operator=(const Split_UDP_Socket &);
    
    struct Chunk {
      Chunk() : size(0), data(0) {}
      ~Chunk() {delete [] data;}
      
      Chunk(const Chunk &rhs)
        : size(rhs.size),
          data(rhs.data)
      {
        rhs.size = 0;
        rhs.data = 0;
      }
      
      Chunk & operator=(Chunk &rhs) {
        size = rhs.size;
        data = rhs.data;
        
        rhs.size = 0;
        rhs.data = 0;
        
        return *this;
      }
      
      mutable size_t size;
      mutable char * data;
    };
    
    class Chunk_Set {
      Chunk_Set(const Chunk_Set &);
      Chunk_Set operator=(const Chunk_Set &);
    
    public:
      IPaddress ip;
      Nonce nonce;
      std::vector<Chunk> chunks;
      
      Chunk_Set() {}
      
      Chunk_Set(const IPaddress &sender, const Nonce &incoming, const Uint16 &num_chunks, const Uint16 &which, Chunk &chunk);
      
      bool add_chunk(const IPaddress &sender, const Nonce &incoming, const Uint16 &num_chunks, const Uint16 &which, Chunk &chunk);
      bool complete() const;
      
      Chunk receive() const;
    };
    
    class Chunk_Collector {
      Chunk_Collector(const Chunk_Collector &);
      Chunk_Collector operator=(const Chunk_Collector &);
      
    public:
      Chunk_Collector(const Uint16 &size = 64)
        : m_size(size)
      {
        assert(m_size);
      }
      
      ~Chunk_Collector() {
        for(std::list<Chunk_Set *>::iterator it = chunk_sets.begin(); it != chunk_sets.end(); ++it)
          delete *it;
      }
      
      const Chunk_Set * add_chunk(const IPaddress &sender, const Nonce &incoming, const Uint16 &num_chunks, const Uint16 &which, Chunk &chunk);
      
    private:
      std::list<Chunk_Set *> chunk_sets;
      Uint16 m_size;
    };
    
  public:
    Split_UDP_Socket(const unsigned short &port, const Uint16 &chunk_sets = 64, const Uint16 &chunk_size = 960);

    /// Send data to an IPaddress
    virtual void send(const IPaddress &ip, const void * const &data, const Uint16 &num_bytes);
    virtual void send(const IPaddress &ip, const std::string &data);
    
    /// Receive data of up to data.size() from the returned IPaddress; Will error if num_bytes/data.size() is too low
    virtual int receive(IPaddress &ip, const void * const &data, const Uint16 &num_bytes);
    virtual int receive(IPaddress &ip, std::string &data);
    
  private:
    Uint16 m_chunk_size;
    
    Chunk_Collector m_chunk_collector;
    
    Nonce m_nonce_send;
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
#include <Zeni/Net.hxx>
#endif

#endif
