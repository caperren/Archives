/* 
 * File:   ftserver.c
 * Author: Corwin Perren
 *
 * Created on March 11, 2017, 3:11 PM
 * 
 * This file contains all the functions needed for the server.
 */

#include "ftserver.h"

int get_listen_sfd(void){
    ///// Create our listening socket, check if created successfully /////
    int listen_sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sfd < 0){
        printf("Could not create listening socket! Exiting...\n");
        exit(EXIT_FAILURE);
    }
    
    ///// Return our new listening socket /////
    return listen_sfd;
}

void setup_server_address_struct(struct sockaddr_in *server_address, int port){
    ////// Setup the server struct with port and other info /////
    memset((char *)server_address, '\0', sizeof(*server_address));
    server_address->sin_port = htons(port);
    server_address->sin_addr.s_addr = INADDR_ANY;
    server_address->sin_family = AF_INET;
}

void bind_listen_socket(int listen_sfd, struct sockaddr_in *server_address){
    ///// Bind server socket /////
    int bind_result = bind(listen_sfd, (struct sockaddr *)server_address, sizeof(*server_address));
    if(bind_result < 0){
        fprintf(stderr, "Failed to bind listening port! "
                        "Please choose a different port! Exiting...\n");
        exit(EXIT_FAILURE);
    }  
}

void get_client_control_sfd_and_info(int listen_sfd, int *control_sfd, struct sockaddr_in *client_address){
    ///// This accepts a new client when a request comes in, and sets the control socket file descriptor /////
    socklen_t client_length = sizeof(*client_address);
    *control_sfd = accept(listen_sfd, (struct sockaddr *) client_address, &client_length);
}

int get_client_data_sfd_and_info(int port, struct sockaddr_in *control_address, struct sockaddr_in *data_address){
    ///// Create a new socket for data /////
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0){
        printf("Could not create data socket! Exiting...\n");
        fflush(stdout);
        return -1;
    }
    
    ///// Set up the server parameters, specifically the port send in /////
    memset((char *)data_address, '\0', sizeof(*data_address));
    data_address->sin_port = htons(port);
    memcpy(&data_address->sin_addr, &control_address->sin_addr, sizeof(control_address->sin_addr));
    data_address->sin_family = AF_INET;
    
    ///// Make the connection and verify it didn't fail. /////
    int connect_result = connect(sfd, (struct sockaddr *)data_address, sizeof(*data_address));
    if(connect_result < 0){
        fprintf(stderr, "Connection failed! Is the port correct? Exiting...\n");
        fflush(stdout);
        return -1;
    }
    
    ///// Return our new data transfer socket file descriptor /////
    return sfd;
}


void send_file_listing(int control_sfd, int data_sfd){
    ///// Variables /////
    DIR *dir;
    struct dirent *directory;
    struct stat s;
    
    ///// Open the current directory /////
    dir = opendir(".");
    
    ///// Make and write a header to the client /////
    char header[] = "########################################\n"
                    "            Directory Listing           \n"
                    "########################################\n";
    write(data_sfd, header, strlen(header));
    wait_for_ok(data_sfd);
    
    ///// Get ready to traverse directory /////
    int end_reached = 0;
    while(!end_reached){
        ///// Get the next item in the directory /////
        directory = readdir(dir);

        ///// If it's NULL, we've reached the end, break out /////
        if(directory == NULL){
            end_reached = 1;
            continue;
        }
        
        ///// Get and send only files, no folders /////
        ///// Core of getting only files was taken from the link below, modified  ////
        ///// to work on the flip servers with S_ISREG /////
        ///// https://cboard.cprogramming.com/linux-programming/78236-list-files-directory-without-folders.html /////
        
        ///// Get information about the current item in the directory /////
        stat(directory->d_name, &s);
        
        ///// If it's a regular file, write it to the data socket /////
        if(S_ISREG(s.st_mode)){
            write(data_sfd, directory->d_name, strlen(directory->d_name));
            write(data_sfd, "\n", 2);
            wait_for_ok(data_sfd);
        }
    }
    ///// Send a final OK to let the client know we're done /////
    write(data_sfd, "OK", 3);
    wait_for_ok(data_sfd);
    
}

void send_file_if_available(char *filename, int control_sfd, int data_sfd){
    ///// Variables /////
    char buffer[1024];
    memset(buffer, '\0', 1024);
    long int file_size = 0;
    FILE *file_pointer;
    
    ///// Send the header /////
    char header[] = "########################################\n"
                    "              File Transfer             \n"
                    "########################################\n";
    write(data_sfd, header, strlen(header));
    wait_for_ok(data_sfd);
    
    ///// Open the file /////
    file_pointer = fopen(filename, "rb");
    
    if(file_pointer != NULL){
        ///// Seek to the end and get the file size, then rewind to the beginning /////
        fseek(file_pointer, 0L, SEEK_END);
        file_size = ftell(file_pointer);
        rewind(file_pointer);

        ///// Send the file size to the client /////
        sprintf(buffer, "%ld", file_size);
        write(data_sfd, buffer, strlen(buffer));
        wait_for_ok(data_sfd);

        ///// If the client wants the file (duplicate name handling), start the transfer /////
        if(should_transfer(data_sfd)){
            ///// Get the nubmer of 1KB blocks, and remainder, so we can be efficient about transfer /////
            long int blocks = file_size / 1024;
            int remainder = file_size % 1024;

            ///// Read 1KB block as needed and send to client/////
            for(long int i = 0 ; i < blocks ; i++){
                fread(buffer, sizeof(buffer), 1, file_pointer); //Reads one 1024 byte block
                write(data_sfd, buffer, 1024);
                wait_for_ok(data_sfd);
            }

            ///// If the file isn't a multiple of 1KB, send the remainder /////
            if(remainder > 0){
                fread(buffer, remainder, 1, file_pointer); //Reads one remainder byte block
                write(data_sfd, buffer, remainder);
                wait_for_ok(data_sfd);
            }
            
            ///// Close the file /////
            fclose(file_pointer);
        }
        
    }else{
        ///// The requested file doesn't exist. Let the client know. /////
        write(data_sfd, "0", 2);
        wait_for_ok(data_sfd);
        
        char no_file_message[] = "File not found!\n";
        write(data_sfd, no_file_message, strlen(no_file_message));
        wait_for_ok(data_sfd);
    }
}

void send_unknown_command_received(int sfd){
    ///// Send the header, so the client knows it's invalid. /////
    char header[] = "########################################\n"
                    "            UNKNOWN COMMAND             \n"
                    "########################################\n"
                    "Please double check your command.\n";
    write(sfd, header, strlen(header));
    wait_for_ok(sfd);
}

void wait_for_ok(int sfd){
    ///// This reads from the sfd until an OK response is received /////
    char read_buffer[1024];
    memset(read_buffer, '\0', 1024);
    
    while(strcmp(read_buffer, "OK") != 0){
        memset(read_buffer, '\0', 1024);
        read(sfd, read_buffer, 1023);
    }
}

int should_transfer(int sfd){
    ///// This reads an OK or EXIT from the client to determine if a transfer should occur. /////
    ///// EXIT will happen if the file is already at the destination and the client does not want to overwrite. /////
    char read_buffer[1024];
    memset(read_buffer, '\0', 1024);
    read(sfd, read_buffer, 1023);
    
    if(strcmp(read_buffer, "OK") == 0){
        return 1;
    }else if(strcmp(read_buffer, "EXIT") == 0){
        return 0;
    }
    return -1;
}
