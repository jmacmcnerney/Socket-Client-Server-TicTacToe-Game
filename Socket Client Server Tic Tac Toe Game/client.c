/* John McNerney
   Programming Project
   CS 321
   client.c
   To compile: gcc -o client client.c
   To run: ./client <server_ip_address_> <server port>
   Example run: ./client 192.168.1.8 8080 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char board[9]; //initializes board which is a 9 character array (1 character for each space)
int turn = 1; //to keep track of turns
int catsGame = 10; //for when 9 moves happen and no one has won, it is 10 because turn begins at 1 instead of 0
int ascii_ = 95; //for compare statements
int minimumSpot = 1;
int maximumSpot = 9;
bool play = true; //while true the game is played
bool win = false; //while false the game is played

//function that sets "_" into each cell of the board array
void clearBoard() {
  //puts "_" in each spot of board[]
  for (int i = 0; i < 9; i++) {
    board[i] = '_';
  }
}

//function that prints the board in its current state
void printBoard() {
  printf("%c %c %c\n", board[0], board[1], board[2]);
  printf("%c %c %c\n", board[3], board[4], board[5]);
  printf("%c %c %c\n", board[6], board[7], board[8]);
}

//function that overwrites "_" with "x" or "o" depending on turn
void placePiece(int move) {
  //char named xo because it is equal to "x" or "o" and used to replace a "_" on the board 
  char xo;
  //if the remainder of turn is 0 then "o" will replace "_" on board[move]. This means that any even turn will use "o"
  if (turn % 2 == 0) {
    xo = 'o';
  }
  //any odd turn will use "x"
  else {
    xo = 'x';
  }
  //substitudes board[move-1] with "x" or "o" depending on turn. It is move-1 because the board is set up 1-9 while the array is 0-8
  board[move - 1] = xo;
  printBoard();
}

void checkWin() {
  //top horizontal win condition
  //checks if spaces 1, 2 and 3 are all equal and that space 1 is not empty. This is the case for all of the win condition statements.
  if ((board[0] == board[1] && board[1] == board[2]) && board[0] != '_')
    win = true;

  //middle horitzontal win condition
  if ((board[3] == board[4] && board[4] == board[5]) && board[3] != '_')
    win = true;

  //bottom horitzontal win condition
  if ((board[6] == board[7] && board[7] == board[8]) && board[6] != '_')
    win = true;

  //left verticle win condition
  if ((board[0] == board[3] && board[3] == board[6]) && board[0] != '_')
    win = true;

  //middle verticle win condition
  if ((board[1] == board[4] && board[4] == board[7]) && board[1] != '_')
    win = true;

  //right verticle win condition
  if ((board[2] == board[5] && board[5] == board[8]) && board[2] != '_')
    win = true;

  //diagonal win condition
  if ((board[0] == board[4] && board[4] == board[8]) && board[0] != '_')
    win = true;

  //diagonal win condition
  if ((board[2] == board[4] && board[4] == board[6]) && board[2] != '_')
    win = true;

  //if win is true aka if one of the conditional statements above is true
  if (win) {
    printf("Game Over!\n");
    printf("Disconnected\n");
    //game ends
    play = false;
  }
}

//lines 93-126 were the client code provided for the project from the pdf file
void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
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

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
    (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
//lines 93-126 were the client code provided for the project from the pdf file

    printf("Connection Successful\n");
    //creates and prints the board
    clearBoard();
    printBoard();
    //while play == true
    while (play) {
  	printf("Make move (1 - 9): ");
	//waits user input for value of move
  	int move;
  	scanf("%d", &move);
	//while loop that is entered if the space entered by the user for move is not 
	//occupied by "_" which is represented by ascii_ since board[move - 1] is an integer
  	while (board[move - 1] != ascii_) {
		//if the move is out of the range (1-9)
		if (move < minimumSpot || move > maximumSpot) {
			printf("Move Invalid\n");
		}
		//if the space is already taken
		else {
  			printf("That space has been taken\n");
		}
  		printf("Make move (1 - 9): ");
  		scanf("%d", &move);
    	}

	//writes the move to the server
	n = write(sockfd, &move, sizeof(move));

    	placePiece(move);
    	checkWin();
	//if play is true, add 1 to turn
    	if (play)
        	turn++;
	//when all 9 spaces have been filled
    	if (turn == catsGame) {
  		printf("Cats Game!\n");
		printf("Disconnected\n");
		//game is ended
  		play = false;
	}

	//only runs if game is still going on because it would be bad for the client to try and read a move from the server if the game has already ended
	if (play) {
		//int to hold the value sent by the server
		int x;
		//receiving value of the server and assigning it to x
		read(sockfd,&x,sizeof(x));
		printf("Move from server received:%d ", x);
		printf("\n");
		//places the server's piece
		placePiece(x);
		checkWin();
		//if game has not ended add 1 to turn
		if (play)
             	  turn++;
	}
    }
    return 0;
}
