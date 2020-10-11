/*******************************************************************
*  server.c
*  Created by Leo Hoedl
*  Date: 10/5/2020
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

//Function for printing error message
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

//Function created to establish connection and to communicate with client on a new thread
void *ThreadFunction(int mysockfd) {
    char buffer[256];
    int n;

    //Displays that a client has connected at specified socket
    printf("Connection Established @ Socket %d", mysockfd);
    printf("\n");

    //Loop such that the client will exit when once the keyword 'EXIT' is recieved
    while (1) {
        bzero(buffer, 256);
        n = read(mysockfd, buffer, 255);

        //Prints error if cannot read from socket
        if (n < 0) error("ERROR reading from socket");

        //If buffer is the same as the keyword 'EXIT', close the socket and break
        if (strcmp(buffer, "EXIT\n") == 0) {
            printf("Connection Closed @ Socket %d\n", mysockfd);
            n = write(mysockfd, "Connection Closed", 17);
            close(mysockfd);
            break;
        }

        //If message is not exit, print the message and what socket it is from
        printf("Message recieved at socket %d: ", mysockfd);
        printf("%s", buffer);

        //Let client know that message is recieved by server
        n = write(mysockfd, "Message Recieved", 16);

        //Prints error if cannot write to socket
        if (n < 0) error("ERROR writing to socket");
    }
        close(mysockfd);
        return 0;
}

int main(int argc, char *argv[])
{
    //Given server code 
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     printf("\n\n"); 
    
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }


     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");


     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);


     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     //End of given server code

     //Loop so that a new thread is created each time a client connects to the server. If socket is accepted, create new thread
     while (1) {
         listen(sockfd, 5);

         clilen = sizeof(cli_addr);
         newsockfd = accept(sockfd,
             (struct sockaddr*)&cli_addr,
             &clilen);


         if (newsockfd < 0)
             error("ERROR on accept");

         pthread_t pth;
         pthread_create(&pth, NULL, ThreadFunction, newsockfd);

     }

     close(sockfd);
     return 0; 
}
