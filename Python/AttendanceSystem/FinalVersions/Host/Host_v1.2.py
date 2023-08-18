import sys
import os
import face_recognition
import pickle
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QPushButton, QVBoxLayout, QWidget, \
    QLineEdit, QDialog, QFormLayout, QMessageBox, QTableWidget, QTableWidgetItem
from PyQt5.QtGui import QImage, QPixmap, QIcon
from PyQt5.QtCore import Qt
import cv2
import numpy as np
import sqlite3


# Initialize the SQLite database
conn = sqlite3.connect('attendance.db')
c = conn.cursor()

# Create the table to store person information
c.execute('''
    CREATE TABLE IF NOT EXISTS persons (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        age INTEGER,
        gender TEXT,
        phone_number TEXT,
        email TEXT,
        photo_path TEXT NOT NULL
    )
''')
conn.commit()


class AddPersonDialog(QDialog):
    def __init__(self, table):
        super().__init__()

        self.setWindowTitle("Add New Person")
        self.setWindowIcon(QIcon("icons/add_person.png"))
        self.table = table

        # Create the input fields
        self.name_edit = QLineEdit()
        self.age_edit = QLineEdit()
        self.gender_edit = QLineEdit()
        self.phone_edit = QLineEdit()
        self.email_edit = QLineEdit()

        # Create the image capture button
        self.capture_button = QPushButton("Capture Image")
        self.capture_button.setIcon(QIcon("icons/camera.png"))
        self.capture_button.clicked.connect(self.capture_image)

        # Create the image label
        self.image_label = QLabel()
        self.image_label.setAlignment(Qt.AlignCenter)
        self.image_label.setMinimumSize(320, 240)
        self.image_label.setStyleSheet("border: 1px solid gray")

        # Create the save button
        self.save_button = QPushButton("Save")
        self.save_button.setIcon(QIcon("icons/save.png"))
        self.save_button.clicked.connect(self.save_person)

        # Create the layout and add widgets
        layout = QVBoxLayout()
        layout.addWidget(QLabel("Full Name:"))
        layout.addWidget(self.name_edit)
        layout.addWidget(QLabel("Age:"))
        layout.addWidget(self.age_edit)
        layout.addWidget(QLabel("Gender:"))
        layout.addWidget(self.gender_edit)
        layout.addWidget(QLabel("Phone Number:"))
        layout.addWidget(self.phone_edit)
        layout.addWidget(QLabel("Email:"))
        layout.addWidget(self.email_edit)
        layout.addWidget(self.capture_button)
        layout.addWidget(self.image_label)
        layout.addWidget(self.save_button)

        self.setLayout(layout)

        # Initialize variables
        self.captured_image = None

    def capture_image(self):
        # Open the default camera
        cap = cv2.VideoCapture(0)

        while True:
            ret, frame = cap.read()

            # Convert the frame to QImage
            rgb_image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            h, w, ch = rgb_image.shape
            bytes_per_line = ch * w
            qt_image = QImage(rgb_image.data, w, h, bytes_per_line, QImage.Format_RGB888)

            # Display the QImage in QLabel
            self.image_label.setPixmap(QPixmap.fromImage(qt_image))

            # Store the last captured frame
            self.captured_image = frame.copy()

            # Break the loop when 'q' is pressed
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        # Release the camera and destroy the window
        cap.release()
        cv2.destroyAllWindows()

    def save_person(self):
        name = self.name_edit.text()
        age = self.age_edit.text()
        gender = self.gender_edit.text()
        phone_number = self.phone_edit.text()
        email = self.email_edit.text()

        if self.captured_image is not None:
            # Insert the person's information into the database
            c.execute(
                'INSERT INTO persons (name, age, gender, phone_number, email, photo_path) VALUES (?, ?, ?, ?, ?, ?)',
                (name, age, gender, phone_number, email, ''))
            person_id = c.lastrowid
            conn.commit()

            # Create the images folder if it doesn't exist
            if not os.path.exists('images'):
                os.makedirs('images')

            # Update the database with the photo path
            photo_path = f'images/person_{person_id}.jpg'
            c.execute('UPDATE persons SET photo_path = ? WHERE id = ?', (photo_path, person_id))
            conn.commit()

            # Save the captured image with the person's ID as the filename in the images folder
            cv2.imwrite(photo_path, self.captured_image)

            QMessageBox.information(self, "Success", "Person added successfully!")
            self.accept()

            # Update the table widget with the added person
            self.table.insertRow(self.table.rowCount())
            self.table.setItem(self.table.rowCount() - 1, 0, QTableWidgetItem(str(person_id)))
            self.table.setItem(self.table.rowCount() - 1, 1, QTableWidgetItem(name))
            self.table.setItem(self.table.rowCount() - 1, 2, QTableWidgetItem(str(age)))
            self.table.setItem(self.table.rowCount() - 1, 3, QTableWidgetItem(gender))
            self.table.setItem(self.table.rowCount() - 1, 4, QTableWidgetItem(phone_number))
            self.table.setItem(self.table.rowCount() - 1, 5, QTableWidgetItem(email))
        else:
            QMessageBox.warning(self, "Error", "No image captured!")

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        # Initialize the GUI
        self.setWindowTitle("Attendance System")
        self.setGeometry(100, 100, 800, 600)
        self.setWindowIcon(QIcon("icons/attendance.png"))

        # Create the table widget
        self.table = QTableWidget()
        self.table.setColumnCount(6)
        self.table.setHorizontalHeaderLabels(["ID", "Name", "Age", "Gender", "Phone Number", "Email"])
        self.table.setEditTriggers(QTableWidget.NoEditTriggers)

        # Create the add person button
        self.button = QPushButton("Add Person")
        self.button.setIcon(QIcon("icons/add_person.png"))
        self.button.clicked.connect(self.open_add_person_dialog)

        # Create the train button
        self.train_button = QPushButton("Train Model")
        self.train_button.clicked.connect(self.train_model)

        # Create a layout for the buttons
        button_layout = QVBoxLayout()
        button_layout.addWidget(self.button)
        button_layout.addWidget(self.train_button)

        # Create the layout and add widgets
        layout = QVBoxLayout()
        layout.addWidget(self.table)
        layout.addWidget(self.button)
        layout.addWidget(self.train_button)

        # Create a central widget and set the layout
        central_widget = QWidget()
        central_widget.setLayout(layout)
        self.setCentralWidget(central_widget)

        # Populate the table with existing persons
        self.populate_table()

    def train_model(self):
        # Path to the folder containing the captured images
        images_folder = "images"

        Encodings = []
        Names = []

        image_dir = 'images'
        for root,dirs,files in os.walk(image_dir):
            for file in files:
                path = os.path.join(root,file)

                # Retrieve the person's name from the database
                person_id = int(file.split("_")[1].split(".")[0])
                c.execute('SELECT name FROM persons WHERE id = ?', (person_id,))
                result = c.fetchone()
                if result:
                    name = result[0]
                else:
                    name = "Unknown"

                person = face_recognition.load_image_file(path)
                encoding = face_recognition.face_encodings(person)[0]
                Encodings.append(encoding)
                Names.append(name)

        with open('train.pkl','wb') as f:
            pickle.dump(Names,f)
            pickle.dump(Encodings,f)
            QMessageBox.information(None, "Training", "Model training completed!")


    def populate_table(self):
        # Fetch the persons from the database
        c.execute('SELECT * FROM persons')
        persons = c.fetchall()

        # Clear the table
        self.table.setRowCount(0)

        # Populate the table with fetched persons
        for person in persons:
            row_position = self.table.rowCount()
            self.table.insertRow(row_position)
            for column, data in enumerate(person):
                self.table.setItem(row_position, column, QTableWidgetItem(str(data)))

        # After populating the table with data
        self.table.resizeRowsToContents()
        self.table.resizeColumnsToContents()

    def open_add_person_dialog(self):
        dialog = AddPersonDialog(self.table)
        if dialog.exec_() == QDialog.Accepted:
            pass  # Perform any necessary actions after successfully adding a person


if __name__ == '__main__':
    # Create the application and window
    app = QApplication(sys.argv)
    app.setStyleSheet(open("style.qss").read())  # Apply custom stylesheet
    window = MainWindow()
    window.show()
    window.showFullScreen()

    # Start the application event loop
    sys.exit(app.exec_())

# Close the database connection
conn.close()
