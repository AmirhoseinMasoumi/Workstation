import subprocess
import socket
import os
import sys

def receive_model(save_path, host, port):
    while True:
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
                server_socket.bind((host, port))
                server_socket.listen()

                print("Waiting for a connection...")
                client_socket, client_address = server_socket.accept()
                print("Connected to:", client_address)

                with open(save_path, 'wb') as model_file:
                    print("Receiving model...")
                    while True:
                        data = client_socket.recv(1024)
                        if not data:
                            break
                        model_file.write(data)

                print("Model received successfully!")

        except Exception as e:
            print("Error:", e)

if __name__ == "__main__":
    save_model_path = "train2.pkl"
    host = '127.0.0.1'
    port = 12345

    # Paths to the two Python scripts
    script1_path = "attendance.py"
    script2_path = "camera_app.py"

    # Execute the two scripts using subprocess
    subprocess.Popen(["python", script1_path])
    subprocess.Popen(["python", script2_path])

    receive_model(save_model_path, host, port)

    # Restart the current script
    python_executable = sys.executable
    subprocess.Popen([python_executable, __file__])
