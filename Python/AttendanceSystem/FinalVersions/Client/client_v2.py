import cv2
import base64
import select
import socket

# Create socket
sender_addr = ('192.168.1.6', 5000)
receiver_addr = ('192.168.1.5', 5001)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(sender_addr)

# Set socket to non-blocking mode
sock.setblocking(0)

font = cv2.FONT_HERSHEY_SIMPLEX
text = ""
frame_counter = 0

def gstreamer_pipeline(
    sensor_id=0,
    capture_width=1920,
    capture_height=1080,
    display_width=960,
    display_height=540,
    framerate=30,
    flip_method=0,
):
    return (
        "nvarguscamerasrc sensor-id=%d ! "
        "video/x-raw(memory:NVMM), width=(int)%d, height=(int)%d, framerate=(fraction)%d/1 ! "
        "nvvidconv flip-method=%d ! "
        "video/x-raw, width=(int)%d, height=(int)%d, format=(string)BGRx ! "
        "videoconvert ! "
        "video/x-raw, format=(string)BGR ! appsink"
        % (
            sensor_id,
            capture_width,
            capture_height,
            framerate,
            flip_method,
            display_width,
            display_height,
        )
    )

def encode_frame(frame):
    # Encode frame to jpg and convert to base64 string
    encoded, buffer = cv2.imencode('.jpg', frame)
    jpg_as_text = base64.b64encode(buffer)
    return jpg_as_text

def detect_and_draw_face(frame, face_cascade):
    global text, frame_counter

    resized_frame = cv2.resize(frame, (0, 0), fx=0.2, fy=0.2)
    gray = cv2.cvtColor(resized_frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    for (x, y, w, h) in faces:
        x = (int)(x * 5)
        y = (int)(y * 5)
        w = (int)(w * 5)
        h = (int)(h * 5)
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        
    return frame

def show_camera():
    global text, frame_counter

    window_title = "CSI Camera"
    face_cascade_path = "haarcascade_frontalface_default.xml"  # Replace with the actual path
    face_cascade = cv2.CascadeClassifier(face_cascade_path)

    print(gstreamer_pipeline(flip_method=0))
    video_capture = cv2.VideoCapture(gstreamer_pipeline(flip_method=0), cv2.CAP_GSTREAMER)
    if video_capture.isOpened():
        try:
            while True:
                ret_val, frame = video_capture.read()
                frame = cv2.flip(frame, 1)

                if ret_val:
                    # Encode and send UDP
                    small_frame = cv2.resize(frame, (320, 240))
                    encoded_frame = encode_frame(small_frame)
                    sock.sendto(encoded_frame, receiver_addr)

                    # Check for incoming data on the socket
                    ready_to_read, _, _ = select.select([sock], [], [], 0)  # Timeout is set to 0 (non-blocking)

                    if ready_to_read:
                        ack, _ = sock.recvfrom(1024)
                        stringdata = ack.decode('utf-8')
                        split_input = stringdata.split('_')
                        if split_input[0] == "E":
                            text = "Welcome " + split_input[1]
                            print(text)
                        if split_input[0] == "L":
                            text = "Have a good day " + split_input[1]
                            print(text)

                    # Resize and process the frame
                    processed_frame = detect_and_draw_face(frame, face_cascade)
                    cv2.putText(processed_frame, text, (20, 20), font, 0.5, (255, 255, 255), 1)  # Adjust text placement
                    frame_counter += 1
                    if frame_counter == 200:
                        text = ""
                        frame_counter = 0
                    
                    processed_frame = cv2.resize(processed_frame, (0, 0), fx=2, fy=2)
                    
                    # Display the frame in fullscreen
                    cv2.namedWindow(window_title, cv2.WND_PROP_FULLSCREEN)
                    cv2.setWindowProperty(window_title, cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
                    cv2.imshow(window_title, processed_frame)

                keyCode = cv2.waitKey(10) & 0xFF
                if keyCode == 27 or keyCode == ord('q'):
                    break
        finally:
            video_capture.release()
            cv2.destroyAllWindows()
    else:
        print("Error: Unable to open camera")

if __name__ == "__main__":
    show_camera()
