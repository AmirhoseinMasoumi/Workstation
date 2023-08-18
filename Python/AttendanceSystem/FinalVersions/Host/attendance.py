import face_recognition
import cv2
import os
import pickle
import csv
from datetime import datetime, timedelta
from face_recognition.api import face_encodings, face_locations
import socket
import base64
import numpy as np

# Create socket 
receiver_addr = ('0.0.0.0', 5001)
sender_addr = ('192.168.1.6', 5000)  

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(receiver_addr)

def decode_frame(encoded_frame):
    # Decode base64 string to jpg and convert to OpenCV frame
    jpg_buffer = base64.b64decode(encoded_frame) 
    frame = cv2.imdecode(np.frombuffer(jpg_buffer, dtype='uint8'), 1)
    return frame 

Encodings = []
Names = []
font = cv2.FONT_HERSHEY_SIMPLEX

lastName = ''

with open('Host/train.pkl','rb') as f:
    Names = pickle.load(f)
    Encodings = pickle.load(f)

# Create the present_employees list
present_employees = []

# Initialize the employ data dictionary
employ_data = {}

while True:
    # Receive frame
    encoded_frame, _ = sock.recvfrom(65535)
    # Decode and show frame
    frame = decode_frame(encoded_frame)
    
    frameSmall = cv2.resize(frame,(0,0),fx=0.25,fy=0.25)
    frameRGB = cv2.cvtColor(frameSmall,cv2.COLOR_BGR2RGB)

    facePositions = face_recognition.face_locations(frameRGB,model='cnn')
    allEncodings = face_recognition.face_encodings(frameRGB,facePositions)
   
    # Create the attendance file 
    fileName = 'Host/attendance.csv'
    newFile = False
    if os.path.isfile(fileName) == False:
            newFile = True

    with open(fileName, 'a', newline='') as attendance_file:
        csv_writer = csv.writer(attendance_file)
        if newFile == True:
            csv_writer.writerow(['Name', 'Date', 'Entry Time', 'Leaving Time']) 

        for (top,right,bottom,left),face_encoding in zip(facePositions,allEncodings):
            name = 'Unknown Person'
            matches = face_recognition.compare_faces(Encodings,face_encoding)
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
                            csv_writer.writerow([name, employ['Entry Date'],  employ['Entry Time'], employ['Leaving Time']]) 
                            lastName = name
                            text = "L_" + name
                            # cv2.putText(frame,text,(10,30),font,0.75,(0,255,255),2)
                            # Send UDP
                            ack_msg = text.encode()
                            sock.sendto(ack_msg, sender_addr)
                        else:
                            text = "E_" + name
                            # cv2.putText(frame,text,(10,30),font,0.75,(0,255,255),2)
                            # Send UDP
                            ack_msg = text.encode()
                            sock.sendto(ack_msg, sender_addr)
                    else:
                        if (datetime.strptime(current_time, '%H:%M:%S') - datetime.strptime(employ['Leaving Time'], '%H:%M:%S')) > timedelta(seconds=10):
                            lastName = ''
                            employ = employ_data[name]
                            employ['Entry Date'] = last_entry_date
                            employ['Entry Time'] = current_time
                            print("here")
                else:
                    # Add a new entry for the employ
                    employ_data[name] = {
                        'Entry Date': current_date,
                        'Entry Time': current_time,
                        'Leaving Time': ''
                    }

            top = top*4
            right = right*4
            bottom = bottom*4
            left = left*4

