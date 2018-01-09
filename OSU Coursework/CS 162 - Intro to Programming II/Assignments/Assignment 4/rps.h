#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "player.h"
using namespace std;

class Rps{

public:
	void newgame(){
		/*Extra Challenge 5: 
		Make this program a 3 player game*/
		
		Player player1;
		Player player2;
		
		string player1guess;
		string player2guess;
		
		/*Extra Challenge 3
		Make this program Rock, Paper, Scissors, Lizard, Spock*/
		
		/*Extra Challenge 4
		Make this program best 2 out of 3*/
		
		cout << "Welcome to RPS (Rock, Paper, Scissors) v1.0" << endl;
		cout << "Preparing to run the guess() method on both player objects" << endl;
		
		/*Extra Challenge 1
		Get User input for one of the guesses*/
		
		player1guess = player1.guess();
		player2guess = player2.guess();
			
	while (player1guess.compare(player2guess) == 0){
		player1guess = player1.guess();
		player2guess = player2.guess();
		}	
		
	if ((player1guess.compare("Rock") == 0)  && (player2guess.compare("Scissors") == 0) )
		cout << "Player 1 smashes the heck out of player 2 with a rock! Die scissors die!" << endl;
	if ((player1guess.compare("Scissors") == 0)  && (player2guess.compare("Rock") == 0) )
		cout << "Player 2 smashes the heck out of player 1 with a rock! Die scissors die!" << endl;
	if ((player1guess.compare("Paper") == 0)  && (player2guess.compare("Rock") == 0) )
		cout << "Player 1 smothers player 2 with paper! Die rock die!" << endl;
	if ((player1guess.compare("Rock") == 0)  && (player2guess.compare("Paper") == 0) )
		cout << "Player 2 smothers player 1 with paper! Die rock die!" << endl;
	if ((player1guess.compare("Scissors") == 0)  && (player2guess.compare("Paper") == 0) )
		cout << "Player 1 slices and dices player 2 with scissors! Die paper die!" << endl;
	if ((player1guess.compare("Paper") == 0)  && (player2guess.compare("Scissors") == 0) )
		cout << "Player 2 slices and dices player 1 with scissors! Die paper die!" << endl;
	
		/*Extra Challenge 6
		Actually declare a winner*/
	}
	
	/*Extra Challenge 7
		Keep track of number of wins and number of losses for each object of this class*/
};


