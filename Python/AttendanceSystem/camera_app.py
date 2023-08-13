import sys
import cv2
from PyQt5.QtWidgets import QApplication, QMainWindow, QGraphicsView, QGraphicsScene, QGraphicsPixmapItem, QLabel, QMessageBox
from PyQt5.QtGui import QImage, QPixmap, QPainter
from PyQt5.QtCore import Qt, QTimer
import socket
import numpy as np
import base64
import select  

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

def encode_frame(frame):
    # Encode frame to jpg and convert to base64 string
    encoded, buffer = cv2.imencode('.jpg', frame)
    jpg_as_text = base64.b64encode(buffer)
    return jpg_as_text

class FaceDetectionApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Face Detection")
        self.setGeometry(100, 100, 800, 600)

        self.scene = QGraphicsScene(self)
        self.view = QGraphicsView(self.scene)
        self.view.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.view.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.view.setRenderHint(QPainter.Antialiasing)
        self.setCentralWidget(self.view)

        self.camera = cv2.VideoCapture(0)  # Use camera index 0 (default camera)

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.detect_face)
        self.timer.start(33)  # 30 FPS

    def detect_face(self):
        global text, frame_counter
        ret, frame = self.camera.read()
        
        # creating label
        self.label = QLabel(self)
        # loading image
        self.pixmap = QPixmap('icons/face/default.png')
        # adding image to label
        self.label.setPixmap(self.pixmap)
        # Optional, resize label to image size
        self.label.resize(self.pixmap.width(),
                          self.pixmap.height())
        
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
                    text = "Welcome " + self.split_input[1];
                    print(text)   
                if self.split_input[0] == "L":
                    text = "Have a good day " + self.split_input[1];
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
                    cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
                    cv2.putText(frame, text, (x, y-20), font, 0.50, (0, 0, 0), 2)
                    frame_counter += 1
                    if frame_counter == 120:
                        text = ""
                        frame_counter = 0

            rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            height, width, channel = rgb_frame.shape
            bytes_per_line = 3 * width
            q_image = QImage(rgb_frame.data, width, height, bytes_per_line, QImage.Format_RGB888)
            pixmap = QPixmap.fromImage(q_image)

            self.scene.clear()
            pixmap_item = QGraphicsPixmapItem(pixmap)
            self.scene.addItem(pixmap_item)
            self.view.setSceneRect(pixmap_item.boundingRect())
            self.view.fitInView(pixmap_item.boundingRect(), Qt.KeepAspectRatio)

    def closeEvent(self, event):
        self.camera.release()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = FaceDetectionApp()
    window.show()
    sys.exit(app.exec_())
