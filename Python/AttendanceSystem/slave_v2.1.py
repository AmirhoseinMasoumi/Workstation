import face_recognition
import cv2
import os
import pickle
import csv
from datetime import datetime, timedelta
import threading

Encodings = []
Names = []
font = cv2.FONT_HERSHEY_SIMPLEX

lastName = ''
text = ''  # Global variable to store the text for display

with open('train.pkl', 'rb') as f:
    Names = pickle.load(f)
    Encodings = pickle.load(f)

# Create the present_employees list
present_employees = []

# Initialize the employ data dictionary
employ_data = {}

# Function to update the attendance text and handle attendance logic
def update_attendance(frameRGB):
    global text, lastName

    facePositions = face_recognition.face_locations(frameRGB, model='hog')
    allEncodings = face_recognition.face_encodings(frameRGB, facePositions)

    for (top, right, bottom, left), face_encoding in zip(facePositions, allEncodings):
        name = 'Unknown Person'
        matches = face_recognition.compare_faces(Encodings, face_encoding)
        if True in matches:
            first_match_index = matches.index(True)
            name = Names[first_match_index]

            # Get the current date and time
            current_date = datetime.now().strftime('%Y-%m-%d')
            current_time = datetime.now().strftime('%H:%M:%S')

            # Check if the employ is already in the dictionary
            if name in employ_data:
                employ = employ_data[name]
                last_entry_date = employ['Entry Date']
                last_entry_time = employ['Entry Time']

                # Check if it's a new day or the last entry was more than 10 seconds ago
                if lastName != name:
                    if last_entry_date != current_date or (datetime.strptime(current_time, '%H:%M:%S') - datetime.strptime(last_entry_time, '%H:%M:%S')) > timedelta(seconds=10):
                        # Update the entry time for the current entry
                        employ['Entry Date'] = last_entry_date
                        employ['Entry Time'] = last_entry_time
                        employ['Leaving Time'] = current_time
                        last_leaving_time = employ['Leaving Time']

                        # Write the employ entry to the csv file
                        with open('attendance.csv', 'a', newline='') as attendance_file:
                            csv_writer = csv.writer(attendance_file)
                            csv_writer.writerow([name, employ['Entry Date'],  employ['Entry Time'], employ['Leaving Time']])
                        lastName = name
                        text = "Have a good day " + name
                        print(text)
                    else:
                        text = "Welcome " + name
                        print(text)
                else:
                    if (datetime.strptime(current_time, '%H:%M:%S') - datetime.strptime(employ['Leaving Time'], '%H:%M:%S')) > timedelta(seconds=10):
                        lastName = ''
                        employ = employ_data[name]
                        employ['Entry Date'] = last_entry_date
                        employ['Entry Time'] = current_time
            else:
                # Add a new entry for the employ
                employ_data[name] = {
                    'Entry Date': current_date,
                    'Entry Time': current_time,
                    'Leaving Time': ''
                }

# Function to read frames from the camera and display them
def display_camera_frames():
    global text

    # Load the pre-trained Haar Cascade classifier for face detection
    face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

    cam = cv2.VideoCapture(0)

    while True:
        _, frame = cam.read()

        frameSmall = cv2.resize(frame, (0, 0), fx=0.25, fy=0.25)
        frameRGB = cv2.cvtColor(frameSmall, cv2.COLOR_BGR2RGB)

        update_attendance(frameRGB)

        # Display the attendance text on the frame
        cv2.putText(frame, text, (10, 30), font, 0.75, (0, 255, 255), 2)

        # Detect faces using the Haar Cascade classifier
        gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray_frame, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

        # Draw rectangles around the detected faces
        for (x, y, w, h) in faces:
            x *= 4
            y *= 4
            w *= 4
            h *= 4
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        cv2.imshow('Camera', frame)
        cv2.moveWindow('Camera', 0, 0)

        if cv2.waitKey(1) == ord('q'):
            break

    cam.release()
    cv2.destroyAllWindows()

# Create and start the attendance system thread
attendance_thread = threading.Thread(target=display_camera_frames)
attendance_thread.daemon = True  # Set the thread as daemon so it terminates when the main thread terminates
attendance_thread.start()

# The main thread simply displays the camera frames along with the text from the attendance system thread
# It doesn't wait for the attendance system thread to finish processing
while True:
    pass  # Do nothing, let the attendance system thread handle everything
