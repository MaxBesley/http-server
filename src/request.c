/* Max Besley. 20 May 2022. */

#include "request.h"


request_t*
parse_request(int8_t *line) {

    // Allocate memory for a request_t struct
    request_t *req = (request_t*)malloc(sizeof(request_t));
    assert(req);
    // Set all bytes in `req` to zero
    memset(req, 0, sizeof(request_t));

    // Count the number of characters before a space
    size_t method_len = strcspn((char*)line, " ");
    // Allocate space for the method string
    req->method = (char*)malloc(method_len + 1);
    assert(req->method);
    // Copy over the method and null-terminate
    memcpy(req->method, line, method_len);
    req->method[method_len] = '\0';
    // Move pointer `line` past the SP (space) character
    line += method_len + 1;

    // Find the length of the URL
    size_t url_len = strcspn((char*)line, " ");
    // Allocate space for the URL string
    req->url = (char*)malloc(url_len + 1);
    assert(req->url);
    // Store the URL and null-terminate
    memcpy(req->url, line, url_len);
    req->url[url_len] = '\0';
    // Move the pointer along like before
    line += url_len + 1;

    // Just like above
    size_t version_len = strcspn((char*)line, "\r\n");
    req->version = (char*)malloc(version_len + 1);
    assert(req->version);
    memcpy(req->version, line, version_len);
    req->method[version_len] = '\0';

    // Note that the server has limited functionality
    if (!strcmp(req->method, "GET") && !strcmp(req->version, "HTTP/1.0")) {
        return req;
    } else {
        // Oops! We actually only support GET requests and HTTP 1.0
        free_request_t(req);
        // Here, returning NULL indicates failure
        return NULL;
    }
}

void
handle_request(int connfd, request_t *req, char *wroot) {
    int filefd;
    char *content_type, *ext, path[2048];
    size_t file_size;
    ssize_t bytes_sent;

    // The server doesn't support requests
    // that contain a "../" path component
    if (strstr(req->url, "/../") != NULL) {
        send_404(connfd);
        return;
    }

    // Construct the path to the file being requested
    memset(path, 0, sizeof(path));
    strcpy(path, wroot);
    strcat(path, req->url);

    // Check if the file doesn't exist or if the
    // file exists but cannot be opened for reading
    if (!file_exists(path) || (filefd = open(path, O_RDONLY)) < 0) {
        // Respond with a 404 error
        send_404(connfd);
        return;
    }

    // If here the file exists and has been opened
    // Now determine the size of the file in bytes
    struct stat sb;
    if (fstat(filefd, &sb) < 0) {
        error_and_exit("fstat()");
    }
    file_size = sb.st_size;

    // Determine and store the MIME type
    ext = strrchr(path, '.');
    if (!ext) {
        // If here then there is no extension
        content_type = "application/octet-stream";
    } else if (!strcasecmp(ext, ".html")) {
        content_type = "text/html";
    } else if (!strcasecmp(ext, ".jpg")) {
        content_type = "image/jpeg";
    } else if (!strcasecmp(ext, ".png")) {
        content_type = "image/png";
    } else if (!strcasecmp(ext, ".css")) {
        content_type = "text/css";
    } else if (!strcasecmp(ext, ".js")) {
        content_type = "text/javascript";
    } else if (!strcasecmp(ext, ".txt")) {
        content_type = "text/plain";
    } else {
        // For all other extensions (e.g. .csv or .php)
        content_type = "application/octet-stream";
    }

    // Headers
    dprintf(connfd, "HTTP/1.0 200 OK\r\n");
    dprintf(connfd, "Content-Type: %s\r\n", content_type);
    dprintf(connfd, "Content-Length: %ld\r\n", file_size);
    dprintf(connfd, "\r\n");

    // Response body
    bytes_sent = sendfile(connfd, filefd, NULL, file_size);
    if (bytes_sent < 0) {
        error_and_exit("sendfile()");
    }
    /* NOTE:
       Using sendfile() is faster than alternatively using a combination of
       read()/write() (or recv()/send()), giving better performance. I/O using read(),
       write() and a buffer involves lots of context switching between user mode
       and kernel mode, since two system calls are being invoked seperately. All
       this context switching will slow performance.
       The sendfile() function, however, copies data between two files all within
       the kernel, meaning there is less context switching and hence better
       performance/efficiency.
       Using sendfile() is also more simple, since it requires only a single line
       of code (as seen above). Using a read()/write() pair often requires
       iteration through a while loop, and more lines of code.
    */
}

void
send_404(int sockfd) {
    dprintf(sockfd, "HTTP/1.0 404 Not Found\r\n");
    dprintf(sockfd, "\r\n");
}

int
file_exists(char *path) {
    if (!access(path, F_OK)) {
        // The file exists
        return 1;
    } else {
        return 0;
    }
}
