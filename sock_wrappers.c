#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#define LISTENQ 1024

int open_clientfd(char *hostname, char *port)
{
    int clientfd;
    struct addrinfo hints, *listp, *p;

    // get a list of potential server addresses
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV; // using a numeric port arg
    hints.ai_flags |= AI_ADDRCONFIG; // recommended for connections
    getaddrinfo(hostname, port, &hints, &listp);

    for (p = listp; p; p = p->ai_next) {
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;
        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
            break;
        close(clientfd);
    }

    freeaddrinfo(listp);
    if (!p)
        return -1;
    else
        return clientfd;
}

int open_listenfd(char *port)
{
    struct addrinfo hints, *listp, *p;
    int listenfd, optval=1;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // on any IP address
    hints.ai_flags |= AI_NUMERICSERV;
    getaddrinfo(NULL, port, &hints, &listp);

    for (p = listp; p; p = p->ai_next) {
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;
        // eliminate "address already in use" error from bind
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break; // success
        close(listenfd);
    }

    freeaddrinfo(listp);
    if (!p)
        return -1;

    if (listen(listenfd, LISTENQ) < 0) {
        close(listenfd);
        return -1;
    }
    return listenfd;
}

