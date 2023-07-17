import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QPushButton, QVBoxLayout, QWidget, \
    QLineEdit, QDialog, QFormLayout, QMessageBox
from PyQt5.QtGui import QImage, QPixmap
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
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Add New Person")

        # Create the input fields
        self.name_edit = QLineEdit()
        self.age_edit = QLineEdit()
        self.gender_edit = QLineEdit()
        self.phone_edit = QLineEdit()
        self.email_edit = QLineEdit()

        # Create the image capture button
        self.capture_button = QPushButton("Capture Image")
        self.capture_button.clicked.connect(self.capture_image)

        # Create the image label
        self.image_label = QLabel()
        self.image_label.setAlignment(Qt.AlignCenter)
        self.image_label.setMinimumSize(320, 240)

        # Create the save button
        self.save_button = QPushButton("Save")
        self.save_button.clicked.connect(self.save_person)

        # Create the layout and add widgets
        layout = QFormLayout()
        layout.addRow("Full Name:", self.name_edit)
        layout.addRow("Age:", self.age_edit)
        layout.addRow("Gender:", self.gender_edit)
        layout.addRow("Phone Number:", self.phone_edit)
        layout.addRow("Email:", self.email_edit)
        layout.addRow(self.capture_button)
        layout.addRow(self.image_label)
        layout.addRow(self.save_button)

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

            # Update the database with the photo path
            c.execute('UPDATE persons SET photo_path = ? WHERE id = ?', (f'person_{person_id}.jpg', person_id))
            conn.commit()

            # Save the captured image with the person's ID as the filename
            cv2.imwrite(f'person_{person_id}.jpg', self.captured_image)

            QMessageBox.information(self, "Success", "Person added successfully!")
            self.accept()
        else:
            QMessageBox.warning(self, "Error", "No image captured!")


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        # Initialize the GUI
        self.setWindowTitle("Attendance System")
        self.setGeometry(100, 100, 400, 200)

        # Create the widgets
        self.label = QLabel("Add New Person")
        self.button = QPushButton("Add Person")
        self.button.clicked.connect(self.open_add_person_dialog)

        # Create the layout and add widgets
        layout = QVBoxLayout()
        layout.addWidget(self.label, alignment=Qt.AlignCenter)
        layout.addWidget(self.button, alignment=Qt.AlignCenter)

        # Create a central widget and set the layout
        central_widget = QWidget()
        central_widget.setLayout(layout)
        self.setCentralWidget(central_widget)

    def open_add_person_dialog(self):
        dialog = AddPersonDialog()
        if dialog.exec_() == QDialog.Accepted:
            pass  # Perform any necessary actions after successfully adding a person


if __name__ == '__main__':
    # Create the application and window
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()

    # Start the application event loop
    sys.exit(app.exec_())

# Close the database connection
conn.close()