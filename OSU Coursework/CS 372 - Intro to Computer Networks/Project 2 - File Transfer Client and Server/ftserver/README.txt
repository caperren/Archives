#########         Project 2        #########
#########          CS 372          #########
######### Written by Corwin Perren #########

##### Extra Credit #####
This implementation allows for the transfer of ANY file, not just text files. 
It also should have no problems with both large and small files of both kinds.
I personally tested it with the ftserver binary (26.3kB) and a 2MB text file.


##### General Notes #####
To the best of my knowledge, this meets all requirements for the assignment.
As would be expected, the server must be started and running before the client.
While running, the server will print out a startup title, minimal debug information,
and nothing else. 
The client will print out a title, some debug information, and all required info
from the assignment guidelines. 
Make sure that the control ports match between the server and client.

################################################################################

##### To compile the server #####
In the same directory as Makefile, main.c, ftserver.c, and ftserver.h, run the following:

make clean all

This should result in an output binary in the same directory called "ftserver".

##### To run the server #####
In the same directory as the newly compiled "ftserver" binary, run the following.

./ftserver [control_port]

an example could be:

./ftserver 33121

##### To run the client #####
In the same directory as the file chatclient.py, run the following:

python3 ftclient.py [control_port] [data_port] [command] [filename] *filename optional*

## Example directory listing

python3 ftclient.py 33121 33122 -l

## Example file transfer

python3 ftclient.py 33121 33122 -g file.txt