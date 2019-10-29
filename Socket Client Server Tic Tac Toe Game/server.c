/* John McNerney
   Programming Project
   CS 321
   server.c
   To compile: gcc -o server server.c
   To run: ./server <server port>
   Example run: ./server 8080 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int movesMade[9] = {0}; //array of all 0s to hold 1 values in slots of numbers that have been used. for example if board[0] is occupied, movesMade[0] = 1

//lines 13-65 were the server code provided for the project from the pdf file
void dostuff(int); /* function prototype */

void error(char *msg) {
     perror(msg);
     exit(1);
}

int main(int argc, char *argv[]) {
     int sockfd, newsockfd, portno, clilen, pid;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

     printf("Socket Created.\n");

     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
              error("ERROR on binding");

     printf("Binding Complete.\n");   

     listen(sockfd,5);
     printf("Waiting for client connection.\n");
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd,
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0)
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
	     printf("Connected to Client.\n");
             close(sockfd);
	     //generates random seed based on time so that the random values are always truly random
	     srand(time(NULL));
             //forces dostuff() to always run
	     while (pid == 0) 
                 dostuff(newsockfd);
             exit(0);
          }
          else {
             close(newsockfd);
          }
      } /* end of while */
      return 0; /* we never get here */
}
//lines 13-65 were the server code provided for the project from the pdf file

//generates move from server
void dostuff (int sock) {

   //variables needed to determine errors and write move to client
   int n;
   int x;

   //receives move from client as x
   n = read(sock, &x, sizeof(x));
   
   //if "o" wins, this prevents any more moves being displayed
   if (movesMade[x - 1] == 1) {
	 printf("Client Disconnected.\n");
	 exit(0);
   }
   
   //checks to make sure n is being sent
   if (n < 0) exit(0);
   printf("Client sent move: %d\n", x);

   //if nothing is received from read(sock, &x, sizeof(x)); then this triggers and connection is severed
   //if (n < 0) exit(0);
   

   //takes move from client and assigns value of its indice in movesMade to 1. It is x-1 because the moves are 1-9 while the board is 0-8
   movesMade[x - 1] = 1;

   //generates random number from 1 to 9
   int random = rand() % 9 + 1;
   //runs while random number's indice in movesMade == 1 meaning that the spot is already occupied by a non "_"
   while (movesMade[random - 1] == 1) {
   	 random = rand() % 9 + 1;
   }

   printf("Sent move to client: %d\n", random);

   //takes move from random and assigns value of its indice in movesMade to 1. It is x-1 because the moves are 1-9 while the board is 0-8
   movesMade[random - 1] = 1;

   //writes random move to client
   write(sock, &random, sizeof(random));

   //if nothing is received from read(sock, &x, sizeof(x)); then this triggers
   if (n < 0) error("ERROR writing to socket");
}
