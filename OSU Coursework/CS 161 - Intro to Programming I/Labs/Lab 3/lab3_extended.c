/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: lab3_extended.c
 * Created: 2012 January 26, 13:42 by perrenc
 * Last Modified: 2012 February 2, 01:31 by perrenc 
 * 
 * This program tries to guess a number the user has though of. 
 * It has five tries before erroring out.
 */	

#include <stdio.h> /** included to allow for printf() to be used*/
#include <time.h> /** included to allow time() to be used*/
#include <stdlib.h> /** include to allow rand() and srand() to be used*/
/** @brief main Function
@param void This function does not accept any input variables
@return This function would return an error code to the OS if needed.
*/
int main (int argc, char **argv) {

	srand(time(NULL)); /* seed the number generator */
	int x = 0; /* variable to hold our random integer */
	int i = 0; /* variable used for user input */
	int guesses = 0; /* used to track number of user guesses */
	int max = 51; /* used as a ceiling for guesses */
	int min = 0; /* used as a floor for guesses */

/* Keeps randomizing x until it reaches a value under 50 */
	x = rand()%51; /* assigns a value to x less than or equal to 50 that is random */
	system("clear"); /* clears the screen */

/* Loops through asking the user for guesses until five guesses have been made */
	while (guesses < 5){

/* Increments the number of guesses, asks the user about how close the computer is to
 *  guessing the number, then takes the users response, narrows its guess range by 
 *  changing upper or lower limits, then randomly guesses a new value withing that range.
 *  After guessing right, it prints success, otherwise after five tries it prints that it
 *  lost.
 */ 	
		guesses = ++guesses;
		printf("Is your number %d? Enter 1 for too low, 2 for correct, or 3 for too high: ", x);
		scanf("%i", &i);
		if(i == 1){
			min = ++x; /* adjusts guess floor */
			if(max == min){ /* is about to guess the right value
					and avoids dividing by zero */
				x = min;
				continue;
			}
			x = rand()%(max-min) + min; /* used to make a guess within a range */
		}else if (i == 2){
			printf("Yay! I guessed right!\n");
			break;
		}else if (i == 3){
			max = --x; /* adjusts guess ceiling */
			if(max == min){ /* same as previous one */
				x = min;
				continue;
			}
			x = rand()%(max-min) + min; /* used to make a guess within a 
						       range */
		}else if (x != (3 | 2 | 1)){ /* prints an error if uncorrect values are
						 entered */
			printf("You have entered an incorrect value, please try again.\n");
			guesses -= 1;
		}
	}
	if (i != 2){ /* prints that the computer lost */
		printf("Awww, I lost.\n");
	}	

}

