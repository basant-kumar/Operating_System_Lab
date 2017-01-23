
 ********************************************************** OS Lab Assignment#2 ************************************************************************
1)Question-->>>>>>>

Suppose you have 100 text files stored in the local drive and you want parallel access to these files by multiple clients. Each client can send a request to the server for transferring one specific file (identified by its name, assume clients know the file names in advance). Upon receiving such a request from a client, the server will fork() a child process to handle the file transfer smoothly with that specific client. Statistics about each transfer session (e.g. number of send(), total bytes transferred, client ID, file name, time taken to transfer the whole file etc.) should be stored in the logfile.txt in the server). Use socket programming related systems calls to implement the client server file transfer mentioned above. Make sure each send() is followed by a sufficient amount of delay such that you can connect multiple clients during the execution to check whether fork() is working properly or not. The simple server socket program can only handle one client at a time, because accept() and read() system calls are blocking. For example, when the server executes read() on the socket of one client, the program blocks until data arrives from that client. Any new clients that try to connect to the server will be unable to do so. Therefore, a good way to build servers capable of handling multiple clients at a time is to spawn a new process (or thread, but we will go with processes for now) for every new client connection at the server. The new process will service the single client it was created for, blocking for the client reads, and terminate when it has finished serving the client. Meanwhile, the original server process will continue to wait for and accept new connections.


Solution ---->>>>>

i)Solution have two source code files and one data folder with contains all for server which will be send to client.

ii)Data folder contains  seven files, and every file contains some data. if someone wants to test on more files than create more files in data folder and pass the name of file from client to get file.

iii)To test programms first run ftp_server.c program where your ip address for socket will set automatically of your machine ip address and port number will be 8888 which can be change from the code where port number define as a macro.
e.g. user:$ gcc ftp_server.c -o server
     user:$ ./server

iv)To run client program you have pass the two argument in terminal.first is ip address of server and seconds is file name than you want to get.
   This program will request server to get file and will store same file as same name in client machine. 
e.g. user:$ gcc ftp_client.c -o client
     user:$ ./client 192.168.1.25 file.txt

v) ALL details of file tranfers will be store in logfile.txt file.
