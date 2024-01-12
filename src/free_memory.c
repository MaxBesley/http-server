/* Max Besley. 20 May 2022. */

#include "free_memory.h"


void
free_request_t(request_t *req) {
    if (req == NULL) {
        return;
    }
    free(req->method);
    free(req->url);
    free(req->version);
    free(req);
}
