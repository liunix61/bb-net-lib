#ifndef hostOBJECT
#define hostOBJECT

#include <netinet/in.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>

#include "defines.h"
#include "socketfd.h"

#define MAX_HOST_CACHES     16
#define MAX_HOSTS_PER_CACHE 1024


typedef struct Host 
{
    const char*        addressStr[INET_ADDRSTRLEN];
    SSL               *ssl;             // Not NULL if we're connected over SSL
    int                id;
    struct sockaddr_in address;         // The socket it contains (IP and port)
    volatile bool      bListen;         // Is this host supposed to be currently listening on a thread 
    socketfd           associatedSocket;// Socket that gets associated with this host.
                                        // This allows the socket from a connection 
                                        // to be saved and reused!
    // Flags I should compress if I have too many
    bool               isCached;        // If the host has been cached,
                                        // it should not be freed until
                                        // the cache is destroyed.
    bool               isWaiting;       // This host has a non-blocking socket
                                        // that's waiting to call send/recv again
} Host;

BBNETAPI extern Host        *createHost            (const char *ip, 
                                                    const uint16_t port);
extern          void         destroyHost           (Host **host);
BBNETAPI extern const char  *getIP                 (Host* host);       // Returns a string representation of the IP address in the client
BBNETAPI extern uint16_t     getPort               (Host* host);
extern void                  callHostPacketHandler (char* data, 
                                                    uint16_t size, 
                                                    Host* host);
// Not thread safe
extern void                  fastCopyHost          (Host* dstHost, 
                                                    Host* srcHost);
// Thread safe copy
BBNETAPI extern void         copyHost              (Host* dstHost, 
                                                    Host* srcHost);

// Host Caching functions
BBNETAPI extern void         cacheHost             (Host* host, 
                                                    int cacheIndex);
BBNETAPI extern void         clearHostCache        (int cacheIndex);
extern const int             getCacheOccupancy     (int cacheIndex);
extern Host                 *getHostFromCache      (int cacheIndex,
                                                    int hostIndex);
extern int                   getHostID             (Host *host);

// Checking or setting an interface for listening
extern void                  setCommunicating      (Host* host);        // Sets the bListen boolean
BBNETAPI extern void         closeConnections      (Host* host);        // Unsets the bListen boolean
extern bool                  isCommunicating       (const Host* host);
extern void                  setSocket             (Host* host, 
                                                    socketfd sockfd);
extern socketfd              getSocket             (const Host* host);

#endif
