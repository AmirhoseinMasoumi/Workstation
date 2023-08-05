import socket

def send_model(file_path, host, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((host, port))
        server_socket.listen()

        print("Waiting for a connection...")
        client_socket, client_address = server_socket.accept()
        print("Connected to:", client_address)

        with open(file_path, 'rb') as model_file:
            data = model_file.read(1024)
            while data:
                client_socket.send(data)
                data = model_file.read(1024)

        print("Model sent successfully!")

if __name__ == "__main__":
    model_file_path = "train.pkl"  # Change this to your actual model file path
    host = '127.0.0.1'  # Replace with your server's IP address
    port = 12345  # Choose a port number

    send_model(model_file_path, host, port)
