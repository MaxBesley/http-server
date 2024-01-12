/* Max Besley. 20 May 2022. */

#ifndef STRUCT_H
#define STRUCT_H

/* Stores the three components of a HTTP 1.0 request. */
typedef struct {
    char *method;
    char *url;
    char *version;
} request_t;

#endif
