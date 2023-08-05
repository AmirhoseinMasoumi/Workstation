import socket

s = socket.socket()
host = '127.0.0.1' 
port = 12345
s.bind((host, port))

s.listen(5)
print('Server listening...')

while True:
    conn, addr = s.accept()
    print('Got connection from', addr)
    data = conn.recv(1024)
    with open('train3.pkl', 'wb') as f:
        f.write(data)
    print('File received successfully!')
    conn.close()