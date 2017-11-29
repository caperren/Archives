/* 
 * File:   main.c
 * Author: Corwin Perren
 *
 * Created on March 11, 2017, 3:11 PM
 * 
 * This file contains the main function for the ftserver application.
 * 
 * ***NOTE***
 * This implementation allows for the transfer of ANY file type, including
 * binary files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "ftserver.h"


int main(int argc, char** argv) {
    ///// Variables /////
    int listen_sfd = -1; //sfd == socked_file_descriptor
    int control_sfd = -1;
    int data_sfd = -1;
    unsigned long int listen_port_number = 0;    //0 - 65535
    
    struct sockaddr_in server_address;
    struct sockaddr_in client_address_control;
    struct sockaddr_in client_address_data;
    
    ///// Verify number of command line arguments /////
    if(argc != 2){
        printf("Incorrect number of arguments. Exiting...\n");
        printf("usage: ftserver [port_number]\n");
        return 0;
    }
    
    ///// Verify port number is valid /////
    listen_port_number = atoi(argv[1]);
    if((listen_port_number > PORT_MAX) || (listen_port_number < PORT_MIN)){
        fprintf(stderr, "Port out of range! Please choose a different port! "
                        "Exiting...\n");
        exit(EXIT_FAILURE);
    }
    
    ///// Print program title /////
    printf("########################################\n");
    printf("     Transfer Server - Version 1.0      \n");
    printf("       Written by Corwin Perren         \n");
    printf("########################################\n");
   
    ///// Get, setup, and bind listening socket /////
    listen_sfd = get_listen_sfd();
    setup_server_address_struct(&server_address, listen_port_number);
    bind_listen_socket(listen_sfd, &server_address);
    
    ///// Main program loop /////
    while(1){
        ///// Variables /////
        int data_port;
        char control_read_buffer[1024];
        char command[3];
        char filename[1024];
        
        ///// Listen for new connection on socket /////
        listen(listen_sfd, 1);
        
        ///// Accept a client, get its socket file descriptor /////
        get_client_control_sfd_and_info(listen_sfd, &control_sfd, &client_address_control);
        
        ///// Verify we connected properly /////
        if(control_sfd < 0){
            fprintf(stderr, "Client accept failed. Trying next "
                            "connection...\n");
        }else{
            ///// Let the server debug know we have a connection /////
            printf("Client connected!\n");
            
            ///// Read the data port into the buffer, convert to int /////
            memset(control_read_buffer, '\0', 1024);
            read(control_sfd, control_read_buffer, 1024);
            data_port = atoi(control_read_buffer);
            printf("PORT: %d\n", data_port);
            write(control_sfd, "OK", 3);
            
            ///// Read the command into the buffer /////
            memset(control_read_buffer, '\0', 1024);
            memset(command, '\0', 3);
            read(control_sfd, control_read_buffer, 1024);
            strncpy(command, control_read_buffer, 2);
            printf("Command: %s\n", command);
            write(control_sfd, "OK", 3);
            
            ///// If this is a get request, read the filename into the buffer /////
            if(strcmp(control_read_buffer, "-g") == 0){
                memset(control_read_buffer, '\0', 1024);
                memset(filename, '\0', 1024);
                read(control_sfd, control_read_buffer, 1024);
                strncpy(filename, control_read_buffer, 1023);
                printf("Filename: %s\n\n", filename);
                write(control_sfd, "OK", 3);
            }
            
            ///// Setup the data connection to the client /////
            data_sfd = get_client_data_sfd_and_info(data_port, &client_address_control, &client_address_data);
            
            ///// Process the command accordingly /////
            if(strcmp(command, "-l") == 0){
                send_file_listing(control_sfd, data_sfd);
            }else if(strcmp(command, "-g") == 0){
                send_file_if_available(filename, control_sfd, data_sfd);
            }else{
                send_unknown_command_received(control_sfd);
            }
            
            printf("Client disconnected.\n");
            ///// Close the sockets /////
            close(control_sfd);
            close(data_sfd);
        }
    }
    
    return (EXIT_SUCCESS);
}

