import socket
import struct

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind(('0.0.0.0', 12345))
s.listen(1)
conn = s.accept()
print("Connected by ", conn[1])

for i in range(0,1,8):
    reply = conn[0].recv(8)
    print(reply);
