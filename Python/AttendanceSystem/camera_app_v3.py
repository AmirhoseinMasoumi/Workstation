import sys
import cv2
import socket
import numpy as np
import base64
import select
import time

# Create socket
sender_addr = ('127.0.0.1', 5000)
receiver_addr = ('127.0.0.1', 5001)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(sender_addr)

# Set socket to non-blocking mode
sock.setblocking(0)

font = cv2.FONT_HERSHEY_SIMPLEX
text = ""
frame_counter = 0

sensor_id=0 
capture_width=1920 
capture_height=1080 
display_width=960 
display_height=540 
framerate=30 
flip_method=0

camset = 'nvarguscamerasrc sensor-id=(int){sensor_id} ! video/x-raw(memory:NVMM), width=(int)%d, height=(int)%d, framerate=(fraction)%d/1 ! " "nvvidconv flip-method=%d ! " "video/x-raw, width=(int)%d, height=(int)%d, format=(string)BGRx ! " "videoconvert ! " "video/x-raw, format=(string)BGR ! appsink" % ( sensor_id, capture_width, capture_height, framerate, flip_method, display_width, display_height, ) )

def encode_frame(frame):
    # Encode frame to jpg and convert to base64 string
    encoded, buffer = cv2.imencode('.jpg', frame)
    jpg_as_text = base64.b64encode(buffer)
    return jpg_as_text

class FaceDetectionApp:
    def __init__(self):
        # camSet0 = "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, format=(string)NV12, framerate=(fraction)30/1 ! nvvidconv flip-method=2 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"
        self.camera = cv2.VideoCapture(0)
        # self.camera = cv2.VideoCapture("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, format=(string)NV12, framerate=(fraction)30/1 ! nvvidconv flip-method=2 ! video/x-raw, width=(int)1280, height=(int)720, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink", cv2.CAP_GSTREAMER)

    def detect_face(self):
        global text, frame_counter
        ret, frame_or = self.camera.read()
        frame = cv2.flip(frame_or, 1)

        if ret:
            # Encode and send UDP
            small_frame = cv2.resize(frame, (320, 240))
            encoded_frame = encode_frame(small_frame)
            sock.sendto(encoded_frame, receiver_addr)

            # Check for incoming data on the socket
            ready_to_read, _, _ = select.select([sock], [], [], 0)  # Timeout is set to 0 (non-blocking)

            if ready_to_read:
                ack, _ = sock.recvfrom(1024)
                self.stringdata = ack.decode('utf-8')
                self.split_input = self.stringdata.split('_')
                # print(self.split_input[0] + " - " + self.split_input[1])
                if self.split_input[0] == "E":
                    text = "Welcome " + self.split_input[1]
                    print(text)
                if self.split_input[0] == "L":
                    text = "Have a good day " + self.split_input[1]
                    print(text)

            gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
            faces = face_cascade.detectMultiScale(gray_frame, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

            nearest_face = None
            nearest_distance = float('inf')  # Initialize with a large value

            for (x, y, w, h) in faces:
                face_center = (x + w // 2, y + h // 2)
                distance = ((frame.shape[1] / 2 - face_center[0]) ** 2 + (frame.shape[0] / 2 - face_center[1]) ** 2) ** 0.5

                if distance < nearest_distance:
                    nearest_distance = distance
                    nearest_face = (x, y, w, h)

            for (x, y, w, h) in faces:
                if (x, y, w, h) == nearest_face:
                    cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
                    cv2.putText(frame, text, (x, y - 20), font, 0.50, (0, 0, 0), 2)
                    frame_counter += 1
                    if frame_counter == 120:
                        text = ""
                        frame_counter = 0

            cv2.imshow("Face Detection", frame)

    def run(self):
        while True:
            self.detect_face()
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        self.camera.release()
        cv2.destroyAllWindows()

if __name__ == '__main__':
    app = FaceDetectionApp()
    app.run()
