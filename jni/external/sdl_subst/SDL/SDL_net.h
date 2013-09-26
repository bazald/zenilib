#ifndef SDL_NET_H
#define SDL_NET_H

#include <SDL/SDL_stdinc.h>
#include <netinet/in.h>

struct IPaddress {
  Uint32 host;
  Uint16 port;
};

inline Uint16 SDLNet_Read16(const void * const &area) {
  return ntohs(*static_cast<const Uint16 *>(area));
}

inline Uint32 SDLNet_Read32(const void * const &area) {
  return ntohl(*static_cast<const Uint32 *>(area));
}

inline void SDLNet_Write16(const Uint32 &value, void * const &area) {
  *static_cast<Uint16 *>(area) = htons(value);
}

inline void SDLNet_Write32(const Uint32 &value, void * const &area) {
  *static_cast<Uint32 *>(area) = htonl(value);
}

#endif
