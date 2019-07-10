/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: lab4_2.c
 * Created: 2012 February 8, 16:58 by perrenc
 * Last Modified: 2012 February 8, 23:09 by perrenc
 *
 * Asks the users for a number between 1 and 20 and prints up to that number from one
 * and down to one from that number.
 * It errors if the value is not within this range.
 */
 
#include <stdio.h>

int main(int argc, char **argv) {

	int userinput; //Variable for user input
	int loop; //Used for looping

// Clears the screen and asks the user to input an integere between one and twenty
	system("clear");
	printf("Please enter an integer between 1 and 20: ");
	scanf("%d", &userinput);

// Quits the program if an incorrect value has been entered
	if((1 > userinput) | (userinput > 20)){
		printf("You have entered an incorrect value, closing program...\n");
		return(-1);
	}

//Prints up from one to the user entered value
	for(loop = 1 ; (loop <= userinput) ; ++loop){
		printf("%d ", loop);
	}
//Prints newline
	printf("\n");

//Prints down to one from the user entered number
	for(loop = userinput ; ((loop <= userinput) & (loop != 0)) ; loop--){
		printf("%d ", loop);
	}
//Prints a newline	
	printf("\n");
}
