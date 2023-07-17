import face_recognition
import cv2
import os
import pickle

from face_recognition.api import face_encodings, face_locations
print(cv2.__version__)

Encodings = []
Names = []
font = cv2.FONT_HERSHEY_SIMPLEX

with open('train.pkl','rb') as f:
    Names = pickle.load(f)
    Encodings = pickle.load(f)

cam = cv2.VideoCapture(0)

while True:
    _,frame = cam.read()
    frameSmall = cv2.resize(frame,(0,0),fx=0.25,fy=0.25)
    frameRGB = cv2.cvtColor(frameSmall,cv2.COLOR_BGR2RGB)
    facePositions = face_recognition.face_locations(frameRGB,model='cnn')
    allEncodings = face_recognition.face_encodings(frameRGB,facePositions)
    for (top,right,bottom,left),face_encoding in zip(facePositions,allEncodings):
        name = 'Unkown Person'
        matches = face_recognition.compare_faces(Encodings,face_encoding)
        if True in matches:
            first_match_index = matches.index(True)
            name = Names[first_match_index]
        top = top*4
        right = right*4
        bottom = bottom*4
        left = left*4
        cv2.rectangle(frame,(left,top),(right,bottom),(255,0,0),2)
        cv2.putText(frame,name,(left,top-6),font,0.75,(0,255,255),2)
    cv2.imshow('Camera',frame)
    cv2.moveWindow('Camera',0,0)
    if cv2.waitKey(1) == ord('q'):
        break
cam.release()
cv2.destroyAllWindows()


