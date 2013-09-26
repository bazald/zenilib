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

#include <zeni_net.h>

#include <SDL/SDL.h>
#include <vector>
#include <list>
#include <sstream>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Singleton.hxx>

namespace Zeni {

  template class ZENI_NET_DLL Singleton<Net>;

  Net * Net::create() {
    return new Net;
  }

  Singleton<Net>::Uninit Net::g_uninit;
  Singleton<Net>::Reinit Net::g_reinit;

  Net::Net() {
    Core::remove_post_reinit(&g_reinit);

    // Ensure Core is initialized
    Core &cr = get_Core();

    if(SDLNet_Init())
      throw Net_Init_Failure();

    cr.lend_pre_uninit(&g_uninit);
    cr.lend_post_reinit(&g_reinit);
  }

  Net::~Net() {
    Core::remove_pre_uninit(&g_uninit);

    SDLNet_Quit();
  }

  IPaddress Net::resolve_host(const String &host, const Uint16 &port) {
    IPaddress ip;
    SDLNet_ResolveHost(&ip, host.c_str(), port);
    return ip;
  }

  String Net::reverse_lookup(IPaddress ip) {
    return SDLNet_ResolveIP(&ip);
  }

  Net & get_Net() {
    return Net::get();
  }

  TCP_Socket::TCP_Socket(IPaddress ip)
    : sock(0),
    sockset(0),
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif
    m_uninit(*this)
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  {
    Net &nr = get_Net();

    if(!ip.host)
      throw TCP_Socket_Init_Failure();

    sock = SDLNet_TCP_Open(&ip);
    sockset = SDLNet_AllocSocketSet(1);
    
    if(!sock ||
       !sockset ||
       SDLNet_TCP_AddSocket(sockset, sock) == -1)
    {
      SDLNet_TCP_Close(sock);
      SDLNet_FreeSocketSet(sockset);
      throw TCP_Socket_Init_Failure();
    }

    nr.lend_pre_uninit(&m_uninit);
  }

  TCP_Socket::TCP_Socket(TCPsocket socket)
    : sock(socket),
    sockset(0),
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif
    m_uninit(*this)
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  {
    if(!sock)
      throw TCP_Socket_Init_Failure();

    sockset = SDLNet_AllocSocketSet(1);

    if(!sockset ||
       SDLNet_TCP_AddSocket(sockset, sock) == -1)
    {
      SDLNet_TCP_Close(sock);
      throw TCP_Socket_Init_Failure();
    }

    get_Net().lend_pre_uninit(&m_uninit);
  }

  TCP_Socket::~TCP_Socket() {
    Net::remove_pre_uninit(&m_uninit);

    if(sock && sockset) {
      SDLNet_TCP_DelSocket(sockset, sock);
      SDLNet_FreeSocketSet(sockset);
      SDLNet_TCP_Close(sock);
    }
  }

  IPaddress TCP_Socket::peer_address() const {
    return *SDLNet_TCP_GetPeerAddress(sock);
  }
  
  int TCP_Socket::try_check_socket() {
    return SDLNet_CheckSockets(sockset, 0);
  }
  
  int TCP_Socket::check_socket() {
    int rv = try_check_socket();

    if(rv == -1)
      throw Socket_Closed();

    return rv;
  }

  int TCP_Socket::try_send(const void * const &data, const Uint16 &num_bytes) {
    return SDLNet_TCP_Send(sock, const_cast<void *>(data), num_bytes) < num_bytes ? -1 : 0;
  }

  int TCP_Socket::try_send(const String &data) {
    return try_send(data.c_str(), Uint16(data.size()));
  }

  void TCP_Socket::send(const void * const &data, const Uint16 &num_bytes) {
    if(try_send(data, num_bytes) == -1)
      throw Socket_Closed();
  }

  void TCP_Socket::send(const String &data) {
    if(try_send(data) == -1)
      throw Socket_Closed();
  }

  int TCP_Socket::try_receive(void * const &data, const Uint16 &num_bytes) {
    int retval = check_socket();
    
    if(retval) {
      retval = SDLNet_TCP_Recv(sock, data, num_bytes);
      if(retval <= 0)
        return -1;
    }

    return retval;
  }

  int TCP_Socket::try_receive(String &data, const Uint16 &num_bytes) {
    data.resize(size_t(num_bytes));

    const int retval = receive(const_cast<char *>(data.c_str()), num_bytes);
    if(retval == -1)
      return -1;

    data.resize(size_t(retval));
    return retval;
  }

  int TCP_Socket::receive(void * const &data, const Uint16 &num_bytes) {
    const int rv = try_receive(data, num_bytes);

    if(rv == -1)
      throw Socket_Closed();

    return rv;
  }

  int TCP_Socket::receive(String &data, const Uint16 &num_bytes) {
    const int rv = try_receive(data, num_bytes);

    if(rv == -1)
      throw Socket_Closed();

    return rv;
  }

  void TCP_Socket::Uninit::operator()() {
    SDLNet_TCP_DelSocket(m_sock.sockset, m_sock.sock);
    SDLNet_FreeSocketSet(m_sock.sockset);
    SDLNet_TCP_Close(m_sock.sock);

    m_sock.sock = 0;
    m_sock.sockset = 0;

    get_Core().remove_pre_uninit(this);
  }

  TCP_Listener::TCP_Listener(const Uint16 &port)
    : sock(0),
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif
    m_uninit(*this)
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  {
    Net &nr = get_Net();

    IPaddress ip = {0, 0};
    SDLNet_Write16(port, &ip.port);

    sock = SDLNet_TCP_Open(&ip);
    if(!sock)
      throw TCP_Socket_Init_Failure();

    nr.lend_pre_uninit(&m_uninit);
  }

  TCP_Listener::~TCP_Listener() {
    get_Net().remove_pre_uninit(&m_uninit);

    if(sock)
      SDLNet_TCP_Close(sock);
  }

  void TCP_Listener::Uninit::operator()() {
    SDLNet_TCP_Close(m_sock.sock);

    m_sock.sock = 0;

    get_Core().remove_pre_uninit(this);
  }

  TCPsocket TCP_Listener::accept() {
    return SDLNet_TCP_Accept(sock);
  }

  UDP_Socket::UDP_Socket(const Uint16 &port)
    : sock(0),
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4355 )
#endif
    m_uninit(*this)
#ifdef _WINDOWS
#pragma warning( pop )
#endif
  {
    Net &nr = get_Net();

    sock = SDLNet_UDP_Open(port);
    if(!sock)
      throw UDP_Socket_Init_Failure();
    
    nr.lend_pre_uninit(&m_uninit);
  }

  UDP_Socket::~UDP_Socket() {
    Net::remove_pre_uninit(&m_uninit);

    if(sock)
      SDLNet_UDP_Close(sock);
  }
  
  IPaddress UDP_Socket::peer_address() const {
    return *SDLNet_UDP_GetPeerAddress(sock, -1);
  }
  
  void UDP_Socket::send(const IPaddress &ip, const void * const &data, const Uint16 &num_bytes) {
    if(num_bytes < 8167u) {
      UDPpacket packet =
      {
        -1,
        reinterpret_cast<Uint8 *>(const_cast<void *>(data)),
        num_bytes,
        num_bytes,
        0, // Will == -1 on error after UDP_Send, otherwise == # of bytes sent
        ip
      };

      if(SDLNet_UDP_Send(sock, -1, &packet))
        return;

      throw Socket_Closed();
    }

    throw UDP_Packet_Overflow();
  }
  
  void UDP_Socket::send(const IPaddress &ip, const String &data) {
    UDP_Socket::send(ip, data.c_str(), Uint16(data.size()));
  }

  int UDP_Socket::receive(IPaddress &ip, const void * const &data, const Uint16 &num_bytes) {
    IPaddress ipaddress = {0, 0};

    UDPpacket packet =
    {
      -1,
      reinterpret_cast<Uint8 *>(const_cast<void *>(data)),
      0,
      num_bytes,
      0,
      ipaddress
    };

    int retval = SDLNet_UDP_Recv(sock, &packet);
    if(retval == -1)
      throw Socket_Closed();
    else if(!retval) {
      packet.address.host = 0;
      packet.address.port = 0;
    }

    ip = packet.address;
    
    return packet.len;
  }
  
  int UDP_Socket::receive(IPaddress &ip, String &data) {
    int retval = UDP_Socket::receive(ip, data.c_str(), Uint16(data.size()));
    
    if(int(data.size()) > retval) {
      data[static_cast<unsigned int>(retval)] = '\0';
      data.resize(size_t(retval));
    }
    
    return retval;
  }

  void UDP_Socket::Uninit::operator()() {
    SDLNet_UDP_Close(m_sock.sock);

    m_sock.sock = 0;

    get_Core().remove_pre_uninit(this);
  }

  Split_UDP_Socket::Chunk_Set::Chunk_Set(const IPaddress &sender,
                                         const Nonce &incoming,
                                         const Uint16 &num_chunks,
                                         const Uint16 &which,
                                         Chunk &chunk)
    : ip(sender),
      nonce(incoming),
      chunks(num_chunks),
      chunks_arrived(1u)
  {
    assert(num_chunks > which);
    
    if(num_chunks > which)
      chunks[which] = chunk;
  }
  
  bool Split_UDP_Socket::Chunk_Set::add_chunk(const IPaddress &sender,
                                              const Nonce &incoming,
                                              const Uint16 &num_chunks,
                                              const Uint16 &which,
                                              Chunk &chunk) {
    if(ip == sender &&
       nonce == incoming &&
       chunks.size() == num_chunks &&
       chunks.size() > which)
    {
      if(!chunks[which].data)
        chunks[which] = chunk;
      ++chunks_arrived;
      return true;
    }
    
    return false;
  }
  
  bool Split_UDP_Socket::Chunk_Set::complete() const {
    return chunks_arrived == chunks.size();

    //for(std::vector<Chunk>::const_iterator it = chunks.begin(); it != chunks.end(); ++it)
    //  if(!it->data)
    //    return false;
    //
    //return true;
  }
  
  Split_UDP_Socket::Chunk Split_UDP_Socket::Chunk_Set::receive() const {
    Chunk packet;
    
    for(std::vector<Chunk>::const_iterator it = chunks.begin(); it != chunks.end(); ++it)
      packet.size += it->size;
    
    char * dst = new char [packet.size];
    packet.data = dst;
    
    for(std::vector<Chunk>::const_iterator it = chunks.begin(); it != chunks.end(); ++it) {
      memcpy(dst, it->data, it->size);
      dst += it->size;
    }
    
    return packet;
  }
  
  const Split_UDP_Socket::Chunk_Set * Split_UDP_Socket::Chunk_Collector::add_chunk(const IPaddress &sender,
                                                                                   const Nonce &incoming,
                                                                                   const Uint16 &num_chunks,
                                                                                   const Uint16 &which,
                                                                                   Chunk &chunk) {
    // Attempt to complete an existing partial packet
    for(std::list<Chunk_Set *>::iterator it = chunk_sets.begin(); it != chunk_sets.end(); ++it) {
      if((*it)->add_chunk(sender, incoming, num_chunks, which, chunk)) {
        if((*it)->complete())
          return *it;
        else
          return 0;
      }
    }
    
    if(int(chunk_sets.size()) == m_size) // LRU Eviction
      chunk_sets.pop_back();
    
    {// Receive fresh chunk
      Chunk_Set *cs = new Chunk_Set(sender, incoming, num_chunks, which, chunk);
      chunk_sets.push_front(cs);
      if(cs->complete())
        return cs;
    }
    
    return 0;
  }
  
  Split_UDP_Socket::Split_UDP_Socket(const Uint16 &port, const Uint16 &chunk_sets, const Uint16 &chunk_size)
    : UDP_Socket(port),
      m_chunk_size(chunk_size),
      m_chunk_collector(chunk_sets)
  {
    assert(chunk_size);
  }
  
  void Split_UDP_Socket::send(const IPaddress &ip, const void * const &data, const Uint16 &num_bytes) {
    ++m_nonce_send;
    
    const Uint16 offset = static_cast<Uint16>(m_nonce_send.size()) + 2u * sizeof(Uint16);
    const Uint16 split_size = Uint16(m_chunk_size - offset);
    const Uint16 num_full_chunks = Uint16(num_bytes / split_size);
    const Uint16 partial_chunk = Uint16(num_bytes % split_size);
    const Uint16 num_chunks = num_full_chunks + (partial_chunk ? 1u : 0u);
    
    const char *ptr = reinterpret_cast<const char *>(data);
    for(Uint16 chunk = 0; chunk < num_full_chunks; ++chunk, ptr += split_size) {
      String s;
      
      {
        std::ostringstream os;
        serialize(serialize(m_nonce_send.serialize(os), num_chunks), chunk);
        s = os.str();
      }
      
      s.resize(m_chunk_size);
      memcpy(const_cast<char *>(s.c_str()) + offset, ptr, split_size);
      
      UDP_Socket::send(ip, s);
    }
    
    if(partial_chunk) {
      String s;
      
      {
        std::ostringstream os;
        serialize(serialize(m_nonce_send.serialize(os), num_chunks), num_full_chunks);
        s = os.str();
      }
      
      s.resize(size_t(offset + partial_chunk));
      memcpy(const_cast<char *>(s.c_str()) + offset, ptr, partial_chunk);
      
      UDP_Socket::send(ip, s);
    }
  }
  
  void Split_UDP_Socket::send(const IPaddress &ip, const String &data) {
    send(ip, data.c_str(), Uint16(data.size()));
  }

  int Split_UDP_Socket::receive(IPaddress &ip, const void * const &data, const Uint16 &num_bytes) {
    for(int retval = -1; retval;) {
      String s;
      s.resize(m_chunk_size);
      
      retval = UDP_Socket::receive(ip, s);
      if(retval) {
        Nonce nonce;
        Uint16 num_chunks;
        Uint16 which;
        Chunk chunk;
        
        {
          std::istringstream is(s.std_str());
          unserialize(unserialize(nonce.unserialize(is), num_chunks), which);

          if(!is)
            continue;

          const Uint16 offset = static_cast<Uint16>(nonce.size()) + 2u * sizeof(Uint16);
          
          chunk.size = s.size() - offset;
          chunk.data = new char [chunk.size];
          memcpy(chunk.data, s.c_str() + offset, chunk.size);
        }
        
        const Chunk_Set * cs = m_chunk_collector.add_chunk(ip, nonce, num_chunks, which, chunk);
        if(!cs)
          continue;
        
        Chunk packet = cs->receive();
        if(num_bytes >= Uint16(packet.size)) {
          memcpy(const_cast<void *>(data), packet.data, packet.size);
          return int(packet.size);
        }
        
        return 0;
      }
    }
    
    return 0;
  }
  
  int Split_UDP_Socket::receive(IPaddress &ip, String &data) {
    int retval = receive(ip, data.c_str(), Uint16(data.size()));
    
    if(int(data.size()) > retval) {
      data[static_cast<unsigned int>(retval)] = '\0';
      data.resize(size_t(retval));
    }
    
    return retval;
  }
  
}
