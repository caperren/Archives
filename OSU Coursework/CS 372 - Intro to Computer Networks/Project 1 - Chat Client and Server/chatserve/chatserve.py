#####################################
# Imports
#####################################
import time
import socket
import sys
import atexit

#####################################
# Global Variables
#####################################
LOCAL_IP = "127.0.0.1"

PORT_MIN = 1024
PORT_MAX = 65535

QUIT_STRING = "\quit"


#####################################
# Chat Server Class Definition
#####################################
class ChatServer:
    def __init__(self, port_number, server_handle):
        super(ChatServer, self).__init__()

        # ########## Thread Flags ##########
        self.run_flag = True
        self.setup_server_flag = True
        self.wait_for_client_flag = True
        self.chat_with_client_flag = True

        # ########## Class Variables ##########
        self.port_number = port_number
        self.handle = server_handle

        self.socket = None

        self.client_address = None
        self.client_port = None
        self.client_connection = None  # type: socket.socket

        # ########## Handle the socket closing so we don't keep it open ##########
        atexit.register(self.on_program_should_exit)

        # ########## Start server ##########
        self.run()

    def run(self):
        while self.run_flag:
            if self.setup_server_flag:
                self.setup_server()
                self.setup_server_flag = False
            elif self.wait_for_client_flag:
                self.wait_for_client()
                self.wait_for_client_flag = False
            elif self.chat_with_client_flag:
                self.chat_with_client()

            time.sleep(0.1)

    def setup_server(self):
        print("########################################")
        print("       Chat Server - Version 1.0        ")
        print("       Written by Corwin Perren         ")
        print("########################################")
        print("Attempting to bind socket at IP \"" + LOCAL_IP + "\" on port " + str(self.port_number) + ".")

        # Try to setup a socket and bind to it. If the bind it successful, begin listening on the socket.
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.bind((LOCAL_IP, self.port_number))
            self.socket.listen(1)
            print("Bind successful!")
            print("########################################")
        except:
            print("ERROR: Failed to bind socket. Please try a different port. Exiting...")
            self.run_flag = False

    def wait_for_client(self):
        print("\n########################################")
        print("Waiting for client connection.")

        # Wait for a client to connect, and accept the connection when they do
        self.client_connection, (self.client_address, self.client_port) = self.socket.accept()

        print("Connected to client at IP \"" + self.client_address + "\" on port " + str(self.client_port) + ".")
        print("Ready to chat. Client sends first!")
        print("########################################")

    def chat_with_client(self):
        # Decode the byte data back into a python string. Allow up to 1024 bytes for the message.
        received_data = self.client_connection.recv(1024).decode("utf-8")  # type: str

        # If the data is not empty
        if received_data:
            # Get the data after the handle printout
            first_word_block_after_handle = received_data.split("> ")[1]

            # If that message is the quit string, close the connection and go back to waiting for a new client
            if first_word_block_after_handle == QUIT_STRING:
                self.client_connection.close()
                self.wait_for_client_flag = True
                print("Client closed connection!")
            else:
                # Otherwise, print the received data
                print(received_data)

                # Then print your own handle, and read in a message to send to the client
                handle_prefix = self.handle + "> "
                print(handle_prefix, end="")
                keyboard_input = input()

                # Send the message to the client
                self.client_connection.send(bytes(handle_prefix + keyboard_input, "utf-8"))

                # Check the message sent, and if it's the quit message, disconnect from the client and wait for a new one
                if keyboard_input == QUIT_STRING:
                    self.client_connection.close()
                    self.wait_for_client_flag = True
                    print("Closed connection with client!")

    def on_program_should_exit(self):
        # This attempts to cleanly close the connection on ctrl-c
        try:
            self.client_connection.close()
            print("Closed connection for Ctrl-C exit...")
        except:
            pass

if __name__ == "__main__":
    # Get the number of command line arguments
    num_args = len(sys.argv)

    # If we at least have the minimum, process the arguments
    if num_args > 3:
        port_num = None
        handle = ""

        # This for loop gets the port number and handle passed in via command line
        for arg_index, _ in enumerate(sys.argv):
            current_arg = sys.argv[arg_index]
            if current_arg == "-port":
                port_num = int(sys.argv[arg_index + 1])

            elif current_arg == "-handle":
                handle = sys.argv[arg_index + 1]

        # Now we do a quick check to make sure the port is valid
        if (port_num < PORT_MIN) or (port_num > PORT_MAX):
            print("Invalid port number. Please try again with a valid port! Exiting...")
            exit()

        # We also check to make sure the handle is valid
        if handle == "":
            print("No handle given. Please try again with a valid handle! Exiting...")
            exit()

        # Now we make an instance of the chat server with the correct arguments
        ChatServer(port_number=port_num, server_handle=handle)

    else:
        print("Not enough arguments. Exiting...")
        print("usage: python3 chatserv.py -port [port_number] -handle \"[handle]\"")