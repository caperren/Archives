/*
 * Original Author: Corwin A. Perren (perrenc)
 * File: perrenc_hw0_7.c
 * Created: Unknown by perrenc
 * Last Modified: 2012 February 21, 20:54 by perrenc
 *
 * Prints the ascii character for the number a user enters.
 *
 */

#include <stdio.h>

int main(int argc, char **argv){

	int x;	// Variable used for user input
	
	/* Ask user for input, then store it on x. */
	printf("Please enter a number between 0 and 255: ");
	scanf("%d", &x);
	
	/* Prints the appropriate ascii character for an entered value as long as it is
	 * within range, else it prints an error, or exits with a "-1" input.
	 */
	if ((x < 0 | x > 255) & (x != -1)){
		printf("You entered an improper value or it was out of range...\n");
	}else if ((x >= 0) & (x <= 255)){
		printf("%c\n", x);
	}		
}

