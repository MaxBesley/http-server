/* Max Besley. 20 May 2022. */

#include "listen_on.h"


int
listen_on(char *port, char *protocol) {
    int sockfd, ipv6, status, enable;
    struct addrinfo hints, *servinfo, *p;

    // Determine which version of IP is required
    if (!strcmp(protocol, "6")) {
        ipv6 = 1;
    } else {
        ipv6 = 0;
    }

    // Fill in `hints` with some relevant information
    memset(&hints, 0, sizeof(hints));
    if (ipv6) {
        hints.ai_family = AF_INET6;
    } else {
        hints.ai_family = AF_INET;
    }
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Use `port` and `hints` to fill in `servinfo` (which is a linked list)
    status = getaddrinfo(NULL, port, &hints, &servinfo);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Create a socket using the information returned from `getaddrinfo()`
    p = servinfo;
    if (ipv6) {
        // Traverse the list and attempt to create a socket
        while (p != NULL) {
            if (p->ai_family == AF_INET6 && (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) >= 0) {
                break;
            }
            p = p->ai_next;
        }
    } else {
        // For IPv4
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            perror("socket()");
            exit(EXIT_FAILURE);
        }
    }

    // Check if the IPv6 branch failed
    if (p == NULL) {
        fprintf(stderr, "failed to create a socket\n");
        exit(EXIT_FAILURE);
    }

    // Use an addrinfo struct to create the socket
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    // Reuse the port number
    enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt()");
        exit(EXIT_FAILURE);
    }

    // Bind the IP address to the socket
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    // Free the linked list
    freeaddrinfo(servinfo);

    // Make the socket passive (i.e. ready to accept connections)
    // We can queue up at most `QUEUE_LEN` connection requests
    if (listen(sockfd, QUEUE_LEN) < 0) {
        perror("listen()");
        exit(EXIT_FAILURE);
    }

    // We have what we want!
    return sockfd;
}
