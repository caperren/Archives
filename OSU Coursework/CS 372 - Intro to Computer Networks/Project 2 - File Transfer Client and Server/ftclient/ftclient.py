#####################################
# ftclient.py
# Author: Corwin Perren
# Created on March 11, 2017, 3:55 PM
#
# This file contains all of the code for the file transfer client application.
#####################################

#####################################
# Imports
#####################################
import time
import socket
import sys
import signal
import os.path

#####################################
# Global Variables
#####################################
PORT_MIN = 1024
PORT_MAX = 65535

USAGE_STRING = "usage: python3 chatserv.py [hostname] [control_port] [data_port] [command] or\n" + \
               "python3 chatserv.py [hostname] [control_port] [data_port] [command] [filename]"

VALID_YES_RESPONSES = ["", "Y", "y"]


#####################################
# FT Client Class Definition
#####################################
class FTClient:
    def __init__(self, hostname, control_port, command, data_port, filename=None):
        super(FTClient, self).__init__()

        # ########## Thread Flags ##########
        self.run_flag = True
        self.check_input_flag = True
        self.setup_sockets_flag = True
        self.send_request_receive_data_flag = True

        # ########## Class Variables ##########
        self.hostname = str(hostname)
        self.control_port = int(control_port)
        self.command = str(command)
        self.data_port = int(data_port)
        self.filename = str(filename)

        self.control_socket = None  # type: socket.socket
        self.data_socket = None  # type: socket.socket
        self.receive_socket = None  # type: socket.socket

        # ########## Handle ctrl-c somewhat nicely ##########
        signal.signal(signal.SIGINT, self.on_exit)

        # ########## Run the client ##########
        self.run()

    def run(self):
        # ########## Main program run loop ##########
        while self.run_flag:
            if self.check_input_flag:
                self.check_input()
                self.check_input_flag = False
            if self.setup_sockets_flag:
                self.setup_sockets()
                self.setup_sockets_flag = False
            if self.send_request_receive_data_flag:
                self.send_request_receive_data()
                self.send_request_receive_data_flag = False
                self.run_flag = False
            time.sleep(0.1)

        # ########## Final socket cleanup when program exits ##########
        self.on_exit()

    def check_input(self):
        # ########## This method checks all inputs to make sure they've reasonably valid ##########

        failure = False

        if self.hostname == "":
            print("No hostname provided! Exiting!")
            failure = True

        if self.control_port > PORT_MAX or self.control_port < PORT_MIN:
            print("Invalid control port! Exiting!")
            failure = True

        if self.data_port > PORT_MAX or self.data_port < PORT_MIN:
            print("Invalid data port! Exiting!")
            failure = True

        if failure:
            print(USAGE_STRING)
            self.run_flag = False

    def setup_sockets(self):
        # ########## Print the program title ##########
        print("########################################")
        print("     Transfer Client - Version 1.0      ")
        print("       Written by Corwin Perren         ")
        print("########################################")

        # ########## Create and connect to the sever on the control port ##########
        self.control_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.control_socket.connect((self.hostname, self.control_port))

        # ########## Setup and listen to the data socket for later ##########
        self.data_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.data_socket.bind((self.hostname, self.data_port))
        self.data_socket.listen(1)

    def send_request_receive_data(self):
        # ########## Send the requested data port ##########
        print("Sending data port: " + str(self.data_port) + "\t\t", end="")
        self.control_socket.send(bytes(str(self.data_port), "utf-8"))
        self.wait_for_ok()

        # ########## Send the command entered ##########
        print("Sending command: " + str(self.command) + "\t\t", end="")
        self.control_socket.send(bytes(str(self.command), "utf-8"))
        self.wait_for_ok()

        # ########## If the user is getting a file, also send the filename to the server ##########
        if self.command == "-g":
            print("Sending filename: " + str(self.filename) + "\t\t", end="")
            self.control_socket.send(bytes(str(self.filename[:1024]), "utf-8"))
            self.wait_for_ok()

        # ########## Based on the command, call pertinent methods ##########
        # ########## We also accept a connection from the server when needed ##########
        if self.command == "-l":
            self.receive_socket, _ = self.data_socket.accept()
            self.receive_listing()
        elif self.command == "-g":
            self.receive_socket, _ = self.data_socket.accept()
            self.receive_file()
        else:
            self.receive_error()

    def receive_listing(self):
        full_string = ""
        temp = ""

        # ########## Print out server data until an OK command is received, noting completion ##########
        while temp != "OK":
            full_string += temp
            temp = self.receive_socket.recv(1024).decode("utf-8").strip('\0')
            self.receive_socket.send(bytes("OK", "utf-8"))

        print(full_string, end="")

    def receive_file(self):
        # ########## Receive and print the header from the server ##########
        print(self.receive_socket.recv(1024).decode("utf-8").strip('\0'), end="")
        self.receive_socket.send(bytes("OK", "utf-8"))

        # ########## Receive the size of the file in bytes ##########
        num_bytes = int(self.receive_socket.recv(1024).decode("utf-8").strip('\0'))
        self.receive_socket.send(bytes("OK", "utf-8"))

        if num_bytes == 0:
            # ########## If zero bytes, the file doesn't exist. Print the error from the server. ##########
            print(self.receive_socket.recv(1024).decode("utf-8").strip('\0'), end="")
            self.receive_socket.send(bytes("OK", "utf-8"))
        else:
            # ########## File exists. Print the file size, and let the user know we're transferring. ##########
            print("File size: " + str(num_bytes) + " bytes.")
            print("Transferring file!")

            should_transfer = True

            # ########## Check if the file exists locally ##########
            if os.path.exists(self.filename):
                # ########## If it does, ask the user if we want to overwrite. ##########
                print("File \"" + self.filename + "\" already exists. Overwrite?")
                response = input("Y/n [Default Y]: ")

                # ########## If we don't, set the pertinent flag and let the user know we heard them. ##########
                if response not in VALID_YES_RESPONSES:
                    print("No selected, or invalid input. Stopping transfer.")
                    should_transfer = False

            # ########## In the case where we will perform a transfer ##########
            if should_transfer:
                # ########## Send okay to let the server know we're ready. ##########
                self.receive_socket.send(bytes("OK", "utf-8"))

                # ########## Get a file pointer to the local file ##########
                file_pointer = open(self.filename, "wb")

                # ########## Keep a running count of the number of bytes we've gotten. ##########
                bytes_received = 0

                # ########## Keep receiving until we have the correct number of bytes for the file. ##########
                while bytes_received != num_bytes:
                    # ########## Receive the data ##########
                    temp = self.receive_socket.recv(1024)

                    # ########## Add the byte count to the number received. ##########
                    bytes_received += len(temp)

                    # ########## Write the data to the file ##########
                    file_pointer.write(temp)

                    # ########## Let the server know we're ready for the next block. ##########
                    self.receive_socket.send(bytes("OK", "utf-8"))

                    # ########## Print the total number of bytes we have now. ##########
                    print("Received " + str(bytes_received) + " bytes.")

                # ########## Let the user know we're done transferring. ##########
                print("File transfer complete!")
            else:
                # ########## In the case where we don't want to overwrite a file, tell the server to exit. ##########
                self.receive_socket.send(bytes("EXIT", "utf-8"))

    def receive_error(self):
        # ########## Print the server's error when an unknown command is received. ##########
        print(self.control_socket.recv(1024).decode("utf-8").strip('\0'), end="")
        self.control_socket.send(bytes("OK", "utf-8"))

    def wait_for_ok(self):
        # ########## Wait for a response from the server, aka. a simple OK, not checking for the word OK ##########
        print(self.control_socket.recv(3).decode("utf-8"))

    def on_exit(self, signum=None, frame=None):
        self.run_flag = False

        # ########## Print a end title ##########
        print("########################################")
        print("           Program Complete             ")
        print("########################################")

        # ########## Attempt to cleanly close sockets if they exist ##########
        try:
            self.control_socket.close()
            print("Closed control socket...")
        except:
            print("No control socket open.")
            pass

        try:
            self.data_socket.close()
            print("Closed data socket...")
        except:
            print("No data socket open.")
            pass

        print("########################################")

#####################################
# Main
#####################################
if __name__ == "__main__":
    # ########## Get the number of command line arguments ##########
    num_args = len(sys.argv)

    # ########## If we don't have the right number of arguments, exit ##########
    if num_args < 5 or num_args > 6:
        print("Not enough arguments. Exiting...")
        print(USAGE_STRING)
        exit()

    # ########## Make an instance of the file transfer client with the correct arguments ##########
    if num_args == 5:
        FTClient(hostname=sys.argv[1], control_port=sys.argv[2], data_port=sys.argv[3], command=sys.argv[4])
    elif num_args == 6:
        FTClient(hostname=sys.argv[1], control_port=sys.argv[2], data_port=sys.argv[3], command=sys.argv[4], filename=sys.argv[5])

