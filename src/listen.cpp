#include <iostream>
#include <sys/types.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "clientObject.h"

int listenForData(char *buffer, uint16_t bufsize, Client &client) 
{
    int n;
    socklen_t len = sizeof(client.Socket);
    while(1)
    {
        n = recvfrom(client.sockfd, buffer, bufsize, MSG_WAITALL, ( struct sockaddr *)&client.Socket, &len);
        buffer[n] = '\0';
        printf("Received a datagram : %s\n", buffer);
    }
    return 0;
}


