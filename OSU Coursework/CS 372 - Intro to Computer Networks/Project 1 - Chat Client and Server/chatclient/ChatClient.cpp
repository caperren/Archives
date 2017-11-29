#include "ChatClient.h"

ChatClient::ChatClient(int port_number, string hostname) {
    // Set the port number and hostname based on the constructor inputs
    this->__port_number = port_number;
    this->__host_name = hostname;
    
    // Set flags to default values
    this->__run_flag = true;
    this->__set_user_handle_flag = true;
    this->__setup_client_connection_flag = true;
    this->__chat_with_server_flag = true;
    
    // Print out the welcome screen
    cout << "########################################" << endl;
    cout << "       Chat Client - Version 1.0        " << endl;
    cout << "       Written by Corwin Perren         " << endl;
    cout << "########################################" << endl;
}

void ChatClient::set_handle(){
    // Ask the user for their handle
    string handle;
    cout << "Please enter your desired handle (10 characters max): ";
    getline(cin, handle);
    
    // Pull out a 10 character substring to make sure we don't go over the limit
    handle = handle.substr(0, 10);
    
    // Set the user's handle and printout
    this->__user_handle = handle;
    cout << "Handle set to: " << handle << "." << endl;
    cout << "########################################" << endl;
}

void ChatClient::setup_connection(){
    // Print out who we're connecting to
    cout << "\n########################################" << endl;
    cout << "Attempting to connect to \"" << this->__host_name << "\" on port " << this->__port_number << "." << endl;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    // Create a new Internet TCP socket file descriptor
    this->__inet_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    // Make sure it was made successfully
    if(this->__inet_file_descriptor < 0 ){
       cout << "ERROR: Error opening socket. Exiting..." << endl;
       this->__run_flag = false;
       return;
    }

    // Translate the input host name to usable struct by the library
    server = gethostbyname(this->__host_name.c_str());

    // Verify that this worked. Will fail if the host doesn't exist
    if(server == NULL){
       cout << "Host does not exist. Exiting..." << endl;
       this->__run_flag = false;
       return;
    }

    // Empty the sockaddr struct
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    // Set the connection type
    serv_addr.sin_family = AF_INET;
    
    // Copy over the connection information from gethostname above
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    
    // Set the port
    serv_addr.sin_port = htons(this->__port_number);

    // Attempt to open the file descriptor with this connection information
    int connection_result = connect(this->__inet_file_descriptor, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // Let the user know if this failed
    if(connection_result < 0){
       cout << "ERROR: Error connecting to server. Please check host and port. Exiting..." << endl;
       this->__run_flag = false;
       return;
    }
    
    // We made it here, so success! Connected!
    cout << "Connection to server successful!" << endl;
    cout << "Ready to chat. You send first!" << endl;
    cout << "########################################" << endl;
}

void ChatClient::chat_with_server(){
    int result;
 
    // Reset variables and print out user's handle for message prompt
    string handle_message(this->__user_handle + "> ");
    string user_message;
    cout << handle_message;
    
    // Read in the user's message
    getline(cin, user_message);
    
    // Make a complete version of the message to send that has their handle
    string full_message(handle_message + user_message);
    
    // Write this data to the server
    result = write(this->__inet_file_descriptor, full_message.c_str(), strlen(full_message.c_str()));
    
    // Let the user know if sending failed
    if(result < 0){
       cout << "ERROR: Error sending message. Connection may have been lost. Exiting..." << endl;
       this->__run_flag = false;
       return;
    }
    
    // If the user sent the quit message, close the socket and exit the program
    if(user_message.find("\\quit") != string::npos){
        close(this->__inet_file_descriptor);
        this->__run_flag = false;
        cout << "Closed connection with server!" << endl;
        return;
    }
    
    // Empty a receive buffer
    char buffer[1025];
    bzero(buffer, 1025);
    
    // Read in a message from the server
    result = read(this->__inet_file_descriptor, buffer, 1024);
    
    // If this failed, let the user know
    if(result < 0){
       cout << "ERROR: Error sending message. Connection may have been lost. Exiting..." << endl;
       this->__run_flag = false;
       return;
    }
    
    // Convert the received byte array to a string
    string received_message(buffer);
    
    // Check if the message from the server included the quit message. If so
    // close the socket and exit the program
    if(received_message.find("\\quit") != string::npos){
        close(this->__inet_file_descriptor);
        this->__run_flag = false;
        cout << "Server closed connection!" << endl;
        return;
    }
    
    // If this was a normal message, print it to the screen
    cout << received_message << endl;
}

void ChatClient::run(){
    
    // This is the main processing loop for the chat client. It starts by
    // getting the user's handle, then attempts to open a connection to the 
    // server. If these are successful, then the user can chat with the server
    // until either the server of client quits.
    while(this->__run_flag){
        if(this->__set_user_handle_flag){
            this->set_handle();
            this->__set_user_handle_flag = false;
        }else if(this->__setup_client_connection_flag){
            this->setup_connection();
            this->__setup_client_connection_flag = false;
        }else if(this->__chat_with_server_flag){
            this->chat_with_server();
        }
    }
    
}
