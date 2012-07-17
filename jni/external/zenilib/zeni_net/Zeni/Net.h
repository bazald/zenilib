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
 * \class Zeni::Net
 *
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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
 * \ingroup zenilib
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

#include <Zeni/Singleton.h>
#include <Zeni/VLUID.h>

#include <SDL/SDL_net.h>

#include <cassert>
#include <Zeni/String.h>
#include <list>
#include <vector>

#include <Zeni/Define.h>

struct SDL_Thread;

namespace Zeni {

  class ZENI_NET_DLL Net;

#ifdef _WINDOWS
  ZENI_NET_EXT template class ZENI_NET_DLL Singleton<Net>;
#endif

  class ZENI_NET_DLL Net : public Singleton<Net> {
    friend class Singleton<Net>;

    static Net * create();

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    static Uninit g_uninit;
    static Reinit g_reinit;
#ifdef _WINDOWS
#pragma warning( pop )
#endif

    Net();
    ~Net();

    // Undefined
    Net(const Net &);
    Net & operator=(const Net &);

  public:
    /// Default port 0 indicates a pure lookup with no intention of connecting.
    IPaddress resolve_host(const String &host, const Uint16 &port = 0);
    /// If you want to find a URL associated with an IP address
    String reverse_lookup(IPaddress ip);
  };

  ZENI_NET_DLL Net & get_Net(); ///< Get access to the singleton.

  class ZENI_NET_DLL TCP_Socket {
    TCP_Socket(const TCP_Socket &);
    TCP_Socket & operator=(const TCP_Socket &);
    
  public:
    TCP_Socket(IPaddress ip); ///< For outgoing connections
    TCP_Socket(TCPsocket sock); ///< For incoming connections
    ~TCP_Socket();

    IPaddress peer_address() const;
    int try_check_socket(); // return 0 if open, -1 on socket closed
    int check_socket(); // return 0 if open, throw Socket_Closed() on socket closed
    
    /// Send data
    int try_send(const void * const &data, const Uint16 &num_bytes); // send, returning 0 on success, -1 on socket closed
    int try_send(const String &data); // send, returning 0 on success, -1 on socket closed
    void send(const void * const &data, const Uint16 &num_bytes); // send, returning 0 on success, throw Socket_Closed() on socket closed
    void send(const String &data); // send, returning 0 on success, throw Socket_Closed() on socket closed

    /// Receive up to num_bytes
    int try_receive(void * const &data, const Uint16 &num_bytes); // receive, returning 0 on success, -1 on socket closed
    int try_receive(String &data, const Uint16 &num_bytes); // receive, returning 0 on success, -1 on socket closed
    int receive(void * const &data, const Uint16 &num_bytes); // receive, returning 0 on success, throw Socket_Closed() on socket closed
    int receive(String &data, const Uint16 &num_bytes); // receive, returning 0 on success, throw Socket_Closed() on socket closed

  private:
    TCPsocket sock;
    SDLNet_SocketSet sockset;

    class ZENI_NET_DLL Uninit : public Event::Handler {
      void operator()();

      Uninit * duplicate() const {
        return new Uninit(m_sock);
      }

      // Undefined
      Uninit(const Uninit &);
      Uninit operator=(const Uninit &);

    public:
      Uninit(TCP_Socket &sock_)
        : m_sock(sock_)
      {
      }

    private:
      TCP_Socket &m_sock;
    } m_uninit;
  };

  class ZENI_NET_DLL TCP_Listener {
    TCP_Listener(const TCP_Listener &);
    TCP_Listener & operator=(const TCP_Listener &);
    
  public:
    TCP_Listener(const Uint16 &port);
    ~TCP_Listener();

    TCPsocket accept(); ///< Returns 0 if no listener

  private:
    TCPsocket sock;

    class ZENI_NET_DLL Uninit : public Event::Handler {
      void operator()();

      Uninit * duplicate() const {
        return new Uninit(m_sock);
      }

      // Undefined
      Uninit(const Uninit &);
      Uninit operator=(const Uninit &);

    public:
      Uninit(TCP_Listener &sock_)
        : m_sock(sock_)
      {
      }

    private:
      TCP_Listener &m_sock;
    } m_uninit;
  };

  class ZENI_NET_DLL UDP_Socket {
    UDP_Socket(const UDP_Socket &);
    UDP_Socket & operator=(const UDP_Socket &);
    
  public:
    UDP_Socket(const Uint16 &port);
    virtual ~UDP_Socket();
    
    IPaddress peer_address() const; ///< Apparently only works if the port was explicitly specified

    /// Send data to an IPaddress
    virtual void send(const IPaddress &ip, const void * const &data, const Uint16 &num_bytes);
    virtual void send(const IPaddress &ip, const String &data);
    
    /// Receive data of up to data.size() from the returned IPaddress; Will error if num_bytes/data.size() is too low
    virtual int receive(IPaddress &ip, const void * const &data, const Uint16 &num_bytes);
    virtual int receive(IPaddress &ip, String &data); ///<
    
  private:
    UDPsocket sock;

    class ZENI_NET_DLL Uninit : public Event::Handler {
      void operator()();

      Uninit * duplicate() const {
        return new Uninit(m_sock);
      }

      // Undefined
      Uninit(const Uninit &);
      Uninit operator=(const Uninit &);

    public:
      Uninit(UDP_Socket &sock_)
        : m_sock(sock_)
      {
      }

    private:
      UDP_Socket &m_sock;
    } m_uninit;
  };
  
  class ZENI_NET_DLL Split_UDP_Socket : public UDP_Socket {
    Split_UDP_Socket(const Split_UDP_Socket &);
    Split_UDP_Socket & operator=(const Split_UDP_Socket &);
    
    struct ZENI_NET_DLL Chunk {
      Chunk() : size(0), data(0) {}
      ~Chunk() {delete [] data;}
      
      Chunk(const Chunk &rhs)
        : size(rhs.size),
          data(rhs.data)
      {
        rhs.size = 0;
        rhs.data = 0;
      }
      
      Chunk & operator=(const Chunk &rhs) {
        size = rhs.size;
        data = rhs.data;
        
        rhs.size = 0;
        rhs.data = 0;
        
        return *this;
      }
      
      mutable size_t size;
      mutable char * data;
    };
    
    class ZENI_NET_DLL Chunk_Set {
      Chunk_Set(const Chunk_Set &);
      Chunk_Set operator=(const Chunk_Set &);
    
    public:
      IPaddress ip;
      Nonce nonce;

#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
      std::vector<Chunk> chunks;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
      Uint16 chunks_arrived;
      
      Chunk_Set() : chunks_arrived(0u) {}
      
      Chunk_Set(const IPaddress &sender, const Nonce &incoming, const Uint16 &num_chunks, const Uint16 &which, Chunk &chunk);
      
      bool add_chunk(const IPaddress &sender, const Nonce &incoming, const Uint16 &num_chunks, const Uint16 &which, Chunk &chunk);
      bool complete() const;
      
      Chunk receive() const;
    };
    
    class ZENI_NET_DLL Chunk_Collector {
      Chunk_Collector(const Chunk_Collector &);
      Chunk_Collector operator=(const Chunk_Collector &);
      
    public:
      Chunk_Collector(const Uint16 &size = ZENI_DEFAULT_CHUNK_SIZE)
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
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
      std::list<Chunk_Set *> chunk_sets;
#ifdef _WINDOWS
#pragma warning( pop )
#endif
      Uint16 m_size;
    };
    
  public:
    Split_UDP_Socket(const Uint16 &port, const Uint16 &chunk_sets = ZENI_DEFAULT_CHUNK_SETS, const Uint16 &chunk_size = ZENI_DEFAULT_CHUNK_SIZE);

    /// Send data to an IPaddress
    virtual void send(const IPaddress &ip, const void * const &data, const Uint16 &num_bytes);
    virtual void send(const IPaddress &ip, const String &data);
    
    /// Receive data of up to data.size() from the returned IPaddress; Will error if num_bytes/data.size() is too low
    virtual int receive(IPaddress &ip, const void * const &data, const Uint16 &num_bytes);
    virtual int receive(IPaddress &ip, String &data);
    
  private:
    Uint16 m_chunk_size;
    
    Chunk_Collector m_chunk_collector;
    
    Nonce m_nonce_send;
  };

  struct ZENI_NET_DLL Net_Init_Failure : public Error {
    Net_Init_Failure() : Error("Zeni Net Failed to Initialize Correctly") {}
  };

  struct ZENI_NET_DLL TCP_Socket_Init_Failure : public Error {
    TCP_Socket_Init_Failure() : Error("Zeni TCP Socket Failed to Initialize Correctly") {}
  };

  struct ZENI_NET_DLL UDP_Socket_Init_Failure : public Error {
    UDP_Socket_Init_Failure() : Error("Zeni UDP Socket Failed to Initialize Correctly") {}
  };

  struct ZENI_NET_DLL UDP_Packet_Overflow : public Error {
    UDP_Packet_Overflow() : Error("Zeni UDP Packet Too Large") {}
  };

  struct ZENI_NET_DLL Socket_Closed : public Error {
    Socket_Closed() : Error("Zeni Socket Unexpectedly Closed") {}
  };

}

#include <Zeni/Undefine.h>

#endif
