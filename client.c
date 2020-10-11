/*******************************************************************
*  client.c
*  Created by Leo Hoedl
*  Date: 10/5/2020
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

//Function for printing error message
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    //Client code provided in example
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    printf("\n\n");

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    //End of provided code

    /*Code implemented such that the client will be able to continually be able to 
    send messages until the key word 'EXIT' is typed and submitted. Upon the keyword
    the socket is closed and client exits.*/
    while (1) {
        //Prompts client to enter message
        printf("Please enter the message: ");
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));

        // If write return value is negative, print error message
        if (n < 0)
            error("ERROR writing to socket");

        //Clears buffer for read
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);

        //If read return value is negative, print error message
        if (n < 0)
            error("ERROR reading from socket");

        /*Compares buffer to expected closing message. If it matches the message passed from server,
        the client is terminated and socket is closed. While loop is exited*/
        if (strcmp(buffer, "Connection Closed") == 0) {
            printf("%s\n", buffer);
            break;
        }
        
        /*If message is not 'EXIT', print message from server stating that message was recieved
        successfully. */
        printf("%s\n", buffer);
    }

    close(sockfd);
    return 0;
}
