import socket

def receive_model(save_path, host, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        client_socket.connect((host, port))

        with open(save_path, 'wb') as model_file:
            print("Receiving model...")
            while True:
                data = client_socket.recv(1024)
                if not data:
                    break
                model_file.write(data)

        print("Model received successfully!")

if __name__ == "__main__":
    save_model_path = "train2.pkl"  # Change this to your desired save path
    host = '127.0.0.1'  # Replace with your server's IP address
    port = 12345  # Choose the same port number used by the server

    receive_model(save_model_path, host, port)
