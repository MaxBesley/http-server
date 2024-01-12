/* Max Besley. 20 May 2022. */

#include "accept_new_conn.h"


int
accept_new_conn(int listenfd) {
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    // Call `accept()`
    int connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_addr_size);
    if (connfd < 0) {
        error_and_exit("accept()");
    }

    return connfd;
}
