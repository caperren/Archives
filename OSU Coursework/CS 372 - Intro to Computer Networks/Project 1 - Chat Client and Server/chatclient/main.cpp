/////////////////////////////////////
// Imports
/////////////////////////////////////
#include <cstdlib>
#include <iostream>
#include <string>

#include "ChatClient.h"

/////////////////////////////////////
// Namespace Declarations
/////////////////////////////////////
using namespace std;

/////////////////////////////////////
// Global Variables
/////////////////////////////////////
#define PORT_MIN 1024
#define PORT_MAX 65535

#define UN_INIT "NOT INITIALIZED"

/////////////////////////////////////
// Main Definition
/////////////////////////////////////
int main(int argc, char** argv) {
    // If there aren't enough arguments, quit and tell the user
    if(argc < 5){
        cout << "Not enough arguments. Exiting..." << endl;
        cout << "usage: chatclient -port [port_number] -hostname [host_name]" << endl;
        return 0;
    }
    
    
    int port = -1;
    string host(UN_INIT);
    
    // Go through the arguments and pull out the port and hostname
    for(int i = 0 ; i < argc ; i++){
        char *argument = argv[i];
        if(string("-port").compare(argument) == 0){
            port = atoi(argv[i+1]);
        }else if(string("-hostname").compare(argument) == 0){
            host = string(argv[i+1]);
        }
    }
    
    // Do a quick check to make sure the port is valid
    if((port < PORT_MIN) || (port > PORT_MAX)){
        cout << "Incorrect port value entered. Exiting..." << endl;
        return 0;
    }
    
    // Do the same with the host
    if(host.compare(UN_INIT) == 0){
        cout << "No hostname entered. Exiting..." << endl;
        return 0;
    }   
    
    // Make and instance of the chat client and run its main processing loop
    ChatClient client = ChatClient(port, host);
    client.run();
    
    return 0;
}





