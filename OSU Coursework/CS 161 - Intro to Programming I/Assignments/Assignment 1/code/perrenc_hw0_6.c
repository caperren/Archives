/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: perrenc_hw0_6.c
 * Created: Unknown by perrenc
 * Last Modified: 2012 February 21, 20:54 by perrenc
 *
 * Calulates and prints out the fibonacci sequence up to a value the user specifies.
 *
 */

#include <stdio.h>

int main (int argc, char **argv){

	int x; 		// Used for loop limiting
	int k; 		// Used for user input
	int a = 0; 	// Beginning of the fibonacci sequences
	int b = 1; 	// Another beginning value of the fibonacci sequence
	int total;

	/*
	 * Asks users to what value of the fibonacci sequence they would like to 
	 * calculate and then stores it in k
	 */
	printf("Enter to what iteration of the fibonacci sequence you want to calculate: ");
	scanf("%d", &k);

	/*
	 * Prints an iteration of the fibonacci sequence, totals the two previous values
	 * to make the next iteration, then loops until the number of iterations reaches 
	 * what the user inputted previously.
	 */
	for(x = 0 ; x < k ; x++){
		printf("%d ", a);
		total = a + b;
		a = b;
		b = total;
	}

	/* Prints a new line to make the output cleaner. */
	printf("\n");
}
