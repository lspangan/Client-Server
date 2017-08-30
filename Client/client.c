#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <stdlib.h>

void DieWithError(char *errorMessage); /* Error handling function */


int main() {
    int sockfd, newsockfd, portno, n, number, clilen;
    int connected = 0, is_logged_in = 0, stop = 0, chat = 0, cntserver = 0;
    int num, chatOpen = 0;
    char ipad[256];
    char ip[256] = "127.0.0.1";
    int portnum, flag = 0, cnt = 0;
    struct sockaddr_in serv_addr,cli1_addr,cli2_addr;
    struct hostent *server;
    char usern[64],usn[256], pw[64], message[256], usern2[64], pw2[64], alice[256] = "Client 1 Username: ", bob[256] = "Client 2 Username: ", bobMess[256], aliceMess[256];
    char *userNames[2];
    char *passWords[2];
    char buffer[256];
    printf("---------------------------\n");
    printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Stop program\n");
    printf("Your option<enter a number>: ");
    scanf("%d", &number);
    
    while (stop == 0) {
        //Need to initialize cntserver and flag in case Client 1 or Client 2 log out
        cntserver = 0;
        flag = 0;
        switch (number) {
            //Connects to server
            case 0:
                while (cntserver == 0) {
                    //User must enter server's IP address and port number
                    //IP address = 127.0.0.1
                    //Port number = 5000
                    while (flag == 0) {
                        printf("Please enter the IP address: ");
                        scanf("%s", ipad);
                        if (strcmp(ipad, ip) != 0) {
                            printf("Incorrect IP address.\n");
                            flag = 0;
                        } else {
                            flag = 1;
                        }
                    }
                    flag = 0;
                    while (flag == 0) {
                        printf("Please enter the port number: ");
                        scanf("%d", &portno);
                        if (portno != 5000) {
                            printf("Incorrect port number\n");
                            flag = 0;
                        } else {
                            flag = 1;
                        }
                    }
                    portno = 5000;
                    sockfd = socket(AF_INET, SOCK_STREAM, 0); //creates a socket
                    if (sockfd < 0)
                        DieWithError("ERROR opening socket");
                    server = gethostbyname("localhost");
                    if (server == NULL) {
                        fprintf(stderr, "ERROR, no such host\n");
                        //exit(0);
                    }
                    bzero((char *) &serv_addr, sizeof (serv_addr));
                    serv_addr.sin_family = AF_INET;
                    bcopy((char *) server->h_addr,
                            (char *) &serv_addr.sin_addr.s_addr,
                            server->h_length);
                    serv_addr.sin_port = htons(portno);
                    //User connects to Server
                    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0)
                        DieWithError("ERROR connecting");
                    printf("Connecting-----\nConnected!\nWelcome! Please log in.\n");
                    connected = 1;
                    while (connected == 1) {
                        bzero(buffer, 256);
                        fgets(buffer, 255, stdin);
                        n = write(sockfd, "Log in User name is Alice\nLog in Password is 12345\n", strlen(buffer));
                        if (n < 0)
                            DieWithError("ERROR writing to socket");
                        bzero(buffer, 256);
                        n = read(sockfd, buffer, 255);
                        if (n < 0)
                            DieWithError("ERROR reading from socket");
                        //Server writes to Client to enter log in information
                        printf("%s\n", buffer);
                        //Client 1 is now connected to server
                        //User must enter Alice's login information
                        //Client 1 Username = Alice
                        //Client 1 Password = 12345
                        if ((strcmp(buffer, alice) == 0)) { 
                            flag = 0;
                            while(flag == 0){
                                scanf("%s", usern);
                                if(strcmp(usern,"Alice") == 0){ flag = 1;} 
                                else { printf("User name does not exist\n"); flag = 0; }
                            }    
                            flag = 0;
                            userNames[0] = usern;
                            printf("Password: \n");
                            while(flag == 0){
                                scanf("%s", pw);
                                if(strcmp(pw,"12345") == 0) { flag = 1; }
                                else { printf("Incorrect password\nTry again\n"); flag = 0; }
                            }
                            flag = 0;
                            passWords[0] = pw;
                            is_logged_in = 1;
                            n = write(sockfd, usern, 255);
                            n = write(sockfd, pw, 255);
                            if (n < 0)
                                //error("ERROR writing to socket");            
                                printf("login success\n---------\n");
                            //Client 1 is now logged in
                            printf("---------------------------\n");
                            printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                            printf("Your option<enter a number>: ");
                            scanf("%d", &num);
                            while (is_logged_in == 1) {
                                switch (num) {
                                    //Informs Client 1 that it is already connected to the server
                                    case 0:
                                        printf("You are already connected to the server\n");
                                        n = write(sockfd,"Client is currently connected to server\n",255);
                                        printf("---------------------------\n");
                                        printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                                        printf("Your option<enter a number>: ");
                                        scanf("%d", &num); 
                                        break;
                                    //Prints user list
                                    case 1:              
                                        printf("There are 2 users:\nAlice\nBob\n");
                                        n = write(sockfd, "Return user1 list!\n", 20);
                                        printf("---------------------------\n");
                                        printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                                        printf("Your option<enter a number>: ");
                                        scanf("%d", &num);
                                        break;
                                    //Sends message to Bob
                                    //Alice and Bob are the only existing users
                                    //Alice can only send a message to Bob
                                    case 2:
                                        flag = 0;
                                        while (flag == 0) {
                                            printf("Please enter the user name: ");
                                            scanf("%s", usn);
                                            if (strcmp(usn, "Bob") == 0) { flag = 1;}
                                            else if (strcmp(usn, "Alice") == 0) { printf("You can't send yourself a message\n"); flag = 0; }
                                            else { printf("User name does not exist\n"); flag = 0;}
                                        }
                                        flag = 0;
                                        n = write(sockfd, usn, 255);
                                        printf("Please enter the message: ");
                                        scanf("%s", message);
                                        n = write(sockfd, message, 50);
                                        if (n < 0)
                                            DieWithError("ERROR writing to socket");
                                        //Reads message from server that message was sent successfully
                                        bzero(buffer, 256);
                                        n = read(sockfd, buffer, 255);
                                        if (n < 0)
                                            DieWithError("ERROR reading from socket");
                                        printf("%s\n", buffer);

                                        printf("---------------------------\n");
                                        printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                                        printf("Your option<enter a number>: ");
                                        scanf("%d", &num);
                                        break;
                                    //Gets messages
                                    //No messages are found because Client 1 is the first user logged in
                                    //No other user could have a sent Client 1 a message
                                    case 3:
                                        printf("You have 0 message(s): \n");
                                        //printf("%s", sentMessage);
                                        n = write(sockfd, "Send back2 Bob's message!\n", 255);
                                        //Reads message from server that message was sent successfully
                                        bzero(buffer, 256);
                                        n = read(sockfd, buffer, 255);
                                        if (n < 0)
                                            DieWithError("ERROR reading from socket");
                                        printf("%s\n", buffer);
                                        printf("---------------------------\n");
                                        printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                                        printf("Your option<enter a number>: ");
                                        scanf("%d", &num);
                                        break;
                                    //Initiates a chat with a friend
                                    //Disconnects from server
                                    //Client 1 IP address = 127.0.0.1
                                    //Client 1 port number = 8000
                                    case 4:
                                        flag = 0;
                                        n = write(sockfd, "Client disconnected!\n", 255);
                                        bzero(buffer, 256);
                                        n = read(sockfd, buffer, 255);
                                        if (n < 0)
                                            DieWithError("ERROR reading from socket");
                                        printf("%s\n", buffer);
                                        //Disconnects from server
                                        close(sockfd);
                                        cnt = 0;
                                        //Client 1 listens on port number = 8000 for any incoming sockets
                                        while (flag == 0) {
                                            printf("Please enter the port number you want to listen on: ");
                                            scanf("%d", &portnum);
                                            printf("I am listening on 127.0.0.1: %d\n", portnum);
                                            if (portnum == 8000 && cnt == 0) {
                                                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                                                if (sockfd < 0)
                                                    DieWithError("ERROR opening socket");
                                                bzero((char *) &cli1_addr, sizeof (cli1_addr));
                                                portnum = 8000;
                                                cli1_addr.sin_family = AF_INET;
                                                cli1_addr.sin_addr.s_addr = INADDR_ANY;
                                                cli1_addr.sin_port = htons(portnum);
                                                if (bind(sockfd, (struct sockaddr *) &cli1_addr,
                                                        sizeof (cli1_addr)) < 0)
                                                    DieWithError("ERROR on binding");
                                                listen(sockfd, 5);
                                                clilen = sizeof (cli2_addr);
                                                newsockfd = accept(sockfd, (struct sockaddr *) &cli2_addr, &clilen);
                                                if (newsockfd < 0)
                                                    DieWithError("ERROR on accept");
                                                else
                                                    bzero(buffer, 256);
                                                n = read(newsockfd, buffer, 255);
                                                if (n < 0) DieWithError("ERROR reading from socket");
                                                //Client 1 and 2 are now connected
                                                printf("Bob is connected\n<Type 'Bye' to stop the conversation>\n");
                                                n = write(newsockfd, "Connected!\n", 13);
                                                flag = 0;
                                                //Client 1 and 2 are chatting since Client 2 connected to Client 1's IP address and port number
                                                //Client 1 keeps reading any messages sent by Client 2
                                                while (chat == 0) {
                                                    bzero(buffer, 256);
                                                    n = read(newsockfd, buffer, 255);
                                                    printf("Bob: %s\n", buffer);    
                                                    //Disconnects from Client 2 if Client 2's message = "Bye"
                                                    if (strcmp(buffer, "Bye") == 0) {
                                                        printf("Disconnect from my friend!\n");
                                                        chat = 1;
                                                        flag = 1;
                                                        portnum = 0;
                                                        cnt = 1;
                                                        cntserver = 1;
                                                        connected = 0;
                                                        is_logged_in = 0;
                                                        close(newsockfd);
                                                    } else {
                                                    //If not, Client 1 can now send a message back to Client 2
                                                    printf("Alice: ");
                                                    scanf("%s", aliceMess);
                                                    printf("<Type 'Bye to end the conversation\n>");
                                                    }
                                                    n = write(newsockfd, aliceMess, 255);
                                                    //Disconnects from Client 2 if Client 1's message = "Bye"
                                                    if (strcmp(aliceMess, "Bye") == 0) {
                                                        printf("Disconnect from my friend!\n");
                                                        chat = 1;
                                                        flag = 1;
                                                        portnum = 0;
                                                        cnt = 1;
                                                        cntserver = 1;
                                                        connected = 0;
                                                        is_logged_in = 0;
                                                        close(newsockfd);
                                                    }              
                                                }
                                            } else {
                                                printf("Incorrect port number\n");
                                            }
                                        }  
                                        break;
                                    //Tells Client 1 to initiate a chat before chatting with a friend
                                    //Client 1 is the first user logged in and has to disconnect from the server
                                    case 5:
                                        printf("You must initiate a chat first\n");
                                        n = write(sockfd,"Friend not connected,initiate chat first!\n", 255);
                                        printf("---------------------------\n");
                                        printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                                        printf("Your option<enter a number>: ");
                                        scanf("%d", &num);
                                        break;
                                    //Logs out Client 1
                                    //Doesn't stop the program
                                    case 6:
                                        printf("You have logged out\n");
                                        n = write(sockfd,"User is logged out\n",255);
                                        is_logged_in = 0;
                                        connected = 0;
                                        cntserver = 1;
                                        close(sockfd);
                                        break;
                                    //Prints options in case Client 1 enters a number != (0-6)
                                    default:
                                        printf("Enter a number 1-5\n");
                                        printf("---------------------------\n");
                                        printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                                        printf("Your option<enter a number>: ");
                                        scanf("%d", &num);
                                        break;
                                }
                            }
                        }
                        //Handles Client 2 with server
                        //Client 2 Username = Bob
                        //Client 2 Password = 56789
                        //User must enter Bob's login information to connect to the server
                        if ((strcmp(buffer, bob) == 0)) {
                            flag = 0;
                            while(flag == 0){
                                scanf("%s", usern2);
                                if(strcmp(usern2,"Bob") == 0){
                                    flag = 1;
                                } else if(strcmp(usern2,"Alice") == 0) {
                                    printf("User is currently logged in\nTry again\n");
                                    flag = 0;
                                } else {
                                    printf("User name does not exist\n");
                                    flag = 0;
                                }
                            }
                            flag = 0;
                            userNames[1] = usern2;
                            printf("Password: \n");
                            while(flag == 0){
                                scanf("%s", pw2);
                                if(strcmp(pw2,"56789") == 0) { flag = 1; }
                                else { printf("Incorrect password\nTry again\n"); flag = 0; }
                            }
                            flag = 0;
                            passWords[1] = pw2;
                            n = write(sockfd, usern2, 255);
                            n = write(sockfd, pw2, 255);
                            if (n < 0)
                                //error("ERROR writing to socket");            
                                printf("login success\n---------\n");
                            is_logged_in = 2;
                            printf("---------------------------\n");
                            printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                            printf("Your option<enter a number>: ");
                            scanf("%d", &num);
                            //Client 2 is now logged in
                            while (is_logged_in == 2) {
                                switch (num) {
                                    //Informs Client 2 that it is already connected to server
                                    case 0:
                                        printf("You are already connected\n");
                                        n = write(sockfd,"Client is currently connected\n",255);
                                        printf("---------------------------\n");
                                        printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                                        printf("Your option<enter a number>: ");
                                        scanf("%d", &num); 
                                        break;  
                                    //Prints user list
                                    case 1:
                                        printf("There are 2 users:\nAlice\nBob\n");
                                        n = write(sockfd, "Return user list!\n", 255);
                                        printf("---------------------------\n");
                                        printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                                        printf("Your option<enter a number>: ");
                                        scanf("%d", &num);
                                        break;
                                    //Sends a message to Alice
                                    //Alice won't get this message since Alice is Client 1
                                    case 2:
                                        flag = 0;
                                        while (flag == 0) {
                                            printf("Please enter the user name: ");
                                            scanf("%s", usn);
                                            if (strcmp(usn, "Alice") == 0) { flag = 1;}
                                            else if (strcmp(usn, "Bob") == 0) { printf("You can't send yourself a message\n"); flag = 0; }
                                            else { printf("User name does not exist\n"); flag = 0;}
                                        }
                                        flag = 0;
                                        n = write(sockfd, usn, 255);
                                        printf("Please enter the message: ");
                                        scanf("%s", message);
                                        n = write(sockfd, message, 50);
                                        if (n < 0)
                                            DieWithError("ERROR writing to socket");
                                        //Reads message from server that message was sent successfully
                                        bzero(buffer, 256);
                                        n = read(sockfd, buffer, 255);
                                        if (n < 0)
                                            DieWithError("ERROR reading from socket");
                                        printf("%s\n", buffer);

                                        printf("---------------------------\n");
                                        printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                                        printf("Your option<enter a number>: ");
                                        scanf("%d", &num);
                                        break;
                                    //Prints message sent from Client 1
                                    case 3:
                                        printf("You have 1 message(s): \n");
                                        //printf("%s", sentMessage);
                                        n = write(sockfd, "Send back2 Bob's message!\n", 255);
                                        //Reads message from server that message was sent successfully
                                        bzero(buffer, 256);
                                        n = read(sockfd, buffer, 255);
                                        if (n < 0)
                                            DieWithError("ERROR reading from socket");
                                        printf("%s\n", buffer);
                                        printf("---------------------------\n");
                                        printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                                        printf("Your option<enter a number>: ");
                                        scanf("%d", &num);
                                        break;
                                    //Informs Client 2 that it cannot initiate a chat
                                    //Client 1 has already initiated the chat
                                    case 4:
                                        printf("Alice has already initiated a chat with you.\n");
                                        n = write(sockfd,"Alice has already initiated a chat with you.\n", 255);
                                        printf("---------------------------\n");
                                        printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                                        printf("Your option<enter a number>: ");
                                        scanf("%d", &num);
                                        break;
                                    //Disconnects from server
                                    //Connects to Client 1's IP address and port number
                                    case 5:
                                        flag = 0;
                                        n = write(sockfd, "Client disconnected!\n", 255);
                                        //Reads server response that client is disconnected
                                        bzero(buffer, 256);
                                        n = read(sockfd, buffer, 255);
                                        if (n < 0)
                                            DieWithError("ERROR reading from socket");
                                        printf("%s\n", buffer);
                                        close(sockfd);
                                        printf("Please enter your friend's IP address: ");
                                        scanf("%s", ipad);
                                        while (flag == 0) {
                                            if (strcmp(ipad, ip) != 0) {
                                                printf("Incorrect IP address.\n");
                                                flag = 0;
                                                printf("Please enter your friend's IP address: ");
                                                scanf("%s", ipad);
                                            } else {
                                                flag = 1;
                                            }
                                        }
                                        flag = 0;
                                        while (flag == 0) {
                                            printf("Please enter your friend's port number: ");
                                            scanf("%d", &portno);
                                            if (portno == 8000) {
                                                portno = 8000;
                                                sockfd = socket(AF_INET, SOCK_STREAM, 0); //creates a socket
                                                if (sockfd < 0)
                                                    DieWithError("ERROR opening socket");
                                                server = gethostbyname("localhost");
                                                if (server == NULL) {
                                                    fprintf(stderr, "ERROR, no such host\n");
                                                    //exit(0);
                                                }
                                                bzero((char *) &serv_addr, sizeof (serv_addr));
                                                serv_addr.sin_family = AF_INET;
                                                bcopy((char *) server->h_addr,
                                                        (char *) &serv_addr.sin_addr.s_addr,
                                                        server->h_length);
                                                serv_addr.sin_port = htons(portno);
                                                if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0)
                                                    DieWithError("ERROR connecting");
                                                printf("Connecting your friend...\n");
                                                n = write(sockfd, "Send back2 Bob's message!\n", 255);
                                                //Client 1 sends Client 2  a message saying it is connected
                                                bzero(buffer, 256);
                                                n = read(sockfd, buffer, 255);
                                                if (n < 0)
                                                    DieWithError("ERROR reading from socket");
                                                printf("%s\n", buffer);
                                                printf("Type 'Bye' to end the conversation\n");
                                                chatOpen = 0;
                                                //Client 1 and Client 2 are now chatting
                                                while (chatOpen == 0) {
                                                    printf("Bob: ");
                                                    scanf("%s", bobMess);
                                                    n = write(sockfd, bobMess, 255);
                                                    //Disconnects from Client 1 if Client 2's message = "Bye"
                                                    if (strcmp(bobMess, "Bye") == 0) {
                                                        printf("Disconnect from my friend!\n");
                                                        chatOpen = 1;
                                                        cntserver = 1;
                                                        connected = 0;
                                                        is_logged_in = 0;
                                                        flag = 1;
                                                        close(sockfd);
                                                    } else {
                                                        //If not, reads and prints Client 1's message
                                                        bzero(buffer, 256);
                                                        n = read(sockfd, buffer, 255);
                                                        printf("Alice: %s\n", buffer);
                                                        printf("Type 'Bye' to end the conversation\n");
                                                    }
                                                    //Disconnects from Client 1 if Client 1's message = "Bye"
                                                    if (strcmp(buffer, "Bye") == 0) {
                                                        printf("Disconnect from my friend!\n");
                                                        chatOpen = 1;
                                                        cntserver = 1;
                                                        connected = 0;
                                                        is_logged_in = 0;
                                                        flag = 1;
                                                        close(sockfd);
                                                    }
                                                }
                                            } else {
                                                printf("Incorrect port number\n");
                                            }
                                        }
                                        break;
                                        //Logs out Client 2
                                        //Doesn't stop program
                                    case 6:
                                        printf("You have logged out\n");
                                        n = write(sockfd, "User is logged out\n", 255);
                                        is_logged_in = 0;
                                        connected = 0;
                                        cntserver = 1;
                                        close(sockfd);
                                        break;
                                    //Prints options in case Client 2 enters a number != (0-6)
                                    default:
                                        printf("Enter a number 1-5\n");
                                        printf("---------------------------\n");
                                        printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Log out\n");
                                        printf("Your option<enter a number>: ");
                                        scanf("%d", &num);
                                        break;
                                }
                            }
                        }
                    }
                }
                printf("---------------------------\n");
                printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Stop program\n");
                printf("Your option<enter a number>: ");
                scanf("%d", &number);
                break;
            //Prints user list
            case 1:
                printf("There are 2 users:\nAlice\nBob\n");
                printf("---------------------------\n");
                printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Stop program\n");
                printf("Your option<enter a number>: ");
                scanf("%d", &number);
                break;
            //Prints message to user to log in first to send a message   
            case 2:
                printf("You must be logged in to send a message\n");
                printf("---------------------------\n");
                printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Stop program\n");
                printf("Your option<enter a number>: ");
                scanf("%d", &number);
                break;
            //Prints message to user to log in first to see messages
            case 3:
                printf("You must be logged in to get your messages\n");
                printf("---------------------------\n");
                printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Stop program\n");
                printf("Your option<enter a number>: ");
                scanf("%d", &number);
                break;
            //Prints message to user to log in first to initiate a chat
            case 4:
                printf("You must be logged in to initial a chat with a friend\n");
                printf("---------------------------\n");
                printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Stop program\n");
                printf("Your option<enter a number>: ");
                scanf("%d", &number);
                break;
            //Prints message to user to log in first to chat with a friend
            case 5:
                printf("You must be logged in to chat with a friend\n");
                printf("---------------------------\n");
                printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Stop program\n");
                printf("Your option<enter a number>: ");
                scanf("%d", &number);
                break;
            //Stops program
            case 6:
                printf("Stop\n");
                stop = 1;
                break;
            //Prints options again if user enters a number != (0-6)
            default:
                printf("Enter a number 1-5\n");
                printf("---------------------------\n");
                printf("Command:\n0. Connect to the server\n1. Get the user list\n2. Send a message\n3. Get my messages\n4. Initial a chat with a friend\n5. Chat with my friend\n6. Stop program\n");
                printf("Your option<enter a number>: ");
                scanf("%d", &number);
        }
    }

    return 0;
}

