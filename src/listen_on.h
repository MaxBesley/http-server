/* Max Besley. 20 May 2022. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

#define QUEUE_LEN 10    // size of the server's TCP backlog

/* Creates a socket that is listening on the port number
   specified by `port`. Returning the socket's file
   descriptor (which is an int). */
int listen_on(char *port, char *protocol);
