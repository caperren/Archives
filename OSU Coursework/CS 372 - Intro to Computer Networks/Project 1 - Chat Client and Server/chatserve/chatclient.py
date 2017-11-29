import socket
import atexit

TCP_IP = '127.0.0.1'
TCP_PORT = 33211
BUFFER_SIZE = 1024
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))


def at_exit():
    global s
    s.close()

atexit.register(at_exit)

while True:
    print("Steve> ", end="")
    my_input = input()
    s.send(bytes("Steve> " + my_input, "utf-8"))

    if my_input == "\quit":
        s.close()
        exit()

    received_data = None
    while not received_data:
        received_data = s.recv(1024).decode("utf-8")

    first_word_block_after_handle = received_data.split("> ")[1]

    if first_word_block_after_handle == "\quit":
        s.close()
        exit()

    print(received_data)
