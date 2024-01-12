/* Max Besley. 20 May 2022. */

#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include "exit.h"
#include "struct.h"
#include "free_memory.h"

/* Parses a client's request and stores the three
   components of the first line in a request_t struct. */
request_t* parse_request(int8_t *line);

/* Sends a client the appropriate HTTP response. */
void handle_request(int connfd, request_t *req, char *wroot);

/* Uses the passed socket descriptor to send
   a simple 404 response to the client. */
void send_404(int sockfd);

/* Determines whether a file exists in the server.
   Uses an (absoulte) path that is meant to identify the file. */
int file_exists(char *path);
