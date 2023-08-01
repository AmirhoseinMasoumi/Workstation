# Sender

import cv2
import socket
import base64

sender_addr = ('127.0.0.1', 5000)
receiver_addr = ('127.0.0.1', 5001)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(sender_addr) 

def encode_frame(frame):
    # Encode frame to jpg and convert to base64 string
    encoded, buffer = cv2.imencode('.jpg', frame)
    jpg_as_text = base64.b64encode(buffer)
    return jpg_as_text

cap = cv2.VideoCapture(0)  

while True:
    ret, frame = cap.read()
    
    small_frame = cv2.resize(frame, (320, 240))
    
    encoded_frame = encode_frame(small_frame)
    
    sock.sendto(encoded_frame, receiver_addr)
    
    ack, _ = sock.recvfrom(1024)
    print("Received:", ack.decode())

    cv2.imshow('Sender', small_frame)
    
    if cv2.waitKey(1) == 13:
        break

cap.release()
sock.close()
cv2.destroyAllWindows()