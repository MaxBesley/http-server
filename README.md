# http-server

A basic HTTP server written using C and the Berkeley Sockets API.

The goal was to implement HTTP version 1.0 and so persistent connections are not supported.

This was made for a university project and is (of course) not a full implementaion.


## How to build

Simply run the command `make` from the root directory of this project.

This will compile an executable into `bin/` and clean up any object files.


## How to run

Usage:
```
./bin/server <protocol number> <port number> <path to web root>
```
Both IPv4 and IPv6 are supported.


## Example usage

To request content from the server you can use the `client.py` script.

For example, from the command-line run
```
./bin/server 4 8000 www
```
then use the `client.py` script like so
```
% python3 client.py
Enter a path: /index.html

HTTP/1.0 200 OK
Content-Type: text/html
Content-Length: 262

<!DOCTYPE html>
<html>
    <head>
        <title>Test Page</title>
    </head>
    <body>
        <h1>This is a test page.</h1>
        <img src="/assets/image.jpg" alt="web" />
        <p><a href="/subdir/other.html">Link</a></p>
    </body>
</html>

%
```

You can look inside the `www/` directory to see what files the server can serve.


## License

This codebase is MIT licensed.
