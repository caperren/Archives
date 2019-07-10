/** ECE 151 Lab 3 Source File
 * Modified by Corwin Perren
 */

#include <stdio.h> /** included to allow for printf() to be used*/
#include <time.h> /** included to allow time() to be used*/
#include <stdlib.h> /** include to allow rand() and srand() to be used*/
/** @brief main Function
@param void This function does not accept any input variables
@return This function would return an error code to the OS if needed.
*/
int main(int argc, char **argv) {

	srand(time(NULL)); /* seed the number generator */
	int x = rand(); /* variable to hold our random integer */
	int i = 0; /* varaible used for user input */
	int guesses = 0; /* used to track number of user guesses */

/* keeps randomizing x until it reaches a value under 50 */
	while (x > 50) {
		x = rand();
	}
	
/* clears the terminal and asks the user for an input */
	system ("clear");
	printf("What is your guess for the random number under 50?\n", x);
	
	
/* loops through asking the user for guesses until five guesses have been made */
	while (guesses < 5){ 
		
		scanf ("%d", &i); /* takes in user input */
		if (i < x) {
/* prints if the user's value is less than x */
			system("clear");
			printf("Your guess is too low.\n");
		}else if (i == x) {
/* prints if the user's value is x, then breaks */
			system("clear");
			printf("You guessed right!\n");
			break;
		}else if (i > x) {
/* prints if the user's value is larger than x */
			system("clear");
			printf("Your guess is too high.\n"); 
		}

		guesses = ++guesses; /* incriments the number of guesses the user has made */
}
}
