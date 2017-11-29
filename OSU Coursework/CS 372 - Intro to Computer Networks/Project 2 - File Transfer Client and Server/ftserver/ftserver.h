/* 
 * File:   ftserver.h
 * Author: Corwin Perren
 *
 * Created on March 11, 2017, 3:11 PM
 * 
 * This file contains all the header information for ftserver.c
 */

#ifndef FTSERVER_H
#define FTSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>

#define LOCAL_IP 127.0.0.1
#define PORT_MIN 1024
#define PORT_MAX 65535
        
int get_listen_sfd(void);
void setup_server_address_struct(struct sockaddr_in *server_address, int port);
void bind_listen_socket(int listen_sfd, struct sockaddr_in *server_address);

void get_client_control_sfd_and_info(int listen_sfd, int *control_sfd, struct sockaddr_in *client_address);
int get_client_data_sfd_and_info(int port, struct sockaddr_in *control_address, struct sockaddr_in *data_address);

void send_file_listing(int control_sfd, int data_sfd);
void send_file_if_available(char *filename, int control_sfd, int data_sfd);
void send_unknown_command_received(int sfd);

void wait_for_ok(int sfd);
int should_transfer(int sfd);

#endif /* FTSERVER_H */

