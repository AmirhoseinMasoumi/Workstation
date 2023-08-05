import socket

s = socket.socket()
host = '127.0.0.1'
port = 12345

s.connect((host, port))
print('Connected to server')

with open('train.pkl', 'rb') as f:
    data = f.read()
    s.send(data)

print('File sent successfully!')
s.close()