from socket import socket, timeout


IP_ADDRESS = # put yours here
PORT = 8000
MAX_BYTES = 1024

s = socket()
s.settimeout(2)
s.connect((IP_ADDRESS, PORT))

path = input('Enter a path: ')
print()
request = 'GET ' + path + ' HTTP/1.0'
s.send(request.encode())

result = ''
while True:
    try:
        response = s.recv(MAX_BYTES).decode()
        if response:
            result += response
    except timeout:
        break

print(result)
