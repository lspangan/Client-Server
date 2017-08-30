/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> /* for close() */
#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */

#define MAXPENDING 5 /* Maximum outstanding connection requests */

void DieWithError(char *errorMessage); /* Error handling function */



int main()
{
     int sockfd, newsockfd, cli2sockfd, portno, clilen, client = 0, n, flag = 0;
     char buffer[256], message[256], message2[256];
     struct sockaddr_in serv_addr, cli_addr;
     //Start up server
     printf("Server started\nListening on 127.0.0.1: 5000\n");
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        DieWithError("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     //assigns IP address and port number to server
     portno = 5000;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     //binds to any incoming sockets
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              DieWithError("ERROR on binding");
     //handles first incoming client
     client = 1;
     while(client == 1){
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     if(listen(sockfd,MAXPENDING) < 0)
         DieWithError("listen() failed") ;
     else
         printf("Listening...\n");     
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          DieWithError("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) DieWithError("ERROR reading from socket");
        printf("Client 1 connected\n");
        //Client 1 is connected
        //Server and client 1 may now communicate
        // Asks for log in
        n = write(newsockfd, "Client 1 Username: ", 20);
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        printf("Log in user name is: %s\n", buffer);
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        printf("Log in Password is: %s\n", buffer);
        flag = 0;
        //server now reads many responses from client 1 and responds as follows
        while (flag == 0) {
            bzero(buffer, 256);
            n = read(newsockfd, buffer, 255);
            //Reads client's message to connect to the server
            //Client 1 has already established a connection with server
            //Do nothing
            if (strcmp(buffer,"Client is currently connected to server\n") == 0) { printf("Client is currently connected to server\n"); }
            //Reads client's message to retrieve user list
            if (strcmp(buffer, "Return user1 list!\n") == 0) { printf("Return user list!\n"); }
            //Handles the case when client 1 wants to send a message to another user
            if (strcmp(buffer, "Bob") == 0) {
                //Reads who client wants to send message to and prints username
                printf("A message to: %s\n", buffer);
                //Reads client's message and prints message
                bzero(buffer, 256);
                n = read(newsockfd, buffer, 255);
                printf("Message is: %s\n", buffer);
                //Assigns the string in buffer to message for Bob to retrieve this string later
                strcpy(message, buffer);
                //Writes to client that message was sent
                n = write(newsockfd, "\nMessage sent successfully", 26);
                flag = 0;
            }
            //Responds to client request to retrieve messages
            //Client 1 should not have any messages since it is the first client connected
            if (strcmp(buffer, "Send back2 Bob's message!\n") == 0) {
                printf("No messages found!\n");
                n = write(newsockfd, "No messages found!\n", 255);
            }
            //Handles Client 1's request to disconnect from server
            if (strcmp(buffer, "Client disconnected!\n") == 0) {
                printf("Client disconnected!\n");
                n = write(newsockfd, "-------disconnect with the server-------\n", 50);
                bzero(buffer, 256);
                n = read(newsockfd, buffer, 255);
                flag = 1;
                client = 0;
            }
            //Server responds to Client 1's request to chat with a friend
            //Cannot chat with a friend, must initiate a chat first
            if (strcmp(buffer, "Friend not connected,initiate chat first!\n") == 0) {
                printf("Friend not connected,initiate chat first!\n");
            }
            //Handles Client 1's request to log out
            //Client 1 disconnects from server
            //Server goes back to listening for any incoming sockets
            //The next incoming socket will still be Client 1 since Client 1 did not initiate a chat
            if (strcmp(buffer, "User is logged out\n") == 0) {
                printf("User is logged out\n");
                flag = 1;
                client = 1;
            }

        }
    }
    client = 2;
    flag = 0;
    //Establishes connection with Client 2 and handles Client 2 requests
    while (client == 2) {
        listen(sockfd, 5);
        clilen = sizeof (cli_addr);
        if (listen(sockfd, MAXPENDING) < 0)
            DieWithError("listen() failed");
        else
            printf("Listening...\n");
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            DieWithError("ERROR on accept");
        else
            bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        if (n < 0) DieWithError("ERROR reading from socket");
        printf("Client 2 connected\n");
        //Client 2 is now connected
        //Server and Client 2 are now ready to communicate
        // Asks for log in
        n = write(newsockfd, "Client 2 Username: ", 20);
        //Reads log in and prints log in info
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        printf("Log in user name is: %s\n", buffer);
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        printf("Log in Password is: %s\n", buffer);
        flag = 0;
        //Reads all of Client 2's requests and responds back
        while(flag == 0){
            bzero(buffer, 256);
            n = read(newsockfd, buffer, 255); 
            //Reads client's message to connect to server
            //Client 2 has already established a connection with server
            //Do nothing
            if(strcmp(buffer,"Client is currently connected\n") == 0) { printf("Client is currently connected\n"); }
            //Reads Client 2's request to retrieve user list
            if(strcmp(buffer,"Return user list!\n") == 0) { printf("Return user list!\n"); }
            //Handles Client 2's request to send a message to a user
            if(strcmp(buffer, "Alice") == 0) {
                //Reads who client wants to send message to and prints username
                printf("A message to: %s\n", buffer);
                //Reads client's message and prints message
                bzero(buffer, 256);
                n = read(newsockfd, buffer, 255);
                printf("Message is: %s\n", buffer);
                //Assigns the string in buffer to message for Bob to retrieve this string later
                strcpy(message2, buffer); 
                //Writes to client that message was sent
                n = write(newsockfd, "\nMessage sent successfully", 26); 
                flag = 0;
            }            
            //Handles Client 2's request to retrieve Client 1's message sent to Client 2
            if(strcmp(buffer,"Send back2 Bob's message!\n") == 0) {
                printf("Send back Bob's message!\n");
                //Writes Bob's message
                n = write(newsockfd,message,255);
            }
            //Handles Client 2's request to initiate a chat
            //Client 2 cannot initiate a chat because Client 1 has already done so
            if(strcmp(buffer,"Alice has already initiated a chat with you.\n") == 0) { printf("Alice has already initiated a chat with you!\n"); }
            //Handles Client 2's request to disconnect from server
            if(strcmp(buffer,"Client disconnected!\n") == 0) {
                printf("Client disconnected!\n");
                n = write(newsockfd,"-------disconnect with the server-------\n", 50);
                bzero(buffer, 256);
                n = read(newsockfd, buffer, 255);
                flag = 1;
                client = 0;
            }  
            //Handles Client 2's request to log out
            //Client 2 disconnects from server
            //Server goes back to listening for any incoming sockets
            //The next incoming socket will still be Client 2 since Client 2 chat with Client 1           
            if(strcmp(buffer,"User is logged out\n") == 0) { printf("User is logged out\n"); flag = 1; client = 2; }
        }
     }
     
     return 0; 
}