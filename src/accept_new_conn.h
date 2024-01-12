/* Max Besley. 20 May 2022. */

#include <sys/socket.h>
#include "exit.h"

/* Uses a passive socket to accept a
   connection request from a client. */
int accept_new_conn(int listenfd);
