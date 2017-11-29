#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <cstdlib>
#include <iostream>
#include <string>

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>


using namespace std;

class ChatClient {
public:
    ChatClient(int port_number, string hostname);
    
    void set_handle();
    void setup_connection();
    void chat_with_server();
    void run();
    
private:
    int __port_number;
    string __host_name;
    
    string __user_handle;
    
    int __inet_file_descriptor;
    
    bool __run_flag;
    
    bool __set_user_handle_flag;
    bool __setup_client_connection_flag;
    bool __chat_with_server_flag;
};

#endif /* CHATCLIENT_H */

