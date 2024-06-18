/* The entry point of the program.
   Usage: ./<executable name> <protocol number> <port number> <path to web root>
   Max Besley. 20 May 2022.
*/

#include "request.h"
#include "listen_on.h"
#include "accept_new_conn.h"

#define NUM_CL_ARGS 4              // number of command line arguments expected
#define _POSIX_C_SOURCE 200809L    // a necessary macro
#define MULTITHREADED 0            // toggle concurrency on/off

/* Handles a connection with a client. Much of the work is done inside here. */
void handle_connection(int connfd, char *wroot);


int
main(int argc, char **argv) {
    int listenfd, connfd;
    char *protocol, *port, wroot[2048];

    // Handle command line arguments
    if (argc != NUM_CL_ARGS) {
        // Invalid command line arguments, so by
        // default just use these parameters.
        protocol = "4";
        port = "0";
        strcpy(wroot, "/");
    } else {
        // Store the parameters from the command line
        protocol = argv[1];
        port = argv[2];
        strcpy(wroot, argv[3]);
    }

    // Handle ctrl+c in a user-defined way
    signal(SIGINT, interrupt_handler);

    // Create a socket that will listen on the specified port
    listenfd = listen_on(port, protocol);

#if !MULTITHREADED
    while (1) {
        connfd = accept_new_conn(listenfd);
        handle_connection(connfd, wroot);
    }
#else
    // Used for concurrency
    fd_set allfds;             // Will keep track of all sockets the server knows of
    fd_set readablefds;        // Will store the set of sockets that're ready for reading (a subset of `allfds`)
    FD_ZERO(&allfds);          // Zero out `allfds`
    FD_SET(listenfd, &allfds); // Add the passive listening socket to `allfds`

    while (1) {
        // Store a copy of all the file descriptors being tracked
        readablefds = allfds;

        // Wait forever for a socket to be ready for reading
        if (select(FD_SETSIZE, &readablefds, NULL, NULL, NULL) < 0) {
            error_and_exit("select()");
        }

        /* At this point `readablefds` only contains the
           sockets that are ready to be read from. */

        // Loop over every possible file descriptor value
        for (int fd = 0; fd < FD_SETSIZE; fd++) {
            // Check if a file descriptor is ready to be read
            if (FD_ISSET(fd, &readablefds)) {
                // Check if the socket ready for reading
                // is the socket we are listening on
                if (fd == listenfd) {
                    // Accept the new connection request
                    connfd = accept_new_conn(listenfd);
                    // Add the new connection to our collection of sockets/connections
                    FD_SET(connfd, &allfds);
                } else {
                    // If here then `fd` is connected to a client
                    // So read in the HTTP request and then respond
                    handle_connection(fd, wroot);
                    // We are done with this connection (i.e. non-persistent)
                    FD_CLR(fd, &allfds);
                    close(fd);
                }
            }
        }
        // LOOP FOREVER!
    }
#endif

    // This line will actually never be reached
    return EXIT_SUCCESS;
}

void
handle_connection(int connfd, char *wroot) {
    int8_t readbuff[2048];
    request_t *req;

    // Attempt to read in a request in one `read()` call
    memset(readbuff, 0, sizeof(readbuff));
    ssize_t bytes_read = read(connfd, readbuff, sizeof(readbuff));
    if (bytes_read < 0) {
        error_and_exit("read()");
    }

    // Parse the HTTP request
    req = parse_request(readbuff);

    if (req == NULL) {
        // We cannot support the request
        dprintf(connfd, "Sorry, the server does not support that request\n");
    } else {
        // Process the request and then send a response
        handle_request(connfd, req, wroot);
    }

    free_request_t(req);
}
