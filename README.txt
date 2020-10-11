/*******************************************************************
*  Assignment 2 ReadMe
*  Created by Leo Hoedl
*  Date: 10/5/2020
*******************************************************************/
1. Open MobaXterm or your SSH Client of Choice
2. Log into one of the NDSU lab244 virtual labs. They will have the following formats: labxx.cs.ndsu.nodak.edu
3. Create an empty folder for testing purposes and upload the client.c and server.c files and compile them to create an executable
4. To compile use the following format:
	gcc [filename].c -o [filename] -lpthread
	->replace [filename] with the name of the filename
5. Log into another different lab244 virtual lab for as many clients as you'd like to test
	NOTE: It must be different from the first lab
6. Start the server executable with the following:
	./server [portnum] <- pick a number for port number
7. Start the client executable on a different virtual lab using the following
	./client labxx.cs.ndsu.nodak.edu [portnum] <- replace labxx with the name of the server computer and [portnum] with the same port number from server
8. Type a message in the client executable. The message will be displayed in the server with the socket that applies.
9. Type EXIT in client to close execution. In server a message will appear saying the connection at the following socket has terminated
10. Use CTRL+C to close the server. It will not close on its own. 
11. Note, one client leaving the server will not cause other clients to leave.