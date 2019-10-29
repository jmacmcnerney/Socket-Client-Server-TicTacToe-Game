John McNerney
CS 321
Programming Project

For this project I initially tried to use 4 files: client.c, server.c, tictactoe.c and tictactoe.h. This however gave me compiler issues
in regards to multiple variable declarations so I decided to instead use 2 files: client.c and server.c and have the tic tac toe game reside
within client.c. server.c would be solely responsible for generating random numbers to send back to the client for the "o" moves.
I decided to have the client always go first, thus being "x" and the server always going second, thus being being "o".
The game is played by having the user enter a number 1-9 to place their piece with the top 3 slots being 1, 2 and 3, the middle 3 slots being
4, 5 and 6, and the bottom 3 slots being 7, 8 and 9 from left to right. I decided to go with this over the coordinate (x, y) system because
it was easier to pass 1 integer between the client and server rather than 2.
To place the pieces I have a global turn counter that results in the move filling the specified slot with "x" when the turn is odd and
filling the specified slot with "o" when the turn is even.

server.c
To compile server.c: gcc -o server server.c
To run server.c: ./server <port>
Example run of server.c: ./server 8080

server.c is a basic socket server given by the project specs. The only modifications made to server.c were those necessary to have it read a
number from client.c, product a random number from 1-9, and to send that random number to client.c for the "o" move.
server.c also holds an array of ints that represent the 9 possible moves. Every time a move is received by the client or created randomly by the
server, this array is updated so that that move can't be made again since an "x" or an "o" is already in that spot.
To generate random moves, server.c uses rand() as well as srand(time(NULL)) to generate a random seed for random.

Issues:
I was not able to successfully implement any printf() statements in server.c to let the user know when the 
client has disconnected from the server in the event of a cats game.

client.c
To compile client.c: gcc -o client client.c
To run client.c: ./client <ip_of_server's_computer> <server's port>
Example run of client.c: ./client 192.168.1.8 8080

client.c is a basic socket client given by the project specs. Unlike server.c however, client.c had a lot of modifications made to it.
All functions necessary for the tic tac toe game are in client.c and the game is processed and played entirely in this program aside from
the random generated moves for "o" from server.c. This file clears the board, prints the board, places the pieces based on user input or
random generated moves, and checks for wins after every piece is placed.

Issues:
The client is very sensitive to invalid inputs such as symbols or letters. Basically anything that is not a number creates issues that
require ctrl+c to fix. I did not see anything in the requirements regarding this issue however so I was not too concerned with it.

Sidenotes:
I apologize for any inconsistencies with style. I edited all of this in VIM and atom and it might look different in other programs such as coderunner.
I went through the files in VIM and tried to properly line everything up but some things were a little finicky. Please view in VIM for the best 
syntax.
