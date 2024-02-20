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
#define MAX_HOSTS           2048


typedef struct Host  // Structure that can be expanded and used to hold any needed info
{
    int                id;
    struct sockaddr_in address;         // The socket it contains (IP and port)
    const char*        addressStr[INET_ADDRSTRLEN];
    volatile bool      bListen;         // Is this host supposed to be currently listening on a thread 
    socketfd           associatedSocket;// Socket that gets associated with this host.
                                        // This allows the socket from a connection 
                                        // to be saved and reused!
    SSL               *ssl;             // Not NULL if we're connected over SSL
    void              *customAttribute; // library user can store whatever in here
} Host;

BBNETAPI extern Host        *createHost            (const char *ip, 
                                                    const uint16_t port);
BBNETAPI extern void         deleteHost            (Host* host);       // Removes a client, closing the socket and freeing the memory
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

// Custom attribute stuff
BBNETAPI extern void        *getHostCustomAttr     (Host* host); 
BBNETAPI extern void         setHostCustomAttr     (Host* host,
                                                    void* ptr); 

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
