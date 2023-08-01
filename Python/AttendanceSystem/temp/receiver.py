import cv2
import socket
import base64
import numpy as np

# Create socket 
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('127.0.0.1', 5000))

while True:
    # Receive frame
    data, address = sock.recvfrom(65535) 
    jpeg_buffer = base64.b64decode(data)
    
    # Decode and show frame
    frame = cv2.imdecode(np.frombuffer(jpeg_buffer, dtype='uint8'), 1) 
    cv2.imshow('Receiver', frame)
    
    if cv2.waitKey(1) == 13:
        break
        
cv2.destroyAllWindows()